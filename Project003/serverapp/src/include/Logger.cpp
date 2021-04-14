
#include "Logger.h"


Logger *Logger::instance = nullptr;
std::ofstream Logger::logFileStream;

Logger::Logger() { }

Logger::~Logger() {
	if (instance != nullptr) {
		logFileStream << "Closing..." << std::endl;
		logFileStream.close();
    }
}

Logger *Logger::getLogger(std::string logFilenameInput) {
	if (instance == nullptr) {
		instance = new Logger();

		//Handling log folder (/log inside binary location)
			boost::filesystem::path binFolder = boost::dll::program_location().parent_path();
			std::string logFolder = binFolder.string() + "/log/";
			if(!boost::filesystem::exists(logFolder)) {
				if (!boost::filesystem::create_directory(logFolder))
					logFolder = "/tmp/";
			}

		//Handle filename (it is optional, so it has to be handled in case it was passed by argument)
			//It assumes that "log/" folder exists (creation not handled, it will not log anything in case it doesn't exist)
			std::string logFullname;
			if (logFilenameInput == "default.log")
				logFullname = logFolder + DateTimeUtils::currentDate()+".log";
			else {
				std::size_t lastSlashPos = logFilenameInput.find_last_of("/");
				bool slashFound = (lastSlashPos != std::string::npos);
				bool slashFoundAtEnd = (lastSlashPos == logFilenameInput.size()-1);
				
				if (slashFound and !slashFoundAtEnd)				logFullname = logFolder + logFilenameInput.substr(lastSlashPos+1);
				else if (slashFound and slashFoundAtEnd)			logFullname = logFolder + DateTimeUtils::currentDate()+".log";
				else if (!slashFound and !logFilenameInput.empty())	logFullname = logFolder + logFilenameInput;
				else												logFullname = logFolder + DateTimeUtils::currentDate()+".log";
			}

		std::cout << ">> Logging at <" << logFullname << ">\n";
		logFileStream.open(logFullname, std::fstream::app);
		logFileStream << std::endl << std::endl;
	}
	return instance;
}

void Logger::log(std::string message, message_types::type type) {
	std::string strMessageType;
	logFileStream << DateTimeUtils::currentTime() << ": [" << message_types::to_string(type) << "]: " << message << std::endl;
}

std::string Logger::message_types::to_string(type t) {
	switch (t) {
		case type::INFO:		return "INFO "; break;
		case type::ERROR:		return "ERROR"; break;
		case type::WARNING:		return "WARN "; break;
		case type::DEBUG:		return "DEBUG";
	}
	return "DEBUG";
}