#ifndef ACTIVITIES_HPP
#define ACTIVITIES_HPP

#include "table.hpp"
#include <functional>

class Activities : public Table {
public:
    Activities(
        std::function<void()> changeValues,
        str title,
        vec<str> columns,
        QWidget *parent = nullptr
    );
    ~Activities();
    void init() override;
    std::function<void()> changeValues;
    void destroy();
};

#endif // ACTIVITIES_HPP
