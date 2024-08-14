// Kent
#pragma once

#include <sstream>
#include <iostream>
#include <functional>

enum LogLevel {
	Info,
	Warning,
	Error
};

class Debug
{

private:
	using Callback = std::function<void(LogLevel,const std::string&)>;
	static Callback logCallback;
;
	template<typename T>
	static void logInternal(LogLevel logLevel, const char* source, const T& message)
	{
		std::ostringstream oss;
		oss << "[ " << source << " ] ";

		switch (logLevel) {
		case Info:
			oss << "INFO: ";
			break;
		case Warning:
			oss << "WARNING: ";
			break;
		case Error:
			oss << "ERROR: ";
			break;
		}
		oss << message << std::endl;
		
		std::cout << oss.str();

		if (logCallback) {
			logCallback(logLevel, oss.str());
		}
		else {
			std::cout << "[ Debug ] Warning: No callback set" << std::endl;
		}
	}

public:
	// the following three functions will print out a formatted message to the console
	// the format looks like this:
	// [ source ] LOGLEVEL: message
	// eg: [ Camera ] INFO: xPos - 10
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

	static void setCallback(Callback callback) {
		logCallback = callback;
	}

};

