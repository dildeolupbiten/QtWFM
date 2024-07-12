#include "../include/utils.hpp"
#include "../include/msgbox.hpp"
#include "../include/analysis.hpp"
#include "../include/analysis_worker.hpp"
#include "../include/shift_plan_worker.hpp"
#include "../include/break_plan_worker.hpp"

#include <cmath>

Analysis::Analysis(QWidget *parent): QFrame(parent) 
{
    left = new QFrame(this);
    right = new QFrame(this);
    left_left = new QFrame(left);
    left_right = new QFrame(left);
    btn_frame = new QFrame(left_right);
    profile_frame = new QFrame(left_right);
    label = new QLabel("Select Profile", profile_frame);
    combobox = new QComboBox(profile_frame);
    entry_frame = new QFrame(profile_frame);
    entry = new QLineEdit(entry_frame);
    profile_btn_frame = new QFrame(profile_frame);
    btn_add = new QPushButton("Add", profile_btn_frame);
    btn_del = new QPushButton("Remove", profile_btn_frame);
    values = new Table(
        "",
        {"Headcount", "Daily Headcount", "Utilization Rate", "Total Trend"},
        left_right
    );
    inputs = new Inputs(
        std::bind(&Analysis::changeValues, this),
        "",
        {"Volume", "AHT", "Shrinkage", "Work Hour", "Off Day"},
        left_right
    );
    activities = new Activities(
        std::bind(&Analysis::changeValues, this),
        "Activities",
        {"Start", "End", "Duration"},
        left_right
    );
    trend = new Trend(
        std::bind(&Analysis::changeValues, this),
        "",
        {"Trend"},
        left_left
    );
    results = new Results(right);
    shifts = new Shifts(left_right);
    btn_analyze = new QPushButton("Analyze", btn_frame);
    btn_shift_plan = new QPushButton("Create Shift Plan", btn_frame);
    btn_break_plan = new QPushButton("Create Break Plan", btn_frame);
    btn_export = new QPushButton("Export", btn_frame);
    pbar = new QFrame(left_right);
    pbar_analysis = new QProgressBar(pbar);
    pbar_break_plan = new QProgressBar(pbar);
    pbar_shift_plan = new QProgressBar(pbar);
    config();
    loadProfiles();
}

Analysis::~Analysis() {}

void Analysis::config()
{
    setLayout(new QHBoxLayout(this));
    layout() -> addWidget(left);
    layout() -> addWidget(right);
    layout() -> setAlignment(left, Qt::AlignCenter);
    layout() -> setAlignment(right, Qt::AlignCenter);

    left -> setObjectName("frame");
    left -> setLayout(new QHBoxLayout(left));
    left -> layout() -> addWidget(left_left);
    left -> layout() -> addWidget(left_right);
    left -> layout() -> setAlignment(left_left, Qt::AlignCenter);
    left -> layout() -> setAlignment(left_right, Qt::AlignCenter);

    profile_frame -> setLayout(new QVBoxLayout(profile_frame));
    profile_frame -> layout() -> addWidget(label);
    profile_frame -> layout() -> addWidget(combobox);
    profile_frame -> layout() -> addWidget(entry_frame);
    profile_frame -> layout() -> addWidget(profile_btn_frame);
    profile_frame -> layout() -> setAlignment(label, Qt::AlignCenter);
    profile_frame -> layout() -> setAlignment(combobox, Qt::AlignCenter);
    profile_frame -> layout() -> setAlignment(entry_frame, Qt::AlignCenter);
    profile_frame -> layout() -> setAlignment(
        profile_btn_frame, 
        Qt::AlignCenter
    );
    profile_btn_frame -> setLayout(new QHBoxLayout(profile_btn_frame));
    profile_btn_frame -> layout() -> addWidget(btn_add);
    profile_btn_frame -> layout() -> addWidget(btn_del);
    profile_btn_frame -> layout() -> setAlignment(btn_add, Qt::AlignCenter);
    profile_btn_frame -> layout() -> setAlignment(btn_del, Qt::AlignCenter);
    btn_add -> setFixedWidth(100);
    btn_del -> setFixedWidth(100);

    entry_frame -> setLayout(new QHBoxLayout(entry_frame));
    QLabel *entry_label = new QLabel("Profile Name", entry_frame);
    entry_frame -> layout() -> addWidget(entry_label);
    entry_frame -> layout() -> addWidget(entry);
    entry_frame -> layout() -> setAlignment(entry_label, Qt::AlignCenter);
    entry_frame -> layout() -> setAlignment(entry, Qt::AlignCenter);

    left_left -> setLayout(new QVBoxLayout(left_left));
    left_left -> layout() -> addWidget(trend);
    left_left -> layout() -> setAlignment(trend, Qt::AlignCenter);
    left_right -> setLayout(new QVBoxLayout(left_right));
    left_right -> layout() -> addWidget(profile_frame);
    left_right -> layout() -> addWidget(values);
    left_right -> layout() -> addWidget(inputs);
    left_right -> layout() -> addWidget(activities);
    left_right -> layout() -> addWidget(btn_frame);
    left_right -> layout() -> addWidget(pbar);
    left_right -> layout() -> addWidget(shifts);
    left_right -> layout() -> setAlignment(values, Qt::AlignCenter);
    left_right -> layout() -> setAlignment(profile_frame, Qt::AlignCenter);
    left_right -> layout() -> setAlignment(inputs, Qt::AlignCenter);
    left_right -> layout() -> setAlignment(activities, Qt::AlignCenter);
    left_right -> layout() -> setAlignment(btn_frame, Qt::AlignCenter);
    left_right -> layout() -> setAlignment(pbar, Qt::AlignCenter);
    left_right -> layout() -> setAlignment(shifts, Qt::AlignCenter);
    right -> setLayout(new QVBoxLayout(right));
    right -> setObjectName("frame");
    right -> layout() -> addWidget(results);
    right -> layout() -> setAlignment(results, Qt::AlignCenter);
    btn_frame -> setLayout(new QHBoxLayout(btn_frame));
    btn_frame -> layout() -> addWidget(btn_analyze);
    btn_frame -> layout() -> addWidget(btn_shift_plan);
    btn_frame -> layout() -> addWidget(btn_break_plan);
    btn_frame -> layout() -> addWidget(btn_export);
    btn_frame -> layout() -> setAlignment(btn_analyze, Qt::AlignCenter);
    btn_frame -> layout() -> setAlignment(btn_shift_plan, Qt::AlignCenter);
    btn_frame -> layout() -> setAlignment(btn_break_plan, Qt::AlignCenter);
    btn_frame -> layout() -> setAlignment(btn_export, Qt::AlignCenter);
    pbar -> setLayout(new QHBoxLayout(pbar));
    pbar -> layout() -> addWidget(pbar_analysis);
    pbar -> layout() -> addWidget(pbar_shift_plan);
    pbar -> layout() -> addWidget(pbar_break_plan);
    pbar -> layout() -> setAlignment(pbar_analysis, Qt::AlignCenter);
    pbar -> layout() -> setAlignment(pbar_shift_plan, Qt::AlignCenter);
    pbar -> layout() -> setAlignment(pbar_break_plan, Qt::AlignCenter);
    pbar_analysis -> setValue(0);
    pbar_shift_plan -> setValue(0);
    pbar_break_plan -> setValue(0);
    pbar_shift_plan -> setVisible(0);
    pbar_break_plan -> setVisible(0);
    results -> analysis_results -> init();
    inputs -> init();
    values -> init();
    trend -> init();
    right -> setFixedSize(850, 850);
    left -> setFixedSize(850, 850);
    btn_analyze -> setFixedWidth(125);
    btn_shift_plan -> setFixedWidth(125);
    btn_break_plan -> setFixedWidth(125);
    btn_export -> setFixedWidth(125);
    connect(
        btn_analyze,
        &QPushButton::clicked,
        [this](){ analyze(); }
    );
    connect(
        btn_shift_plan,
        &QPushButton::clicked,
        [this](){ createShiftPlan(); }
    );
    connect(
        btn_break_plan,
        &QPushButton::clicked,
        [this](){ createBreakPlan(); }
    );
    connect(
        btn_export,
        &QPushButton::clicked,
        [this](){ exportResults(); }
    );
    connect(
        btn_add,
        &QPushButton::clicked,
        [this](){ addProfile(); }
    );
    connect(
        btn_del,
        &QPushButton::clicked,
        [this](){ delProfile(); }
    );
}

void Analysis::exportResults()
{
    str date = getNow();
    str filename1 = "export_analysis_" + date + ".csv";
    std::ofstream file1 = openFile(filename1);
    vec<map<str, double>> analysis_values = results -> \
        analysis_results -> values(results -> analysis_results -> labels);
    vec<str> headers = {
        "Hour",
        "AHT",
        "Trend",
        "Volume",
        "Need",
        "Actual",
        "Coverage",
        "Occupancy",
        "Waiting Time"
    };
    file1 << join(headers, ",") + "\n";
    for (int row = 0; row < 24; row++)
    {
        str hour = zfill(std::to_string(row), 2) + ":00";
        str aht = results -> analysis_results -> \
            labels[row]["AHT"] -> text().toStdString();
        str trend = results -> analysis_results -> \
            labels[row]["Trend"] -> text().toStdString();
        str volume = results -> analysis_results -> \
            labels[row]["Volume"] -> text().toStdString();
        str need = results -> analysis_results -> \
            labels[row]["Need"] -> text().toStdString();
        str actual = results -> analysis_results -> \
            labels[row]["Actual"] -> text().toStdString();
        str coverage = results -> analysis_results -> \
            labels[row]["Coverage"] -> text().toStdString();
        str occupancy = results -> analysis_results -> \
            labels[row]["Occupancy"] -> text().toStdString();
        str waiting_time = results -> analysis_results -> \
            labels[row]["Waiting Time"] -> text().toStdString();
        aht = replace(aht, ",", ".");
        trend = replace(trend, ",", ".");
        volume = replace(volume, ",", ".");
        need = replace(need, ",", ".");
        actual = replace(actual, ",", ".");
        coverage = replace(coverage, ",", ".");
        occupancy = replace(occupancy, ",", ".");
        waiting_time = replace(waiting_time, ",", ".");

        vec<str> vals = {
            hour,
            aht,
            trend,
            volume,
            need,
            actual,
            coverage,
            occupancy,
            waiting_time
        };
        file1 << join(vals, ",") + "\n";
    }
    file1.close();
    if (results -> shift_plan_results -> done)
    {
        headers = {};
        str filename2 = "export_shift_plan_" + date + ".csv";
        std::ofstream file2 = openFile(filename2);

        vec<vec<str>> shift_plan_values = results -> \
            shift_plan_results -> results -> data;
        int rows = results -> shift_plan_results -> results -> rows;
        int cols = results -> shift_plan_results -> results -> cols;
        for (int col = 0; col < cols; col++)
        {
            headers.push_back(std::to_string(col));
        }
        file2 << join(headers, ",") + "\n";
        for (int row = 0; row < rows; row++) {
            str joined = join(
                results -> shift_plan_results -> results -> data[row],
                ","
            );
            file2 << joined + "\n";
        }
        file2.close();
    }
    if (results -> break_plan_results -> done)
    {
        headers = {"Day", "Shift"};
        str filename3 = "export_break_plan_" + date + ".csv";
        std::ofstream file3 = openFile(filename3);
        int cols = results -> \
            break_plan_results -> results -> data[0][0].size();
        for (int col = 1; col < cols; col++)
        {
            headers.push_back("Activity-" + std::to_string(col + 1));
        }
        file3 << join(headers, ",") + "\n";
        vec<vec<vec<str>>> break_plan_values = results -> \
            break_plan_results -> results -> data;
        int days = results -> break_plan_results -> results -> data.size();
        for (int day = 0; day < days; day++)
        {
            vec<vec<str>> vals = \
                results -> break_plan_results -> results -> data[day];
            for (size_t row = 0; row < vals.size(); row++)
            {
                vec<str> row_data(cols);
                for (size_t col = 0; col < vals[row].size(); col++)
                {
                    if (!col)
                    {
                        row_data[col] = \
                            std::to_string(std::stoi(vals[row][col]) + 1);
                    }
                    else
                    {
                        str val = vals[row][col];
                        vec<str> t_value = split(split(val, " ")[1], ":");
                        row_data[col] = t_value[0] + ":" + t_value[1];
                    }
                }
                file3 << std::to_string(day) + "," + \
                    join(row_data, ",") + "\n";
            }
        }
        file3.close();
    }
    MsgBox::information(
        nullptr,
        "Info",
        "Exported successfully."
    );
    return;
}

void Analysis::addProfile()
{
    str name = entry -> text().toStdString();
    if (name == "")
    {
        return;
    }
    if (name == "Default")
    {
        MsgBox::information(
            nullptr,
            "Info",
            QString::fromStdString(
                "'" + name +  "' is used by another profile.."
            )
        );
        return;
    }
    for (Profile& prof : profiles)
    {
        if (prof.name == name)
        {
            MsgBox::information(
                nullptr,
                "Info",
                QString::fromStdString(
                    "'" + name +  "' is used by another profile.."
                )
            );
            return;
        }
    }
    Profile profile;
    profile.activities = activities -> values(activities -> widgets);
    profile.inputs = inputs -> values(inputs -> widgets);
    profile.trends = trend -> values(trend -> widgets);
    profile.name = entry -> text().toStdString();
    profiles.push_back(profile);
    createProfiles("profiles.xml", profiles);
    combobox -> addItem(QString::fromStdString(name));
    MsgBox::information(
        nullptr,
        "Info",
        QString::fromStdString(
            "Added profile '" + name +  "'."
        )
    );
}

void Analysis::delProfile()
{
    str name = entry -> text().toStdString();
    if (name == "")
    {
        return;
    }
    if (name == "Default" || profiles.size() == 1)
    {
        MsgBox::information(
            nullptr,
            "Info",
            QString::fromStdString(
                "Default profile can not be deleted."
            )
        );
        return;
    }
    vec<Profile> temp;
    int index;
    for (size_t i = 0; i < profiles.size(); i++)
    {
        if (profiles[i].name != name)
        {
            temp.push_back(profiles[i]);
        }
        else
        {
            index = i;
        }
    }
    profiles = temp;
    createProfiles("profiles.xml", profiles);
    combobox -> removeItem(index);
    MsgBox::information(
        nullptr,
        "Info",
        QString::fromStdString(
            "Deleted profile '" + name +  "'."
        )
    );
}

void Analysis::changeProfile()
{
    for (size_t i = 0; i < profiles.size(); i++)
    {
        if (profiles[i].name == combobox-> currentText().toStdString())
        {
            activities -> destroy();
            loadProfileValues(profiles[i]);
            MsgBox::information(
                nullptr,
                "Info",
                QString::fromStdString(
                    "Profiles changed."
                )
            );
            break;
        }
    }
}

void Analysis::changeValues()
{
    vec<map<str, double>> inputs_values = \
        inputs -> values(inputs -> widgets);
    vec<map<str, double>> activities_values = \
        activities -> values(activities -> widgets);
    vec<map<str, double>> trend_values = \
        trend -> values(trend -> widgets);
    vec<map<str, double>> results_values = results -> analysis_results -> \
        values(results -> analysis_results -> labels);
    double vol = inputs_values[0]["Volume"];
    double aht = inputs_values[0]["AHT"];
    double shr = (double)1 - inputs_values[0]["Shrinkage"];
    double wh = inputs_values[0]["Work Hour"];
    double off = 1 - inputs_values[0]["Off Day"] / (double)7;
    double total_activity = 0;
    for (auto& m : activities_values)
    {
        total_activity += m["Duration"];
    }
    double ur = (1 - total_activity / wh);
    int hc, daily_hc;
    if (ur < 0) {
        daily_hc = 0;
        hc = 0;
    }
    else {
        daily_hc = vol * aht / (3600 * shr * off * wh * ur);
        hc = daily_hc / (off * shr);
    }
    if (hc > 0 || daily_hc > 0 || ur > 0)
    {
        std::stringstream ss_ur;
        ss_ur << std::fixed << std::setprecision(2) << ur * 100;
        values -> labels[0]["Headcount"] -> setText(
            QString::fromStdString(std::to_string(hc))
        );
        values -> labels[0]["Daily Headcount"] -> setText(
            QString::fromStdString(std::to_string(daily_hc))
        );
        values -> labels[0]["Utilization Rate"] -> setText(
            QString::fromStdString(ss_ur.str() + " %")
        );
    }
    double total_trend = 0;
    map<str, vec<double>> vals;
    vals["Trend"] = {};
    vals["Volume"] = {};
    vals["Need"] = {};
    for (int row = 0; row < 24; row++) {
        double t = trend_values[row]["Trend"];
        double v = vol * t;
        double n = vol * t * aht / (double)3600;
        total_trend += t;
        std::stringstream ss_aht, ss_trend, ss_vol, ss_need;
        ss_aht << std::fixed << std::setprecision(2) << aht;
        ss_trend << std::fixed << std::setprecision(2) << t * 100;
        ss_vol << std::fixed << std::setprecision(2) << v;
        ss_need << std::fixed << std::setprecision(2) << n;
        results -> analysis_results -> labels[row]["AHT"] -> setText(
            QString::fromStdString(ss_aht.str())
        );
        results -> analysis_results -> labels[row]["Trend"] -> setText(
            QString::fromStdString(ss_trend.str() + " %")
        );
        results -> analysis_results -> labels[row]["Volume"] -> setText(
            QString::fromStdString(ss_vol.str())
        );
        results -> analysis_results -> labels[row]["Need"] -> setText(
            QString::fromStdString(ss_need.str())
        );
        vals["Trend"].push_back(t);
        vals["Volume"].push_back(vol * t);
        vals["Need"].push_back(n);
    }
    map<str, vec<str>> mapped;
    mapped["Trend"] = getColors(vals["Trend"], 1, 1, 0);
    mapped["Volume"] = getColors(vals["Volume"], 1, 1, 0);
    mapped["Need"] = getColors(vals["Need"], 1, 1, 0);
    for (int row = 0; row < 24; row++) {
        for (auto& it : mapped)
        {
            str bg = (
                "color: black; background: " + mapped[it.first][row] + ";"
            );
            results -> analysis_results -> labels[row][it.first] -> \
                setStyleSheet(QString::fromStdString(bg));
        }
    }
    if (std::round(total_trend * 100) / 100 <= 1)
    {
        std::stringstream ss_total;
        ss_total << std::fixed << std::setprecision(2) << total_trend * 100;
        values -> labels[0]["Total Trend"] -> setText(
            QString::fromStdString(ss_total.str() + " %")
        );
    }
}

void Analysis::loadProfileValues(Profile profile)
{
    inputs -> widgets[0]["Volume"] -> \
        setValue(profile.inputs[0]["Volume"]);
    inputs -> widgets[0]["AHT"] -> \
        setValue(profile.inputs[0]["AHT"]);
    inputs -> widgets[0]["Shrinkage"] -> \
        setValue(profile.inputs[0]["Shrinkage"]);
    inputs -> widgets[0]["Work Hour"] -> \
        setValue(profile.inputs[0]["Work Hour"]);
    inputs -> widgets[0]["Off Day"] -> \
        setValue(profile.inputs[0]["Off Day"]);
    for (int row = 0; row < 24; row++)
    {
        trend -> widgets[row]["Trend"] -> \
            setValue(profile.trends[row]["Trend"]);
    }
    for (size_t row = 0; row < profile.activities.size(); row++)
    {
        activities -> init();
        activities -> widgets[row]["Start"] -> \
            setValue(profile.activities[row]["Start"]);
        activities -> widgets[row]["End"] -> \
            setValue(profile.activities[row]["End"]);
        activities -> widgets[row]["Duration"] -> \
            setValue(profile.activities[row]["Duration"]);
    }
}

void Analysis::loadProfiles()
{
    str filename = "profiles.xml";
    if (!std::filesystem::exists(filename))
    {
        Profile profile;
        profile.name = "Default";
        profile.trends = {
            {{"Trend", .057510}},
            {{"Trend", .056508}},
            {{"Trend", .049436}},
            {{"Trend", .043375}},
            {{"Trend", .033160}},
            {{"Trend", .022255}},
            {{"Trend", .015329}},
            {{"Trend", .015370}},
            {{"Trend", .023577}},
            {{"Trend", .031358}},
            {{"Trend", .035873}},
            {{"Trend", .041110}},
            {{"Trend", .042382}},
            {{"Trend", .045827}},
            {{"Trend", .043823}},
            {{"Trend", .044729}},
            {{"Trend", .047697}},
            {{"Trend", .048407}},
            {{"Trend", .049870}},
            {{"Trend", .047794}},
            {{"Trend", .047432}},
            {{"Trend", .050575}},
            {{"Trend", .051211}},
            {{"Trend", .055396}}
        };
        profile.inputs = {
            {{"AHT", 24}},
            {{"Off Day", 2}},
            {{"Shrinkage", .1}},
            {{"Volume", 25000}},
            {{"Work Hour", 9}}
        };
        profile.activities = {
            {{"Start", 1}, {"End", 3}, {"Duration", .25}},
            {{"Start", 3}, {"End", 6}, {"Duration", .75}},
            {{"Start", 6}, {"End", 8}, {"Duration", .25}},
            {{"Start", 0}, {"End", 8}, {"Duration", .25}}
        };
        profiles.push_back(profile);
        createProfiles("profiles.xml", profiles);
    }
    profiles = getProfiles(filename);
    QStringList strList;
    for (auto& it : profiles)
    {
        strList.append(QString::fromStdString(it.name));
    }
    combobox -> addItems(strList);
    loadProfileValues(profiles[0]);
    connect(
        combobox,
        &QComboBox::currentTextChanged,
        [this](){ changeProfile(); }
    );
}

vec<double> Analysis::getNeed()
{
    vec<double> need(24);
    for (int row = 0; row < 24; row++)
    {
        need[row] = results -> analysis_results -> \
            labels[row]["Need"] -> text().toDouble();
    }
    return need;
}

void Analysis::analyze()
{
    vec<double> need = getNeed();
    pbar_analysis -> setMinimum(0);
    pbar_analysis -> setVisible(1);
    pbar_shift_plan -> setVisible(0);
    pbar_break_plan -> setVisible(0);
    AnalysisWorker *worker = new AnalysisWorker(
        activities, shifts, trend, pbar_analysis, need, inputs, values
    );
    connect(
        worker,
        &AnalysisWorker::finished,
        this,
        &Analysis::finished_analyze
    );
    connect(
        worker,
        &AnalysisWorker::progress,
        this,
        &Analysis::progress_analyze
    );
    worker -> start();
}

void Analysis::progress_analyze(int value)
{
    pbar_analysis -> setValue(value);
}

void Analysis::finished_analyze(map<int, vec<ShiftValues>> output)
{
    int done = 0;
    combs = output;
    for (auto& k1 : combs)
    {
        if (done == 0)
        {
            done++;
            changeColors(combs, k1.first, 0);
            for (auto& k2: k1.second)
            {
                shifts -> shifts -> addItem(QString::fromStdString(k2.shift));
            }
        }
        shifts -> n_shift -> addItem(
            QString::fromStdString(std::to_string(k1.first))
        );
    }
    connect(
        shifts -> n_shift,
        &QComboBox::currentTextChanged,
        [this](){
            str n_shift_current = shifts -> n_shift -> \
                currentText().toStdString();
            int n_shift_int = std::stoi(n_shift_current);
            shifts -> shifts -> clear();
            for (size_t row = 0; row < combs[n_shift_int].size(); row++)
            {
                shifts -> shifts -> addItem(
                    QString::fromStdString(combs[n_shift_int][row].shift)
                );
            }
            changeColors(combs, n_shift_int, 0);
        }
    );
    connect(
        shifts -> shifts,
        &QComboBox::currentTextChanged,
        [this](){
            str n_shift_current = shifts -> n_shift -> \
                currentText().toStdString();
            int n_shift_int = std::stoi(n_shift_current);
            str current_shift = shifts -> shifts -> \
                currentText().toStdString();
            for (int row = 0; row < combs[n_shift_int].size(); row++)
            {
                if (combs[n_shift_int][row].shift == current_shift) {
                    changeColors(combs, n_shift_int, row);
                    break;
                }
            }
        }
    );
    QStringList strList;
    for (int i = 0; i < 10; i++)
    {
        strList.append(QString::fromStdString(std::to_string(7 * (i + 1))));
    }
    shifts -> days -> addItems(strList);
    pbar_analysis -> setValue(0);
    MsgBox::information(
        nullptr,
        "Info",
        QString::fromStdString(
            "Analyzing finished successfully."
        )
    );
}

void Analysis::changeColors(map<int, vec<ShiftValues>>& data, int r, int index)
{
    vec<str> colors_actual = getColors(
        data[r][index].actual, 0, 1, 1
    );
    vec<str> colors_coverage = getColors(
        data[r][index].coverage, 0, 0, 0
    );
    vec<str> colors_occupancy = getColors(
        data[r][index].occupancy, 1, 0, 0
    );
    vec<str> colors_waiting_time = getColors(
        data[r][index].waiting_time, 0, 0, 0
    );
    vec<int> units = {24, 24 * 60, 24 * 3600};
    for (int row = 0; row < 24; row++)
    {
        str actual = std::to_string((int)data[r][index].actual[row]);
        results -> analysis_results -> labels[row]["Actual"] -> \
            setText(QString::fromStdString(actual));
        str style_actual = (
            "color: black; background: " + colors_actual[row] + ";"
        );
        results -> analysis_results -> labels[row]["Actual"] -> \
            setStyleSheet(QString::fromStdString(style_actual));

        std::stringstream ss_coverage;
        ss_coverage << std::fixed << std::setprecision(2) << \
            data[r][index].coverage[row] * 100;
        results -> analysis_results -> labels[row]["Coverage"] -> \
            setText(QString::fromStdString(ss_coverage.str() + " %"));
        str style_coverage = (
            "color: black; background: " + colors_coverage[row] + ";"
        );
        results -> analysis_results -> labels[row]["Coverage"] -> \
            setStyleSheet(QString::fromStdString(style_coverage));

        std::stringstream ss_occupancy;
        ss_occupancy << std::fixed << std::setprecision(2) << \
            data[r][index].occupancy[row] * 100;
        results -> analysis_results -> labels[row]["Occupancy"] -> \
            setText(QString::fromStdString(ss_occupancy.str() + " %"));
        str style_occupancy = (
            "color: black; background: " + colors_occupancy[row] + ";"
        );
        results -> analysis_results -> labels[row]["Occupancy"] -> \
            setStyleSheet(QString::fromStdString(style_occupancy));

        str waiting_time = floatToStr(
            data[r][index].waiting_time[row], units
        );
        results -> analysis_results -> labels[row]["Waiting Time"] -> \
            setText(QString::fromStdString(waiting_time));
        str style_waiting_time = (
            "color: black; background: " + colors_waiting_time[row] + ";"
        );
        results -> analysis_results -> labels[row]["Waiting Time"] -> \
            setStyleSheet(QString::fromStdString(style_waiting_time));
    }
    results -> analysis_results -> update();
}

void Analysis::createShiftPlan()
{
    if (!shifts -> days -> count() || !shifts -> shifts -> count())
    {
        return;
    }
    pbar_analysis -> setVisible(0);
    pbar_break_plan -> setVisible(0);
    pbar_shift_plan -> setVisible(1);
    pbar_shift_plan -> setMinimum(0);
    pbar_shift_plan -> setMaximum(4);
    str shift = shifts -> shifts -> currentText().toStdString();
    vec<str> splitted = split(shift, " ");
    int rows = values -> labels[0]["Headcount"] -> text().toInt();
    int cols = shifts -> days -> currentText().toInt();
    int off = inputs -> widgets[0]["Off Day"] -> value();
    int wh = inputs -> widgets[0]["Work Hour"] -> value();
    ShiftPlanWorker *worker = new ShiftPlanWorker(
        splitted, 
        rows, 
        cols, 
        off, 
        wh
    );
    connect(
        worker,
        &ShiftPlanWorker::finished,
        this,
        &Analysis::finished_shift_plan
    );
    connect(
        worker,
        &ShiftPlanWorker::progress,
        this,
        &Analysis::progress_shift_plan
    );
    worker -> start();
}

void Analysis::progress_shift_plan(int value)
{
    pbar_shift_plan -> setValue(value);
}

void Analysis::finished_shift_plan(ShiftPlan* plan)
{
    pbar_shift_plan -> setValue(0);
    results -> shift_plan_results -> setVisible(1);
    results -> shift_plan_results -> init(plan);
    results -> shift_plan_results -> done = 1;
    results -> shift_plan_results -> activities_values = \
        activities -> values(activities -> widgets);
    results -> need = getNeed();
    results -> trends = vec<double>(24);
    for (int i = 0; i < 24; i++)
    {
        results -> trends[i] = trend -> widgets[i]["Trend"] -> value();
    }
    MsgBox::information(
        nullptr,
        "Info",
        QString::fromStdString(
            "Created shift plan successfully."
        )
    );
}

void Analysis::createBreakPlan()
{
    if (
        !shifts -> days -> count() 
        || 
        !shifts -> shifts -> count() 
        || 
        !results -> shift_plan_results -> done
    )
    {
        return;
    }
    pbar_break_plan -> setMinimum(0);
    pbar_analysis -> setVisible(0);
    pbar_shift_plan -> setVisible(0);
    pbar_break_plan -> setVisible(1);
    vec<str> _shifts = split(
        shifts -> shifts -> currentText().toStdString(), 
        " "
    );
    vec<vec<int>> _hc;
    for (
        auto& [key, value] : 
        results -> shift_plan_results -> results -> sum()
    )
    {
        _hc.push_back(value);
    }
    vec<map<str, double>> _activities_values = \
        activities -> values(activities -> widgets);
    vec<double> _need = getNeed();
    double _work_hour = inputs -> widgets[0]["Work Hour"] -> value();
    BreakPlanWorker *worker = new BreakPlanWorker(
        _shifts,
        _hc,
        _activities_values,
        _need,
        _work_hour
    );
    connect(
        worker,
        &BreakPlanWorker::finished,
        this,
        &Analysis::finished_break_plan
    );
    connect(
        worker,
        &BreakPlanWorker::progress,
        this,
        &Analysis::progress_break_plan
    );
    worker -> start();
}

void Analysis::progress_break_plan(int value)
{
    pbar_break_plan -> setValue(value);
}

void Analysis::finished_break_plan(BreakPlan *plan)
{
    pbar_break_plan -> setValue(0);
    results -> break_plan_results -> setVisible(1);
    results -> break_plan_results -> init(plan);
    results -> break_plan_results -> done = 1;
    MsgBox::information(
        nullptr,
        "Info",
        QString::fromStdString(
            "Created break plan successfully."
        )
    );
}
