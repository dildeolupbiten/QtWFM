#include "../include/utils.hpp"
#include "../lib/rapidxml/rapidxml_utils.hpp"

#include <cstdlib>
#include <algorithm>
#include <functional>
#include <iostream>

str getNow() {
    std::time_t currentTime = std::time(nullptr);
    char buffer[80];
    std::strftime(
        buffer,
        sizeof(buffer),
        "%Y-%m-%d_%H-%M-%S", std::localtime(&currentTime)
    );
    str s(buffer);
    return s;
}

std::ofstream openFile(str filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open the file." << std::endl;
    }
    return file;
}

str empty(int n) {
    str s = "";
    for (int i = 0; i < n; i++) { s += " "; }
    return s;
}

str zfill(str s, int n)
{
    str new_s;
    int diff = n - s.length();
    if (diff > 0)
    {
        for (int i = 0; i < diff; i++)
        {
            new_s += "0";
        }
        return new_s + s;
    }
    return s;
}

str replace(str string, str old, str val) {
    vec<str> splitted = split(string, old);
    str result;
    size_t n = splitted.size();
    for (size_t i = 0; i < n; i++) {
        result += splitted[i];
        result += i + 1 == n ? "" : val;
    }
    return result;
}

void createProfiles(str filename, vec<Profile>& profiles)
{
    std::ofstream file = openFile(filename);
    file << "<profiles>\n";
    for (Profile profile : profiles)
    {
        file << empty(4) + "<profile name=\"" + profile.name + "\">\n";
        file << empty(8) + "<trends>\n";
        for (int i = 0; i < 24; i++)
        {
            double value = profile.trends[i]["Trend"];
            str s = std::to_string(value);
#if defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__))
            s = replace(s, ".", ",");
#endif
            file << empty(12) + "<trend hour=\"" + \
                zfill(std::to_string(i), 2) + ":00\" value=\"" + s + "\"/>\n";
        }
        file << empty(8) + "</trends>\n";
        file << empty(8) + "<inputs>\n";
        for (size_t i = 0; i < profile.inputs.size(); i++)
        {
            map<str, double> input = profile.inputs[i];
            for (auto& [key, value]: input)
            {
                std::stringstream ss_value;
                ss_value << std::fixed << std::setprecision(2) << value;
                str v = ss_value.str();
#if defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__))
                v = replace(v, ".", ",");
#endif
                file << empty(12) + \
                    "<input name=\"" + key + "\" value=\"" + v + "\"/>\n";
            }
        }
        file << empty(8) + "</inputs>\n";
        file << empty(8) + "<activities>\n";
        for (size_t i = 0; i < profile.activities.size(); i++)
        {
            std::stringstream ss_start;
            ss_start << std::fixed << std::setprecision(2) << \
                profile.activities[i]["Start"];
            str start = ss_start.str();
            std::stringstream ss_end;
            ss_end << std::fixed << std::setprecision(2) << \
                profile.activities[i]["End"];
            str end = ss_end.str();
            std::stringstream ss_duration;
            ss_duration << std::fixed << std::setprecision(2) << \
                profile.activities[i]["Duration"];
            str duration = ss_duration.str();
#if defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__))
            start = replace(start, ".", ",");
            end = replace(end, ".", ",");
            duration = replace(duration, ".", ",");
#endif
            file << empty(12) + "<activity start=\"" + start + \
                "\" end=\"" + end + "\" duration=\"" + duration + "\"/>\n";
        }
        file << empty(8) + "</activities>\n";
        file << empty(4) + "</profile>\n";
    }
    file << "</profiles>\n";
    file.close();
}

vec<Profile> getProfiles(str& filename)
{
    vec<Profile> profiles;
    rapidxml::file<> xmlFile(filename.c_str());
    rapidxml::xml_document<> doc;
    doc.parse<0>(xmlFile.data());
    rapidxml::xml_node<> *root = doc.first_node("profiles");
    for (
        rapidxml::xml_node<>* child = root -> first_node("profile");
        child;
        child = child -> next_sibling()
    )
    {
        Profile profile;
        profile.name = child -> first_attribute("name") -> value();
        rapidxml::xml_node<> *trends = child -> first_node("trends");
        rapidxml::xml_node<> *inputs = child -> first_node("inputs");
        rapidxml::xml_node<> *activities = child -> first_node("activities");
        for (
            rapidxml::xml_node<>* trend = trends -> first_node();
            trend;
            trend = trend -> next_sibling()
            )
        {
            double value = std::stod(
                trend -> first_attribute("value") -> value()
                );
            profile.trends.push_back({{"Trend", value}});
        }
        map<str, double> map_inputs;
        for (
            rapidxml::xml_node<>* input = inputs -> first_node();
            input;
            input = input -> next_sibling()
            )
        {
            str name = input -> first_attribute("name") -> value();
            double value = std::stod(
                input -> first_attribute("value") -> value()
                );
            map_inputs[name] = value;
        }
        profile.inputs.push_back(map_inputs);
        for (
            rapidxml::xml_node<>* activity = activities -> first_node();
            activity;
            activity = activity -> next_sibling()
            )
        {
            double start = std::stod(
                activity -> first_attribute("start") -> value()
                );
            double end = std::stod(
                activity -> first_attribute("end") -> value()
                );
            double duration = std::stod(
                activity -> first_attribute("duration") -> value()
                );
            profile.activities.push_back(
                {{"Start", start}, {"End", end}, {"Duration", duration}}
                );
        }
        profiles.push_back(profile);
    }
    return profiles;
}

vec<str> getColors(vec<double> values, int reverse, int pallette, int change) 
{
    vec<double> sorted;
    sorted.insert(sorted.end(), values.begin(), values.end());
    if (reverse)
    {
        std::sort(sorted.begin(), sorted.end(), std::greater<double>());
    }
    else
    {
        std::sort(sorted.begin(), sorted.end());
    }
    map<double, str> mapped;
    vec<str> colors(24);
    int r, g, b, v1, v2;
    for (size_t row = 0; row < sorted.size(); row++)
    {
        if (row <= 11)
        {
            v1 = 255;
            v2 = pallette ? row * 256 / (double)12 : row * 256 / (double)24;
            r = change ? v2 : v1;
            g = change ? v1 : v2;;
            b = pallette ? 0 : row * 256 / (double)24;
        }
        else
        {
            v1 = pallette ? (23 - row) * 256 / (double)12 : 255;
            v2 = pallette ? 255 : row * 256 / (double)24;
            r = change ? v2 : v1;
            g = change ? v1 : v2;;
            b = pallette ? 0 : row * 256 / (double)24;
        }
        mapped[sorted[row]] = rgb(r, g, b);
    }
    for (int row = 0; row < 24; row++)
    {
        colors[row] = mapped[values[row]];
    }
    return colors;
}

str hex(int number) {
    std::stringstream ss;
    ss << std::hex << number;
    str s = ss.str();
    return s.length() == 1 ? "0" + s : s;
}

str rgb(int r, int g, int b)
{
    str color = "#";
    for (auto& i : {r, g, b})
    {
        color.append(hex(i));
    }
    return color;
}

vec<double> getHeadcount(
    vec<int> perm,
    vec<double> hc_per_shift,
    vec<map<str, double>>& activities,
    double work_hour
)
{
    vec<double> headcount(24);
    int count = 0;
    for (int p : perm)
    {
        for (int i = 0; i < work_hour; i++)
        {
            int hour = (p + i) % 24;
            headcount[hour] += hc_per_shift[count];
            for (int row = 0; row < activities.size(); row++)
            {
                double start = activities[row]["Start"] + p;
                double end = (activities[row]["End"] + p);
                int s = (int)start % 24;
                int e = (int)end % 24;
                if (s <= hour && hour <= e)
                {
                    double duration = activities[row]["Duration"];
                    double rate = duration / (end - start);
                    headcount[hour] *= (1 - rate);
                }
            }
        }
        count++;
    }
    return headcount;
}

vec<double> getActual(
    vec<int> perm,
    vec<double> hc_per_shift1,
    vec<double> hc_per_shift2,
    vec<map<str, double>>& activities,
    double work_hour
)
{
    vec<double> headcount(24);

    vec<vec<double>> hcs = {hc_per_shift1, hc_per_shift2};
    for (int day = 0; day < 2; day++)
    {
        int count = 0;
        for (int p : perm)
        {
            for (int i = 0; i < work_hour; i++)
            {
                int hour = (p + i);
                if (!day && hour > 23)
                {
                    continue;
                }
                if (day && hour < 24)
                {
                    continue;
                }
                hour = hour % 24;
                headcount[hour] += hcs[day][count];
                for (int row = 0; row < activities.size(); row++)
                {
                    double start = activities[row]["Start"] + p;
                    double end = (activities[row]["End"] + p);
                    int s = (int)start % 24;
                    int e = (int)end % 24;
                    if (s <= hour && hour <= e)
                    {
                        double duration = activities[row]["Duration"];
                        double rate = duration / (end - start);
                        headcount[hour] *= (1 - rate);
                    }
                }
            }
            count++;
        }
    }
    return headcount;
}

void innerFloatToStr(
    double arg, 
    vec<int> units, 
    vec<int>& list, 
    int n, 
    int col
)
{
    if (col == n)
    {
        return;
    }
    list[col] = (int)(arg * units[col]);
    innerFloatToStr(
        arg - (int)(arg * units[col]) / (double)units[col], 
        units, 
        list, 
        n, 
        col + 1
    );
}

str convertNumber(int n)
{
    str s = std::to_string(n);
    return n < 10 ? "0" + s : s;
}

str floatToStr(double arg, vec<int> units)
{
    int n = units.size();
    vec<int> list(n);
    str result;
    innerFloatToStr(arg, units, list, n, 0);
    str m = convertNumber(list[0]);
    str s = convertNumber(list[1]);
    return m + ":" + s;
}

map<str, vec<double>> hasCoverage(
    vec<double> need, 
    vec<double> actual, 
    vec<double> trend
)
{
    map<str, vec<double>> values;
    values["coverage"] = vec<double>(24);
    values["occupancy"] = vec<double>(24);
    values["waiting_time"] = vec<double>(24);
    double occ, cov;
    for (int row = 0; row < 24; row++)
    {
        double cov = (int)actual[row] / need[row];
        if (cov < 1)
        {
            map<str, vec<double>> res;
            res["coverage"] = {};
            res["occupancy"] = {};
            res["waiting_time"] = {};
            return res;
        }
        values["coverage"][row] = cov;
        occ = 1 / cov;
        values["occupancy"][row] = occ;
        values["waiting_time"][row] = trend[row] * (1 - occ) / occ;
    }
    return values;
}

int combNumber(int n, int r) {
    int res = 1;
    for (int i = 0; i < r; i++) {
        res *= (n - i);
        res /= (i + 1);
    }
    return res;
}

vec<str> innerSplit(str string, str sep, vec<str>& splitted)
{
    int first = 0;
    int last = string.find(sep);
    str item = string.substr(first, last);
    if (item.length())
    {
        splitted.push_back(item);
    }
    if (last == -1) { return splitted; }
    return innerSplit(
        string.substr(last + sep.length(), string.length()),
        sep,
        splitted
    );
}

vec<str> split(str string, str sep)
{
    vec<str> splitted;
    return innerSplit(string, sep, splitted);
}

str join(vec<str> strings, str sep)
{
    str joined;
    int size = strings.size();
    for (size_t i = 0; i < size; i++)
    {
        if (i == size - 1)
        {
            joined += strings[i];
        }
        else
        {
            joined += strings[i] + sep;
        }
    }
    return joined;
}

double harmonicMean(vec<double> values)
{
    int n = values.size();
    double divisor = 0;
    for (double& value : values)
    {
        divisor += 1 / value;
    }
    return n / divisor;
}

