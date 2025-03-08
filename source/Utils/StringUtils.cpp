#include "StringUtils.h"
#include <codecvt>

namespace StringUtils
{
std::string WstringToString(const std::wstring& wstr)
{
	// todo: deprecated and leaks memory on windows
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	return converter.to_bytes(wstr);
}
} // namespace StringUtils