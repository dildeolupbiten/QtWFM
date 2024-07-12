#include "../include/shift_plan_worker.hpp"


ShiftPlanWorker::ShiftPlanWorker(
    vec<str> shifts, 
    int rows, 
    int cols, 
    int off, 
    double work_hour
)
{
    shift_plan = new ShiftPlan(shifts, rows, cols, off, work_hour);
}

void ShiftPlanWorker::run() {
    shift_plan -> createShiftPlan();
    emit finished(shift_plan);
}
