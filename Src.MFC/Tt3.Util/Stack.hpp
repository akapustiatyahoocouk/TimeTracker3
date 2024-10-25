//
//  Tt3.Util/Stack.hpp
//  
//  Tt3 dynamic stack
//
//////////
BEGIN_TT3_NAMESPACE

//////////
//  A dynamically resizable stack
template <class T, class TC = CNaturalComparer<T>>
class CStack : public CCollection<T>
{
    //////////
    //  Construction/destruction/assignment
public:
    CStack();
    CStack(const CStack<T, TC>& src);
    virtual ~CStack();

    CStack<T, TC>&      operator = (const CStack<T, TC>& src);

    //////////
    //  CReadOnlyCollection<T>
public:
    virtual size_t      GetSize() const override;
    virtual bool        Contains(const T& value) const override;
    virtual CIterator<T>GetElements() const override;   //  top-to-bottom

    //////////
    //  CCollection<T>
public:
    virtual void        RemoveAll() override;
    virtual bool        Add(const T& value) override;
    virtual size_t      Remove(const T& value) override;

    //////////
    //  Operations
public:
    //  TODO document
    virtual void        Push(const T& value);
    virtual const T&    Top();  //  throws CNoSuchItemError* if empty
    virtual T           Pop();  //  throws CNoSuchItemError* if empty

    //////////
    //  Implementation
private:
    struct _Impl
    {
        int             referenceCount = 0;
        TC              comparer;
        CArray<T>       data;
    };
    _Impl*              m_pImpl;

    class _TopToBottomIteratorImpl : public CIterator<T>::Impl
    {
    public:
        explicit _TopToBottomIteratorImpl(const CStack<T, TC>& stack)
        {
            for (INT_PTR i = stack.m_pImpl->data.GetSize(); i > 0; i--)
            {
                data.Add(stack.m_pImpl->data[i - 1]);
            }
        }

    public:
        virtual bool    HasFinished() const override { return currentIndex >= data.GetSize(); }
        virtual T       GetItem() const override { return (currentIndex < data.GetSize()) ? data[currentIndex] : T(); }
        virtual void    Advance() override { if (currentIndex < data.GetSize()) currentIndex++; }

    private:
        CArray<T>       data;   //  bottom at [0], top at [GetSize() - 1]
        INT_PTR         currentIndex = 0;
    };

    class _TopToBottomIterator : public CIterator<T>
    {
    public:
        explicit _TopToBottomIterator(_TopToBottomIteratorImpl* pImpl) : CIterator<T>(pImpl) {}
    };

    //  Helpers
    void                _EnsureNonsharedImpl();
};

//////////
//  Construction/destruction/assignment
template <class T, class TC>
CStack<T, TC>::CStack()
    :   m_pImpl(new _Impl())
{
    m_pImpl->referenceCount++;
}

template <class T, class TC>
CStack<T, TC>::CStack(const CStack<T, TC>& src)
    :   m_pImpl(src.m_pImpl)
{
    m_pImpl->referenceCount++;
}

template <class T, class TC>
CStack<T, TC>::~CStack()
{
    if (--m_pImpl == 0)
    {
        delete m_pImpl;
    }
}

template <class T, class TC>
CStack<T, TC>& CStack<T, TC>::operator = (const CStack<T, TC>& src)
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

//////////
//  CReadOnlyCollection<T>
template <class T, class TC>
size_t CStack<T, TC>::GetSize() const
{
    return m_pImpl->data.GetSize();
}

template <class T, class TC>
bool CStack<T, TC>::Contains(const T& value) const
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
CIterator<T> CStack<T, TC>::GetElements() const
{
    return _TopToBottomIterator(new _TopToBottomIteratorImpl(m_pImpl->data));
}

//////////
//  CCollection<T>
template <class T, class TC>
void CStack<T, TC>::RemoveAll()
{
    _EnsureNonsharedImpl();
    m_pImpl->data.RemoveAll();
}

template <class T, class TC>
bool CStack<T, TC>::Add(const T& value)
{
    _EnsureNonsharedImpl();
    m_pImpl->data.Add(value);
    return true;
}

template <class T, class TC>
size_t CStack<T, TC>::Remove(const T& value)
{
    _EnsureNonsharedImpl();

    size_t result = 0;
    for (INT_PTR i = m_pImpl->data.GetSize(); i > 0; i--)
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
void CStack<T, TC>::Push(const T& value)
{
    this->Add(value);
}

template <class T, class TC>
const T& CStack<T, TC>::Top()
{
    ASSERT(m_pImpl->data.GetSize() > 0);
    return m_pImpl->data[m_pImpl->data.GetSize() - 1];
}

template <class T, class TC>
T CStack<T, TC>::Pop()
{
    ASSERT(m_pImpl->data.GetSize() > 0);
    T result = m_pImpl->data[m_pImpl->data.GetSize() - 1];
    m_pImpl->data.RemoveAt(m_pImpl->data.GetSize() - 1);
    return result;
}

//////////
//  Implementation helpers
template <class T, class TC>
void CStack<T, TC>::_EnsureNonsharedImpl()
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

END_TT3_NAMESPACE
//  End of Tt3.Util/Stack.hpp
