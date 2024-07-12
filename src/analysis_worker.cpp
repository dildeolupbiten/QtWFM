#include "../include/analysis_worker.hpp"
#include "../include/utils.hpp"

AnalysisWorker::AnalysisWorker(
    Activities *activities,
    Shifts* shifts,
    Trend* trend,
    QProgressBar* pbar,
    vec<double> need,
    Inputs *inputs,
    Table *values
) : activities(activities), shifts(shifts), \
    trend(trend), pbar(pbar), need(need), inputs(inputs), values(values) {}

void AnalysisWorker::run() {
    disconnect(
        shifts -> n_shift, 
        &QComboBox::currentTextChanged, 
        nullptr, 
        nullptr
    );
    disconnect(
        shifts -> shifts, 
        &QComboBox::currentTextChanged, 
        nullptr, 
        nullptr
    );
    shifts -> shifts -> clear();
    shifts -> n_shift -> clear();
    vec<int> hours = {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
        13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23
    };
    vec<map<str, double>> activities_values = \
        activities -> values(activities -> widgets);
    map<int, vec<ShiftValues>> output = {};
    vec<double> actual;
    vec<double> trends(24);
    double off = inputs -> widgets[0]["Off Day"] -> value();
    map<str, vec<double>> coverage;
    str shift;
    int index = 0;
    for (int i = 0; i < 24; i++)
    {
        trends[i] = trend -> widgets[i]["Trend"] -> value();
    }
    int total1 = combNumber(24, 3);
    int total2 = combNumber(24, 4);
    int total3 = combNumber(24, 5);
    int total = total1 + total2 + total3;
    pbar -> setMaximum(total);
    for (int r = 3; r < 6; r++)
    {
        output[r] = vec<ShiftValues>{};
        for (auto& comb : combinations(hours, r))
        {
            int n = comb.size();
            vec<double> hcs(n);
            for (size_t i = 0; i < n; i++)
            {
                hcs[i] = (
                    (double)(1 - off / (double)7) 
                    * 
                    (values -> labels[0]["Headcount"] -> value() / (double)(n))
                );
            }
            actual = getHeadcount(
                comb,
                hcs,
                activities_values,
                inputs -> widgets[0]["Work Hour"] -> value()
            );
            coverage = hasCoverage(need, actual, trends);
            if (coverage["coverage"].size())
            {
                shift = "";
                for (int i = 0; i < comb.size(); i++)
                {
                    shift += std::to_string(comb[i]) + " ";
                }
                output[r].push_back(
                    ShiftValues{
                        shift,
                        actual,
                        coverage["coverage"],
                        coverage["occupancy"],
                        coverage["waiting_time"]
                    }
                );
            }
            emit progress(index + 1);
            index++;
        }
        if (!output[r].size())
        {
            output.erase(r);
        }
    }
    emit finished(output);
}
