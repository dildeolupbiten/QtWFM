#ifndef WIDGET_HPP
#define WIDGET_HPP

#include "analysis.hpp"

class Widget : public QWidget {
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    Analysis *analysis;
    void config();
};

#endif // WIDGET_HPP
