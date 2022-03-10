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
 * @file DDSDataEx.h
 * This header file contains the declaration of the described types in the IDL file.
 *
 * This file was generated by the tool gen.
 */

#ifndef _DDSDATAEX_H_
#define _DDSDATAEX_H_


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
#if defined(DDSDataEx_SOURCE)
#define DDSDataEx_DllAPI __declspec( dllexport )
#else
#define DDSDataEx_DllAPI __declspec( dllimport )
#endif // DDSDataEx_SOURCE
#else
#define DDSDataEx_DllAPI
#endif  // EPROSIMA_USER_DLL_EXPORT
#else
#define DDSDataEx_DllAPI
#endif // _WIN32

namespace eprosima {
namespace fastcdr {
class Cdr;
} // namespace fastcdr
} // namespace eprosima

namespace scada_ate
{
    namespace typetopics
    {
        void SetMaxSizeDDSDataExVectorInt(size_t size);
        void SetMaxSizeDDSDataExVectorFloat(size_t size);
        void SetMaxSizeDDSDataExVectorDouble(size_t size);
        void SetMaxSizeDDSDataExVectorChar(size_t size);
        void SetMaxSizeDataExVectorChar();

        size_t GetMaxSizeDDSDataExVectorInt();
        size_t GetMaxSizeDDSDataExVectorFloat();
        size_t GetMaxSizeDDSDataExVectorDouble();
        size_t GetMaxSizeDDSDataExVectorChar();
        size_t GetMaxSizeDataExVectorChar();
    }
}

/*!
 * @brief This class represents the structure DataExInt defined by the user in the IDL file.
 * @ingroup DDSDATAEX
 */
class DataExInt
{
public:

    /*!
     * @brief Default constructor.
     */
    eProsima_user_DllExport DataExInt();

    /*!
     * @brief Default destructor.
     */
    eProsima_user_DllExport ~DataExInt();

    /*!
     * @brief Copy constructor.
     * @param x Reference to the object DataExInt that will be copied.
     */
    eProsima_user_DllExport DataExInt(
            const DataExInt& x);

    /*!
     * @brief Move constructor.
     * @param x Reference to the object DataExInt that will be copied.
     */
    eProsima_user_DllExport DataExInt(
            DataExInt&& x);

    /*!
     * @brief Copy assignment.
     * @param x Reference to the object DataExInt that will be copied.
     */
    eProsima_user_DllExport DataExInt& operator =(
            const DataExInt& x);

    /*!
     * @brief Move assignment.
     * @param x Reference to the object DataExInt that will be copied.
     */
    eProsima_user_DllExport DataExInt& operator =(
            DataExInt&& x);

    /*!
     * @brief This function sets a value in member time_source
     * @param _time_source New value for member time_source
     */
    eProsima_user_DllExport void time_source(
            int64_t _time_source);

    /*!
     * @brief This function returns the value of member time_source
     * @return Value of member time_source
     */
    eProsima_user_DllExport int64_t time_source() const;

    /*!
     * @brief This function returns a reference to member time_source
     * @return Reference to member time_source
     */
    eProsima_user_DllExport int64_t& time_source();

    /*!
     * @brief This function sets a value in member id_tag
     * @param _id_tag New value for member id_tag
     */
    eProsima_user_DllExport void id_tag(
            uint32_t _id_tag);

    /*!
     * @brief This function returns the value of member id_tag
     * @return Value of member id_tag
     */
    eProsima_user_DllExport uint32_t id_tag() const;

    /*!
     * @brief This function returns a reference to member id_tag
     * @return Reference to member id_tag
     */
    eProsima_user_DllExport uint32_t& id_tag();

    /*!
     * @brief This function sets a value in member value
     * @param _value New value for member value
     */
    eProsima_user_DllExport void value(
            int32_t _value);

    /*!
     * @brief This function returns the value of member value
     * @return Value of member value
     */
    eProsima_user_DllExport int32_t value() const;

    /*!
     * @brief This function returns a reference to member value
     * @return Reference to member value
     */
    eProsima_user_DllExport int32_t& value();

    /*!
     * @brief This function sets a value in member quality
     * @param _quality New value for member quality
     */
    eProsima_user_DllExport void quality(
            char _quality);

    /*!
     * @brief This function returns the value of member quality
     * @return Value of member quality
     */
    eProsima_user_DllExport char quality() const;

    /*!
     * @brief This function returns a reference to member quality
     * @return Reference to member quality
     */
    eProsima_user_DllExport char& quality();


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
            const DataExInt& data,
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

    int64_t m_time_source;
    uint32_t m_id_tag;
    int32_t m_value;
    char m_quality;
};
/*!
 * @brief This class represents the structure DataExFloat defined by the user in the IDL file.
 * @ingroup DDSDATAEX
 */
class DataExFloat
{
public:

    /*!
     * @brief Default constructor.
     */
    eProsima_user_DllExport DataExFloat();

    /*!
     * @brief Default destructor.
     */
    eProsima_user_DllExport ~DataExFloat();

    /*!
     * @brief Copy constructor.
     * @param x Reference to the object DataExFloat that will be copied.
     */
    eProsima_user_DllExport DataExFloat(
            const DataExFloat& x);

    /*!
     * @brief Move constructor.
     * @param x Reference to the object DataExFloat that will be copied.
     */
    eProsima_user_DllExport DataExFloat(
            DataExFloat&& x);

    /*!
     * @brief Copy assignment.
     * @param x Reference to the object DataExFloat that will be copied.
     */
    eProsima_user_DllExport DataExFloat& operator =(
            const DataExFloat& x);

    /*!
     * @brief Move assignment.
     * @param x Reference to the object DataExFloat that will be copied.
     */
    eProsima_user_DllExport DataExFloat& operator =(
            DataExFloat&& x);

    /*!
     * @brief This function sets a value in member time_source
     * @param _time_source New value for member time_source
     */
    eProsima_user_DllExport void time_source(
            int64_t _time_source);

    /*!
     * @brief This function returns the value of member time_source
     * @return Value of member time_source
     */
    eProsima_user_DllExport int64_t time_source() const;

    /*!
     * @brief This function returns a reference to member time_source
     * @return Reference to member time_source
     */
    eProsima_user_DllExport int64_t& time_source();

    /*!
     * @brief This function sets a value in member id_tag
     * @param _id_tag New value for member id_tag
     */
    eProsima_user_DllExport void id_tag(
            uint32_t _id_tag);

    /*!
     * @brief This function returns the value of member id_tag
     * @return Value of member id_tag
     */
    eProsima_user_DllExport uint32_t id_tag() const;

    /*!
     * @brief This function returns a reference to member id_tag
     * @return Reference to member id_tag
     */
    eProsima_user_DllExport uint32_t& id_tag();

    /*!
     * @brief This function sets a value in member value
     * @param _value New value for member value
     */
    eProsima_user_DllExport void value(
            float _value);

    /*!
     * @brief This function returns the value of member value
     * @return Value of member value
     */
    eProsima_user_DllExport float value() const;

    /*!
     * @brief This function returns a reference to member value
     * @return Reference to member value
     */
    eProsima_user_DllExport float& value();

    /*!
     * @brief This function sets a value in member quality
     * @param _quality New value for member quality
     */
    eProsima_user_DllExport void quality(
            char _quality);

    /*!
     * @brief This function returns the value of member quality
     * @return Value of member quality
     */
    eProsima_user_DllExport char quality() const;

    /*!
     * @brief This function returns a reference to member quality
     * @return Reference to member quality
     */
    eProsima_user_DllExport char& quality();


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
            const DataExFloat& data,
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

    int64_t m_time_source;
    uint32_t m_id_tag;
    float m_value;
    char m_quality;
};
/*!
 * @brief This class represents the structure DataExDouble defined by the user in the IDL file.
 * @ingroup DDSDATAEX
 */
class DataExDouble
{
public:

    /*!
     * @brief Default constructor.
     */
    eProsima_user_DllExport DataExDouble();

    /*!
     * @brief Default destructor.
     */
    eProsima_user_DllExport ~DataExDouble();

    /*!
     * @brief Copy constructor.
     * @param x Reference to the object DataExDouble that will be copied.
     */
    eProsima_user_DllExport DataExDouble(
            const DataExDouble& x);

    /*!
     * @brief Move constructor.
     * @param x Reference to the object DataExDouble that will be copied.
     */
    eProsima_user_DllExport DataExDouble(
            DataExDouble&& x);

    /*!
     * @brief Copy assignment.
     * @param x Reference to the object DataExDouble that will be copied.
     */
    eProsima_user_DllExport DataExDouble& operator =(
            const DataExDouble& x);

    /*!
     * @brief Move assignment.
     * @param x Reference to the object DataExDouble that will be copied.
     */
    eProsima_user_DllExport DataExDouble& operator =(
            DataExDouble&& x);

    /*!
     * @brief This function sets a value in member time_source
     * @param _time_source New value for member time_source
     */
    eProsima_user_DllExport void time_source(
            int64_t _time_source);

    /*!
     * @brief This function returns the value of member time_source
     * @return Value of member time_source
     */
    eProsima_user_DllExport int64_t time_source() const;

    /*!
     * @brief This function returns a reference to member time_source
     * @return Reference to member time_source
     */
    eProsima_user_DllExport int64_t& time_source();

    /*!
     * @brief This function sets a value in member id_tag
     * @param _id_tag New value for member id_tag
     */
    eProsima_user_DllExport void id_tag(
            uint32_t _id_tag);

    /*!
     * @brief This function returns the value of member id_tag
     * @return Value of member id_tag
     */
    eProsima_user_DllExport uint32_t id_tag() const;

    /*!
     * @brief This function returns a reference to member id_tag
     * @return Reference to member id_tag
     */
    eProsima_user_DllExport uint32_t& id_tag();

    /*!
     * @brief This function sets a value in member value
     * @param _value New value for member value
     */
    eProsima_user_DllExport void value(
            double _value);

    /*!
     * @brief This function returns the value of member value
     * @return Value of member value
     */
    eProsima_user_DllExport double value() const;

    /*!
     * @brief This function returns a reference to member value
     * @return Reference to member value
     */
    eProsima_user_DllExport double& value();

    /*!
     * @brief This function sets a value in member quality
     * @param _quality New value for member quality
     */
    eProsima_user_DllExport void quality(
            char _quality);

    /*!
     * @brief This function returns the value of member quality
     * @return Value of member quality
     */
    eProsima_user_DllExport char quality() const;

    /*!
     * @brief This function returns a reference to member quality
     * @return Reference to member quality
     */
    eProsima_user_DllExport char& quality();


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
            const DataExDouble& data,
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

    int64_t m_time_source;
    uint32_t m_id_tag;
    double m_value;
    char m_quality;
};
/*!
 * @brief This class represents the structure DataExChar defined by the user in the IDL file.
 * @ingroup DDSDATAEX
 */
class DataExChar
{
public:

    /*!
     * @brief Default constructor.
     */
    eProsima_user_DllExport DataExChar();

    /*!
     * @brief Default destructor.
     */
    eProsima_user_DllExport ~DataExChar();

    /*!
     * @brief Copy constructor.
     * @param x Reference to the object DataExChar that will be copied.
     */
    eProsima_user_DllExport DataExChar(
            const DataExChar& x);

    /*!
     * @brief Move constructor.
     * @param x Reference to the object DataExChar that will be copied.
     */
    eProsima_user_DllExport DataExChar(
            DataExChar&& x);

    /*!
     * @brief Copy assignment.
     * @param x Reference to the object DataExChar that will be copied.
     */
    eProsima_user_DllExport DataExChar& operator =(
            const DataExChar& x);

    /*!
     * @brief Move assignment.
     * @param x Reference to the object DataExChar that will be copied.
     */
    eProsima_user_DllExport DataExChar& operator =(
            DataExChar&& x);

    /*!
     * @brief This function sets a value in member time_source
     * @param _time_source New value for member time_source
     */
    eProsima_user_DllExport void time_source(
            int64_t _time_source);

    /*!
     * @brief This function returns the value of member time_source
     * @return Value of member time_source
     */
    eProsima_user_DllExport int64_t time_source() const;

    /*!
     * @brief This function returns a reference to member time_source
     * @return Reference to member time_source
     */
    eProsima_user_DllExport int64_t& time_source();

    /*!
     * @brief This function sets a value in member id_tag
     * @param _id_tag New value for member id_tag
     */
    eProsima_user_DllExport void id_tag(
            uint32_t _id_tag);

    /*!
     * @brief This function returns the value of member id_tag
     * @return Value of member id_tag
     */
    eProsima_user_DllExport uint32_t id_tag() const;

    /*!
     * @brief This function returns a reference to member id_tag
     * @return Reference to member id_tag
     */
    eProsima_user_DllExport uint32_t& id_tag();

    /*!
     * @brief This function copies the value in member value
     * @param _value New value to be copied in member value
     */
    eProsima_user_DllExport void value(
            const std::vector<char>& _value);

    /*!
     * @brief This function moves the value in member value
     * @param _value New value to be moved in member value
     */
    eProsima_user_DllExport void value(
            std::vector<char>&& _value);

    /*!
     * @brief This function returns a constant reference to member value
     * @return Constant reference to member value
     */
    eProsima_user_DllExport const std::vector<char>& value() const;

    /*!
     * @brief This function returns a reference to member value
     * @return Reference to member value
     */
    eProsima_user_DllExport std::vector<char>& value();
    /*!
     * @brief This function sets a value in member quality
     * @param _quality New value for member quality
     */
    eProsima_user_DllExport void quality(
            char _quality);

    /*!
     * @brief This function returns the value of member quality
     * @return Value of member quality
     */
    eProsima_user_DllExport char quality() const;

    /*!
     * @brief This function returns a reference to member quality
     * @return Reference to member quality
     */
    eProsima_user_DllExport char& quality();


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
            const DataExChar& data,
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

    int64_t m_time_source;
    uint32_t m_id_tag;
    std::vector<char> m_value;
    char m_quality;
};
/*!
 * @brief This class represents the structure DDSDataEx defined by the user in the IDL file.
 * @ingroup DDSDATAEX
 */
class DDSDataEx
{
public:

    /*!
     * @brief Default constructor.
     */
    eProsima_user_DllExport DDSDataEx();

    /*!
     * @brief Default destructor.
     */
    eProsima_user_DllExport ~DDSDataEx();

    /*!
     * @brief Copy constructor.
     * @param x Reference to the object DDSDataEx that will be copied.
     */
    eProsima_user_DllExport DDSDataEx(
            const DDSDataEx& x);

    /*!
     * @brief Move constructor.
     * @param x Reference to the object DDSDataEx that will be copied.
     */
    eProsima_user_DllExport DDSDataEx(
            DDSDataEx&& x);

    /*!
     * @brief Copy assignment.
     * @param x Reference to the object DDSDataEx that will be copied.
     */
    eProsima_user_DllExport DDSDataEx& operator =(
            const DDSDataEx& x);

    /*!
     * @brief Move assignment.
     * @param x Reference to the object DDSDataEx that will be copied.
     */
    eProsima_user_DllExport DDSDataEx& operator =(
            DDSDataEx&& x);

    /*!
     * @brief This function sets a value in member time_service
     * @param _time_service New value for member time_service
     */
    eProsima_user_DllExport void time_service(
            int64_t _time_service);

    /*!
     * @brief This function returns the value of member time_service
     * @return Value of member time_service
     */
    eProsima_user_DllExport int64_t time_service() const;

    /*!
     * @brief This function returns a reference to member time_service
     * @return Reference to member time_service
     */
    eProsima_user_DllExport int64_t& time_service();

    /*!
     * @brief This function copies the value in member data_int
     * @param _data_int New value to be copied in member data_int
     */
    eProsima_user_DllExport void data_int(
            const std::vector<DataExInt>& _data_int);

    /*!
     * @brief This function moves the value in member data_int
     * @param _data_int New value to be moved in member data_int
     */
    eProsima_user_DllExport void data_int(
            std::vector<DataExInt>&& _data_int);

    /*!
     * @brief This function returns a constant reference to member data_int
     * @return Constant reference to member data_int
     */
    eProsima_user_DllExport const std::vector<DataExInt>& data_int() const;

    /*!
     * @brief This function returns a reference to member data_int
     * @return Reference to member data_int
     */
    eProsima_user_DllExport std::vector<DataExInt>& data_int();
    /*!
     * @brief This function copies the value in member data_float
     * @param _data_float New value to be copied in member data_float
     */
    eProsima_user_DllExport void data_float(
            const std::vector<DataExFloat>& _data_float);

    /*!
     * @brief This function moves the value in member data_float
     * @param _data_float New value to be moved in member data_float
     */
    eProsima_user_DllExport void data_float(
            std::vector<DataExFloat>&& _data_float);

    /*!
     * @brief This function returns a constant reference to member data_float
     * @return Constant reference to member data_float
     */
    eProsima_user_DllExport const std::vector<DataExFloat>& data_float() const;

    /*!
     * @brief This function returns a reference to member data_float
     * @return Reference to member data_float
     */
    eProsima_user_DllExport std::vector<DataExFloat>& data_float();
    /*!
     * @brief This function copies the value in member data_double
     * @param _data_double New value to be copied in member data_double
     */
    eProsima_user_DllExport void data_double(
            const std::vector<DataExDouble>& _data_double);

    /*!
     * @brief This function moves the value in member data_double
     * @param _data_double New value to be moved in member data_double
     */
    eProsima_user_DllExport void data_double(
            std::vector<DataExDouble>&& _data_double);

    /*!
     * @brief This function returns a constant reference to member data_double
     * @return Constant reference to member data_double
     */
    eProsima_user_DllExport const std::vector<DataExDouble>& data_double() const;

    /*!
     * @brief This function returns a reference to member data_double
     * @return Reference to member data_double
     */
    eProsima_user_DllExport std::vector<DataExDouble>& data_double();
    /*!
     * @brief This function copies the value in member data_char
     * @param _data_char New value to be copied in member data_char
     */
    eProsima_user_DllExport void data_char(
            const std::vector<DataExChar>& _data_char);

    /*!
     * @brief This function moves the value in member data_char
     * @param _data_char New value to be moved in member data_char
     */
    eProsima_user_DllExport void data_char(
            std::vector<DataExChar>&& _data_char);

    /*!
     * @brief This function returns a constant reference to member data_char
     * @return Constant reference to member data_char
     */
    eProsima_user_DllExport const std::vector<DataExChar>& data_char() const;

    /*!
     * @brief This function returns a reference to member data_char
     * @return Reference to member data_char
     */
    eProsima_user_DllExport std::vector<DataExChar>& data_char();

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
            const DDSDataEx& data,
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

    int64_t m_time_service;
    std::vector<DataExInt> m_data_int;
    std::vector<DataExFloat> m_data_float;
    std::vector<DataExDouble> m_data_double;
    std::vector<DataExChar> m_data_char;
};

#endif // _DDSDATAEX_H_