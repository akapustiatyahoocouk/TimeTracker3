//
//  Tt3.Util/ToString.hpp
//  
//  Tt3 value formatting framework
//
//////////
BEGIN_TT3_NAMESPACE

template <class T> CString ToString(const T& value) = delete;

//  Formatting of C++ basic types
template <> TT3_UTIL_PUBLIC CString ToString<bool>(const bool& value);
template <> TT3_UTIL_PUBLIC CString ToString<char>(const char& value);
template <> TT3_UTIL_PUBLIC CString ToString<signed char>(const signed char& value);
template <> TT3_UTIL_PUBLIC CString ToString<unsigned char>(const unsigned char& value);
template <> TT3_UTIL_PUBLIC CString ToString<signed short>(const signed short& value);
template <> TT3_UTIL_PUBLIC CString ToString<unsigned short>(const unsigned short& value);
template <> TT3_UTIL_PUBLIC CString ToString<signed int>(const signed int& value);
template <> TT3_UTIL_PUBLIC CString ToString<unsigned int>(const unsigned int& value);
template <> TT3_UTIL_PUBLIC CString ToString<signed long>(const signed long& value);
template <> TT3_UTIL_PUBLIC CString ToString<unsigned long>(const unsigned long& value);
template <> TT3_UTIL_PUBLIC CString ToString<signed long long>(const signed long long& value);
template <> TT3_UTIL_PUBLIC CString ToString<unsigned long long>(const unsigned long long& value);
template <> TT3_UTIL_PUBLIC CString ToString<float>(const float& value);
template <> TT3_UTIL_PUBLIC CString ToString<double>(const double& value);

template <> TT3_UTIL_PUBLIC CString ToString<const char*>(const char* const& value) { return CString(value); }
template <> TT3_UTIL_PUBLIC CString ToString<const wchar_t*>(const wchar_t* const& value) { return CString(value); }

//  Formatting of MFC types
template <> TT3_UTIL_PUBLIC CString ToString<CString>(const CString & value) { return value; }

END_TT3_NAMESPACE

inline CString operator + (const char* a, const CString& b) { return CString(a) + b; }
inline CString operator + (const CString& a, const char * b) { return a + CString(b); }

//  End of Tt3.Util/ToString.hpp
