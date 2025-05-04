#pragma once

#include "spdlog/spdlog.h"
#include <codecvt>
#include <filesystem>
#include <string>
#include <vector>

using log_level = spdlog::level::level_enum;

class Logger
{
	friend class LoggerRegistrator;

public:
	constexpr static std::string_view log_filename = "log.log";
	constexpr static std::string_view logs_dir = "Logs";
	constexpr static size_t max_log_num = 10;

	static void Initialize();
	static void Shutdown();

	static Logger Create(const std::string& name, log_level default_level = log_level::info);

	[[nodiscard]] const std::string& GetName() const;

private:
	Logger(log_level level, std::string name);

	log_level _level = log_level::info;
	std::string _name;
};

class LoggerRegistrator
{
public:
	static LoggerRegistrator& Get();

	void PushLogger(const Logger& logger);
	void RegisterLoggers();
	void Register(const Logger& logger);

private:
	std::vector<Logger> _loggers_to_register;
};

void Log(const Logger& log, log_level level, const std::string& message);

#define DEFINE_LOGGER(logger) inline const Logger log_##logger = Logger::Create(#logger);
