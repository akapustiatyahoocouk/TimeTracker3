//
//  Tt3.Util/ToString.cpp
//  
//  CSha1 class implementation
//
//////////
#include "Tt3.Util/API.hpp"
BEGIN_TT3_NAMESPACE

//  Formatting of C++ basic types
template <> TT3_UTIL_PUBLIC CString ToString<bool>(const bool& value)
{
    static const CString TrueString("true"), FalseString("false");
    return value ? TrueString : FalseString;
}

template <> TT3_UTIL_PUBLIC CString ToString<char>(const char& value)
{
    return CString(&value, 1);
}

template <> TT3_UTIL_PUBLIC CString ToString<signed char>(const signed char& value)
{
    return ToString(signed long long(value));
}

template <> TT3_UTIL_PUBLIC CString ToString<unsigned char>(const unsigned char& value)
{
    return ToString(unsigned long long(value));
}

template <> TT3_UTIL_PUBLIC CString ToString<signed short>(const signed short& value)
{
    return ToString(signed long long(value));
}

template <> TT3_UTIL_PUBLIC CString ToString<unsigned short>(const unsigned short& value)
{
    return ToString(unsigned long long(value));
}

template <> TT3_UTIL_PUBLIC CString ToString<signed int>(const signed int& value)
{
    return ToString(signed long long(value));
}

template <> TT3_UTIL_PUBLIC CString ToString<unsigned int>(const unsigned int& value)
{
    return ToString(unsigned long long(value));
}

template <> TT3_UTIL_PUBLIC CString ToString<signed long>(const signed long& value)
{
    return ToString(signed long long(value));
}

template <> TT3_UTIL_PUBLIC CString ToString<unsigned long>(const unsigned long& value)
{
    return ToString(unsigned long long(value));
}

template <> TT3_UTIL_PUBLIC CString ToString<signed long long>(const signed long long& value)
{
    char s[32];
    sprintf(s, "%lld", value);
    return CString(s);
}

template <> TT3_UTIL_PUBLIC CString ToString<unsigned long long>(const unsigned long long& value)
{
    char s[32];
    sprintf(s, "%llu", value);
    return CString(s);
}

template <> TT3_UTIL_PUBLIC CString ToString<float>(const float& value)
{
    char s[32];
    sprintf(s, "%g", value);
    return CString(s);
}

template <> TT3_UTIL_PUBLIC CString ToString<double>(const double& value)
{
    char s[32];
    sprintf(s, "%g", value);
    return CString(s);
}

//  Formatting of MFC types

END_TT3_NAMESPACE
//  End of Tt3.Util/ToString.cpp
