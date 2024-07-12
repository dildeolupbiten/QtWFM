#include "../include/trend.hpp"

Trend::Trend(
    std::function<void()> changeValues,
    str title,
    vec<str> columns,
    QWidget *parent
    ): changeValues(changeValues), Table(title, columns, parent) {}

Trend::~Trend() {}

void Trend::init()
{
    table -> setRowCount(24);
    for (int row = 0; row < 24; row++) {
        addCellWidget<double>(0, 1, .000001, row, 0, 6);
        str r = std::to_string(row);
        str s = row > 9 ? r : "0" + r;
        s += ":00";
        QTableWidgetItem *item = new QTableWidgetItem(
            QString::fromStdString(s)
        );
        table -> setVerticalHeaderItem(row, item);
        widgets[row][columns[0]] -> setValue(1 / (double)24);
        connect(
            widgets[row][columns[0]],
            &QDoubleSpinBox::valueChanged,
            [this](){ changeValues(); }
        );
    }
    changeValues();
}
