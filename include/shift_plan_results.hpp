#ifndef SHIFT_PLAN_RESULTS_HPP
#define SHIFT_PLAN_RESULTS_HPP

#include "shift_plan.hpp"

class ShiftPlanResults : public QFrame {
public:
    ShiftPlanResults(QWidget *parent = nullptr);
    ~ShiftPlanResults();
    QTableWidget *shift_plan;
    QTableWidget *hc;
    QFrame *fill;
    void config();
    void init(ShiftPlan* plan);
    void init_shift_plan(ShiftPlan* plan);
    void init_hc(ShiftPlan* plan);
    ShiftPlan *results;
    int done;
    vec<map<str, double>> activities_values;
};

#endif // SHIFT_PLAN_RESULTS_HPP

