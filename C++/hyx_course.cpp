/* Copyright 2021 Michael Pollak.
 *
 * Use of this source code is governed by an MIT-style
 * licence that can be found in the LICENSE file.
 */

#include "hyx_course.h"

#include <algorithm> //transform, for_each, max_element
#include <cmath> //floor
#include <cstdlib> //strtod
#include <ctime> //tm, strftime
#include <functional> //divides
#include <iomanip> //setw, left, right
#include <iterator> //back_inserter
#include <numeric> //accumulate
#include <ostream> //ostream
#include <sstream> //stringstream
#include <string> //string

static const std::string vec_helper(std::vector<std::string> vec) noexcept;

const std::vector<std::string> weave(const std::vector<std::unordered_map<std::string, std::string>>& vec) noexcept;

static const std::string to_ISO_date(std::tm date_time) noexcept;

static const std::string to_ISO_time(std::tm date_time) noexcept;


std::ostream& hyx::operator<<(std::ostream& os, const hyx::Grade_scale& scale) noexcept
{
    for (auto& itr : scale)
    {
        os << std::left << std::setw(2) << itr.first << ": [" << itr.second.first << "-" << itr.second.second << "]\n";
    }

    return os;
}

const std::string vec_helper(std::vector<std::string> vec) noexcept
{
    std::string str_vec;

    if (not vec.empty())
    {
        for (size_t i = 0; i < vec.size() - 1; ++i)
        {
            str_vec.append(' ' + vec[i] + '\n');
        }

        str_vec.append(' ' + vec[vec.size() - 1]);
    }

    return str_vec;
}

const std::vector<std::string> weave(const std::vector<std::unordered_map<std::string, std::string>>& vec) noexcept
{
    std::vector<std::string> fabric;

    for (const auto &item : vec[0])
    {
        fabric.push_back(item.first);
    }

    size_t pad = 2 + (*std::max_element(fabric.begin(), fabric.end(), [](const std::string& Lstr, const std::string& Rstr) { return Lstr.length() < Rstr.length(); })).length();

    for (size_t i = 0; i < vec[0].size(); ++i)
    {
        std::stringstream ss;

        ss << std::left << std::setw(pad) << fabric[i]
           << "[" << std::setw(7);

        // not clean but works to get either a weight number or string (like N/A)
        if (fabric[i] == "EXTRA")
        {
            ss << vec[1].at(fabric[i]);
        }
        else
        {
            ss << std::setprecision(4) << std::strtod(vec[1].at(fabric[i]).c_str(), nullptr);
        }
        
        ss << "weight | "
           << std::setw(3) << vec[2].at(fabric[i]) << " drops]"
           << std::right << std::setw(0) << "  " << vec[0].at(fabric[i]);

        // we overwrite with the full line.
        fabric[i] = ss.str();
    }

    return fabric;
}

std::ostream& hyx::operator<<(std::ostream& os, const hyx::Course& course) noexcept
{
    std::stringstream ss;

    ss << "Course Name: " << course.get_name() << '\n'
        << "Institution: " << course.get_institution() << '\n'
        << "Details: " << course.get_details() << '\n'
        << "CRN: " << course.get_crn() << '\n'
        << "Days: " << course.get_week_days() << '\n'
        << "Time: " << to_ISO_time(course.get_start_time()) << " - " << to_ISO_time(course.get_end_time()) << '\n'
        << "Date: " << to_ISO_date(course.get_start_date()) << " -- " << to_ISO_date(course.get_end_date()) << '\n'
        << "Location: " << course.get_location() << '\n'
        << "Instructor: " << course.get_instructor() << '\n'
        << "Grade: " << ((course.get_grade() == -1) ? "N/A" : std::to_string(course.get_grade()) + "%") << '\n'
        << "Letter: " << ((course.get_letter() == "") ? "N/A" : course.get_letter()) << '\n'
        << "Credit Hours: " << course.get_units() << '\n'
        << "Grade Points: "
        << ((course.get_grade_points() == -1) ? "N/A" : std::to_string(course.get_grade_points())) << '\n'
        << "Book(s):" << ((not course.get_books().empty()) ? "\n" : "") << vec_helper(course.get_books()) << '\n'
        << "Points:" << ((course.get_points().empty()) ? "" : "\n" + vec_helper(weave({course.get_points(), course.get_weights(), course.get_drops()}))) << '\n';

    return os << ss.str();
}

std::ostream& hyx::operator<<(std::ostream& os, const hyx::CourseWLAB& course) noexcept
{
    std::stringstream ss;

    ss << "Course Name: " << course.get_name() << '\n'
        << "Institution: " << course.get_institution() << '\n'
        << "Details: " << course.get_details() << '\n'
        << "CRN: " << course.get_crn() << '\n'
        << "Days: " << course.get_week_days() << '\n'
        << "Lab Days: " << course.get_lab_week_days() << '\n'
        << "Time: " << to_ISO_time(course.get_start_time()) << " - " << to_ISO_time(course.get_end_time()) << '\n'
        << "Date: " << to_ISO_date(course.get_start_date()) << " - " << to_ISO_date(course.get_end_date()) << '\n'
        << "Lab Time: " << to_ISO_time(course.get_lab_start_time()) << " - " << to_ISO_time(course.get_lab_end_time()) << '\n'
        << "Lab Date: " << to_ISO_date(course.get_lab_start_date()) << " - " << to_ISO_date(course.get_lab_end_date()) << '\n'
        << "Location: " << course.get_location() << '\n'
        << "Lab Location: " << course.get_location() << '\n'
        << "Instructor: " << course.get_instructor() << '\n'
        << "Grade: " << ((course.get_grade() == -1) ? "N/A" : std::to_string(course.get_grade())) << '\n'
        << "Letter: " << ((course.get_letter() == "") ? "N/A" : course.get_letter()) << '\n'
        << "Credit Hours: " << course.get_units() << '\n'
        << "Grade Points: "
        << ((course.get_grade_points() == -1) ? "N/A" : std::to_string(course.get_grade_points())) << '\n'
        << "Book(s):" << ((not course.get_books().empty()) ? "\n" : "") << vec_helper(course.get_books()) << '\n'
        << "Points:" << ((course.get_points().empty()) ? "" : "\n" + vec_helper(weave({course.get_points(), course.get_weights(), course.get_drops()}))) << '\n';

    return os << ss.str();
}

void hyx::Course::update_letter() noexcept
{
    if (this->is_point_based())
    {
        for (auto &itr : this->scale_)
        {
             if (std::floor(this->grade_) >= (itr.second.first / this->base_points_ * 100) && std::floor(this->grade_) <= (itr.second.second / this->base_points_ * 100))
            {
                this->letter_ = itr.first;
            }
        }
    }
    else
    {
        for (auto &itr : this->scale_)
        {
            if (std::floor(this->grade_) >= itr.second.first && std::floor(this->grade_) <= itr.second.second)
            {
                this->letter_ = itr.first;
            }
        }
    }
}

void hyx::Course::update_grade_points() noexcept
{
    if (this->is_replaced() || this->is_withdrawn())
    {
        this->grade_points_ = -1.0;
    }
    else if (this->letter_ == "A" || this->letter_ == "A+")
    {
        this->grade_points_ = 4.0 * this->units_;
    }
    else if (this->letter_ == "A-")
    {
        this->grade_points_ = 3.7 * this->units_;
    }
    else if (this->letter_ == "B+")
    {
        this->grade_points_ = 3.3 * this->units_;
    }
    else if (this->letter_ == "B")
    {
        this->grade_points_ = 3.0 * this->units_;
    }
    else if (this->letter_ == "B-")
    {
        this->grade_points_ = 2.7 * this->units_;
    }
    else if (this->letter_ == "C+")
    {
        this->grade_points_ = 2.3 * this->units_;
    }
    else if (this->letter_ == "C")
    {
        this->grade_points_ = 2.0 * this->units_;
    }
    else if (this->letter_ == "C-")
    {
        this->grade_points_ = 1.7 * this->units_;
    }
    else if (this->letter_ == "D+")
    {
        this->grade_points_ = 1.3 * this->units_;
    }
    else if (this->letter_ == "D")
    {
        this->grade_points_ = 1.0 * this->units_;
    }
    else if (this->letter_ == "D-")
    {
        this->grade_points_ = 0.7 * this->units_;
    }
    else if (this->letter_ == "F")
    {
        this->grade_points_ = 0.0 * this->units_;
    }
}

bool hyx::Course::has_good_weights() noexcept
{
    double total_weight = 0.0f;

    std::for_each(this->points_.begin(), this->points_.end(),
        [&](auto& itr) { total_weight += std::get<2>(itr.second); });

    return (std::abs(1 - total_weight) < std::numeric_limits<float>::epsilon()) ? true : false;
}

bool hyx::Course::update_grade() noexcept
{
    if (not this->is_withdrawn() && not this->is_replaced() && (this->has_good_weights() || this->is_point_based()))
    {
        double final_grade = 0.0f;
        double unused_weight = 0.0f;

        // needed for point based courses.
        double total_earned_points = 0.0f;
        double total_possible_points = 0.0f;

        for (auto& itr : this->points_)
        {
            // if there are points to calculate
            if (std::get<0>(itr.second).empty())
            {
                unused_weight += std::get<2>(itr.second);
            }
            else
            {
                std::vector<double> points_earned(std::get<0>(itr.second));
                std::vector<double> points_poss(std::get<1>(itr.second));

                // if we need to drop or replace
                if (std::get<3>(itr.second) > 0 || std::get<4>(itr.second).first > 0)
                {
                    std::vector<double> points_perc;
                    std::transform(points_earned.begin(), points_earned.end(), points_poss.begin(), std::back_inserter(points_perc), std::divides<double>());//divide(points_earned, points_poss);
                
                    // drop lowest grades as necessary
                    for (int i = 0; i < std::get<3>(itr.second) && points_perc.size() != 0; ++i)
                    {
                        ptrdiff_t min_index = std::distance(points_perc.begin(), std::min_element(points_perc.begin(), points_perc.end()));
                    
                        points_perc.erase(points_perc.begin() + min_index);
                        points_earned.erase(points_earned.begin() + min_index);
                        points_poss.erase(points_poss.begin() + min_index);
                    }

                    // replace grades as necessary
                    for (int i = 0; i < std::get<4>(itr.second).first && this->points_.count(std::get<4>(itr.second).second) && not std::get<0>(this->points_[std::get<4>(itr.second).second]).empty() && not points_perc.empty(); ++i)
                    {
                        double repl_perc = std::get<0>(this->points_[std::get<4>(itr.second).second]).front() / std::get<1>(this->points_[std::get<4>(itr.second).second]).front();

                        if (repl_perc > *std::min_element(points_perc.begin(), points_perc.end()))
                        {
                            ptrdiff_t min_index = std::distance(points_perc.begin(), std::min_element(points_perc.begin(), points_perc.end()));

                            points_perc.erase(points_perc.begin() + min_index);
                            points_earned[min_index] = std::get<0>(this->points_[std::get<4>(itr.second).second]).front();
                            points_poss[min_index] = std::get<1>(this->points_[std::get<4>(itr.second).second]).front();
                        }
                    }
                }

                // if all of the grades have been dropped then treat as if no grades have been given
                if (points_earned.empty())
                {
                    unused_weight += std::get<2>(itr.second);
                }
                else
                {
                    if (this->is_point_based())
                    {
                        total_earned_points += std::accumulate(points_earned.begin(), points_earned.end(), 0.0);
                        total_possible_points += std::accumulate(points_poss.begin(), points_poss.end(), 0.0);
                    }
                    else
                    {
                        // sum group's grades and apply its weights
                        final_grade += std::get<2>(itr.second) * (std::accumulate(points_earned.begin(), points_earned.end(), 0.0) / std::accumulate(points_poss.begin(), points_poss.end(), 0.0));
                    }
                }
            }
        }

        // update stats if there are grades left over.
        if (unused_weight != 1 || total_possible_points != 0)
        {
            this->grade_ = (this->is_point_based()) ? ((total_earned_points + this->extra_) / total_possible_points) * 100 : final_grade * 100 / (1 - unused_weight) + this->extra_;
            this->update_letter();
            this->update_grade_points();
        }

        return true;
    }
    else
    {
        return false;
    }
}

hyx::Course::Course(
    std::string name,
    long crn,
    int units,
    Grade_scale scale,
    std::string institution,
    std::string location,
    std::string instructor,
    std::string details,
    std::array<bool, 8> week_days,
    std::array<int, 3> start_date,
    std::array<int, 3> end_date,
    std::array<int, 2> start_time,
    std::array<int, 2> end_time
) :
    name_(name),
    crn_(crn),
    units_(units),
    scale_(scale),
    institution_(institution),
    location_(location),
    instructor_(instructor),
    details_(details),
    week_days_(week_days),
    start_datetime_({ start_date[0], start_date[1], start_date[2], start_time[0], start_time[1] }),
    end_datetime_({ end_date[0], end_date[1], end_date[2], end_time[0], end_time[1] }),
    books_(),
    grade_(-1),
    letter_(),
    grade_points_(-1),
    points_(),
    extra_(0),
    base_points_(0)
{
}

const std::string& hyx::Course::get_name() const noexcept
{
    return this->name_;
}

long hyx::Course::get_crn() const noexcept
{
    return this->crn_;
}

int hyx::Course::get_units() const noexcept
{
    return this->units_;
}

const std::string hyx::Course::get_scale() const noexcept
{
    std::stringstream ss;

    ss << this->scale_;

    std::string str_scale = ss.str();

    if (not str_scale.empty())
    {
        str_scale.pop_back();
    }

    return str_scale;
}

const std::string& hyx::Course::get_institution() const noexcept
{
    return this->institution_;
}

const std::string& hyx::Course::get_location() const noexcept
{
    return this->location_;
}

const std::string& hyx::Course::get_instructor() const noexcept
{
    return this->instructor_;
}

const std::string& hyx::Course::get_details() const noexcept
{
    return this->details_;
}

const std::string week_day_helper(const std::array<bool, 8>& week_days)
{
    std::string str_week_days;

    std::string LUT_week_days = { '-', 'U', 'M', 'T', 'W', 'R', 'F', 'S' };

    int i = 0;

    // if we start with monday - skip checking sunday for now.
    if (week_days[7] == true)
    {
        ++i;
    }

    for (; i < 7; ++i)
    {
        str_week_days.push_back(LUT_week_days[week_days[i] * (i + 1)]);
    }

    // if we start with monday - we can check sunday now.
    if (week_days[7] == true)
    {
        str_week_days.push_back(LUT_week_days[week_days[0] * 1]);
    }

    return str_week_days;
}

const std::string hyx::Course::get_week_days() const noexcept
{
    return week_day_helper(this->week_days_);
}

const std::string to_ISO_date(std::tm date) noexcept
{
    char buff[11];

    std::strftime(buff, 11, "%Y-%m-%d", &date);

    return buff;
}

const std::tm hyx::Course::get_start_date() const noexcept
{
    std::tm date{};
    date.tm_year = this->start_datetime_[0] - 1900;
    date.tm_mon = this->start_datetime_[1] - 1;
    date.tm_mday = this->start_datetime_[2];

    return date;
}

const std::tm hyx::Course::get_end_date() const noexcept
{
    std::tm date{};
    date.tm_year = this->end_datetime_[0] - 1900;
    date.tm_mon = this->end_datetime_[1] - 1;
    date.tm_mday = this->end_datetime_[2];

    return date;
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

const std::tm hyx::Course::get_start_time() const noexcept
{
    std::tm time{};
    time.tm_hour = this->start_datetime_[3];
    time.tm_min = this->start_datetime_[4];

    return time;
}

const std::tm hyx::Course::get_end_time() const noexcept
{
    std::tm time{};
    time.tm_hour = this->end_datetime_[3];
    time.tm_min = this->end_datetime_[4];

    return time;
}

const std::vector<std::string>& hyx::Course::get_books() const noexcept
{
    return this->books_;
}

double hyx::Course::get_grade() const noexcept
{
    return this->grade_;
}

const std::string hyx::Course::get_letter() const noexcept
{
    return this->letter_;
}

float hyx::Course::get_grade_points() const noexcept
{
    return this->grade_points_;
}

const std::unordered_map<std::string, std::string> hyx::Course::get_points() const noexcept
{
    std::unordered_map<std::string, std::string> vstr_points;

    for (auto& itr : this->points_)
    {
        if (itr.first != "")
        {
            std::stringstream ss;

            // both earned and possible point vectors WILL always be the same size.
            for (size_t i = 0; i < std::get<0>(itr.second).size(); ++i)
            {
                ss << std::get<0>(itr.second)[i] << '/' << std::get<1>(itr.second)[i];

                if (i != std::get<0>(itr.second).size() - 1)
                {
                    ss << ", ";
                }
            }

            vstr_points.emplace(itr.first, ss.str());
        }
    }

    if (this->extra_ != 0)
    {

        vstr_points.emplace("EXTRA", std::to_string(this->extra_) + ((this->is_point_based()) ? "" : "%"));
    }

    return vstr_points;
}

const std::unordered_map<std::string, std::string> hyx::Course::get_weights() const noexcept
{
    std::unordered_map<std::string, std::string> vstr_weights;
    
    for (auto& itr : this->points_)
    {
        if (itr.first != "")
        {
            vstr_weights.emplace(itr.first, std::to_string(std::get<2>(itr.second)));
        }
    }

    if (this->extra_ != 0)
    {

        vstr_weights.emplace("EXTRA", "N/A");
    }

    return vstr_weights;
}

const std::unordered_map<std::string, std::string> hyx::Course::get_drops() const noexcept
{
    std::unordered_map<std::string, std::string> vstr_drops;

    for (auto& itr : this->points_)
    {
        if (itr.first != "")
        {

            vstr_drops.emplace(itr.first, std::to_string(std::get<3>(itr.second)));
        }
    }

    if (this->extra_ != 0)
    {

        vstr_drops.emplace("EXTRA", "N/A");
    }

    return vstr_drops;
}

bool hyx::Course::is_withdrawn() const noexcept
{
    return this->get_letter() == "W";
}

bool hyx::Course::is_replaced() const noexcept
{
    return this->get_letter() == "R";
}

bool hyx::Course::is_incomplete() const noexcept
{
    return this->get_letter() == "I";
}

bool hyx::Course::is_included_in_gpa() const noexcept
{
    return !(this->is_withdrawn() || this->is_replaced() || this->is_incomplete() || this->get_grade_points() == -1.0f || this->scale_ == hyx::scale::PF);
}

bool hyx::Course::is_point_based() const noexcept
{
    return this->base_points_ != 0.0;
}

void hyx::Course::set_withdrawn() noexcept
{
    this->grade_ = 0;

    this->letter_ = "W";
    this->update_grade_points();
}

void hyx::Course::set_replaced() noexcept
{
    this->letter_ = "R";

    this->update_grade_points();
}

void hyx::Course::set_incomplete() noexcept
{
    this->letter_ = "I";

    this->update_grade_points();
}

void hyx::Course::set_pass_fail() noexcept
{
    this->scale_ = hyx::scale::PF;

    this->update_grade();
}

void hyx::Course::set_point_based(double total_base_points) noexcept
{
    this->base_points_ = total_base_points;
}

bool hyx::Course::add_book(std::string book) noexcept
{
    if (not this->is_withdrawn() && not this->is_replaced())
    {
        this->books_.push_back(book);

        return true;
    }

    return false;
}

bool hyx::Course::add_category(std::string name, double weight, int drop, std::pair<int, std::string> replace)
{
    if (not this->is_withdrawn() && not this->is_replaced())
    {
        std::transform(name.begin(), name.end(), name.begin(),
            [](unsigned char c) { return toupper(c); });

        std::transform(replace.second.begin(), replace.second.end(), replace.second.begin(),
            [](unsigned char c) { return toupper(c); });

        this->points_[name];
        std::get<2>(this->points_[name]) = weight;
        std::get<3>(this->points_[name]) = drop;
        std::get<4>(this->points_[name]) = replace;

        return true;
    }

    return false;
}

bool hyx::Course::add_grade(std::string name, double earn, double poss) noexcept
{
    std::transform(name.begin(), name.end(), name.begin(),
        [](unsigned char c) { return toupper(c); });

    if (this->points_.count(name) && not this->is_withdrawn() && not this->is_replaced())
    {
        std::get<0>(this->points_[name]).push_back(earn);
        std::get<1>(this->points_[name]).push_back(poss);

        this->update_grade();

        return true;
    }
    
    return false;
}

void hyx::Course::add_extra_to_total(double extra)
{
    this->extra_ += extra;

    this->update_grade();
}

hyx::CourseWLAB::CourseWLAB(
    std::string name,
    long crn,
    int units,
    Grade_scale scale,
    std::string institution,
    std::string location,
    std::string lab_location,
    std::string instructor,
    std::string details,
    std::array<bool, 8> week_days,
    std::array<bool, 8> lab_week_days,
    std::array<int, 3> start_date,
    std::array<int, 3> end_date,
    std::array<int, 3> lab_start_date,
    std::array<int, 3> lab_end_date,
    std::array<int, 2> start_time,
    std::array<int, 2> end_time,
    std::array<int, 2> lab_start_time,
    std::array<int, 2> lab_end_time
) :
    Course(
        name,
        crn,
        units,
        scale,
        institution,
        location,
        instructor,
        details,
        week_days,
        start_date,
        end_date,
        start_time,
        end_time),
    lab_location_(lab_location),
    lab_week_days_(lab_week_days),
    lab_start_datetime_({ lab_start_date[0], lab_start_date[1], lab_start_date[2], lab_start_time[0], lab_start_time[1] }),
    lab_end_datetime_({ lab_end_date[0], lab_end_date[1], lab_end_date[2], lab_end_time[0], lab_end_time[1] })
{
}

const std::string& hyx::CourseWLAB::get_lab_location() const noexcept
{
    return this->lab_location_;
}

const std::string hyx::CourseWLAB::get_lab_week_days() const noexcept
{
    return week_day_helper(this->lab_week_days_);
}

const std::tm hyx::CourseWLAB::get_lab_start_date() const noexcept
{
    std::tm date{};
    date.tm_year = this->lab_start_datetime_[0] - 1900;
    date.tm_mon = this->lab_start_datetime_[1] - 1;
    date.tm_mday = this->lab_start_datetime_[2];

    return date;
}

const std::tm hyx::CourseWLAB::get_lab_end_date() const noexcept
{
    std::tm date{};
    date.tm_year = this->lab_end_datetime_[0] - 1900;
    date.tm_mon = this->lab_end_datetime_[1] - 1;
    date.tm_mday = this->lab_end_datetime_[2];

    return date;
}

const std::tm hyx::CourseWLAB::get_lab_start_time() const noexcept
{
    std::tm time{};
    time.tm_hour = this->lab_start_datetime_[3];
    time.tm_min = this->lab_start_datetime_[4];

    return time;
}

const std::tm hyx::CourseWLAB::get_lab_end_time() const noexcept
{
    std::tm time{};
    time.tm_hour = this->lab_end_datetime_[3];
    time.tm_min = this->lab_end_datetime_[4];

    return time;
}

float hyx::get_GPA(const std::vector<hyx::Course>& courses) noexcept
{
    return std::accumulate(courses.begin(), courses.end(), 0.0f, [](float sum, const hyx::Course &crs) { return (crs.is_included_in_gpa()) ? sum + crs.get_grade_points() : sum; })
    / std::accumulate(courses.begin(), courses.end(), 0.0f, [](float sum, const hyx::Course &crs) { return (crs.is_included_in_gpa()) ? sum + crs.get_units() : sum; });
}
