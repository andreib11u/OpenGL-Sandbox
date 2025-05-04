#include "Log.h"

#include "Utils/DateTimeUtils.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include <iostream>
#include <utility>

void Logger::Initialize()
{
	namespace fs = std::filesystem;

	// get all log files
	std::vector<fs::path> logs;
	for (const auto& log_file : fs::directory_iterator(logs_dir))
	{
		if (log_file.is_regular_file() && log_file.path().extension() == ".log")
		{
			logs.push_back(log_file.path());
		}
	}

	// sort log files by last write time (oldest first)
	std::sort(logs.begin(), logs.end(),
			  [](const fs::path& a, const fs::path& b) { return fs::last_write_time(a) < fs::last_write_time(b); });

	// Rename the last log file (if it exists)
	if (!logs.empty())
	{
		const fs::path& last_log = logs.back();
		std::string new_name = "log_" + DateTimeUtils::GetCurrentDateTime() + ".log";
		fs::path new_path = last_log.parent_path() / new_name;
		std::error_code err_code;

		auto sourceStatus = fs::status(last_log);
		auto destStatus = fs::status(new_path.parent_path());

		if ((sourceStatus.permissions() & fs::perms::owner_read) == fs::perms::none)
		{
			std::cerr << "No read permissions for source file: " << last_log << std::endl;
			return;
		}

		if ((destStatus.permissions() & fs::perms::owner_write) == fs::perms::none)
		{
			std::cerr << "No write permissions for destination directory: " << new_path.parent_path() << std::endl;
			return;
		}

		fs::rename(last_log, new_path, err_code);
	}

	// Remove old log files if there are more than 'max_log_num'
	if (logs.size() > max_log_num)
	{
		for (size_t i = 0; i < logs.size() - max_log_num; ++i)
		{
			fs::remove(logs[i]);
		}
	}

	// register loggers
	auto& logger_registrator = LoggerRegistrator::Get();
	logger_registrator.RegisterLoggers();
}

void Logger::Shutdown()
{
	spdlog::shutdown();
}

Logger Logger::Create(const std::string& name, log_level default_level)
{
	auto logger = Logger{default_level, name};
	auto& logger_registrator = LoggerRegistrator::Get();
	logger_registrator.PushLogger(logger);
	return logger;
}

const std::string& Logger::GetName() const
{
	return _name;
}

Logger::Logger(log_level level, std::string name)
	: _level{level}
	, _name{std::move(name)}
{
}

void LoggerRegistrator::Register(const Logger& logger)
{
	auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
	const auto& log_dir = fmt::format("{}\\{}", Logger::logs_dir, Logger::log_filename);
	auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(log_dir);

	std::vector<spdlog::sink_ptr> sinks;
	sinks.push_back(console_sink);
	sinks.push_back(file_sink);

	auto multi_sink_logger = std::make_shared<spdlog::logger>(logger._name, begin(sinks), end(sinks));
	multi_sink_logger->set_level(logger._level);
	spdlog::register_logger(multi_sink_logger);
}

void Log(const Logger& log, log_level level, const std::string& message)
{
	auto logger = spdlog::get(log.GetName());
	logger->log(level, message);
}

void LoggerRegistrator::PushLogger(const Logger& logger)
{
	_loggers_to_register.push_back(logger);
}

void LoggerRegistrator::RegisterLoggers()
{
	for (const auto& logger : _loggers_to_register)
	{
		Register(logger);
	}
}

LoggerRegistrator& LoggerRegistrator::Get()
{
	static LoggerRegistrator registrator;
	return registrator;
}
