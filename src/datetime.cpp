#include "../include/datetime.hpp"

#include <iomanip>
#include <sstream>
#include <chrono>

Datetime::Datetime(
    int year,
    int month,
    int day,
    int hour,
    int minute,
    int second
) : year(year), 
    month(month), 
    day(day), 
    hour(hour), 
    minute(minute), 
    second(second)
{
    tm = {};
    tm.tm_year = year - 1900;
    tm.tm_mon = month - 1; 
    tm.tm_mday = day;
    tm.tm_hour = hour;
    tm.tm_min = minute;
    tm.tm_sec = second;   
}

str Datetime::add(int minutes)
{
    std::time_t mktime_before = std::mktime(&tm);
    std::chrono::system_clock::time_point time_point = \
        std::chrono::system_clock::from_time_t(mktime_before);
    time_point += std::chrono::minutes(minutes);
    std::time_t mktime_after = \
        std::chrono::system_clock::to_time_t(time_point);
    std::tm *new_tm = std::localtime(&mktime_after);
    std::ostringstream oss;
    oss << std::put_time(new_tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

Datetime::~Datetime() {}

str Datetime::string()
{    
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

