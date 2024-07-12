#ifndef ANALYSIS_WORKER_HPP
#define ANALYSIS_WORKER_HPP

#include "activities.hpp"
#include "shifts.hpp"
#include "trend.hpp"
#include "inputs.hpp"

class AnalysisWorker : public QThread {
    Q_OBJECT
public:
    AnalysisWorker(
        Activities *activities,
        Shifts* shifts,
        Trend* trend, 
        QProgressBar* pbar, 
        vec<double> need,
        Inputs *inputs,
        Table *values
    );
signals:
    void finished(map<int, vec<ShiftValues>> output);
    void progress(int value);
protected:
    void run() override;
private:
    Activities* activities;
    Shifts* shifts;
    Trend* trend;
    vec<double> need;
    QProgressBar* pbar;
    Inputs *inputs;
    Table *values;
};

#endif // ANALYSIS_WORKER_HPP
