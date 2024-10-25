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
