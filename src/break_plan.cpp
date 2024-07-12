#include "../include/break_plan.hpp"
#include "../include/utils.hpp"

BreakPlan::BreakPlan(
    vec<str> shifts,
    vec<vec<int>> hc,
    vec<map<str, double>> activities_values,
    vec<double> need,
    double work_hour
) 
: shifts(shifts), 
  hc(hc), 
  activities_values(activities_values), 
  need(need),
  work_hour(work_hour),
  cols(hc[0].size())
{
    datetime = new Datetime(
        2024,
        1,
        1,
        0,
        0,
        0
    );
    getNeeds();
    getActuals();
}

BreakPlan::~BreakPlan() {}

vec<vec<double>> BreakPlan::hourlyActuals()
{
    vec<vec<double>> values;
    str previous = "";
    int i = -1;
    int j = 0;
    map<str, vec<double>> vals;
    int r = 0;
    for (auto& [key, value] : actuals)
    {
        str date = split(key, " ")[0];
        if (vals[date].empty())
        {
            vals[date] = {};
        }
        if (r % 4 == 0)
        {
            vals[date].push_back(value);
        }
        r++;
    }
    for (auto& [key, value] : vals)
    {
        values.push_back(value);
    }
    return values;
}

void BreakPlan::createBreakPlan()
{
    str date_str;
    for (int col = 0; col < cols; col++)
    {
        vec<vec<str>> users;
        for (size_t row = 0; row < shifts.size(); row++)
        {
            int shift = std::stoi(shifts[row]);
            for (int user = 0; user < hc[row][col]; user++)
            {
                vec<str> u = {shifts[row]};
                for (auto& activity : activities_values)
                {
                    double start = shift + activity["Start"];
                    double end = shift + activity["End"];
                    double duration = activity["Duration"];
                    map<double, vec<double>> alternatives;
                    for (int i = 0; i < (end - start) / duration; i++)
                    {
                        double time = start + duration * i;
                        alternatives[time] = {};
                        for (int j = 0; j < duration / .25; j++)
                        {
                            double new_time = time + j * .25;
                            date_str = datetime -> add(
                                col * 24 * 60 + 60 * new_time
                            );
                            alternatives[new_time].push_back(
                                coverages[date_str]
                            );
                        }
                    }
                    vec<double> values;
                    vec<double> keys;
                    for (auto& [key, value] : alternatives)
                    {
                        values.push_back(harmonicMean(value));
                        keys.push_back(key);
                    }
                    int best_result_index = max(values);
                    double t = keys[best_result_index];
                    date_str = datetime -> add(col * 24 * 60 + 60 * t);
                    u.push_back(date_str);
                    for (int j = 0; j < duration / .25; j++)
                    {
                        double new_time = col * 24 * 60 + 60 * (t + .25 * j);
                        date_str = datetime -> add(new_time);
                        actuals[date_str]--;
                        coverages[date_str] = \
                            actuals[date_str] / needs[date_str];
                    }
                }
                users.push_back(u);
            }
        }
        data.push_back(users);
    }
}

int BreakPlan::max(vec<double> values)
{
    double maximum = -INFINITY;
    int index = -1;
    for (size_t i = 0; i < values.size(); i++)
    {
        if (values[i] >= maximum)
        {
            maximum = values[i];
            index = i;
        }
    }
    return index;
}

void BreakPlan::getNeeds()
{
    for (int col = 0; col < cols + 1; col++)
    {
        for (int hour = 0; hour < 24; hour++)
        {
            for (int quarter = 0; quarter < 4; quarter++)
            {
                double key = col * 24 * 60 + 60 * (hour + quarter / (double)4);
                str date_str = datetime -> add(key);
                needs[date_str] = need[hour];
                actuals[date_str] = 0;
                coverages[date_str] = 0;
            }
        }
    }
}

void BreakPlan::getActuals()
{
    if (work_hour != (int)work_hour)
    {
        work_hour = (int)work_hour + 1;
    }
    for (int col = 0; col < cols; col++)
    {
        for (size_t row = 0; row < shifts.size(); row++)
        {
            int shift = std::stoi(shifts[row]);
            for (int user = 0; user < hc[row][col]; user++)
            {
                for (int hour = 0; hour < work_hour; hour++)
                {
                    int current_hour = shift + hour;
                    for (int quarter = 0; quarter < 4; quarter++)
                    {
                        double key = (
                            col * 24 * 60 + 
                            60 * (current_hour + quarter / (double)4)
                        );
                        str date_str = datetime -> add(key);
                        actuals[date_str]++;
                        coverages[date_str] = \
                            actuals[date_str] / needs[date_str];
                    }
                }
            }
        }
    }
}

