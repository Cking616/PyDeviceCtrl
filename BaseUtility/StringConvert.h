#pragma once
#include <string>


namespace StringConvert
{
	bool StringtoWstring(const std::string& szSrc_,std::wstring& wszDrc_);
	bool WstringtoString(const std::wstring& wszSrc_,std::string& szDrc_);
	bool StringToInt_A(const std::string&	szLsrc_, int& dResult_);
	bool StringToInt_W(const std::wstring&	wszLsrc_, int& dResult_);
	bool StringToUpper_A(std::string& szLsrc_);
	bool StringToUpper_W(std::wstring& wszLsrc_);
	bool AnalysisExpression_A(const std::string& szSrc_,  std::string& szLeftValue_, std::string& szRightValue_);
	bool AnalysisExpression_W(const std::wstring& szSrc_,  std::wstring& szLeftValue_, std::wstring& szRightValue_);
	bool RemoveBrackets_A(std::string& szSrc_);
	bool RemoveBrackets_W(std::wstring& szSrc_);
	bool TrimSpace_A(std::string& szSrc_); 
	bool TrimSpace_W(std::wstring& szSrc_);
	bool HexStringToByte_A(const std::string&	szLsrc_, unsigned int& dResult_, unsigned char dLength_);
	bool HexStringToByte_W(const std::wstring&	szLsrc_, unsigned int& dResult_, unsigned char dLength_);
}

