/* Copyright 2022 Michael Pollak.
 *
 * Use of this source code is governed by an MIT-style
 * licence that can be found in the LICENSE file.
 */

#include "hyx_school.h"

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

const hyx::grade_point_scale& hyx::school::get_scale() const noexcept
{
    return this->m_scale;
}