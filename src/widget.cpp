#include "../include/widget.hpp"

Widget::Widget(QWidget *parent): QWidget(parent) 
{
    analysis = new Analysis();
    config();
}

Widget::~Widget() {}

void Widget::config()
{
    setLayout(new QVBoxLayout(this));
    layout() -> addWidget(analysis);
}
