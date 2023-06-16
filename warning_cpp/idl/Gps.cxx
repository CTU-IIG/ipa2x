// Copyright 2016 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/*!
 * @file Gps.cpp
 * This source file contains the definition of the described types in the IDL file.
 *
 * This file was generated by the tool gen.
 */

#ifdef _WIN32
// Remove linker warning LNK4221 on Visual Studio
namespace {
char dummy;
}  // namespace
#endif  // _WIN32

#include "Gps.h"
#include <fastcdr/Cdr.h>

#include <fastcdr/exceptions/BadParamException.h>
using namespace eprosima::fastcdr::exception;

#include <utility>

#define gps_max_cdr_typesize 16ULL;
#define gps_max_key_cdr_typesize 0ULL;

gps::gps()
{
    // double m_longitude
    m_longitude = 0.0;
    // double m_latitude
    m_latitude = 0.0;

}

gps::~gps()
{


}

gps::gps(
        const gps& x)
{
    m_longitude = x.m_longitude;
    m_latitude = x.m_latitude;
}

gps::gps(
        gps&& x) noexcept 
{
    m_longitude = x.m_longitude;
    m_latitude = x.m_latitude;
}

gps& gps::operator =(
        const gps& x)
{

    m_longitude = x.m_longitude;
    m_latitude = x.m_latitude;

    return *this;
}

gps& gps::operator =(
        gps&& x) noexcept
{

    m_longitude = x.m_longitude;
    m_latitude = x.m_latitude;

    return *this;
}

bool gps::operator ==(
        const gps& x) const
{

    return (m_longitude == x.m_longitude && m_latitude == x.m_latitude);
}

bool gps::operator !=(
        const gps& x) const
{
    return !(*this == x);
}

size_t gps::getMaxCdrSerializedSize(
        size_t current_alignment)
{
    static_cast<void>(current_alignment);
    return gps_max_cdr_typesize;
}

size_t gps::getCdrSerializedSize(
        const gps& data,
        size_t current_alignment)
{
    (void)data;
    size_t initial_alignment = current_alignment;


    current_alignment += 8 + eprosima::fastcdr::Cdr::alignment(current_alignment, 8);


    current_alignment += 8 + eprosima::fastcdr::Cdr::alignment(current_alignment, 8);



    return current_alignment - initial_alignment;
}

void gps::serialize(
        eprosima::fastcdr::Cdr& scdr) const
{

    scdr << m_longitude;
    scdr << m_latitude;

}

void gps::deserialize(
        eprosima::fastcdr::Cdr& dcdr)
{

    dcdr >> m_longitude;
    dcdr >> m_latitude;
}

/*!
 * @brief This function sets a value in member longitude
 * @param _longitude New value for member longitude
 */
void gps::longitude(
        double _longitude)
{
    m_longitude = _longitude;
}

/*!
 * @brief This function returns the value of member longitude
 * @return Value of member longitude
 */
double gps::longitude() const
{
    return m_longitude;
}

/*!
 * @brief This function returns a reference to member longitude
 * @return Reference to member longitude
 */
double& gps::longitude()
{
    return m_longitude;
}

/*!
 * @brief This function sets a value in member latitude
 * @param _latitude New value for member latitude
 */
void gps::latitude(
        double _latitude)
{
    m_latitude = _latitude;
}

/*!
 * @brief This function returns the value of member latitude
 * @return Value of member latitude
 */
double gps::latitude() const
{
    return m_latitude;
}

/*!
 * @brief This function returns a reference to member latitude
 * @return Reference to member latitude
 */
double& gps::latitude()
{
    return m_latitude;
}



size_t gps::getKeyMaxCdrSerializedSize(
        size_t current_alignment)
{
    static_cast<void>(current_alignment);
    return gps_max_key_cdr_typesize;
}

bool gps::isKeyDefined()
{
    return false;
}

void gps::serializeKey(
        eprosima::fastcdr::Cdr& scdr) const
{
    (void) scdr;
}

