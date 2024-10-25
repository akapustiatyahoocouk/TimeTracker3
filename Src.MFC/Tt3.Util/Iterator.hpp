//
//  Tt3.Util/Iterator.hpp
//  
//  Tt3 Iterator framework
//
//////////
BEGIN_TT3_NAMESPACE

//  An iterator over a collection of values
template <class T>
class CIterator
{
    //////////
    //  Types
public:
    class Impl
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Impl)

        friend class CIterator;

        //////////
        //  Construction/destruction
    public:
        Impl() = default;
        virtual ~Impl() = default;

        //////////
        //  Operations
    public:
        //  Checks whether the iterator has reached the end of the sequence
        virtual bool    HasFinished() const = 0;

        //  Returns the "current" item, or T() if the iterator has ffinished
        virtual T       GetItem() const = 0;

        //  Advances the iterator to the "next" item if possible
        virtual void    Advance() = 0;

        //////////
        //  Implementation
    private:
        int             m_ReferenceCount = 0;
    };

    //////////
    //  Construction/destruction/assignment
protected:
    explicit CIterator(Impl * pImpl);
public:
    CIterator() : m_pImpl(NULL) {}
    CIterator(const CIterator & src);
    CIterator(CIterator && src) noexcept;
    ~CIterator();

    CIterator &         operator = (const CIterator & src);
    CIterator &         operator = (CIterator && src);

    //////////
    //  Operations
public:
    //  Checks whether the iterator has reached the end of the sequence
    bool                HasFinished() const;

    //  Returns the "current" item, or T() if the iterator has ffinished
    T                   GetItem() const;

    //  Advances the iterator to the "next" item if possible
    void                Advance();

    //////////
    //  Implementation
private:
    Impl *              m_pImpl;    //  can be NULL
};

//////////
//  Construction/destruction/assignment
template <class T>
CIterator<T>::CIterator(Impl * pImpl)
    :   m_pImpl(pImpl)
{
    if (m_pImpl != NULL)
    {
        m_pImpl->m_ReferenceCount++;
    }
}

template <class T>
CIterator<T>::CIterator(const CIterator<T> & src)
    :   m_pImpl(src.m_pImpl)
{
    if (m_pImpl != NULL)
    {
        m_pImpl->m_ReferenceCount++;
    }
}

template <class T>
CIterator<T>::CIterator(CIterator<T> && src) noexcept
    :   m_pImpl(src.m_pImpl)
{
    src.m_pImpl = NULL;
}

template <class T>
CIterator<T>::~CIterator()
{
    if (m_pImpl != NULL)
    {
        if (--m_pImpl->m_ReferenceCount == 0)
        {
            delete m_pImpl;
        }
    }
}

template <class T>
CIterator<T> & CIterator<T>::operator = (const CIterator<T> & src)
{
    if (this != &src)
    {
        if (m_pImpl != NULL)
        {
            if (--m_pImpl->m_ReferenceCount == 0)
            {
                delete m_pImpl;
            }
        }
        m_pImpl = src.m_pImpl;
        if (m_pImpl != NULL)
        {
            m_pImpl->m_ReferenceCount++;
        }
    }
    return *this;
}

template <class T>
CIterator<T> & CIterator<T>::operator = (CIterator<T> && src)
{
    src = src.m_pImpl;
    src.m_pImpl = NULL;
}

//////////
//  Operations
template <class T>
bool CIterator<T>::HasFinished() const
{
    return (m_pImpl == NULL) ? true : m_pImpl->HasFinished();
}

template <class T>
T CIterator<T>::GetItem() const
{
    return (m_pImpl == NULL || m_pImpl->HasFinished()) ? T() : m_pImpl->GetItem();
}

template <class T>
void CIterator<T>::Advance()
{
    if (m_pImpl != NULL)
    {
        m_pImpl->Advance();
    }
}

END_TT3_NAMESPACE
//  End of Tt3.Util/Iterator.hpp
