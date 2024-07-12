#ifndef DATETIME_HPP
#define DATETIME_HPP

#include "consts.hpp"

#include <ctime>

class Datetime {
public:
    Datetime(
        int year,
        int month,
        int day,
        int hour,
        int minute,
        int second
    );
    ~Datetime();
    str string();
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
    std::tm tm;
    str add(int minutes);
};

#endif // DATETIME_HPP

