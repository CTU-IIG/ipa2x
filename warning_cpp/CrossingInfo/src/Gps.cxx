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

gps::gps()
{
    // m_longtitude com.eprosima.idl.parser.typecode.PrimitiveTypeCode@4eb7f003
    m_longtitude = 0.0;
    // m_latitude com.eprosima.idl.parser.typecode.PrimitiveTypeCode@eafc191
    m_latitude = 0.0;

}

gps::~gps()
{


}

gps::gps(
        const gps& x)
{
    m_longtitude = x.m_longtitude;
    m_latitude = x.m_latitude;
}

gps::gps(
        gps&& x)
{
    m_longtitude = x.m_longtitude;
    m_latitude = x.m_latitude;
}

gps& gps::operator =(
        const gps& x)
{

    m_longtitude = x.m_longtitude;
    m_latitude = x.m_latitude;

    return *this;
}

gps& gps::operator =(
        gps&& x)
{

    m_longtitude = x.m_longtitude;
    m_latitude = x.m_latitude;

    return *this;
}

bool gps::operator ==(
        const gps& x) const
{

    return (m_longtitude == x.m_longtitude && m_latitude == x.m_latitude);
}

bool gps::operator !=(
        const gps& x) const
{
    return !(*this == x);
}

size_t gps::getMaxCdrSerializedSize(
        size_t current_alignment)
{
    size_t initial_alignment = current_alignment;


    current_alignment += 8 + eprosima::fastcdr::Cdr::alignment(current_alignment, 8);


    current_alignment += 8 + eprosima::fastcdr::Cdr::alignment(current_alignment, 8);



    return current_alignment - initial_alignment;
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

    scdr << m_longtitude;
    scdr << m_latitude;

}

void gps::deserialize(
        eprosima::fastcdr::Cdr& dcdr)
{

    dcdr >> m_longtitude;
    dcdr >> m_latitude;
}

/*!
 * @brief This function sets a value in member longtitude
 * @param _longtitude New value for member longtitude
 */
void gps::longtitude(
        double _longtitude)
{
    m_longtitude = _longtitude;
}

/*!
 * @brief This function returns the value of member longtitude
 * @return Value of member longtitude
 */
double gps::longtitude() const
{
    return m_longtitude;
}

/*!
 * @brief This function returns a reference to member longtitude
 * @return Reference to member longtitude
 */
double& gps::longtitude()
{
    return m_longtitude;
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
    size_t current_align = current_alignment;





    return current_align;
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
