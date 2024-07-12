#ifndef SHIFTS_HPP
#define SHIFTS_HPP

#include "consts.hpp"

class Shifts : public QFrame {
public:
    Shifts(QWidget *parent = nullptr);
    ~Shifts();
    QFrame *f_n_shift;
    QFrame *f_shift;
    QFrame *f_days;
    QComboBox *n_shift;
    QComboBox *shifts;
    QComboBox *days;
    void config();
    map<str, vec<str>> values;
};

#endif // SHIFTS_HPP
