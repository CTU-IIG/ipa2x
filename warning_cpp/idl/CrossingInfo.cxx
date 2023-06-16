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
 * @file CrossingInfo.cpp
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

#include "CrossingInfo.h"
#include <fastcdr/Cdr.h>

#include <fastcdr/exceptions/BadParamException.h>
using namespace eprosima::fastcdr::exception;

#include <utility>

#define CrossingInfoType_max_cdr_typesize 280ULL;
#define gps_max_cdr_typesize 16ULL;
#define CrossingInfoType_max_key_cdr_typesize 0ULL;
#define gps_max_key_cdr_typesize 0ULL;

CrossingInfoType::CrossingInfoType()
{
    // boolean m_danger
    m_danger = false;
    // boolean m_crossing
    m_crossing = false;
    // string m_dummy
    m_dummy ="";
    // gps m_coords


}

CrossingInfoType::~CrossingInfoType()
{




}

CrossingInfoType::CrossingInfoType(
        const CrossingInfoType& x)
{
    m_danger = x.m_danger;
    m_crossing = x.m_crossing;
    m_dummy = x.m_dummy;
    m_coords = x.m_coords;
}

CrossingInfoType::CrossingInfoType(
        CrossingInfoType&& x) noexcept 
{
    m_danger = x.m_danger;
    m_crossing = x.m_crossing;
    m_dummy = std::move(x.m_dummy);
    m_coords = std::move(x.m_coords);
}

CrossingInfoType& CrossingInfoType::operator =(
        const CrossingInfoType& x)
{

    m_danger = x.m_danger;
    m_crossing = x.m_crossing;
    m_dummy = x.m_dummy;
    m_coords = x.m_coords;

    return *this;
}

CrossingInfoType& CrossingInfoType::operator =(
        CrossingInfoType&& x) noexcept
{

    m_danger = x.m_danger;
    m_crossing = x.m_crossing;
    m_dummy = std::move(x.m_dummy);
    m_coords = std::move(x.m_coords);

    return *this;
}

bool CrossingInfoType::operator ==(
        const CrossingInfoType& x) const
{

    return (m_danger == x.m_danger && m_crossing == x.m_crossing && m_dummy == x.m_dummy && m_coords == x.m_coords);
}

bool CrossingInfoType::operator !=(
        const CrossingInfoType& x) const
{
    return !(*this == x);
}

size_t CrossingInfoType::getMaxCdrSerializedSize(
        size_t current_alignment)
{
    static_cast<void>(current_alignment);
    return CrossingInfoType_max_cdr_typesize;
}

size_t CrossingInfoType::getCdrSerializedSize(
        const CrossingInfoType& data,
        size_t current_alignment)
{
    (void)data;
    size_t initial_alignment = current_alignment;


    current_alignment += 1 + eprosima::fastcdr::Cdr::alignment(current_alignment, 1);


    current_alignment += 1 + eprosima::fastcdr::Cdr::alignment(current_alignment, 1);


    current_alignment += 4 + eprosima::fastcdr::Cdr::alignment(current_alignment, 4) + data.dummy().size() + 1;

    current_alignment += gps::getCdrSerializedSize(data.coords(), current_alignment);

    return current_alignment - initial_alignment;
}

void CrossingInfoType::serialize(
        eprosima::fastcdr::Cdr& scdr) const
{

    scdr << m_danger;
    scdr << m_crossing;
    scdr << m_dummy.c_str();
    scdr << m_coords;

}

void CrossingInfoType::deserialize(
        eprosima::fastcdr::Cdr& dcdr)
{

    dcdr >> m_danger;
    dcdr >> m_crossing;
    dcdr >> m_dummy;
    dcdr >> m_coords;
}

/*!
 * @brief This function sets a value in member danger
 * @param _danger New value for member danger
 */
void CrossingInfoType::danger(
        bool _danger)
{
    m_danger = _danger;
}

/*!
 * @brief This function returns the value of member danger
 * @return Value of member danger
 */
bool CrossingInfoType::danger() const
{
    return m_danger;
}

/*!
 * @brief This function returns a reference to member danger
 * @return Reference to member danger
 */
bool& CrossingInfoType::danger()
{
    return m_danger;
}

/*!
 * @brief This function sets a value in member crossing
 * @param _crossing New value for member crossing
 */
void CrossingInfoType::crossing(
        bool _crossing)
{
    m_crossing = _crossing;
}

/*!
 * @brief This function returns the value of member crossing
 * @return Value of member crossing
 */
bool CrossingInfoType::crossing() const
{
    return m_crossing;
}

/*!
 * @brief This function returns a reference to member crossing
 * @return Reference to member crossing
 */
bool& CrossingInfoType::crossing()
{
    return m_crossing;
}

/*!
 * @brief This function copies the value in member dummy
 * @param _dummy New value to be copied in member dummy
 */
void CrossingInfoType::dummy(
        const std::string& _dummy)
{
    m_dummy = _dummy;
}

/*!
 * @brief This function moves the value in member dummy
 * @param _dummy New value to be moved in member dummy
 */
void CrossingInfoType::dummy(
        std::string&& _dummy)
{
    m_dummy = std::move(_dummy);
}

/*!
 * @brief This function returns a constant reference to member dummy
 * @return Constant reference to member dummy
 */
const std::string& CrossingInfoType::dummy() const
{
    return m_dummy;
}

/*!
 * @brief This function returns a reference to member dummy
 * @return Reference to member dummy
 */
std::string& CrossingInfoType::dummy()
{
    return m_dummy;
}
/*!
 * @brief This function copies the value in member coords
 * @param _coords New value to be copied in member coords
 */
void CrossingInfoType::coords(
        const gps& _coords)
{
    m_coords = _coords;
}

/*!
 * @brief This function moves the value in member coords
 * @param _coords New value to be moved in member coords
 */
void CrossingInfoType::coords(
        gps&& _coords)
{
    m_coords = std::move(_coords);
}

/*!
 * @brief This function returns a constant reference to member coords
 * @return Constant reference to member coords
 */
const gps& CrossingInfoType::coords() const
{
    return m_coords;
}

/*!
 * @brief This function returns a reference to member coords
 * @return Reference to member coords
 */
gps& CrossingInfoType::coords()
{
    return m_coords;
}


size_t CrossingInfoType::getKeyMaxCdrSerializedSize(
        size_t current_alignment)
{
    static_cast<void>(current_alignment);
    return CrossingInfoType_max_key_cdr_typesize;
}

bool CrossingInfoType::isKeyDefined()
{
    return false;
}

void CrossingInfoType::serializeKey(
        eprosima::fastcdr::Cdr& scdr) const
{
    (void) scdr;
}

