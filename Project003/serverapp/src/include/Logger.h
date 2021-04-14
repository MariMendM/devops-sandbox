
#pragma once

#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/dll/runtime_symbol_info.hpp>

#include "DateTimeUtils.h"


class Logger {
	private:
		static Logger *instance;
		static std::ofstream logFileStream;
		
		Logger();
		~Logger();

	public:

		class message_types {
			public:
				enum class type {INFO, ERROR, WARNING, DEBUG};
				static std::string to_string(type t);
		};

		static Logger *getLogger(std::string logFilenameInput = "default.log");
		void log(std::string message, message_types::type type = message_types::type::INFO);		
};

