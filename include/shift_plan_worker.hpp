#ifndef SHIFT_PLAN_WORKER_HPP
#define SHIFT_PLAN_WORKER_HPP

#include "shift_plan.hpp"

class ShiftPlanWorker : public QThread 
{
    Q_OBJECT
public:
    ShiftPlanWorker(
        vec<str> shifts, 
        int rows, 
        int cols, 
        int off, 
        double work_hour
    );
    ShiftPlan *shift_plan;
signals:
    void finished(ShiftPlan* plan);
    void progress(int value);
protected:
    void run() override;
};

#endif // SHIFT_PLAN_WORKER_HPP
