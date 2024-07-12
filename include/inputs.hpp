#ifndef INPUTS_HPP
#define INPUTS_HPP

#include "table.hpp"

class Inputs : public Table {
public:
    Inputs(
        std::function<void()> changeValues,
        str title,
        vec<str> columns,
        QWidget *parent = nullptr
    );
    ~Inputs();
    void init() override;
    std::function<void()> changeValues;
};

#endif // INPUTS_HPP
