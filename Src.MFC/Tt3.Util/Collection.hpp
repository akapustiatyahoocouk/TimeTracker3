//
//  Tt3.Util/Collection.hpp
//  
//  Tt3 Collection framework base
//
//////////
BEGIN_TT3_NAMESPACE

//  An optional value - can be viewed as "a collection of 0 or 1 items)
template <class T>
class COptional
{
    //////////
    //  Construction/destruction/assignment
public:
    COptional() : m_HasValue(false), m_Value() {}
    COptional(const T & value) : m_HasValue(true), m_Value(value) {}

    //////////
    //  Operations
public:
    //  TODO document
    bool        HasValue() const { return m_HasValue; }
    T           GetValue() const { return m_Value; }

    //////////
    //  Implementation
private:
    bool        m_HasValue;
    T           m_Value;
};

//  A pair of values - can be viewed as a "collection of exactly 2 items"
template <class T1, class T2>
class CPair
{
    //////////
    //  Construction/destruction/assignment
public:
    CPair() : first(), second() {}
    CPair(const T1 & firstParam, const T2 & secondParam)
        :   first(firstParam), second(secondParam) {}

    //////////
    //  Properties
public:
    T1          first;
    T2          second;
};

//  A generic read-only collection
template <class T>
class CReadOnlyCollection
{
    //////////
    //  Construction/destruction/assignment
public:
    virtual ~CReadOnlyCollection() = default;

    //////////
    //  Operations
public:
    //  TODO document
    virtual size_t      GetSize() const = 0;
    virtual bool        IsEmpty() const { return GetSize() == 0; }
    virtual bool        Contains(const T & value) const = 0;

    //  Returns an iterator over all elements of this collection.
    //  The returned iterator behaves AS IF it has made a snapshot of
    //  the collection's contents at the moment of its creation and
    //  will iterate over that imagined "snapshot" even if the collection
    //  if modified in the process, or even destroyed
    virtual CIterator<T>GetElements() const = 0;

    //////////
    //  Iterators - C++ - style
public:
    class it final
    {
        //////////
        //  Construction/destruction/assignment
    public:
        it() = default;
        explicit it(const CIterator<T> & impl) : m_Iterator(impl) {}

        //////////
        //  Operators
    public:
        bool            operator == (const it & op2) const { return m_Iterator.HasFinished() && op2.m_Iterator.HasFinished(); }
        bool            operator != (const it & op2) const { return !m_Iterator.HasFinished() || !op2.m_Iterator.HasFinished(); }
                        operator bool() const { return m_Iterator.HasFinished(); }
        it &            operator++() { m_Iterator.Advance(); return *this; }
        it &            operator++(int) { m_Iterator.Advance(); return *this; }

        T               operator *() const { return m_Iterator.GetItem(); }

        //////////
        //  Implementation
    private:
        CIterator<T>    m_Iterator;
    };

    it                  begin() const { return it(GetElements()); }
    it                  end() const { return it(); }
};

//  A generic modifyable collection
template <class T>
class CCollection : public CReadOnlyCollection<T>
{
    //////////
    //  Operations
public:
    //  TODO document
    virtual void        RemoveAll() = 0;
    virtual bool        Add(const T & value) = 0;
    virtual size_t      AddAll(const CReadOnlyCollection<T> & values);
    virtual size_t      Remove(const T & value) = 0;
    virtual size_t      RemoveAll(const CReadOnlyCollection<T> & values);
    //  TODO virtual size_t      RetainAll(const CReadOnlyCollection<T> & values);
};

template <class T>
size_t CCollection<T>::AddAll(const CReadOnlyCollection<T> & values)
{
    size_t result = 0;
    for (auto value : values)
    {
        if (this->Add(value))
        {
            result++;
        }
    }
    return result;
}

template <class T>
size_t CCollection<T>::RemoveAll(const CReadOnlyCollection<T> & values)
{
    size_t result = 0;
    for (auto value : values)
    {
        result += this->Remove(value);
    }
    return result;
}

END_TT3_NAMESPACE
//  End of Tt3.Util/Collection.hpp
