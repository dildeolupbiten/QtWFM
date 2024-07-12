#include "../include/activities.hpp"

Activities::Activities(
    std::function<void()> changeValues,
    str title,
    vec<str> columns,
    QWidget *parent
) : changeValues(changeValues), Table(title, columns, parent) {}

Activities::~Activities() {}

void Activities::init()
{
    int row = table -> rowCount();
    for (size_t col = 0; col < columns.size(); col++)
    {
        addCellWidget<double>(0, 24, .25, row, col, 2);
        connect(
            widgets[row][columns[col]],
            &QDoubleSpinBox::valueChanged,
            [this](){ changeValues(); }
        );
    }
}


void Activities::destroy()
{
    for (int row = 0; row < table -> rowCount(); row++)
    {
        for (size_t col = 0; col < columns.size(); col++)
        {
            table -> removeCellWidget(row, col);
        }
        table -> removeRow(row);
    }
    widgets = {};
    table -> setRowCount(0);
}
