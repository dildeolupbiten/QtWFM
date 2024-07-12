#ifndef BREAK_PLAN_RESULTS_HPP
#define BREAK_PLAN_RESULTS_HPP

#include "break_plan.hpp"

class BreakPlanResults : public QFrame {
public:
    BreakPlanResults(QWidget *parent = nullptr);
    ~BreakPlanResults();
    QTableWidget *break_plan;
    void config();
    BreakPlan *results;
    void init_break_plan(BreakPlan* plan, int col);
    void init(BreakPlan *plan);
    int done;
};

#endif // BREAK_PLAN_RESULTS_HPP

