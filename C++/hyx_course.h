/* Copyright 2021 Michael Pollak.
 *
 * Use of this source code is governed by an MIT-style
 * licence that can be found in the LICENSE file.
 */

#ifndef HYX_COURSE_H
#define HYX_COURSE_H

#include <array> // array
#include <climits> // INT_MAX
#include <ostream> // ostream
#include <string> // string
#include <tuple> // tuple
#include <unordered_map> // unordered_map
#include <utility> // pair
#include <vector> // vector


namespace hyx
{
    typedef std::unordered_map<std::string, std::pair<int, int>> Grade_scale;

    namespace scale
    {
        inline Grade_scale STD({
            {"A", {90, INT_MAX}},
            {"B", {80, 89}},
            {"C", {70, 79}},
            {"D", {60, 69}},
            {"F", {0, 59}}
            });

        inline Grade_scale G11({
            {"A", {90, INT_MAX}},
            {"A-", {85, 89}},
            {"B+", {80, 84}},
            {"B", {75, 79}},
            {"B-", {70, 74}},
            {"C+", {67, 69}},
            {"C", {64, 66}},
            {"C-", {60, 63}},
            {"D+", {57, 59}},
            {"D", {54, 56}},
            {"D-", {50, 53}},
            {"F", {0, 49}}
            });

        inline Grade_scale U12({
            {"A", {93, INT_MAX}},
            {"A-", {90, 92}},
            {"B+", {87, 89}},
            {"B", {83, 86}},
            {"B-", {80, 82}},
            {"C+", {77, 79}},
            {"C", {73, 76}},
            {"C-", {70, 72}},
            {"D", {60, 69}},
            {"F", {0, 59}}
            });

        inline Grade_scale U11({
            {"A", {93, INT_MAX}},
            {"A-", {90, 92}},
            {"B+", {87, 89}},
            {"B", {83, 86}},
            {"B-", {80, 82}},
            {"C+", {77, 79}},
            {"C", {73, 76}},
            {"C-", {70, 72}},
            {"D+", {67, 69}},
            {"D", {63, 66}},
            {"D-", {60, 62}},
            {"F", {0, 59}}
            });

        inline Grade_scale PF({
            {"P", {60, INT_MAX}},
            {"NP", {0, 59}}
            });
    }

    class Course
    {
    public:
        // name; earned points, possible points, weight, drops, (replacements, name to replace with)
        typedef std::unordered_map<std::string, std::tuple<std::vector<double>, std::vector<double>, double, int, std::pair<int, std::string>>> Grade_container;

    private:

        std::string name_;
        long crn_;
        int units_;
        Grade_scale scale_;
        std::string institution_;
        std::string location_;
        std::string instructor_;
        std::string details_;
        std::array<bool, 8> week_days_;
        std::array<int, 5> start_datetime_;
        std::array<int, 5> end_datetime_;

        std::vector<std::string> books_;
        double grade_;
        std::string letter_;
        float grade_points_;
        Grade_container points_;
        double extra_;
        double base_points_;

        void update_letter() noexcept;

        void update_grade_points() noexcept;

        bool has_good_weights() noexcept;

        bool update_grade() noexcept;

    public:

        Course(
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
            std::array<int, 2> start_time = { -1, -1 },
            std::array<int, 2> end_time = { -1, -1 }
        );

        [[nodiscard]] const std::string& get_name() const noexcept;

        [[nodiscard]] long get_crn() const noexcept;

        [[nodiscard]] int get_units() const noexcept;

        [[nodiscard]] const std::string get_scale() const noexcept;

        [[nodiscard]] const std::string& get_institution() const noexcept;

        [[nodiscard]] const std::string& get_location() const noexcept;

        [[nodiscard]] const std::string& get_instructor() const noexcept;

        [[nodiscard]] const std::string& get_details() const noexcept;

        [[nodiscard]] const std::string get_week_days() const noexcept;

        [[nodiscard]] const std::string get_start_date() const noexcept;

        [[nodiscard]] const std::string get_end_date() const noexcept;

        [[nodiscard]] const std::string get_start_time() const noexcept;

        [[nodiscard]] const std::string get_end_time() const noexcept;

        [[nodiscard]] const std::vector<std::string>& get_books() const noexcept;

        [[nodiscard]] double get_grade() const noexcept;

        [[nodiscard]] const std::string get_letter() const noexcept;

        [[nodiscard]] float get_grade_points() const noexcept;

        [[nodiscard]] const std::unordered_map<std::string, std::string> get_points() const noexcept;

        [[nodiscard]] const std::unordered_map<std::string, std::string> get_weights() const noexcept;

        [[nodiscard]] const std::unordered_map<std::string, std::string> get_drops() const noexcept;

        bool is_withdrawn() const noexcept;

        bool is_replaced() const noexcept;

        bool is_incomplete() const noexcept;

        bool is_included_in_gpa() const noexcept;

        bool is_point_based() const noexcept;

        void set_withdrawn() noexcept;

        void set_replaced() noexcept;

        void set_incomplete() noexcept;

        void set_pass_fail() noexcept;

        void set_point_based(double total_base_points) noexcept;

        bool add_book(std::string book) noexcept;

        bool add_category(std::string name, double weight = 0, int drop = 0, std::pair<int, std::string> replace = { 0, "" });
    
        bool add_grade(std::string name, double earn, double poss) noexcept;

        void add_extra_to_total(double extra);

    };

    class CourseWLAB
        : public Course
    {
    private:

        std::string lab_location_;
        std::array<bool, 8> lab_week_days_;
        std::array<int, 5> lab_start_datetime_;
        std::array<int, 5> lab_end_datetime_;

    public:

        CourseWLAB(
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
            std::array<int, 2> start_time = { -1, -1 },
            std::array<int, 2> end_time = { -1, -1 },
            std::array<int, 2> lab_start_time = { -1, -1 },
            std::array<int, 2> lab_end_time = { -1, -1 }
        );

        [[nodiscard]] const std::string& get_lab_location() const noexcept;

        [[nodiscard]] const std::string get_lab_week_days() const noexcept;

        [[nodiscard]] const std::string get_lab_start_date() const noexcept;

        [[nodiscard]] const std::string get_lab_end_date() const noexcept;

        [[nodiscard]] const std::string get_lab_start_time() const noexcept;

        [[nodiscard]] const std::string get_lab_end_time() const noexcept;

    };

    [[nodiscard]] float get_GPA(const std::vector<hyx::Course>& courses) noexcept;

    std::ostream& operator<< (std::ostream& os, const hyx::Grade_scale& scale) noexcept;

    std::ostream& operator<< (std::ostream& os, const hyx::Course& course) noexcept;

    std::ostream& operator<< (std::ostream& os, const hyx::CourseWLAB& course) noexcept;

} // hyx

#endif // !HYX_COURSE_H