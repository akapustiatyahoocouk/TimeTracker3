//
//  Tt3.Util/Set.hpp
//  
//  Tt3 set
//
//////////
BEGIN_TT3_NAMESPACE

//////////
//  A hash set
template <class T, class TH = CDefaultHash<T>, class TC = CNaturalComparer<T>>
class CSet : public CCollection<T>
{
    //////////
    //  Construction/destruction/assignment
public:
    CSet();
    CSet(const CSet<T, TH, TC>& src);
    template<class P> explicit CSet(const CReadOnlyCollection<P>& src);
    virtual ~CSet();

    CSet<T, TH, TC>& operator = (const CSet<T, TH, TC>& src);

    template<class P>
    CSet<T, TH, TC>& operator = (const CReadOnlyCollection<P>& src);

    //////////
    //  Operators
public:
    bool                operator == (const CSet<T, TH, TC>& op2) const;
    bool                operator != (const CSet<T, TH, TC>& op2) const { return !(*this == op2); }

    //////////
    //  CReadOnlyCollection<T>
public:
    virtual size_t      GetSize() const override;
    virtual bool        Contains(const T& value) const override;
    virtual CIterator<T>GetElements() const override;

    //////////
    //  CCollection<T>
public:
    virtual void        RemoveAll() override;
    virtual bool        Add(const T& value) override;
    virtual size_t      Remove(const T& value) override;

    //////////
    //  Operations
public:

    //////////
    //  Implementation
private:
    CDictionary<T, bool, TH, TC>    m_Impl;
};

//////////
//  Construction/destruction/assignment
/*
template <class T, class TC>
CVector<T, TC>::CVector()
    : m_pImpl(new _Impl())
{
    m_pImpl->referenceCount++;
}

template <class T, class TC>
CVector<T, TC>::CVector(const CVector<T, TC>& src)
    : m_pImpl(src.m_pImpl)
{
    m_pImpl->referenceCount++;
}

template <class T, class TC>
template<class P>
CVector<T, TC>::CVector(const CReadOnlyCollection<P>& src)
    : m_pImpl(new _Impl())
{
    m_pImpl->referenceCount++;
    for (auto value : src)
    {
        this->Add(value);
    }
}

template <class T, class TC>
CVector<T, TC>::~CVector()
{
    if (--m_pImpl == 0)
    {
        delete m_pImpl;
    }
}

template <class T, class TC>
CVector<T, TC>& CVector<T, TC>::operator = (const CVector<T, TC>& src)
{
    if (this != &src)
    {
        if (--m_pImpl == 0)
        {
            delete m_pImpl;
        }
        m_pImpl = src.m_pImpl;
        m_pImpl->referenceCount++;
    }
    return *this;
}

template <class T, class TC>
template<class P>
CVector<T, TC>& CVector<T, TC>::CVector<T, TC>::operator = (const CReadOnlyCollection<P>& src)
{
    if (this != &src)
    {
        this->Clear();
        for (auto value : src)
        {
            this->Add(value);
        }
    }
    return *this;
}

//////////
//  Operators
template <class T, class TC>
T CVector<T, TC>::operator[] (size_t index) const
{
    return m_pImpl->data[index];
}

template <class T, class TC>
T& CVector<T, TC>::operator[] (size_t index)
{
    _EnsureNonsharedImpl();
    return m_pImpl->data[index];
}

template <class T, class TC>
bool CVector<T, TC>::operator == (const CVector<T, TC>& op2) const
{
    if (this->GetSize() != op2.GetSize())
    {
        return false;
    }
    for (INT_PTR i = 0; i < m_pImpl->data.GetSize(); i++)
    {
        if (!m_pImpl->comparer.Equal(m_pImpl->data[i], op2.m_pImpl->data[i]))
        {
            return false;
        }
    }
    return true;
}

//////////
//  CReadOnlyCollection<T>
template <class T, class TC>
size_t CVector<T, TC>::GetSize() const
{
    return m_pImpl->data.GetSize();
}

template <class T, class TC>
bool CVector<T, TC>::Contains(const T& value) const
{
    for (int i = 0; i < m_pImpl->data.GetSize(); i++)
    {
        if (m_pImpl->comparer.Equal(value, m_pImpl->data[i]))
        {
            return true;
        }
    }
    return false;
}

template <class T, class TC>
CIterator<T> CVector<T, TC>::GetElements() const
{
    return _Iterator(new _IteratorImpl(m_pImpl->data));
}

//////////
//  CCollection<T>
template <class T, class TC>
void CVector<T, TC>::RemoveAll()
{
    _EnsureNonsharedImpl();
    m_pImpl->data.RemoveAll();
}

template <class T, class TC>
bool CVector<T, TC>::Add(const T& value)
{
    _EnsureNonsharedImpl();
    m_pImpl->data.Add(value);
    return true;
}

template <class T, class TC>
size_t CVector<T, TC>::Remove(const T& value)
{
    _EnsureNonsharedImpl();

    size_t result = 0;
    for (size_t i = m_pImpl->data.GetSize(); i > 0; i--)
    {
        if (m_pImpl->comparer.Equal(value, m_pImpl->data[i - 1]))
        {
            m_pImpl->data.RemoveAt(i - 1);
            result++;
        }
    }
    return result;
}

//////////
//  Operations
template <class T, class TC>
void CVector<T, TC>::SetSize(size_t size)
{
    _EnsureNonsharedImpl();

    m_pImpl->data.SetSize(size);
}

//////////
//  Implementation helpers
template <class T, class TC>
void CVector<T, TC>::_EnsureNonsharedImpl()
{
    if (m_pImpl->referenceCount > 1)
    {   //  need a new _Impl just for this CVector
        _Impl* pNewImpl = new _Impl;
        pNewImpl->data.Append(m_pImpl->data);
        m_pImpl->referenceCount--;
        m_pImpl = pNewImpl;
        m_pImpl->referenceCount++;
    }
}
*/
END_TT3_NAMESPACE
//  End of Tt3.Util/Set.hpp

