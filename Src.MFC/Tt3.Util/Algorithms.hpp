//
//  Tt3.Util/Algorithms.hpp
//  
//  Tt3 Helper algorithms
//
//////////
BEGIN_TT3_NAMESPACE

//  An agent that known how to compare values for equality
template <class T>
class IComparer
{
    //////////
    //  This is an interface
public:
    virtual ~IComparer() = default;

    //////////
    //  Operations
public:
    //  Checks whether two values are "equal" to each other
    virtual bool        Equal(const T & a, const T & b) = 0;
};

//  A comparator that uses "operator =="
template <class T>
class CNaturalComparer : public virtual IComparer<T>
{
    //////////
    //  IComparer<T>
public:
    virtual bool        Equal(const T & a, const T & b) override { return a == b; }
};

//  An agent that known how to compare values for order
template <class T>
class IOrder
{
    //////////
    //  This is an interface
public:
    virtual ~IOrder() = default;

    //////////
    //  Operations
public:
    //  Checks whether one value is "less than" another
    virtual bool        Less(const T & a, const T & b) = 0;
};

//  An order that uses "operator <"
template <class T>
class CNaturalOrder: public virtual IOrder<T>
{
    //////////
    //  IOrder<T>
public:
    virtual bool        Less(const T & a, const T & b) override { return a < b; }
};

//  An agent that known how to calculate hash codes for values
template <class T>
class IHash
{
    //////////
    //  This is an interface
public:
    virtual ~IHash() = default;

    //////////
    //  Operations
public:
    //  Calculates the hash code for the specified valie
    virtual size_t      HashCode(const T & a) = 0;
};

//  Default hash code implementations
template <class T>
class CDefaultHash : public virtual IHash<T>
{
    //////////
    //  IHash<T>
public:
    virtual size_t      HashCode(const T & a) override = delete;
};

template <>
class CDefaultHash<char> : public virtual IHash<char>
{
    //////////
    //  IHash<T>
public:
    virtual size_t      HashCode(const char& a) override { return unsigned char(a); }
};

template <>
class CDefaultHash<wchar_t> : public virtual IHash<wchar_t>
{
    //////////
    //  IHash<T>
public:
    virtual size_t      HashCode(const wchar_t& a) override { return a; }
};

template <>
class CDefaultHash<signed char> : public virtual IHash<signed char>
{
    //////////
    //  IHash<T>
public:
    virtual size_t      HashCode(const signed char& a) override { return unsigned char(a); }
};

template <>
class CDefaultHash<unsigned char> : public virtual IHash<unsigned char>
{
    //////////
    //  IHash<T>
public:
    virtual size_t      HashCode(const unsigned char& a) override { return a; }
};

template <>
class CDefaultHash<signed short> : public virtual IHash<signed short>
{
    //////////
    //  IHash<T>
public:
    virtual size_t      HashCode(const signed short& a) override { return unsigned short(a); }
};

template <>
class CDefaultHash<unsigned short> : public virtual IHash<unsigned short>
{
    //////////
    //  IHash<T>
public:
    virtual size_t      HashCode(const unsigned short& a) override { return a; }
};

template <>
class CDefaultHash<signed int> : public virtual IHash<signed int>
{
    //////////
    //  IHash<T>
public:
    virtual size_t      HashCode(const signed int& a) override { return unsigned int(a); }
};

template <>
class CDefaultHash<unsigned int> : public virtual IHash<unsigned int>
{
    //////////
    //  IHash<T>
public:
    virtual size_t      HashCode(const unsigned int& a) override { return a; }
};

template <>
class CDefaultHash<signed long> : public virtual IHash<signed long>
{
    //////////
    //  IHash<T>
public:
    virtual size_t      HashCode(const signed long& a) override { return size_t(a); }
};

template <>
class CDefaultHash<unsigned long> : public virtual IHash<unsigned long>
{
    //////////
    //  IHash<T>
public:
    virtual size_t      HashCode(const unsigned long& a) override { return size_t(a); }
};

template <>
class CDefaultHash<signed long long> : public virtual IHash<signed long long>
{
    //////////
    //  IHash<T>
public:
    virtual size_t      HashCode(const signed long long& a) override { return size_t(a); }
};

template <>
class CDefaultHash<unsigned long long> : public virtual IHash<unsigned long long>
{
    //////////
    //  IHash<T>
public:
    virtual size_t      HashCode(const unsigned long long& a) override { return size_t(a); }
};

template <>
class CDefaultHash<CString> : public virtual IHash<CString>
{
    //////////
    //  IHash<T>
public:
    virtual size_t      HashCode(const CString & a) override
    {
        size_t result = 0;
        for (int i = 0; i < a.GetLength(); i++)
        {
            result = result * 31 + a[i];
        }
        return result;
    }
};

template <class T>
class CDefaultHash<T*> : public virtual IHash<T*>
{
    //////////
    //  IHash<T>
public:
    using TP = T *;
    virtual size_t      HashCode(const TP & a) override { return size_t(uintptr_t(a) >> 5); }
};

END_TT3_NAMESPACE
//  End of Tt3.Util/Algorithms.hpp
