#include "../include/shifts.hpp"

Shifts::Shifts(QWidget *parent) : QFrame(parent) {
    f_n_shift = new QFrame(this);
    f_shift = new QFrame(this);
    f_days = new QFrame(this);
    n_shift = new QComboBox(f_n_shift);
    shifts = new QComboBox(f_shift);
    days = new QComboBox(f_days);
    config();
}

Shifts::~Shifts() {}

void Shifts::config()
{
    setLayout(new QHBoxLayout(this));
    layout() -> addWidget(f_n_shift);
    layout() -> addWidget(f_shift);
    layout() -> addWidget(f_days);
    layout() -> setAlignment(f_n_shift, Qt::AlignCenter);
    layout() -> setAlignment(f_shift, Qt::AlignCenter);
    layout() -> setAlignment(f_days, Qt::AlignCenter);
    f_n_shift -> setLayout(new QVBoxLayout(f_n_shift));
    QLabel *label_n_shift = new QLabel("N-Shift" ,f_n_shift);
    f_n_shift -> layout() -> addWidget(label_n_shift);
    f_n_shift -> layout() -> addWidget(n_shift);
    f_n_shift -> layout() -> setAlignment(label_n_shift, Qt::AlignCenter);
    f_n_shift -> layout() -> setAlignment(n_shift, Qt::AlignCenter);
    f_shift -> setLayout(new QVBoxLayout(f_shift));
    QLabel *label_shifts = new QLabel("Shifts" ,f_shift);
    f_shift -> layout() -> addWidget(label_shifts);
    f_shift -> layout() -> addWidget(shifts);
    f_shift -> layout() -> setAlignment(label_shifts, Qt::AlignCenter);
    f_shift -> layout() -> setAlignment(shifts, Qt::AlignCenter);
    f_days -> setLayout(new QVBoxLayout(f_days));
    QLabel *label_days = new QLabel("Days" ,f_days);
    f_days -> layout() -> addWidget(label_days);
    f_days -> layout() -> addWidget(days);
    f_days -> layout() -> setAlignment(label_days, Qt::AlignCenter);
    f_days -> layout() -> setAlignment(days, Qt::AlignCenter);
}
