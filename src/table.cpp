#include "../include/table.hpp"

Table::Table(str title, vec<str> columns, QWidget *parent)
    : title(title), columns(columns), QFrame(parent)
{
    label = new QLabel(QString::fromStdString(title), this);
    frame = new QFrame(this);
    table = new QTableWidget(this);
    add = new QPushButton("Add", frame);
    remove = new QPushButton("Remove", frame);
    config();
}

Table::~Table() {}

void Table::config()
{
    setLayout(new QVBoxLayout(this));
    layout() -> addWidget(label);
    layout() -> addWidget(frame);
    layout() -> addWidget(table);
    layout() -> setAlignment(label, Qt::AlignCenter);
    layout() -> setAlignment(frame, Qt::AlignCenter);
    layout() -> setAlignment(table, Qt::AlignCenter);
    frame -> setLayout(new QHBoxLayout(frame));
    frame -> layout() -> addWidget(add);
    frame -> layout() -> addWidget(remove);
    frame -> layout() -> setAlignment(add, Qt::AlignCenter);
    frame -> layout() -> setAlignment(remove, Qt::AlignCenter);
    if (title != "") {
        table -> setFixedHeight(175);
    }
    add -> setFixedWidth(100);
    remove -> setFixedWidth(100);
    connect(add, &QPushButton::clicked, [this](){ init(); });
    connect(remove, &QPushButton::clicked, [this](){ removeItem(); });
    configTable();
    if (title == "") {
        reconfig();
    }
}

void Table::configTable()
{
    size_t size = columns.size();
    table -> setColumnCount(size);
    for (size_t i = 0; i < size; i++) 
    {
        QTableWidgetItem *item = new QTableWidgetItem(
            QString::fromStdString(columns[i])
        );
        table -> setHorizontalHeaderItem(i, item);
        table -> setColumnWidth(i, 100);
    }
    table -> setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
}

void Table::removeItem()
{
    vec<int> rows;
    for (
        const QModelIndex& i :
        table -> selectionModel() -> selectedRows()
    )
    {
        rows.push_back(i.row());
        table -> removeRow(i.row());
    }
    std::sort(rows.rbegin(), rows.rend());
    for (int& row : rows) {
        table -> removeRow(row);
    }
}

void Table::reconfig()
{
    frame -> layout() -> removeWidget(add);
    frame -> layout() -> removeWidget(remove);
    layout() -> removeWidget(label);
    layout() -> removeWidget(frame);
    delete add;
    delete remove;
    delete label;
    delete frame;
}

void Table::init() {
    table -> verticalHeader() -> setVisible(0);
    table -> setRowCount(1);
    table -> setRowHeight(0, 25);
    for (size_t col = 0; col < columns.size(); col++) {
        Label *widget = new Label();
        widget -> setText(QString::fromStdString("0"));
        widget -> setAlignment(Qt::AlignCenter);
        widget -> setObjectName("table");
        table -> setCellWidget(0, col, widget);
        labels[0][columns[col]] = widget;
    }
};
