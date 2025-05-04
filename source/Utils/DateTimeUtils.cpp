#include "DateTimeUtils.h"
#include <ctime>
#include <iomanip>
#include <sstream>

namespace DateTimeUtils
{
std::string GetCurrentDateTime()
{
	auto now = std::time(nullptr);
	std::tm tm = {};
#ifdef _WIN32
	localtime_s(&tm, &now);
#else
	localtime_r(&now, &tm);
#endif
	std::ostringstream oss;
	oss << std::put_time(&tm, "%Y-%m-%d_%H-%M-%S");
	return oss.str();
}
} // namespace DateTimeUtils