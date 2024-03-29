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
 * This file was generated by the tool gen.
 */

#ifndef _FAST_DDS_GENERATED_GPS_H_
#define _FAST_DDS_GENERATED_GPS_H_


#include <fastrtps/utils/fixed_size_string.hpp>

#include <stdint.h>
#include <array>
#include <string>
#include <vector>
#include <map>
#include <bitset>

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
#if defined(Gps_SOURCE)
#define Gps_DllAPI __declspec( dllexport )
#else
#define Gps_DllAPI __declspec( dllimport )
#endif // Gps_SOURCE
#else
#define Gps_DllAPI
#endif  // EPROSIMA_USER_DLL_EXPORT
#else
#define Gps_DllAPI
#endif // _WIN32

namespace eprosima {
namespace fastcdr {
class Cdr;
} // namespace fastcdr
} // namespace eprosima


/*!
 * @brief This class represents the structure gps defined by the user in the IDL file.
 * @ingroup GPS
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
            gps&& x);

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
            gps&& x);

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


    /*!
     * @brief This function returns the maximum serialized size of an object
     * depending on the buffer alignment.
     * @param current_alignment Buffer alignment.
     * @return Maximum serialized size.
     */
    eProsima_user_DllExport static size_t getMaxCdrSerializedSize(
            size_t current_alignment = 0);

    /*!
     * @brief This function returns the serialized size of a data depending on the buffer alignment.
     * @param data Data which is calculated its serialized size.
     * @param current_alignment Buffer alignment.
     * @return Serialized size.
     */
    eProsima_user_DllExport static size_t getCdrSerializedSize(
            const gps& data,
            size_t current_alignment = 0);


    /*!
     * @brief This function serializes an object using CDR serialization.
     * @param cdr CDR serialization object.
     */
    eProsima_user_DllExport void serialize(
            eprosima::fastcdr::Cdr& cdr) const;

    /*!
     * @brief This function deserializes an object using CDR serialization.
     * @param cdr CDR serialization object.
     */
    eProsima_user_DllExport void deserialize(
            eprosima::fastcdr::Cdr& cdr);



    /*!
     * @brief This function returns the maximum serialized size of the Key of an object
     * depending on the buffer alignment.
     * @param current_alignment Buffer alignment.
     * @return Maximum serialized size.
     */
    eProsima_user_DllExport static size_t getKeyMaxCdrSerializedSize(
            size_t current_alignment = 0);

    /*!
     * @brief This function tells you if the Key has been defined for this type
     */
    eProsima_user_DllExport static bool isKeyDefined();

    /*!
     * @brief This function serializes the key members of an object using CDR serialization.
     * @param cdr CDR serialization object.
     */
    eProsima_user_DllExport void serializeKey(
            eprosima::fastcdr::Cdr& cdr) const;

private:

    double m_longitude;
    double m_latitude;
};

#endif // _FAST_DDS_GENERATED_GPS_H_