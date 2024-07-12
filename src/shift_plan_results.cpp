#include "../include/shift_plan_results.hpp"

ShiftPlanResults::ShiftPlanResults(QWidget *parent): QFrame(parent)
{
    shift_plan = new QTableWidget(this);
    hc = new QTableWidget(this);
    fill = new QFrame(this);
    done = 0;
    config();
}

ShiftPlanResults::~ShiftPlanResults() {};

void ShiftPlanResults::config()
{
    setLayout(new QVBoxLayout(this));
    layout() -> addWidget(shift_plan);
    layout() -> addWidget(fill);
    layout() -> addWidget(hc);
    layout() -> setAlignment(shift_plan, Qt::AlignCenter);
    layout() -> setAlignment(fill, Qt::AlignCenter);
    layout() -> setAlignment(hc, Qt::AlignCenter);
    fill -> setFixedHeight(10);
    connect(
        shift_plan -> horizontalScrollBar(),
        &QScrollBar::valueChanged,
        [this](int index){
            hc -> horizontalScrollBar() -> setValue(index);
        }

    );
    connect(
        hc -> horizontalScrollBar(),
        &QScrollBar::valueChanged,
        [this](int index){
            shift_plan -> horizontalScrollBar() -> setValue(index);
        }
    );
}

void ShiftPlanResults::init_shift_plan(ShiftPlan* plan)
{
    results = plan;
    shift_plan -> setRowCount(plan -> rows);
    shift_plan -> setColumnCount(plan -> cols);
    for (int row = 0; row < plan -> rows; row++)
    {
        for (int col = 0; col < plan -> cols; col++)
        {
            QLabel *label = new QLabel(
                QString::fromStdString(plan -> data[row][col])
            );
            label -> setAlignment(Qt::AlignCenter);
            str bg = COLORS[plan -> data[row][col]]["background"];
            str fg = COLORS[plan -> data[row][col]]["foreground"];
            str style = "background: " + bg + ";color: " + fg + ";";
            label -> setStyleSheet(QString::fromStdString(style));
            shift_plan -> setCellWidget(row, col, label);
            shift_plan -> setColumnWidth(col, 80);
            if (!row)
            {
                QTableWidgetItem *item = new QTableWidgetItem(
                    QString::fromStdString(std::to_string(col))
                );
                item -> setToolTip(
                    QString(
                        "Click on the column headers to view daily results."
                    )
                );
                shift_plan -> setHorizontalHeaderItem(col, item);
            }
        }
        shift_plan -> setRowHeight(row, 25);
    }
    shift_plan -> setFixedHeight(shift_plan -> rowHeight(0) * 10 + 50);
    shift_plan -> setFixedWidth(shift_plan -> columnWidth(0) * 7 + 50);
}

void ShiftPlanResults::init_hc(ShiftPlan* plan)
{
    hc -> setRowCount(plan -> size);
    hc -> setColumnCount(plan -> cols);
    int height = 0;
    for (int row = 0; row < plan -> size; row++)
    {
        for (int col = 0; col < plan -> cols; col++)
        {
            QLabel *label = new QLabel(
                QString::fromStdString(
                    std::to_string(plan -> sum()[plan -> shifts[row]][col])
                )
            );
            label -> setAlignment(Qt::AlignCenter);
            str s = "background: #777777;";
            label -> setStyleSheet(QString::fromStdString(s));
            hc -> setCellWidget(row, col, label);
            hc -> setColumnWidth(col, 80);
            QTableWidgetItem *item = new QTableWidgetItem(
                QString::fromStdString(plan -> shifts[row])
            );
            str bg = COLORS[plan -> shifts[row]]["background"];
            str fg = COLORS[plan -> shifts[row]]["foreground"];
            item -> setBackground(QBrush(QColor(QString::fromStdString(bg))));
            item -> setForeground(QBrush(QColor(QString::fromStdString(fg))));
            hc -> setVerticalHeaderItem(row, item);
            if (!row)
            {
                item = new QTableWidgetItem(
                    QString::fromStdString(std::to_string(col))
                );
                item -> setToolTip(
                    QString(
                        "Click on the column headers to view daily results."
                    )
                );
                hc -> setHorizontalHeaderItem(col, item);
            }
        }
        hc -> setRowHeight(row, 25);
        height += hc -> rowHeight(row);
    }
    hc -> setFixedHeight(height + 50);
    hc -> setFixedWidth(hc -> columnWidth(0) * 7 + 50);
}

void ShiftPlanResults::init(ShiftPlan* plan)
{
    init_shift_plan(plan);
    init_hc(plan);
}

