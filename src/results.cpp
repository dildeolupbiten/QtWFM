#include "../include/results.hpp"
#include "../include/utils.hpp"

Results::Results(QWidget *parent): QFrame(parent)
{
    area = new QScrollArea(this);
    frame = new QFrame(area);
    analysis_results = new AnalysisResults(
        "",
        {
           "AHT",
           "Trend",
           "Volume",
           "Need",
           "Actual",
           "Coverage",
           "Occupancy",
           "Waiting Time"
       },
       frame
    );
    shift_plan_results = new ShiftPlanResults(frame);
    break_plan_results = new BreakPlanResults(frame);
    label = new QLabel("Results", this);
    config();
}

Results::~Results() {};

void Results::config()
{
    setLayout(new QVBoxLayout(this));
    layout() -> addWidget(label);
    layout() -> addWidget(area);
    layout() -> setAlignment(label, Qt::AlignCenter);
    layout() -> setAlignment(area, Qt::AlignCenter);
    setStyleSheet("border: none;");
    area -> setWidget(frame);
    area -> setWidgetResizable(1);
    area -> setFixedSize(775, 700);
    frame -> setLayout(new QVBoxLayout(frame));
    frame -> layout() -> addWidget(shift_plan_results);
    frame -> layout() -> addWidget(break_plan_results);
    frame -> layout() -> addWidget(analysis_results);
    frame -> layout() -> setAlignment(shift_plan_results, Qt::AlignCenter);
    frame -> layout() -> setAlignment(break_plan_results, Qt::AlignCenter);
    frame -> layout() -> setAlignment(analysis_results, Qt::AlignCenter);
    label -> setStyleSheet("font-size: 30px;");
    setFixedSize(800, 800);
    shift_plan_results -> setVisible(0);
    break_plan_results -> setVisible(0);
    connect(
        shift_plan_results -> shift_plan -> horizontalHeader(),
        &QHeaderView::sectionClicked,
        [this](int index){ changeValues(index); }
    );
    connect(
        shift_plan_results -> hc -> horizontalHeader(),
        &QHeaderView::sectionClicked,
        [this](int index){ changeValues(index); }
    );
}

void Results::changeValues(int index)
{
    if (!shift_plan_results -> done)
    {
        return;
    }
    int rows = shift_plan_results -> hc -> rowCount();
    vec<double> hc_per_shift1(rows), hc_per_shift2(rows);
    vec<int> perm(rows);
    vec<map<str, double>> activities_values = \
        shift_plan_results -> activities_values;
    double work_hour = shift_plan_results -> results -> work_hour;
    for (int row = 0; row < rows; row++)
    {
        hc_per_shift1[row] = shift_plan_results -> results -> sum()\
            [shift_plan_results -> results -> shifts[row]][index];
        if (index)
        {
            hc_per_shift2[row] = shift_plan_results -> results -> sum()\
                [shift_plan_results -> results -> shifts[row]][index - 1];
        }
        else
        {
            hc_per_shift2[row] = 0;
        }
        perm[row] = std::stoi(shift_plan_results -> results -> shifts[row]);

    }
    str shift = join(shift_plan_results -> results -> shifts, " ");
    vec<double> actual = getActual(
        perm, 
        hc_per_shift1, 
        hc_per_shift2, 
        activities_values, 
        work_hour
    );
    vec<double> coverage(24);
    vec<double> occupancy(24);
    vec<double> waiting_time(24);
    if (break_plan_results -> done)
    {
        break_plan_results -> init_break_plan(
            break_plan_results -> results, 
            index
        );
        vec<vec<double>> actuals = \
            break_plan_results -> results -> hourlyActuals();
        actual = actuals[index];
    }
    ShiftValues shift_values = {
        shift, 
        actual, 
        coverage, 
        occupancy, 
        waiting_time
    };
    for (int i = 0; i < 24; i++)
    {
        coverage[i] = actual[i] / need[i];
        occupancy[i] = 1 / coverage[i];
        waiting_time[i] = trends[i] * (1 - occupancy[i]) / occupancy[i];
    }
    vec<str> colors_actual = getColors(
        actual, 0, 1, 1
        );
    vec<str> colors_coverage = getColors(
        coverage, 0, 0, 0
        );
    vec<str> colors_occupancy = getColors(
        occupancy, 1, 0, 0
        );
    vec<str> colors_waiting_time = getColors(
        waiting_time, 0, 0, 0
        );
    vec<int> units = {24, 24 * 60, 24 * 3600};
    for (int row = 0; row < 24; row++)
    {
        str s_actual = std::to_string((int)actual[row]);
        analysis_results -> labels[row]["Actual"] -> \
            setText(QString::fromStdString(s_actual));
        str style_actual = (
            "color: black; background: " + colors_actual[row] + ";"
            );
        analysis_results -> labels[row]["Actual"] -> \
            setStyleSheet(QString::fromStdString(style_actual));

        std::stringstream ss_coverage;
        ss_coverage << std::fixed << std::setprecision(2) << \
            coverage[row] * 100;
        analysis_results -> labels[row]["Coverage"] -> \
            setText(QString::fromStdString(ss_coverage.str() + " %"));
        str style_coverage = (
            "color: black; background: " + colors_coverage[row] + ";"
            );
        analysis_results -> labels[row]["Coverage"] -> \
            setStyleSheet(QString::fromStdString(style_coverage));

        std::stringstream ss_occupancy;
        ss_occupancy << std::fixed << std::setprecision(2) << \
            occupancy[row] * 100;
        analysis_results -> labels[row]["Occupancy"] -> \
            setText(QString::fromStdString(ss_occupancy.str() + " %"));
        str style_occupancy = (
            "color: black; background: " + colors_occupancy[row] + ";"
            );
        analysis_results -> labels[row]["Occupancy"] -> \
            setStyleSheet(QString::fromStdString(style_occupancy));

        str s_waiting_time = floatToStr(
            waiting_time[row], units
            );
        analysis_results -> labels[row]["Waiting Time"] -> \
            setText(QString::fromStdString(s_waiting_time));
        str style_waiting_time = (
            "color: black; background: " + colors_waiting_time[row] + ";"
            );
        analysis_results -> labels[row]["Waiting Time"] -> \
            setStyleSheet(QString::fromStdString(style_waiting_time));
    }
    analysis_results -> update();
}

