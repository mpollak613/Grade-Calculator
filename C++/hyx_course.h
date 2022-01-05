/* Copyright 2021 Michael Pollak.
 *
 * Use of this source code is governed by an MIT-style
 * licence that can be found in the LICENSE file.
 */

#ifndef HYX_COURSE_H
#define HYX_COURSE_H


#include <array>         // array
#include <ctime>         // tm
#include <string>        // string
#include <tuple>         // tuple
#include <unordered_map> // unordered_map
#include <utility>       // pair
#include <vector>        // vector


namespace hyx
{

    enum week_day
    {
        NO_DAY,
        SUNDAY,
        MONDAY,
        TUESDAY,
        WEDNESDAY,
        THURSDAY,
        FRIDAY,
        SATURDAY
    };
    
    typedef std::unordered_map<std::string, std::pair<int, int>> grade_scale;

    namespace scale
    {
        inline const static grade_scale STD({
            {"A", {90, 100}},
            {"B", {80, 89}},
            {"C", {70, 79}},
            {"D", {60, 69}},
            {"F", {0, 59}}
            });

        inline const static grade_scale G11({
            {"A", {90, 100}},
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

        inline const static grade_scale U12({
            {"A", {93, 100}},
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

        inline const static grade_scale U11({
            {"A", {93, 100}},
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

        inline const static grade_scale PF({
            {"P", {60, 100}},
            {"NP", {0, 59}}
            });
    }

    class school
    {
    public:
        typedef std::unordered_map<std::string, float> grade_point_scale;

    private:
        std::string m_name;
        grade_point_scale m_scale;

    public:
        school(
            std::string t_name,
            grade_point_scale t_scale
            );

        [[nodiscard]] const std::string& get_name() const noexcept;

        [[nodiscard]] const grade_point_scale& get_scale() const noexcept;
    };

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
        grade_scale m_scale;
        hyx::school* m_institution;
        std::string m_location;
        std::string m_instructor;
        std::string m_subject_level;
        std::string m_details;
        std::vector<week_day> m_week_days;
        std::tm m_start_datetime;
        std::tm m_end_datetime;
        
        std::vector<std::string> m_book_list;
        std::string m_grade_letter;
        float m_grade_points;

    protected:
        std::unordered_map<std::string, grade_container> m_grade;

        double m_extra;

        double m_grade_percent;

        void update_letter() noexcept;

        bool update_grade_points() noexcept;

        void drop_grades(grade_container& grades) noexcept;

        void replace_grades(grade_container& grades) noexcept;

        void virtual update_grade() = 0;

    public:
        course(
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
            std::vector<week_day> t_week_days = {},
            std::array<int, 3> t_start_date = {-1, -1, -1},
            std::array<int, 3> t_end_date = {-1, -1, -1},
            std::array<int, 2> t_start_time = { -1, -1 },
            std::array<int, 2> t_end_time = { -1, -1 }
        );

        [[nodiscard]] const std::string& get_name() const noexcept;

        [[nodiscard]] long get_crn() const noexcept;

        [[nodiscard]] int get_units() const noexcept;

        [[nodiscard]] const std::string get_scale() const noexcept;

        [[nodiscard]] const std::string& get_location() const noexcept;

        [[nodiscard]] const std::string& get_instructor() const noexcept;

        [[nodiscard]] const std::string get_subject_level() const noexcept;

        [[nodiscard]] const std::string& get_details() const noexcept;

        [[nodiscard]] const std::string get_week_days() const noexcept;

        [[nodiscard]] const std::tm get_start_datetime() const noexcept;

        [[nodiscard]] const std::tm get_end_datetime() const noexcept;

        [[nodiscard]] const std::vector<std::string>& get_books() const noexcept;

        [[nodiscard]] double get_grade() const noexcept;

        [[nodiscard]] const std::unordered_map<std::string, std::string> get_points() const noexcept;

        [[nodiscard]] const std::unordered_map<std::string, std::string> get_drops() const noexcept;

        [[nodiscard]] const std::string get_letter() const noexcept;

        [[nodiscard]] float get_grade_points() const noexcept;

        bool is_withdrawn() const noexcept;

        bool is_replaced() const noexcept;

        bool is_incomplete() const noexcept;

        bool is_included_in_gpa() const noexcept;

        void set_withdrawn() noexcept;

        void set_replaced() noexcept;

        void set_incomplete() noexcept;

        void set_pass_fail() noexcept;

        bool add_book(std::string book) noexcept;
    
        bool add_grade(std::string name, double earn, double poss) noexcept;

        //bool virtual add_category() = 0;

        void add_extra_to_total(double extra);

    };

    class weighted_course
        : public course
    {
    private:
        bool has_good_weights() noexcept;

        void update_grade() noexcept;

    public:
        weighted_course(
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
            std::vector<week_day> t_week_days = {},
            std::array<int, 3> t_start_date = {-1, -1, -1},
            std::array<int, 3> t_end_date = {-1, -1, -1},
            std::array<int, 2> t_start_time = { -1, -1 },
            std::array<int, 2> t_end_time = { -1, -1 }
        );

        [[nodiscard]] const std::unordered_map<std::string, std::string> get_weights() const noexcept;

        bool add_category(std::string name, double weight, int drop = 0, std::pair<int, std::string> replace = { 0, "" }) noexcept;

    };

    class point_course
        : public course
    {
    private:
        double m_total_points;

        void update_grade() noexcept;

    public:
        point_course(
            std::string t_name,
            long t_crn,
            int t_units,
            double total_points,
            grade_scale t_scale,
            hyx::school* t_institution,
            std::string t_location,
            std::string t_instructor,
            std::string t_details,
            std::vector<week_day> t_week_days = {},
            std::array<int, 3> t_start_date = {-1, -1, -1},
            std::array<int, 3> t_end_date = {-1, -1, -1},
            std::array<int, 2> t_start_time = { -1, -1 },
            std::array<int, 2> t_end_time = { -1, -1 }
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