#include "../include/inputs.hpp"

Inputs::Inputs(
    std::function<void()> changeValues,
    str title,
    vec<str> columns,
    QWidget *parent
) : changeValues(changeValues), Table(title, columns, parent) {}

Inputs::~Inputs() {}

void Inputs::init()
{
    table -> verticalHeader() -> setVisible(0);
    addCellWidget<int>(0, 1000000, 1, 0, 0, 0);
    addCellWidget<double>(0, 864000, .01, 0, 1, 2);
    addCellWidget<double>(0, 1, .01, 0, 2, 2);
    addCellWidget<double>(0, 24, .25, 0, 3, 2);
    addCellWidget<int>(0, 7, 1, 0, 4, 0);
    for (str col : columns)
    {
        connect(
            widgets[0][col],
            &QDoubleSpinBox::valueChanged,
            [this](){ changeValues(); }
        );
    }
}
