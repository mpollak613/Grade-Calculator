/* Copyright 2022 Michael Pollak.
 *
 * Use of this source code is governed by an MIT-style
 * licence that can be found in the LICENSE file.
 */

#ifndef HYX_SCALES_H
#define HYX_SCALES_H

#include <string>        // string
#include <unordered_map> // unordered_map

namespace hyx
{
    typedef std::unordered_map<std::string, std::pair<short, short>> grade_scale;

    typedef std::unordered_map<std::string, float> grade_point_scale;

    namespace scale
    {
        inline const static grade_scale STD({{"A", {90, 100}},
                                             {"B", {80, 89}},
                                             {"C", {70, 79}},
                                             {"D", {60, 69}},
                                             {"F", {0, 59}}});

        inline const static grade_scale G11({{"A", {90, 100}},
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
                                             {"F", {0, 49}}});

        inline const static grade_scale U12({{"A", {93, 100}},
                                             {"A-", {90, 92}},
                                             {"B+", {87, 89}},
                                             {"B", {83, 86}},
                                             {"B-", {80, 82}},
                                             {"C+", {77, 79}},
                                             {"C", {73, 76}},
                                             {"C-", {70, 72}},
                                             {"D", {60, 69}},
                                             {"F", {0, 59}}});

        inline const static grade_scale U11({{"A", {93, 100}},
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
                                             {"F", {0, 59}}});

        inline const static grade_scale PF({{"P", {60, 100}},
                                            {"NP", {0, 59}}});

        inline const static grade_point_scale UNI_GPSCALE({{"A", 4.0},
                                                           {"A-", 3.7},
                                                           {"B+", 3.3},
                                                           {"B", 3.0},
                                                           {"B-", 2.7},
                                                           {"C+", 2.3},
                                                           {"C", 2.0},
                                                           {"C-", 1.7},
                                                           {"D+", 1.3},
                                                           {"D", 1.0},
                                                           {"D-", 0.7},
                                                           {"F", 0.0}});

        inline const static grade_point_scale HIG_GPSCALE({{"A", 4.0},
                                                           {"B", 3.0},
                                                           {"C", 2.0},
                                                           {"D", 1.0},
                                                           {"F", 0.0}});
    }
}

#endif // ! HYX_SCALES_H