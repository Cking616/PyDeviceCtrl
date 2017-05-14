#include "StdAfx.h"
#include "DriverModel_Edition1.h"
#include <windows.h>  
#include <newdev.h>  
#include <SetupAPI.h>
#include <cfgmgr32.h>
#include "StringConvert.h"

#pragma comment(lib, "newdev.lib")  
#pragma comment(lib, "setupapi.lib")

using namespace StringConvert;

//static const wchar_t* N_HARDWARE_ID[] = {L"VEN_1C00&DEV_5831",
//										 L"VEN_4348&DEV_5049"};

bool CDriverModel_Edition1::TestDevices(const std::wstring& szasHID_)
{
	HDEVINFO dev_info;
	SP_DEVINFO_DATA dev_info_data;
	SP_DRVINFO_DATA drv_info_data;
	unsigned char _Buffer[2000] = {0};
	PSP_DRVINFO_DETAIL_DATA pdrv_info_detail_data = (PSP_DRVINFO_DETAIL_DATA)_Buffer;

	std::wstring _wszCombHID = L"";
	std::wstring _wszPresentInfName;
	std::wstring _wszHID;

	int _nDevIndex;
	WCHAR _szid[MAX_PATH];
	ULONG _dproblem;
	ULONG _dstatus;
	int _nDevConnect = 0;


	dev_info = SetupDiGetClassDevs(NULL, NULL, NULL, DIGCF_ALLCLASSES|DIGCF_PRESENT);

	if(dev_info == INVALID_HANDLE_VALUE)
	{
		m_dStatus = 0x23000000;
		SetupDiDestroyDeviceInfoList(dev_info);
		printf("SetupDiGetClassDevs 错误!\n");
		return false;
	}

	_nDevIndex = 0;

	// 枚举设备，从0开始枚举设备
	dev_info_data.cbSize = sizeof(dev_info_data);
	while(SetupDiEnumDeviceInfo(dev_info, _nDevIndex, &dev_info_data))
	{
		if(CR_SUCCESS != CM_Get_Device_ID(dev_info_data.DevInst, _szid, MAX_PATH, 0))
			continue;

		std::wstring _wszTempCombHID;
		_wszTempCombHID = _szid;
		if(_wszTempCombHID.find(szasHID_) != _wszTempCombHID.npos)
		{
			if(CM_Get_DevNode_Status(&_dstatus,
				&_dproblem,
				dev_info_data.DevInst,
				0) != CR_NO_SUCH_DEVINST)
			{
				if(_nDevConnect > 0)
				{
					m_dStatus = 0x22000000;
					SetupDiDestroyDriverInfoList(dev_info, &dev_info_data, SPDIT_COMPATDRIVER);
					SetupDiDestroyDeviceInfoList(dev_info);
					printf("存在过多此类型的设备!\n");
					return false;
				}

				_nDevConnect ++;

				SetupDiBuildDriverInfoList(dev_info, &dev_info_data, SPDIT_COMPATDRIVER);
				drv_info_data.cbSize = sizeof(drv_info_data);
				int _dRetryTim = 0;
				while(!SetupDiEnumDriverInfo(dev_info, &dev_info_data, SPDIT_COMPATDRIVER, 0, &drv_info_data))
				{
					//DWORD _dwErrorCode = ::GetLastError(); 
					//printf("SetupDiEnumDriverInfo 错误, 错误码： %d\n",_dwErrorCode);  
					if(_dRetryTim == 3)
					{
						m_dStatus = 0x26000000;
						SetupDiDestroyDriverInfoList(dev_info, &dev_info_data, SPDIT_COMPATDRIVER);
						SetupDiDestroyDeviceInfoList(dev_info);
						return false;
					}
					_dRetryTim++;
					SetupDiBuildDriverInfoList(dev_info, &dev_info_data, SPDIT_COMPATDRIVER);
					drv_info_data.cbSize = sizeof(drv_info_data);
				}
				DWORD _dRequiredSize;

				pdrv_info_detail_data->cbSize = sizeof(SP_DRVINFO_DETAIL_DATA);
				if (!SetupDiGetDriverInfoDetail(dev_info,  &dev_info_data, &drv_info_data, pdrv_info_detail_data, sizeof(SP_DRVINFO_DETAIL_DATA), &_dRequiredSize))
				{

					memset(pdrv_info_detail_data, 0, sizeof(SP_DRVINFO_DETAIL_DATA));
					pdrv_info_detail_data->cbSize = sizeof(SP_DRVINFO_DETAIL_DATA);

					if (!SetupDiGetDriverInfoDetail(dev_info,  &dev_info_data, &drv_info_data, pdrv_info_detail_data, _dRequiredSize, &_dRequiredSize))
					{
						DWORD _dwErrorCode = ::GetLastError(); 
						printf("SetupDiGetDriverInfoDetail 错误,错误码： %d\n",_dwErrorCode);  
						m_dStatus = 0x26000000;
						SetupDiDestroyDriverInfoList(dev_info, &dev_info_data, SPDIT_COMPATDRIVER);
						SetupDiDestroyDeviceInfoList(dev_info);
						return false;
					}
				}

				DWORD _dDataType;
				WCHAR _szHIDBuffer[500];
				DWORD _dBufferSize = 500;

				SetupDiGetDeviceRegistryProperty(dev_info,  &dev_info_data, SPDRP_HARDWAREID, &_dDataType, (PBYTE)_szHIDBuffer, _dBufferSize, &_dBufferSize);

				_wszCombHID = _wszTempCombHID;
				_wszPresentInfName = pdrv_info_detail_data->InfFileName;
				_wszHID = _szHIDBuffer;
			}
		}

		_nDevIndex++;
	}

	SetupDiDestroyDriverInfoList(dev_info, &dev_info_data, SPDIT_COMPATDRIVER);
	SetupDiDestroyDeviceInfoList(dev_info);

	if(_nDevConnect > 0)
	{
		m_wszasHID = szasHID_;
		m_wszHID = _wszHID;
		StringToUpper_W(m_wszHID);
		StringToUpper_W(m_wszasHID);

		//		bool _bIsFind = false;
		//		if(m_wszHID.find(N_HARDWARE_ID[0]) != m_wszHID.npos)
		//		{
		////			_tprintf(_T("Find this HID! HID is %s, wszHID is %s\n"), m_wszHID.c_str(), N_HARDWARE_ID[0]);
		//			_bIsFind = true;
		//		}
		//
		//		if(m_wszHID.find(N_HARDWARE_ID[1]) != m_wszHID.npos)
		//		{
		////			_tprintf(_T("Find this HID! HID is %s, wszHID is %s\n"), m_wszHID.c_str(), N_HARDWARE_ID[1]);
		//			_bIsFind = true;
		//		}
		//
		if(m_wszHID.find(m_wszasHID) == m_wszHID.npos)
		{
			_tprintf(_T("HID 错误! HID 为 %s"), m_wszHID.c_str());
			m_dStatus = 0x27000000;
			return false;
		}

		m_wszPresentInfName = _wszPresentInfName;
		m_wszCombHID = _wszCombHID;
		return true;
	}

	//printf("找不此HID设备!asHID 为 %s\n", szasHID_.c_str());
	m_dStatus = 0x28000000;
	return false;
}

bool WaitForInstallEvents()
{
	long long _dTick = ::GetTickCount();
	long long _dLastTick = _dTick;
	while(!(CM_WaitNoPendingInstallEvents(0) == WAIT_OBJECT_0) )
	{
		_dTick = ::GetTickCount();
		if(_dTick -  _dLastTick> 10000)
		{
			printf("等待InstallEvents超时\n");
			return false;
		}
	}
	return true;
}

bool CDriverModel_Edition1::UpdateDriver_Base(const std::wstring& szInfPath_)
{
	if(!WaitForInstallEvents())
	{
		m_dStatus = 0x22000000;
		return false;
	}

	WIN32_FIND_DATA _filedata;
	HANDLE _handle = FindFirstFile(szInfPath_.c_str(), &_filedata);
	if(_handle == INVALID_HANDLE_VALUE)
	{
		m_dStatus = 0x23000000;
		printf("找不到此inf文件!\n");
		return false;
	}

	if(!TestDevices(m_wszasHID))
	{
		return false;
	}


	BOOL _bRebootRequired;  

	_bRebootRequired = FALSE;  


	if (!UpdateDriverForPlugAndPlayDevices(NULL, m_wszHID.c_str(), szInfPath_.c_str(),   
		INSTALLFLAG_FORCE, &_bRebootRequired))  
	{  
		//DWORD _dwErrorCode = GetLastError();  
		m_dStatus = 0x24000000; 

		//printf("UpdateDriverForPlugAndPlayDevices false,ErrorCode %d\n",_dwErrorCode);
		printf("UpdateDriverForPlugAndPlayDevice 失败!\n");
		return false;  
	}  

	return true;  
}

bool CDriverModel_Edition1::RollbackDriver_Base()
{
	HDEVINFO dev_info;
	SP_DEVINFO_DATA dev_info_data;
	unsigned char _Buffer[2000] = {0};
	PSP_DRVINFO_DETAIL_DATA pdrv_info_detail_data = (PSP_DRVINFO_DETAIL_DATA)_Buffer;

	std::wstring _wszCombHID = L"";
	std::wstring _wszPresentInfName;
	std::wstring _wszHID;

	int _nDevIndex;
	WCHAR _szid[MAX_PATH];
	ULONG _dproblem;
	ULONG _dstatus;
	int _nDevConnect = 0;


	dev_info = SetupDiGetClassDevs(NULL, NULL, NULL, DIGCF_ALLCLASSES|DIGCF_PRESENT);

	if(dev_info == INVALID_HANDLE_VALUE)
	{
		m_dStatus = 0x23000000;
		SetupDiDestroyDeviceInfoList(dev_info);
		printf("SetupDiGetClassDevs 错误!\n");
		return false;
	}

	_nDevIndex = 0;
	// 枚举设备，从0开始枚举设备
	dev_info_data.cbSize = sizeof(dev_info_data);
	while(SetupDiEnumDeviceInfo(dev_info, _nDevIndex, &dev_info_data))
	{
		if(CR_SUCCESS != CM_Get_Device_ID(dev_info_data.DevInst, _szid, MAX_PATH, 0))
			continue;

		std::wstring _wszTempCombHID;
		_wszTempCombHID = _szid;
		if(_wszTempCombHID.find(m_wszasHID) != _wszTempCombHID.npos)
		{
			if(CM_Get_DevNode_Status(&_dstatus,
				&_dproblem,
				dev_info_data.DevInst,
				0) != CR_NO_SUCH_DEVINST)
			{
				//if(!DiRollbackDriver(dev_info, &dev_info_data, NULL, 1, NULL))
				//{
						if (!UpdateDriverForPlugAndPlayDevices(NULL, m_wszHID.c_str(), m_wszLastInfName.c_str(),   
								INSTALLFLAG_FORCE, NULL)) 
						{
							return false;
						}
						else
						{
							return true;
						}
				//}
				//else
				//{
				//	return true;
				//}
			}
		}
		_nDevIndex++;
	}

	return true;
}
