#ifndef TREND_HPP
#define TREND_HPP

#include "table.hpp"

class Trend : public Table {
public:
    Trend(
        std::function<void()> changeValues,
        str title,
        vec<str> columns,
        QWidget *parent = nullptr
    );
    ~Trend();
    void init() override;
    std::function<void()> changeValues;
};

#endif // TREND_HPP
