#include "../include/analysis_results.hpp"

AnalysisResults::AnalysisResults(
    str title,
    vec<str> columns,
    QWidget *parent
): Table(title, columns, parent) {}

AnalysisResults::~AnalysisResults() {}

void AnalysisResults::init()
{
    table -> setRowCount(24);
    int height = 0;
    for (int row = 0; row < 24; row++) {
        for (int col = 0; col < columns.size(); col++) {
            Label *widget = new Label();
            widget -> setText(QString::fromStdString("0"));
            widget -> setAlignment(Qt::AlignCenter);
            widget -> setObjectName("table");
            table -> setCellWidget(row, col, widget);
            labels[row][columns[col]] = widget;
            str r = std::to_string(row);
            str s = row > 9 ? r : "0" + r;
            s += ":00";
            QTableWidgetItem *item = new QTableWidgetItem(
                QString::fromStdString(s)
            );
            table -> setVerticalHeaderItem(row, item);
            table -> setColumnWidth(col, 85);

        }
        table -> setRowHeight(row, 25);
        height += table -> rowHeight(row);
    }
    height += table -> horizontalHeader() -> height();
    height += table -> horizontalScrollBar() -> height();
    table -> setFixedHeight(height);
}
