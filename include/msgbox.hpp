#ifndef MSGBOX_HPP
#define MSGBOX_HPP

#include "lib.hpp"

class MsgBox : public QMessageBox {
public:
    MsgBox(QWidget *parent = nullptr);
    ~MsgBox();
};

#endif // MSGBOX_HPP
