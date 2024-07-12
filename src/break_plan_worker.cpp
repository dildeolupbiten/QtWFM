#include "../include/break_plan_worker.hpp"

BreakPlanWorker::BreakPlanWorker(
    vec<str> shifts,
    vec<vec<int>> hc,
    vec<map<str, double>> activities_values,
    vec<double> need,
    double work_hour
)
{
    break_plan = new BreakPlan(shifts, hc, activities_values, need, work_hour);
}

void BreakPlanWorker::run()
{
    break_plan -> createBreakPlan();
    emit finished(break_plan);
}
