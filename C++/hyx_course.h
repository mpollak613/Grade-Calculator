/* Copyright 2021-2022 Michael Pollak.
 *
 * Use of this source code is governed by an MIT-style
 * licence that can be found in the LICENSE file.
 */

#ifndef HYX_COURSE_H
#define HYX_COURSE_H

#include "hyx_school.h"

#include <array>      // array
#include <ctime>      // tm
#include <functional> // reference_wrapper
#include <string>     // string
#include <tuple>      // tuple
#include <utility>    // pair
#include <vector>     // vector

namespace hyx
{
    enum class week_day
    {
        NO_DAY = 0x0,
        SUNDAY = 0x1,
        MONDAY = 0x2,
        TUESDAY = 0x4,
        WEDNESDAY = 0x8,
        THURSDAY = 0x10,
        FRIDAY = 0x20,
        SATURDAY = 0x40,
        SUN = SUNDAY,
        MON = MONDAY,
        TUE = TUESDAY,
        WED = WEDNESDAY,
        THU = THURSDAY,
        FRI = FRIDAY,
        SAT = SATURDAY,
        TUESDAY_THURSDAY = TUESDAY | THURSDAY,
        MONDAY_WEDNESDAY_FRIDAY = MONDAY | WEDNESDAY | FRIDAY,
        TUE_THU = TUESDAY_THURSDAY,
        MON_WED_FRI = MONDAY_WEDNESDAY_FRIDAY,
        ALL_WEEK_DAYS_NFRIDAY = MONDAY | TUESDAY | WEDNESDAY | THURSDAY,
        ALL_WEEK_DAYS_NFRI = ALL_WEEK_DAYS_NFRIDAY,
        ALL_WEEK_DAYS = ALL_WEEK_DAYS_NFRI | FRIDAY,
        ALL_DAYS = SUNDAY | ALL_WEEK_DAYS | SATURDAY
    };

    constexpr week_day operator| (week_day lhs, week_day rhs)
    {
        using underlying_t = typename std::underlying_type<week_day>::type;

        return static_cast<week_day>(
            static_cast<underlying_t>(lhs) | static_cast<underlying_t>(rhs));
    }

    constexpr week_day operator& (week_day lhs, week_day rhs)
    {
        using underlying_t = typename std::underlying_type<week_day>::type;

        return static_cast<week_day>(
            static_cast<underlying_t>(lhs) & static_cast<underlying_t>(rhs));
    }

    struct grade_container
    {
        size_t num_of_replacements;

        std::string replacement_name;

        std::vector<double> points_earned;

        std::vector<double> points_possible;

        std::vector<double> points_percent;

        size_t num_of_drops;

        double weight;
    };

    class course
    {
    private:
        std::string m_name;
        long m_crn;
        int m_units;
        hyx::grade_scale m_scale;
        const hyx::school* m_institution;
        std::string m_location;
        std::string m_instructor;
        std::string m_subject_level;
        std::string m_details;
        hyx::week_day m_week_days;
        std::tm m_start_datetime;
        std::tm m_end_datetime;
        
        std::vector<std::string> m_book_list;
        std::string m_grade_letter;
        float m_grade_points;

        std::vector<std::reference_wrapper<const hyx::course>> m_linked_courses;

    protected:
        std::unordered_map<std::string, hyx::grade_container> m_grade;

        double m_extra;

        double m_grade_percent;

        void update_letter() noexcept;

        bool update_grade_points() noexcept;

        void drop_grades(hyx::grade_container& grades) noexcept;

        void replace_grades(hyx::grade_container& grades) noexcept;

        void virtual update_grade() = 0;

        course(
            const std::string t_name,
            const long t_crn,
            const int t_units,
            const hyx::grade_scale &t_scale,
            const hyx::school* t_institution,
            const std::string t_location,
            const std::string t_instructor,
            const std::string t_subject,
            const std::string t_level,
            const std::string t_details,
            const hyx::week_day t_week_days = hyx::week_day::NO_DAY,
            const std::array<int, 3> &t_start_date = {-1, -1, -1},
            const std::array<int, 3> &t_end_date = {-1, -1, -1},
            const std::array<int, 2> &t_start_time = { -1, -1 },
            const std::array<int, 2> &t_end_time = { -1, -1 }
        );

    public:

        [[nodiscard]] const std::string& get_name() const noexcept;

        [[nodiscard]] long get_crn() const noexcept;

        [[nodiscard]] int get_units() const noexcept;

        [[nodiscard]] const std::string get_scale() const noexcept;

        [[nodiscard]] const std::string &get_location() const noexcept;

        [[nodiscard]] const std::string &get_instructor() const noexcept;

        [[nodiscard]] const std::string get_subject_level() const noexcept;

        [[nodiscard]] const std::string &get_details() const noexcept;

        [[nodiscard]] const std::string get_week_days() const noexcept;

        [[nodiscard]] const std::tm &get_start_datetime() const noexcept;

        [[nodiscard]] const std::tm &get_end_datetime() const noexcept;

        [[nodiscard]] const std::vector<std::string> &get_books() const noexcept;

        [[nodiscard]] double get_grade() const noexcept;

        [[nodiscard]] const std::unordered_map<std::string, std::string> get_points() const noexcept;

        [[nodiscard]] const std::unordered_map<std::string, std::string> get_drops() const noexcept;

        [[nodiscard]] const std::string get_letter() const noexcept;

        [[nodiscard]] float get_grade_points() const noexcept;

        void link_course(hyx::course &course) noexcept;

        [[nodiscard]] bool is_withdrawn() const noexcept;

        [[nodiscard]] bool is_replaced() const noexcept;

        [[nodiscard]] bool is_incomplete() const noexcept;

        [[nodiscard]] bool is_included_in_gpa() const noexcept;

        void set_withdrawn() noexcept;

        void set_replaced() noexcept;

        void set_incomplete() noexcept;

        void set_pass_fail() noexcept;

        bool add_book(const std::string &book) noexcept;
    
        bool add_grade(std::string name, double earn, double poss) noexcept;

        //bool virtual add_category() = 0;

        void add_extra_to_total(double extra);
    };

    class weighted_course : public course
    {
    private:
        bool has_good_weights() noexcept;

        void update_grade() noexcept;

    public:
        weighted_course(
            const std::string t_name,
            const long t_crn,
            const int t_units,
            const hyx::grade_scale &t_scale,
            const hyx::school* t_institution,
            const std::string t_location,
            const std::string t_instructor,
            const std::string t_subject,
            const std::string t_level,
            const std::string t_details,
            const hyx::week_day t_week_days = hyx::week_day::NO_DAY,
            const std::array<int, 3> &t_start_date = {-1, -1, -1},
            const std::array<int, 3> &t_end_date = {-1, -1, -1},
            const std::array<int, 2> &t_start_time = { -1, -1 },
            const std::array<int, 2> &t_end_time = { -1, -1 }
        );

        [[nodiscard]] const std::unordered_map<std::string, std::string> get_weights() const noexcept;

        bool add_category(std::string name, double weight, int drop = 0, std::pair<int, std::string> replace = { 0, "" }) noexcept;
    };

    class point_course : public course
    {
    private:
        double m_total_points;

        void update_grade() noexcept;

    public:
        point_course(
            const std::string t_name,
            const long t_crn,
            const int t_units,
            const double t_total_points,
            const hyx::grade_scale &t_scale,
            const hyx::school* t_institution,
            const std::string t_location,
            const std::string t_instructor,
            const std::string t_subject,
            const std::string t_level,
            const std::string t_details,
            const hyx::week_day t_week_days = hyx::week_day::NO_DAY,
            const std::array<int, 3> &t_start_date = {-1, -1, -1},
            const std::array<int, 3> &t_end_date = {-1, -1, -1},
            const std::array<int, 2> &t_start_time = { -1, -1 },
            const std::array<int, 2> &t_end_time = { -1, -1 }
        );

        [[nodiscard]] double get_total_points() const noexcept;

        bool add_category(std::string name, int drop = 0, std::pair<int, std::string> replace = { 0, "" }) noexcept;
    };

    [[nodiscard]] float get_GPA(const std::vector<hyx::course>& courses) noexcept;

    std::ostream& operator<< (std::ostream& os, const hyx::grade_scale& scale) noexcept;

    std::ostream& operator<< (std::ostream& os, const hyx::course& course) noexcept;

    std::ostream& operator<< (std::ostream& os, const hyx::point_course& course) noexcept;

} // hyx

#endif // !HYX_COURSE_H