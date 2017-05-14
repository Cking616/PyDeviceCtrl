#pragma once
#include <string>



class CDriverModel_Edition1	
{
public:
	CDriverModel_Edition1(void);
	virtual ~CDriverModel_Edition1(void);
	bool IsInitialize();
	bool Initialize(const char* szHID_);
	bool UpdateDriver(const  char* szInfpath_);
	bool RollbackDriver();
	std::wstring& ShowDriverInfo();
	std::wstring& ShowDeviceInfo();
	unsigned int GetLastError();

private:
	std::wstring		m_wszHID;
	std::wstring		m_wszasHID;
	std::wstring		m_wszCombHID;
	std::wstring		m_wszPresentInfName;
	std::wstring		m_wszLastInfName;
	bool				m_bIsInitialize;
	unsigned int		m_dStatus;

private:
	bool TestDevices(const std::wstring& szasHID_);
	bool UpdateDriver_Base(const std::wstring& szInfPath_);
	bool RollbackDriver_Base(void);
};

