#include "StdAfx.h"
#include "DriverModel_Edition1.h"
#include "StringConvert.h"

CDriverModel_Edition1::CDriverModel_Edition1(void)
	:	m_wszHID(L""),
		m_wszCombHID(L""),
		m_wszPresentInfName(L""),
		m_wszLastInfName(L""),
		m_bIsInitialize(false),
		m_dStatus(0)
{
}

CDriverModel_Edition1::~CDriverModel_Edition1(void)
{
}

bool CDriverModel_Edition1::IsInitialize()
{
	if(m_dStatus != 0)
	{
		return false;
	}

	return m_bIsInitialize;
}

bool CDriverModel_Edition1::Initialize(const char* szHID_)
{
	//if(m_dStatus != 0)
	//{
	//	return false;
	//}
	std::wstring _wszHID;
	StringConvert::StringtoWstring(std::string(szHID_), _wszHID);
	m_bIsInitialize = false;
	if(!TestDevices(_wszHID))
	{
		m_dStatus = m_dStatus;
		return false;
	}

	m_bIsInitialize = true;
	m_dStatus = 0;
	return true;
}

bool CDriverModel_Edition1::UpdateDriver(const char* szInfpath_)
{
	//if(m_dStatus != 0)
	//{
	//	return false;
	//}

	std::wstring _wszInfpath;
	StringConvert::StringtoWstring(std::string(szInfpath_), _wszInfpath);

	if(!m_bIsInitialize)
	{
		m_dStatus = 0x25000000;
		return false;
	}
	
	if(_wszInfpath == L"")
	{
		m_dStatus = 0x23000000;
		return false;
	}

	if(!UpdateDriver_Base(_wszInfpath))
	{
		m_dStatus = m_dStatus;
		return false;
	}
	m_wszLastInfName = m_wszPresentInfName;
	m_wszPresentInfName = _wszInfpath;
	return true;
}

bool CDriverModel_Edition1:: RollbackDriver()
{
	if(m_dStatus != 0)
	{
		return false;
	}

	if(!RollbackDriver_Base())
	{
		return false;
	}
	return true;
}

std::wstring& CDriverModel_Edition1::ShowDriverInfo()
{
	return m_wszPresentInfName;
}

std::wstring& CDriverModel_Edition1::ShowDeviceInfo()
{
	return m_wszHID;
}

unsigned int CDriverModel_Edition1::GetLastError()
{
	return m_dStatus;
}
