
#pragma once

#include <string>
#include <iomanip>
#include <boost/date_time.hpp>


class DateTimeUtils {
	public:
		static std::string currentTime();
		static std::string currentDate();
		static std::string currentDateTime();
};