#pragma once

#include <iostream>

enum LogLevel {
	Info,
	Warning,
	Error
};

class Debug
{
private:
;
	template<typename T>
	static void logInternal(LogLevel logLevel, const char* source, const T& message)
	{
		std::cout << "[ " << source << " ] ";

		switch (logLevel) {
		case Info:
			std::cout << "INFO: ";
			break;
		case Warning:
			std::cout << "WARNING: ";
			break;
		case Error:
			std::cout << "ERROR: ";
			break;
		}
		std::cout << message << std::endl;
	}

public:
	template <typename T>
	static void log(const char* source, const T& message) {
		logInternal(LogLevel::Info, source, message);
	}

	template <typename T>
	static void logWarning(const char* source, const T& message) {
		logInternal(LogLevel::Warning, source, message);
	}

	template <typename T>
	static void logError(const char* source, const T& message) {
		logInternal(LogLevel::Error, source, message);
	}
};