#ifndef RESULTS_HPP
#define RESULTS_HPP

#include "analysis_results.hpp"
#include "shift_plan_results.hpp"
#include "break_plan_results.hpp"

class Results : public QFrame {
public:
    Results(QWidget *parent = nullptr);
    ~Results();
    QScrollArea *area;
    QFrame *frame;
    AnalysisResults *analysis_results;
    ShiftPlanResults *shift_plan_results;
    BreakPlanResults *break_plan_results;
    QLabel *label;
    void config();
    vec<double> need;
    vec<double> trends;
    void changeValues(int index);
};

#endif // RESULTS_HPP

