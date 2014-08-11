// Copyright (c) 2014-2014 Josh Blum
// SPDX-License-Identifier: BSL-1.0

#pragma once
#include <Pothos/Config.hpp>
#include <Pothos/Exception.hpp>
#include <Pothos/Util/TypeInfo.hpp>
#include <Poco/Format.h>
#include <type_traits>
#include <limits>
#include <complex>
#include <string>

/***********************************************************************
 * typedefs that make nice clean registry names since we use #name
 **********************************************************************/
typedef unsigned char uchar;
typedef signed char schar;
typedef unsigned short ushort;
typedef signed short sshort;
typedef unsigned int uint;
typedef signed int sint;
typedef unsigned long ulong;
typedef signed long slong;
typedef unsigned long long ullong;
typedef signed long long sllong;
typedef std::complex<float> cfloat;
typedef std::complex<double> cdouble;

/***********************************************************************
 * make signed that handles floats
 * https://stackoverflow.com/questions/16377736/stdmake-signed-that-accepts-floating-point-types
 **********************************************************************/
template<typename T, typename Enable = void>
struct my_make_signed
{
    typedef typename std::make_signed<T>::type type;
};

template<typename T>
struct my_make_signed<T, typename std::enable_if<std::is_floating_point<T>::value>::type>
{
    typedef T type;
};

/***********************************************************************
 * type conversion checkers
 **********************************************************************/

//! check if the input value can fit into the output type
template <typename OutType, typename InType>
void convertNumCheck(const InType &in)
{
    if (in > std::numeric_limits<OutType>::max() or //too large
        (std::is_signed<InType>::value and typename my_make_signed<InType>::type(in) <
        typename my_make_signed<OutType>::type(std::numeric_limits<OutType>::lowest()))) //too small
    {
        throw Pothos::RangeException(Poco::format("value %s out of range for output type %s",
            std::to_string(in), Pothos::Util::typeInfoToString(typeid(OutType))));
    }
}

//! we will assign real to an integer, imaginary should be zero
template <typename OutType, typename InType>
void convertNumCheck(const std::complex<InType> &in)
{
    if (in.imag() != 0)
    {
        throw Pothos::RangeException(Poco::format("cannot convert value (%s + I*%s) with imaginary component into real type %s",
            std::to_string(in.real()), std::to_string(in.imag()), Pothos::Util::typeInfoToString(typeid(OutType))));
    }
    convertNumCheck<OutType>(in.real());
}

/***********************************************************************
 * template comprehension to handle complex and regular numbers
 **********************************************************************/
template <typename InType, typename OutType>
void convertNumHelper(const InType &in, OutType &out)
{
    convertNumCheck<OutType>(in);
    out = OutType(in);
}

template <typename InType, typename OutType>
void convertNumHelper(const std::complex<InType> &in, OutType &out)
{
    convertNumCheck<OutType>(in);
    out = OutType(in.real());
}

template <typename InType, typename OutType>
void convertNumHelper(const InType &in, std::complex<OutType> &out)
{
    out = std::complex<OutType>(OutType(in));
}

template <typename InType, typename OutType>
void convertNumHelper(const std::complex<InType> &in, std::complex<OutType> &out)
{
    out = std::complex<OutType>(OutType(in.real()), OutType(in.imag()));
}

template <typename InType, typename OutType>
OutType convertNum(const InType &in)
{
    OutType out;
    convertNumHelper(in, out);
    return out;
}
