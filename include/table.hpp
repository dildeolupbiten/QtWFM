#ifndef TABLE_HPP
#define TABLE_HPP

#include "consts.hpp"

class Label : public QLabel {
public:
    Label(QWidget *parent = nullptr) {};
    ~Label() {};
    double value() {
        return text().toDouble();
    }
};

class Table : public QFrame {
public:
    Table(str title, vec<str> columns, QWidget *parent = nullptr);
    ~Table();
    void config();
    void configTable();
    void removeItem();
    void reconfig();
    template <typename T>
    void addCellWidget(T min, T max, T step, int row, int col, int decimal) {
        table -> setRowCount(row + 1);
        QDoubleSpinBox *widget = new QDoubleSpinBox();
        widget -> setMinimum(min);
        widget -> setMaximum(max);
        widget -> setSingleStep(step);
        widget -> setDecimals(decimal);
        table -> setCellWidget(row, col, widget);
        table -> setRowHeight(row, 25);
        widgets[row][columns[col]] = widget;
    }
    QLabel *label;
    QFrame *frame;
    QTableWidget *table;
    QPushButton *add;
    QPushButton *remove;
    vec<str> columns;
    str title;
    virtual void init();
    map<str, QComboBox*> comboboxes;
    map<int, map<str, QDoubleSpinBox*>> widgets;
    map<int, map<str, Label*>> labels;
    template <typename T>
    vec<map<str, double>> values(map<int, map<str, T*>> container) {
        vec<map<str, double>> v;
        for (int row = 0; row < table -> rowCount(); row++) {
            map<str, double> m;
            for (int col = 0; col < table -> columnCount(); col++) {
                m[columns[col]] = container[row][columns[col]] -> value();
            }
            v.push_back(m);
        }
        return v;
    }
};

#endif // TABLE_HPP
