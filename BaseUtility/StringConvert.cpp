#include "StdAfx.h"
#include "StringConvert.h"
#include <locale>
#include <cstdlib>
#include <algorithm>


using std::wstring;
using std::string;

bool StringConvert::StringtoWstring(const std::string& szSrc_,std::wstring& wszDrc_)
{
	string CurLocalse = setlocale(LC_ALL, NULL);
	setlocale(LC_ALL, "chs");
	const char* _strSrc = szSrc_.c_str();
	size_t _Dsize = szSrc_.size() + 1;

	wchar_t* _strDrc = new wchar_t[_Dsize];
	wmemset(_strDrc, 0, _Dsize);

	mbstowcs(_strDrc, _strSrc, _Dsize);
	wszDrc_ = _strDrc;

	delete[] _strDrc;
	setlocale(LC_ALL, CurLocalse.c_str());
	return true;
}

bool StringConvert::WstringtoString(const std::wstring& wszSrc_,std::string& szDrc_)
{
	string CurLocalse = setlocale(LC_ALL, NULL);
	setlocale(LC_ALL, "chs");

	const wchar_t* _strSrc = wszSrc_.c_str();
	size_t _Dsize = 2 * wszSrc_.size() + 1;

	char* _strDrc = new char[_Dsize];
	memset(_strDrc, 0, _Dsize);

	wcstombs(_strDrc, _strSrc, _Dsize);
	szDrc_ = _strDrc;

	delete[] _strDrc;
	setlocale(LC_ALL, CurLocalse.c_str());
	return true;
}

bool StringConvert::StringToUpper_A(std::string&	szLsrc_)
{
	transform(szLsrc_.begin(), szLsrc_.end(), szLsrc_.begin(), ::toupper);
	return true;
}

bool StringConvert::StringToUpper_W(std::wstring& wszLsrc_)
{
	transform(wszLsrc_.begin(), wszLsrc_.end(), wszLsrc_.begin(), ::toupper);
	return true;
}

bool StringConvert::StringToInt_A(const std::string&	szLsrc_, int& dResult_)
{
	char _cInputCh = '\0';
	long long _dRetInt = 0;
	bool _bIsIni = false;
	string::const_iterator _itInpuch = szLsrc_.begin();
	_cInputCh = *_itInpuch;

	if(_cInputCh == '-')
	{
		_bIsIni = true;
		_itInpuch++;
		if(_itInpuch != szLsrc_.end())
		{
			_cInputCh = *_itInpuch;
		}
		else
		{
			return false;
		}
	}

	for(; _itInpuch != szLsrc_.end(); ++_itInpuch)
	{
		_cInputCh = *_itInpuch;
		if(_cInputCh < '0' || _cInputCh >  '9')
		{
			return false;
		}

		_dRetInt = _dRetInt * 10 + _cInputCh - '0';

		if (_dRetInt >= (1 << 31) - 1)
		{
			return false;
		}	
	}

	if(_bIsIni)
	{
		_dRetInt *= -1;
	}

	dResult_ = _dRetInt;
	return true;
}

bool StringConvert::StringToInt_W(const std::wstring&	wszLsrc_, int& dResult_)
{
	wchar_t _cInputCh = L'\0';
	long long _dRetInt = 0;
	bool _bIsIni = false;
	wstring::const_iterator _itInpuch = wszLsrc_.begin();
	_cInputCh = *_itInpuch;

	if(_cInputCh == L'-')
	{
		_bIsIni = true;
		_itInpuch++;
		if(_itInpuch != wszLsrc_.end())
		{
			_cInputCh = *_itInpuch;
		}
		else
		{
			return false;
		}
	}

	for(; _itInpuch != wszLsrc_.end(); ++_itInpuch)
	{
		_cInputCh = *_itInpuch;
		if(_cInputCh < L'0' || _cInputCh >  L'9')
		{
			return false;
		}

		_dRetInt = _dRetInt * 10 + _cInputCh - L'0';

		if (_dRetInt >= (1 << 31) - 1)
		{
			return false;
		}
	}

	if(_bIsIni)
	{
		_dRetInt *= -1;
	}

	dResult_ = _dRetInt;
	return true;
}

bool StringConvert::AnalysisExpression_A(const std::string& szSrc_,  std::string& szLeftValue_, std::string& szRightValue_)
{
	int _dEquSymbel = -1, _i = 0;
	string::const_iterator _itInpuch = szSrc_.begin();

	while(_itInpuch != szSrc_.end())
	{
		if(*_itInpuch == '=')
		{
			if(_dEquSymbel != -1)
			{
				return false;
			}
			_dEquSymbel = _i;
		}
		_i++;
		_itInpuch++;
	}

	if(_dEquSymbel != -1)
	{
		szLeftValue_ = szSrc_.substr(0, _dEquSymbel);
		szRightValue_ = szSrc_.substr(_dEquSymbel + 1, szSrc_.length());

		TrimSpace_A(szLeftValue_);
		TrimSpace_A(szRightValue_);
		return true;
	}
	return false;
}

bool StringConvert::AnalysisExpression_W(const std::wstring& szSrc_,  std::wstring& szLeftValue_, std::wstring& szRightValue_)
{
	int _dEquSymbel = -1, _i = 0;
	wstring::const_iterator _itInpuch = szSrc_.begin();

	while(_itInpuch != szSrc_.end())
	{
		if(*_itInpuch == L'=')
		{
			if(_dEquSymbel != -1)
			{
				return false;
			}
			_dEquSymbel = _i;
		}
		_i++;
		_itInpuch++;
	}

	if(_dEquSymbel != -1)
	{
		szLeftValue_ = szSrc_.substr(0, _dEquSymbel - 1);
		szRightValue_ = szSrc_.substr(_dEquSymbel + 1, szSrc_.length() - 1);

		TrimSpace_W(szLeftValue_);
		TrimSpace_W(szRightValue_);
		return true;
	}
	return false;
}

bool StringConvert::RemoveBrackets_A(std::string& szSrc_)
{
	TrimSpace_A(szSrc_);
	unsigned int _dFirst = szSrc_.find_first_of('[');
	unsigned int _dLast = szSrc_.find_last_of(']');

	if(_dFirst != szSrc_.npos
		&& _dLast != szSrc_.npos)
	{
		szSrc_.erase(_dLast);
		szSrc_.erase(_dFirst, 1);
		return true;
	}
	return false;
}

bool StringConvert::RemoveBrackets_W(std::wstring& szSrc_)
{
	TrimSpace_W(szSrc_);
	unsigned int _dFirst = szSrc_.find_first_of(L'[');
	unsigned int _dLast = szSrc_.find_last_of(L']');
	if(_dFirst != szSrc_.npos
		&& _dLast != szSrc_.npos)
	{
		szSrc_.erase(_dLast);
		szSrc_.erase(_dFirst, 1);
		return true;
	}
	return false;
}

bool StringConvert::TrimSpace_A(std::string& szSrc_)
{
	if(szSrc_.size() == 0)
	{
		return true;
	}

	string::iterator _itInpuch = szSrc_.begin();

	while((*_itInpuch == ' ' 
		|| *_itInpuch == '\t ')
		&& _itInpuch != szSrc_.end())
	{
		szSrc_.erase(_itInpuch);
		if(szSrc_.size() == 0)
		{
			return true;
		}
	}
	_itInpuch = szSrc_.end();
	_itInpuch--;

	while((*_itInpuch == ' ' 
		|| *_itInpuch == '\t ')
		&& _itInpuch != szSrc_.begin())
	{
		szSrc_.erase(_itInpuch);
		_itInpuch--;
	}
	return true;
}

bool StringConvert::TrimSpace_W(std::wstring& szSrc_)
{
	if(szSrc_.size() == 0)
	{
		return true;
	}

	wstring::iterator _itInpuch = szSrc_.begin();

	while((*_itInpuch == L' ' 
		|| *_itInpuch == L'\t ')
		&& _itInpuch != szSrc_.end())
	{
		szSrc_.erase(_itInpuch);

		if(szSrc_.size() == 0)
		{
			return true;
		}

	}
	_itInpuch = szSrc_.end();
	_itInpuch--;

	while((*_itInpuch == L' ' 
		|| *_itInpuch == L'\t ')
		&& _itInpuch != szSrc_.begin())
	{
		szSrc_.erase(_itInpuch);
		_itInpuch--;
	}
	return true;
}

bool StringConvert::HexStringToByte_A(const std::string&	szLsrc_, unsigned int& dResult_, unsigned char dLength_)
{
	if(dLength_ > 8)
	{
		return false;
	}

	if(szLsrc_.length() > dLength_)
	{
		return false;
	}

	char _cInputCh = '\0';
	int _dRetInt = 0;
	//string _szTempsrc(szLsrc_);
	//StringToUpper_A(_szTempsrc);

	string::const_iterator _itInpuch = szLsrc_.begin();


	for(; _itInpuch != szLsrc_.end(); ++_itInpuch)
	{
		_cInputCh = *_itInpuch;
		if(_cInputCh >= '0' && _cInputCh <=  '9')
		{
			_dRetInt = _dRetInt * 16 + _cInputCh - '0';
		}
		else if(_cInputCh >= 'A' && _cInputCh <= 'F')
		{
			_dRetInt = _dRetInt * 16 + _cInputCh - 'A' + 10;
		}
		else if(_cInputCh >= 'a' && _cInputCh >= 'f')
		{
			_dRetInt = _dRetInt * 16 + _cInputCh - 'a' + 10;
		}
		else
		{
			return false;
		}
	}


	dResult_ = _dRetInt;
	return true;
}

bool StringConvert::HexStringToByte_W(const std::wstring&	szLsrc_, unsigned int& dResult_, unsigned char dLength_)
{
	if(dLength_ > 8)
	{
		return false;
	}

	if(szLsrc_.length() > dLength_)
	{
		return false;
	}

	wchar_t _cInputCh = L'\0';
	int _dRetInt = 0;

	wstring::const_iterator _itInpuch = szLsrc_.begin();
	//_cInputCh = *_itInpuch;

	for(; _itInpuch != szLsrc_.end(); ++_itInpuch)
	{
		_cInputCh = *_itInpuch;
		if(_cInputCh >= '0' && _cInputCh <=  '9')
		{
			_dRetInt = _dRetInt * 16 + _cInputCh - '0';
		}
		else if(_cInputCh >= 'A' && _cInputCh <= 'F')
		{
			_dRetInt = _dRetInt * 16 + _cInputCh - 'A' + 10;
		}
		else if(_cInputCh >= 'a' && _cInputCh >= 'f')
		{
			_dRetInt = _dRetInt * 16 + _cInputCh - 'a' + 10;
		}
		else
		{
			return false;
		}
	}

	dResult_ = _dRetInt;
	return true;
}
