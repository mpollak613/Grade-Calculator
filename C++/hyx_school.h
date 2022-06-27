/* Copyright 2022 Michael Pollak.
 *
 * Use of this source code is governed by an MIT-style
 * licence that can be found in the LICENSE file.
 */

#ifndef HYX_SCHOOL_H
#define HYX_SCHOOL_H

#include "hyx_scales.h"

#include <string> // string

namespace hyx
{
    class school
    {
    private:
        std::string m_name;
        grade_point_scale m_scale;

    public:
        school(
            std::string t_name,
            grade_point_scale t_scale);

        [[nodiscard]] const std::string &get_name() const noexcept;

        [[nodiscard]] const grade_point_scale &get_scale() const noexcept;
    };
}

#endif // ! HYX_SCHOOL_H