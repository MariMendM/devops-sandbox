
#include "DateTimeUtils.h"


std::string DateTimeUtils::currentDateTime() {
	return currentDate() + " " + currentTime();
}

std::string DateTimeUtils::currentDate() {
	boost::posix_time::ptime currLocTime = boost::posix_time::second_clock::local_time();

	std::ostringstream dateStream;
	dateStream << currLocTime.date().year() << "." << currLocTime.date().month() << "." << currLocTime.date().day();

	return dateStream.str();
}

std::string DateTimeUtils::currentTime() {
	boost::posix_time::ptime currLocTime = boost::posix_time::second_clock::local_time();
	
	std::ostringstream timeStream;
	timeStream << std::setfill('0') \
		<< std::setw(2) << currLocTime.time_of_day().hours() << "." \
		<< std::setw(2) << currLocTime.time_of_day().minutes() << "." \
		<< std::setw(2) << currLocTime.time_of_day().seconds();

	return timeStream.str();
}