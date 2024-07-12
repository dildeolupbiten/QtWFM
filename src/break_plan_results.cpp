#include "../include/break_plan_results.hpp"
#include "../include/utils.hpp"

BreakPlanResults::BreakPlanResults(QWidget *parent): QFrame(parent)
{
    break_plan = new QTableWidget(this);
    config();
}

BreakPlanResults::~BreakPlanResults() {};

void BreakPlanResults::config()
{
    setLayout(new QVBoxLayout(this));
    layout() -> addWidget(break_plan);
    layout() -> setAlignment(break_plan, Qt::AlignCenter);
}

void BreakPlanResults::init_break_plan(BreakPlan* plan, int col)
{
    results = plan;
    int rows = plan -> data[col].size();
    int cols = plan -> data[col][0].size() - 1;
    break_plan -> setRowCount(rows);
    break_plan -> setColumnCount(cols);
    for (int row = 0; row < rows; row++)
    {
        for (int c = 0; c < cols; c++)
        {
            str s;
            str text = plan -> data[col][row][c + 1];
            vec<str> splitted_date = split(text, " ");
            vec<str> splitted_time = split(splitted_date[1], ":");
            text = splitted_time[0] + ":" + splitted_time[1];
            s = "background: #00cc00;";
            if (!c)
            {
                QTableWidgetItem *item = new QTableWidgetItem(
                    QString::fromStdString(plan -> data[col][row][0])
                );
                str bg = COLORS[plan -> data[col][row][0]]["background"];
                str fg = COLORS[plan -> data[col][row][0]]["foreground"];
                item -> setBackground(
                    QBrush(QColor(QString::fromStdString(bg)))
                );
                item -> setForeground(
                    QBrush(QColor(QString::fromStdString(fg)))
                );
                break_plan -> setVerticalHeaderItem(row, item);
            }
            QLabel *label = new QLabel(QString::fromStdString(text));
            label -> setAlignment(Qt::AlignCenter);
            label -> setStyleSheet(QString::fromStdString(s));
            break_plan -> setCellWidget(row, c, label);
            break_plan -> setColumnWidth(c, 80);

        }
        break_plan -> setRowHeight(row, 25);
    }
    break_plan -> setFixedHeight(break_plan -> rowHeight(0) * 10 + 50);
    break_plan -> setFixedWidth(break_plan -> columnWidth(0) * cols + 50);
}

void BreakPlanResults::init(BreakPlan* plan)
{
    init_break_plan(plan, 0);
}

