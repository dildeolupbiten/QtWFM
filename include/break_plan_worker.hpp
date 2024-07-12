#ifndef BREAK_PLAN_WORKER_HPP
#define BREAK_PLAN_WORKER_HPP

#include "break_plan.hpp"

class BreakPlanWorker : public QThread {
    Q_OBJECT
public:
    BreakPlanWorker(
        vec<str> shifts,
        vec<vec<int>> hc,
        vec<map<str, double>> activities_values,
        vec<double> need,
        double work_hour
    );
    BreakPlan *break_plan;

signals:
    void finished(BreakPlan *plan);
    void progress(int value);

protected:
    void run() override;
};

#endif // BREAK_PLAN_WORKER_HPP
