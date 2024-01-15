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
 * @file Gps.h
 * This header file contains the declaration of the described types in the IDL file.
 *
 * This file was generated by the tool fastddsgen.
 */

#ifndef _FAST_DDS_GENERATED_GPS_H_
#define _FAST_DDS_GENERATED_GPS_H_

#include <array>
#include <bitset>
#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include <fastcdr/cdr/fixed_size_string.hpp>
#include <fastcdr/xcdr/external.hpp>
#include <fastcdr/xcdr/optional.hpp>



#if defined(_WIN32)
#if defined(EPROSIMA_USER_DLL_EXPORT)
#define eProsima_user_DllExport __declspec( dllexport )
#else
#define eProsima_user_DllExport
#endif  // EPROSIMA_USER_DLL_EXPORT
#else
#define eProsima_user_DllExport
#endif  // _WIN32

#if defined(_WIN32)
#if defined(EPROSIMA_USER_DLL_EXPORT)
#if defined(GPS_SOURCE)
#define GPS_DllAPI __declspec( dllexport )
#else
#define GPS_DllAPI __declspec( dllimport )
#endif // GPS_SOURCE
#else
#define GPS_DllAPI
#endif  // EPROSIMA_USER_DLL_EXPORT
#else
#define GPS_DllAPI
#endif // _WIN32

namespace eprosima {
namespace fastcdr {
class Cdr;
class CdrSizeCalculator;
} // namespace fastcdr
} // namespace eprosima





/*!
 * @brief This class represents the structure gps defined by the user in the IDL file.
 * @ingroup Gps
 */
class gps
{
public:

    /*!
     * @brief Default constructor.
     */
    eProsima_user_DllExport gps();

    /*!
     * @brief Default destructor.
     */
    eProsima_user_DllExport ~gps();

    /*!
     * @brief Copy constructor.
     * @param x Reference to the object gps that will be copied.
     */
    eProsima_user_DllExport gps(
            const gps& x);

    /*!
     * @brief Move constructor.
     * @param x Reference to the object gps that will be copied.
     */
    eProsima_user_DllExport gps(
            gps&& x) noexcept;

    /*!
     * @brief Copy assignment.
     * @param x Reference to the object gps that will be copied.
     */
    eProsima_user_DllExport gps& operator =(
            const gps& x);

    /*!
     * @brief Move assignment.
     * @param x Reference to the object gps that will be copied.
     */
    eProsima_user_DllExport gps& operator =(
            gps&& x) noexcept;

    /*!
     * @brief Comparison operator.
     * @param x gps object to compare.
     */
    eProsima_user_DllExport bool operator ==(
            const gps& x) const;

    /*!
     * @brief Comparison operator.
     * @param x gps object to compare.
     */
    eProsima_user_DllExport bool operator !=(
            const gps& x) const;

    /*!
     * @brief This function sets a value in member longitude
     * @param _longitude New value for member longitude
     */
    eProsima_user_DllExport void longitude(
            double _longitude);

    /*!
     * @brief This function returns the value of member longitude
     * @return Value of member longitude
     */
    eProsima_user_DllExport double longitude() const;

    /*!
     * @brief This function returns a reference to member longitude
     * @return Reference to member longitude
     */
    eProsima_user_DllExport double& longitude();


    /*!
     * @brief This function sets a value in member latitude
     * @param _latitude New value for member latitude
     */
    eProsima_user_DllExport void latitude(
            double _latitude);

    /*!
     * @brief This function returns the value of member latitude
     * @return Value of member latitude
     */
    eProsima_user_DllExport double latitude() const;

    /*!
     * @brief This function returns a reference to member latitude
     * @return Reference to member latitude
     */
    eProsima_user_DllExport double& latitude();

private:

    double m_longitude{0.0};
    double m_latitude{0.0};

};

#endif // _FAST_DDS_GENERATED_GPS_H_



