#include "Michelangelo.h"
#include "UnrealHelpers.h"
#include "NonUnreal/Common/Helpers.h"

using namespace Common;

std::string Helpers::FStringToString(const FString& fstr)
{
	return std::string(TCHAR_TO_UTF8(*fstr));
}
FString Helpers::WStringToFString(const std::wstring& wstr)
{
	return StringToFString(WStringToString(wstr));
}
std::wstring Helpers::FStringToWString(const FString& fstr)
{
	return StringToWString(FStringToString(fstr));
}
FString Helpers::StringToFString(const std::string& str)
{
	return FString(str.c_str());
}
FMatrix Helpers::ArrayToMatrix(const std::array<float, 16>& transformArray)
{
	return FMatrix(
		FPlane(transformArray[0], transformArray[1], transformArray[2], transformArray[3]),
		FPlane(transformArray[4], transformArray[5], transformArray[6], transformArray[7]),
		FPlane(transformArray[8], transformArray[9], transformArray[10], transformArray[11]),
		FPlane(transformArray[12], transformArray[13], transformArray[14], transformArray[15])
	);
	/*return FMatrix(
	FPlane(transformArray[0], transformArray[4], transformArray[8], transformArray[12]),
	FPlane(transformArray[1], transformArray[5], transformArray[9], transformArray[13]),
	FPlane(transformArray[2], transformArray[6], transformArray[10], transformArray[14]),
	FPlane(transformArray[3], transformArray[7], transformArray[11], transformArray[15])
	);*/
}