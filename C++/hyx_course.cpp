/* Copyright 2021 Michael Pollak.
 *
 * Use of this source code is governed by an MIT-style
 * licence that can be found in the LICENSE file.
 */


#include "hyx_course.h"

#include <algorithm>  // transform, for_each, max_element
#include <cmath>      // floor
#include <cstdlib>    // strtod
#include <ctime>      // tm, strftime
#include <functional> // divides
#include <iomanip>    // setw, left, right
#include <iterator>   // back_inserter
#include <limits>     // numeric_limits
#include <numeric>    // accumulate
#include <ostream>    // ostream
#include <sstream>    // stringstream
#include <string>     // string


hyx::school::school(
    std::string t_name,
    grade_point_scale t_scale
) :
    m_name(t_name),
    m_scale(t_scale)
{
}

const std::string& hyx::school::get_name() const noexcept
{
    return this->m_name;
}

const hyx::school::grade_point_scale& hyx::school::get_scale() const noexcept
{
    return this->m_scale;
}

void hyx::course::update_letter() noexcept
{
    double grade_percent{this->m_grade_percent};

    if (this->m_grade_percent > 100)
    {
        grade_percent = 100;
    }

    for (const auto& itr : this->m_scale)
    {
        if (std::floor(grade_percent) >= itr.second.first && std::floor(grade_percent) <= itr.second.second)
        {
            this->m_grade_letter = itr.first;
        }
    }
}

bool hyx::course::update_grade_points() noexcept
{
    bool success{false};

    if (this->is_replaced() || this->is_withdrawn())
    {
        this->m_grade_points = -1.0;

        success = true;
    }
    else
    {
        for (const auto& point_scale : this->m_institution->get_scale())
        {
            if (this->get_letter() == point_scale.first)
            {
                this->m_grade_points = point_scale.second * this->m_units;
            }
        }

        success = true;
    }

    return success;
}

void hyx::course::drop_grades(grade_container& grades) noexcept
{
    // drop lowest grades as necessary
    for (size_t i = 0; i < grades.num_of_drops && not grades.points_percent.empty(); ++i)
    {
        ptrdiff_t min_index = std::distance(grades.points_percent.begin(), std::min_element(grades.points_percent.begin(), grades.points_percent.end()));

        grades.points_percent.erase(grades.points_percent.begin() + min_index);
        grades.points_earned.erase(grades.points_earned.begin() + min_index);
        grades.points_possible.erase(grades.points_possible.begin() + min_index);
    }
}

void hyx::course::replace_grades(grade_container& grades) noexcept
{
    // replace grades as necessary
    for (size_t i = 0; i < grades.num_of_replacements && this->m_grade.count(grades.replacement_name) && not this->m_grade[grades.replacement_name].points_percent.empty() && not grades.points_percent.empty(); ++i)
    {
        double repl_perc = this->m_grade[grades.replacement_name].points_percent.front();

        if (repl_perc > *std::min_element(grades.points_percent.begin(), grades.points_percent.end()))
        {
            ptrdiff_t min_index = std::distance(grades.points_percent.begin(), std::min_element(grades.points_percent.begin(), grades.points_percent.end()));

            grades.points_earned[min_index] = this->m_grade[grades.replacement_name].points_earned.front();
            grades.points_possible[min_index] = this->m_grade[grades.replacement_name].points_possible.front();
            grades.points_percent[min_index] = this->m_grade[grades.replacement_name].points_percent.front();
        }
    }
}

hyx::course::course(
    std::string t_name,
    long t_crn,
    int t_units,
    grade_scale t_scale,
    hyx::school* t_institution,
    std::string t_location,
    std::string t_instructor,
    std::string t_subject,
    std::string t_level,
    std::string t_details,
    std::vector<week_day> t_week_days,
    std::array<int, 3> t_start_date,
    std::array<int, 3> t_end_date,
    std::array<int, 2> t_start_time,
    std::array<int, 2> t_end_time
) :
    m_name(t_name),
    m_crn(t_crn),
    m_units(t_units),
    m_scale(t_scale),
    m_institution(t_institution),
    m_location(t_location),
    m_instructor(t_instructor),
    m_subject_level(t_subject + " " + t_level),
    m_details(t_details),
    m_week_days(t_week_days),
    m_book_list(),
    m_grade_letter(),
    m_grade_points(-1),
    m_extra(),
    m_grade_percent(-1)
{
    m_start_datetime.tm_year = t_start_date[0];
    m_start_datetime.tm_mon = t_start_date[1];
    m_start_datetime.tm_mday = t_start_date[2];
    m_start_datetime.tm_hour = t_start_time[0];
    m_start_datetime.tm_min = t_start_time[1];
    m_start_datetime.tm_sec = 0;

    m_end_datetime.tm_year = t_end_date[0];
    m_end_datetime.tm_mon = t_end_date[1];
    m_end_datetime.tm_mday = t_end_date[2];
    m_end_datetime.tm_hour = t_end_time[0];
    m_end_datetime.tm_min = t_end_time[1];
    m_end_datetime.tm_sec = 0;

    std::sort(m_week_days.begin(), m_week_days.end());
}

const std::string& hyx::course::get_name() const noexcept
{
    return this->m_name;
}

long hyx::course::get_crn() const noexcept
{
    return this->m_crn;
}

int hyx::course::get_units() const noexcept
{
    return this->m_units;
}

std::ostream& hyx::operator<<(std::ostream& os, const hyx::grade_scale& scale) noexcept
{
    for (const auto& itr : scale)
    {
        os << std::left << std::setw(2) << itr.first << ": [" << itr.second.first << "-" << itr.second.second << "]\n";
    }

    return os;
}

const std::string hyx::course::get_scale() const noexcept
{
    std::stringstream ss;

    ss << this->m_scale;

    std::string str_scale = ss.str();

    if (not str_scale.empty())
    {
        str_scale.pop_back();
    }

    return str_scale;
}

const std::string& hyx::course::get_location() const noexcept
{
    return this->m_location;
}

const std::string& hyx::course::get_instructor() const noexcept
{
    return this->m_instructor;
}

const std::string& hyx::course::get_details() const noexcept
{
    return this->m_details;
}

const std::string hyx::course::get_week_days() const noexcept
{
    std::string str_week_days;

    std::string LUT_week_days{'-', 'U', 'M', 'T', 'W', 'R', 'F', 'S'};

    // note that m_week_days will be sorted from small to large.
    for (size_t i = 0; i < this->m_week_days.size(); ++i)
    {
        str_week_days.push_back(LUT_week_days[this->m_week_days[i]]);
    }

    return str_week_days;
}

const std::string to_ISO_date(std::tm date) noexcept
{
    char buff[11];

    std::strftime(buff, 11, "%Y-%m-%d", &date);

    return buff;
}

const std::string to_ISO_time(std::tm time) noexcept
{
    if (time.tm_hour == -1 || time.tm_min == -1)
    {
        return "";
    }

    char buff[9];

    std::strftime(buff, 9, "%I:%M %p", &time);

    return buff;
}

const std::tm hyx::course::get_start_datetime() const noexcept
{
    return this->m_start_datetime;
}

const std::tm hyx::course::get_end_datetime() const noexcept
{
    return this->m_end_datetime;
}

const std::vector<std::string>& hyx::course::get_books() const noexcept
{
    return this->m_book_list;
}

double hyx::course::get_grade() const noexcept
{
    return this->m_grade_percent;
}

const std::unordered_map<std::string, std::string> hyx::course::get_points() const noexcept
{
    std::unordered_map<std::string, std::string> vstr_points;

    for (const auto& itr_grade : this->m_grade)
    {
        if (not itr_grade.first.empty())
        {
            std::stringstream ss;

            // both earned and possible point vectors WILL always be the same size.
            for (size_t i = 0; i < itr_grade.second.points_percent.size(); ++i)
            {
                ss << itr_grade.second.points_earned[i] << '/' << itr_grade.second.points_possible[i];

                if (i != itr_grade.second.points_percent.size() - 1)
                {
                    ss << ", ";
                }
            }

            vstr_points.emplace(itr_grade.first, ss.str());
        }
    }

    if (this->m_extra != 0)
    {

        vstr_points.emplace("EXTRA", std::to_string(this->m_extra) + "%");
    }

    return vstr_points;
}

const std::unordered_map<std::string, std::string> hyx::course::get_drops() const noexcept
{
    std::unordered_map<std::string, std::string> vstr_drops;

    for (auto& itr_grade : this->m_grade)
    {
        if (not itr_grade.first.empty())
        {
            vstr_drops.emplace(itr_grade.first, std::to_string(itr_grade.second.num_of_drops));
        }
    }

    if (this->m_extra != 0)
    {

        vstr_drops.emplace("EXTRA", "N/A");
    }

    return vstr_drops;
}

const std::string hyx::course::get_letter() const noexcept
{
    return this->m_grade_letter;
}

float hyx::course::get_grade_points() const noexcept
{
    return this->m_grade_points;
}

bool hyx::course::is_withdrawn() const noexcept
{
    return this->get_letter() == "W";
}

bool hyx::course::is_replaced() const noexcept
{
    return this->get_letter() == "R";
}

bool hyx::course::is_incomplete() const noexcept
{
    return this->get_letter() == "I";
}

bool hyx::course::is_included_in_gpa() const noexcept
{
    return !(this->is_withdrawn() || this->is_replaced() || this->is_incomplete() || this->get_grade_points() == -1.0f || this->m_scale == hyx::scale::PF);
}

void hyx::course::set_withdrawn() noexcept
{
    this->m_grade_points = 0;

    this->m_grade_letter = "W";
    this->update_grade_points();
}

void hyx::course::set_replaced() noexcept
{
    this->m_grade_letter = "R";

    this->update_grade_points();
}

void hyx::course::set_incomplete() noexcept
{
    this->m_grade_letter = "I";

    this->update_grade_points();
}

void hyx::course::set_pass_fail() noexcept
{
    this->m_scale = hyx::scale::PF;

    this->update_grade_points();
}

bool hyx::course::add_book(std::string book) noexcept
{
    bool success{false};

    if (not this->is_withdrawn() && not this->is_replaced())
    {
        this->m_book_list.push_back(book);

        success = true;
    }

    return success;
}

bool hyx::course::add_grade(std::string name, double earn, double poss) noexcept
{
    bool success{false};

    std::transform(name.begin(), name.end(), name.begin(),
        [](unsigned char c) { return toupper(c); });

    if (this->m_grade.count(name) && not this->is_withdrawn() && not this->is_replaced())
    {
        this->m_grade[name].points_earned.push_back(earn);
        this->m_grade[name].points_possible.push_back(poss);
        this->m_grade[name].points_percent.push_back(earn / poss);

        this->update_grade();

        success = true;
    }
    
    return success;
}

void hyx::course::add_extra_to_total(double extra)
{
    this->m_extra += extra;

    this->update_grade();
}

// static const std::string vec_helper(std::vector<std::string> vec) noexcept;

// const std::vector<std::string> weave(const std::vector<std::unordered_map<std::string, std::string>>& vec) noexcept;

// static const std::string to_ISO_date(std::tm date_time) noexcept;

// static const std::string to_ISO_time(std::tm date_time) noexcept;

// const std::string vec_helper(std::vector<std::string> vec) noexcept
// {
//     std::string str_vec;

//     if (not vec.empty())
//     {
//         for (size_t i = 0; i < vec.size() - 1; ++i)
//         {
//             str_vec.append(' ' + vec[i] + '\n');
//         }

//         str_vec.append(' ' + vec[vec.size() - 1]);
//     }

//     return str_vec;
// }

// const std::vector<std::string> weave(const std::vector<std::unordered_map<std::string, std::string>>& vec) noexcept
// {
//     std::vector<std::string> fabric;

//     for (const auto &item : vec[0])
//     {
//         fabric.push_back(item.first);
//     }

//     size_t pad = 2 + (*std::max_element(fabric.begin(), fabric.end(), [](const std::string& Lstr, const std::string& Rstr) { return Lstr.length() < Rstr.length(); })).length();

//     for (size_t i = 0; i < vec[0].size(); ++i)
//     {
//         std::stringstream ss;

//         ss << std::left << std::setw(pad) << fabric[i]
//            << "[" << std::setw(7);

//         // not clean but works to get either a weight number or string (like N/A)
//         if (fabric[i] == "EXTRA")
//         {
//             ss << vec[1].at(fabric[i]);
//         }
//         else
//         {
//             ss << std::setprecision(4) << std::strtod(vec[1].at(fabric[i]).c_str(), nullptr);
//         }
        
//         ss << "weight | "
//            << std::setw(3) << vec[2].at(fabric[i]) << " drops]"
//            << std::right << std::setw(0) << "  " << vec[0].at(fabric[i]);

//         // we overwrite with the full line.
//         fabric[i] = ss.str();
//     }

//     return fabric;
// }

// std::ostream& hyx::operator<<(std::ostream& os, const hyx::Course& course) noexcept
// {
//     std::stringstream ss;

//     ss << "Course Name: " << course.get_name() << '\n'
//         << "Institution: " << course.get_institution() << '\n'
//         << "Details: " << course.get_details() << '\n'
//         << "CRN: " << course.get_crn() << '\n'
//         << "Days: " << course.get_week_days() << '\n'
//         << "Time: " << to_ISO_time(course.get_start_time()) << " - " << to_ISO_time(course.get_end_time()) << '\n'
//         << "Date: " << to_ISO_date(course.get_start_date()) << " -- " << to_ISO_date(course.get_end_date()) << '\n'
//         << "Location: " << course.get_location() << '\n'
//         << "Instructor: " << course.get_instructor() << '\n'
//         << "Grade: " << ((course.get_grade() == -1) ? "N/A" : std::to_string(course.get_grade()) + "%") << '\n'
//         << "Letter: " << ((course.get_letter() == "") ? "N/A" : course.get_letter()) << '\n'
//         << "Credit Hours: " << course.get_units() << '\n'
//         << "Grade Points: "
//         << ((course.get_grade_points() == -1) ? "N/A" : std::to_string(course.get_grade_points())) << '\n'
//         << "Book(s):" << ((not course.get_books().empty()) ? "\n" : "") << vec_helper(course.get_books()) << '\n'
//         << "Points:" << ((course.get_points().empty()) ? "" : "\n" + vec_helper(weave({course.get_points(), course.get_weights(), course.get_drops()}))) << '\n';

//     return os << ss.str();
// }

bool hyx::weighted_course::has_good_weights() noexcept
{
    double total_weight = 0.0;

    std::for_each(this->m_grade.begin(), this->m_grade.end(),
        [&](const auto& itr) { total_weight += itr.second.weight; });

    return (std::abs(1 - total_weight) < std::numeric_limits<float>::epsilon()) ? true : false;
}

void hyx::weighted_course::update_grade() noexcept
{
    if (not this->is_withdrawn() && not this->is_replaced() && this->has_good_weights())
    {
        double final_grade = 0.0f;
        double unused_weight = 0.0f;

        for (const auto& itr_grade : this->m_grade)
        {
            // if there are points to calculate
            if (itr_grade.second.points_percent.empty())
            {
                unused_weight += itr_grade.second.weight;
            }
            else
            {
                hyx::grade_container grades{itr_grade.second};

                this->drop_grades(grades);

                this->replace_grades(grades);

                // if all of the grades have been dropped, then treat as if no grades were given
                if (grades.points_percent.empty())
                {
                    unused_weight += grades.weight;
                }
                else
                {
                    // sum group's grades and apply its weights
                    final_grade += grades.weight * (std::accumulate(grades.points_earned.begin(), grades.points_earned.end(), 0.0) / std::accumulate(grades.points_possible.begin(), grades.points_possible.end(), 0.0));
                }
            }
        }

        // update stats if there are grades left over.
        if ((1.0 - unused_weight) > std::numeric_limits<double>::epsilon())
        {
            this->m_grade_percent = final_grade * 100.0 / (1.0 - unused_weight) + this->m_extra;
            this->update_letter();
            this->update_grade_points();
        }
    }
}

hyx::weighted_course::weighted_course(
    std::string t_name,
    long t_crn,
    int t_units,
    grade_scale t_scale,
    hyx::school* t_institution,
    std::string t_location,
    std::string t_instructor,
    std::string t_subject,
    std::string t_level,
    std::string t_details,
    std::vector<week_day> t_week_days,
    std::array<int, 3> t_start_date,
    std::array<int, 3> t_end_date,
    std::array<int, 2> t_start_time,
    std::array<int, 2> t_end_time
) :
    course(
        t_name,
        t_crn,
        t_units,
        t_scale,
        t_institution,
        t_location,
        t_instructor,
        t_subject,
        t_level,
        t_details,
        t_week_days,
        t_start_date,
        t_end_date,
        t_start_time,
        t_end_time
    )
{
}

const std::unordered_map<std::string, std::string> hyx::weighted_course::get_weights() const noexcept
{
    std::unordered_map<std::string, std::string> vstr_weights;
    
    for (const auto& itr_grade : this->m_grade)
    {
        // only emplace grade categories with a name.
        if (not itr_grade.first.empty())
        {
            vstr_weights.emplace(itr_grade.first, std::to_string(itr_grade.second.weight));
        }
    }

    if (this->m_extra != 0)
    {
        vstr_weights.emplace("EXTRA", "N/A");
    }

    return vstr_weights;
}

bool hyx::weighted_course::add_category(std::string name, double weight, int drop, std::pair<int, std::string> replace) noexcept
{
    bool success{false};

    if (not this->is_withdrawn() && not this->is_replaced())
    {
        std::transform(name.begin(), name.end(), name.begin(),
            [](unsigned char c) { return toupper(c); });

        std::transform(replace.second.begin(), replace.second.end(), replace.second.begin(),
            [](unsigned char c) { return toupper(c); });

        this->m_grade[name];
        this->m_grade[name].weight = weight;
        this->m_grade[name].num_of_drops = drop;
        this->m_grade[name].num_of_replacements = replace.first;
        this->m_grade[name].replacement_name = replace.second;

        success = true;
    }

    return success;
}

double hyx::point_course::get_total_points() const noexcept
{
    return this->m_total_points;
}

bool hyx::point_course::add_category(std::string name, int drop, std::pair<int, std::string> replace) noexcept
{
    bool success{false};

    if (not this->is_withdrawn() && not this->is_replaced())
    {
        std::transform(name.begin(), name.end(), name.begin(),
            [](unsigned char c) { return toupper(c); });

        std::transform(replace.second.begin(), replace.second.end(), replace.second.begin(),
            [](unsigned char c) { return toupper(c); });

        this->m_grade[name];
        this->m_grade[name].num_of_drops = drop;
        this->m_grade[name].num_of_replacements = replace.first;
        this->m_grade[name].replacement_name = replace.second;

        success = true;
    }

    return success;
}

float hyx::get_GPA(const std::vector<hyx::course>& courses) noexcept
{
    return std::accumulate(courses.begin(), courses.end(), 0.0f, [](float sum, const hyx::course &crs) { return (crs.is_included_in_gpa()) ? sum + crs.get_grade_points() : sum; })
    / std::accumulate(courses.begin(), courses.end(), 0.0f, [](float sum, const hyx::course &crs) { return (crs.is_included_in_gpa()) ? sum + crs.get_units() : sum; });
}
