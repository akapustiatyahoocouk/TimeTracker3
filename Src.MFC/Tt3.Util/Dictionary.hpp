//
//  Tt3.Util/Dictionary.hpp
//  
//  Tt3 A key -> value dictionary
//
//////////
BEGIN_TT3_NAMESPACE

//////////
//  A key -> value dictionary
template <class K, class V, class KH = CDefaultHash<K>,
          class KC = CNaturalComparer<K>, class VC = CNaturalComparer<V>>
class CDictionary : public CCollection<CPair<K, V>>
{
    //////////
    //  Construction/destruction/assignment
public:
    CDictionary();
    CDictionary(const CDictionary<K, V, KH, KC, VC> & src);
    virtual ~CDictionary();

    CDictionary<K, V, KH, KC, VC> & operator = (const CDictionary<K, V, KH, KC, VC> & src);

    //////////
    //  Operators
public:
    V                   operator [](const K & key) const;   //  returns V() is no mapping exists for "key"
    V &                 operator [](const K & key); //  creates new mapping for "key" if not yet exists

    //////////
    //  CReadOnlyCollection<CPair<K, V>>
public:
    virtual size_t      GetSize() const override;
    virtual bool        Contains(const CPair<K, V> & value) const override;
    virtual CIterator<CPair<K, V>>  GetElements() const override;

    //////////
    //  CCollection<CPair<K, V>>
public:
    virtual void        RemoveAll() override;
    virtual bool        Add(const CPair<K, V> & value) override;
    virtual size_t      Remove(const CPair<K, V> & value) override;

    //////////
    //  Properties
public:
    //  The read-only view of all keys in this dictionary
    class KeysCollection : public CReadOnlyCollection<K>
    {
        friend class CDictionary<K, V, KH, KC, VC>;

        //////////
        //  Construction/destruction
    private:
        KeysCollection(CDictionary<K, V, KH, KC, VC> * pHost) : m_pHost(pHost) {}

        //////////
        //  CReadOnlyCollection<K>
    public:
        virtual size_t      GetSize() const override { return m_pHost->GetSize(); }
        virtual bool        Contains(const K & value) const override { return m_pHost->ContainsKey(value); }
        virtual CIterator<K>GetElements() const override { return _KeyIterator(new _KeyIteratorImpl(*m_pHost)); }

        //////////
        //  Implementation
    private:
        CDictionary<K, V, KH, KC, VC> * m_pHost;
    };
    KeysCollection      Keys;

    //  The read-only view of all values in this dictionary
    class ValuesCollection : public CReadOnlyCollection<V>
    {
        friend class CDictionary<K, V, KH, KC, VC>;

        //////////
        //  Construction/destruction
    private:
        ValuesCollection(CDictionary<K, V, KH, KC, VC> * pHost) : m_pHost(pHost) {}

        //////////
        //  CReadOnlyCollection<K>
    public:
        virtual size_t      GetSize() const override { return m_pHost->GetSize(); }
        virtual bool        Contains(const V & value) const override { return m_pHost->ContainsValue(value); }
        virtual CIterator<V>GetElements() const override { return _ValueIterator(new _ValueIteratorImpl(*m_pHost)); }

        //////////
        //  Implementation
    private:
        CDictionary<K, V, KH, KC, VC> * m_pHost;
    };
    ValuesCollection    Values;

    //////////
    //  Operations
public:
    //  TODO document
    virtual bool        ContainsKey(const K & key) const;
    virtual bool        ContainsValue(const V & value) const;

    //  If the mapping for the specified key exists in this dictionary
    //  returns the mapping's value, else returns the specified default value
    virtual V           GetAt(const K & key, const V & defaultValue = V());

    //  If this dictionary contains mapping for the specified key then
    //  removes it and returns true; else just returns false
    virtual bool        RemoveKey(const K & key);

    //////////
    //  Implementation
private:
    struct _Element
    {
        _Element(const K & keyParam, const V & valueParam, _Element * pNextInChainParam)
            :   key(keyParam), value(valueParam), pNextInChain(pNextInChainParam) {}

        K               key;
        V               value;
        _Element *      pNextInChain;
    };

    struct _Impl
    {
        _Impl(size_t hashTableSizeParam)
            :   numberOfElements(0),
                hashTableSize(hashTableSizeParam),
                pHashTable(new _Element*[hashTableSizeParam])
        {
            for (size_t i = 0; i < hashTableSizeParam; i++)
            {
                pHashTable[i] = NULL;
            }
        }

        ~_Impl()
        {
            for (size_t i = 0; i < hashTableSize; i++)
            {
                while (pHashTable[i] != NULL)
                {
                    _Element * pElement = pHashTable[i];
                    pHashTable[i] = pElement->pNextInChain;
                    delete pElement;
                }
            }
        }

        int             referenceCount;
        KC              keyComparer;
        VC              valueComparer;
        KH              keyHash;

        size_t          numberOfElements;
        size_t          hashTableSize;
        _Element **     pHashTable; //  array of hashTableSize elements
    };
    _Impl *             m_pImpl;

    class _PairIteratorImpl : public CIterator<CPair<K, V>>::Impl
    {
    public:
        explicit _PairIteratorImpl(const CDictionary<K, V, KH, KC, VC> & dictionary) 
        { 
            for (size_t i = 0; i < dictionary.m_pImpl->hashTableSize; i++)
            {
                for (_Element * pElement = dictionary.m_pImpl->pHashTable[i]; pElement != NULL; pElement = pElement->pNextInChain)
                {
                    data.Add(CPair<K, V>(pElement->key, pElement->value));
                }
            }
        }

    public:
        virtual bool    HasFinished() const override { return currentIndex >= data.GetSize(); }
        virtual CPair<K, V> GetItem() const override { return (currentIndex < data.GetSize()) ? data[currentIndex] : CPair<K, V>(); }
        virtual void    Advance() override { if (currentIndex < data.GetSize()) currentIndex++; }

    private:
        CArray<CPair<K, V>> data;
        int             currentIndex = 0;
    };

    class _PairIterator : public CIterator<CPair<K, V>>
    {
    public:
        explicit _PairIterator(_PairIteratorImpl * pImpl) : CIterator<CPair<K, V>>(pImpl) {}
    };

    class _KeyIteratorImpl : public CIterator<K>::Impl
    {
    public:
        explicit _KeyIteratorImpl(const CDictionary<K, V, KH, KC, VC> & dictionary)
        {
            for (size_t i = 0; i < dictionary.m_pImpl->hashTableSize; i++)
            {
                for (_Element * pElement = dictionary.m_pImpl->pHashTable[i]; pElement != NULL; pElement = pElement->pNextInChain)
                {
                    data.Add(pElement->key);
                }
            }
        }

    public:
        virtual bool    HasFinished() const override { return currentIndex >= data.GetSize(); }
        virtual K       GetItem() const override { return (currentIndex < data.GetSize()) ? data[currentIndex] : K(); }
        virtual void    Advance() override { if (currentIndex < data.GetSize()) currentIndex++; }

    private:
        CArray<K>       data;
        INT_PTR         currentIndex = 0;
    };

    class _KeyIterator : public CIterator<K>
    {
    public:
        explicit _KeyIterator(_KeyIteratorImpl * pImpl) : CIterator<K>(pImpl) {}
    };

    class _ValueIteratorImpl : public CIterator<V>::Impl
    {
    public:
        explicit _ValueIteratorImpl(const CDictionary<K, V, KH, KC, VC> & dictionary)
        {
            for (size_t i = 0; i < dictionary.m_pImpl->hashTableSize; i++)
            {
                for (_Element * pElement = dictionary.m_pImpl->pHashTable[i]; pElement != NULL; pElement = pElement->pNextInChain)
                {
                    data.Add(pElement->value);
                }
            }
        }

    public:
        virtual bool    HasFinished() const override { return currentIndex >= data.GetSize(); }
        virtual V       GetItem() const override { return (currentIndex < data.GetSize()) ? data[currentIndex] : V(); }
        virtual void    Advance() override { if (currentIndex < data.GetSize()) currentIndex++; }

    private:
        CArray<V>       data;
        INT_PTR         currentIndex = 0;
    };

    class _ValueIterator : public CIterator<V>
    {
    public:
        explicit _ValueIterator(_ValueIteratorImpl * pImpl) : CIterator<V>(pImpl) {}
    };

    //  Helpers
    void                _EnsureNonsharedImpl();
    void                _ExpandHashTable(); //  ...only if >75% full
};

//////////
//  Construction/destruction/assignment
template <class K, class V, class KH, class KC, class VC>
CDictionary<K, V, KH, KC, VC>::CDictionary()
    :   Keys(this),
        Values(this),
        m_pImpl(new _Impl(7))
{
    m_pImpl->referenceCount++;
}

template <class K, class V, class KH, class KC, class VC>
CDictionary<K, V, KH, KC, VC>::CDictionary(const CDictionary<K, V, KH, KC, VC> & src)
    :   Keys(this),
        Values(this),
        m_pImpl(src.m_pImpl)
{
    m_pImpl->referenceCount++;
}

template <class K, class V, class KH, class KC, class VC>
CDictionary<K, V, KH, KC, VC>::~CDictionary()
{
    if (--m_pImpl->referenceCount == 0)
    {
        delete m_pImpl;
    }
}

template <class K, class V, class KH, class KC, class VC>
CDictionary<K, V, KH, KC, VC> & CDictionary<K, V, KH, KC, VC>::operator = (const CDictionary<K, V, KH, KC, VC> & src)
{
    if (this != &src)
    {
        if (--m_pImpl->referenceCount == 0)
        {
            delete m_pImpl;
        }
        m_pImpl = src.m_pImpl;
        m_pImpl->referenceCount++;
    }
    return *this;
}

//////////
//  Operators
template <class K, class V, class KH, class KC, class VC>
V CDictionary<K, V, KH, KC, VC>::operator [](const K & key) const
{
    return GetAt(key, V());
}

template <class K, class V, class KH, class KC, class VC>
V & CDictionary<K, V, KH, KC, VC>::operator [](const K & key)
{
    _EnsureNonsharedImpl();

    //  Does a mapping for "key" exist ?
    size_t hashCode = m_pImpl->keyHash.HashCode(key) % m_pImpl->hashTableSize;
    for (_Element * pElement = m_pImpl->pHashTable[hashCode]; pElement != NULL; pElement = pElement->pNextInChain)
    {
        if (m_pImpl->keyComparer.Equal(key, pElement->key))
        {   //  Yes!
            return pElement->value;
        }
    }
    //  Need a new mapping
    _Element * pNewElement = new _Element(key, V(), m_pImpl->pHashTable[hashCode]);
    m_pImpl->pHashTable[hashCode] = pNewElement;
    m_pImpl->numberOfElements++;
    _ExpandHashTable();
    return pNewElement->value;
}

//////////
//  CReadOnlyCollection<CPair<K, V>>
template <class K, class V, class KH, class KC, class VC>
size_t CDictionary<K, V, KH, KC, VC>::GetSize() const
{
    return m_pImpl->numberOfElements;
}

template <class K, class V, class KH, class KC, class VC>
bool CDictionary<K, V, KH, KC, VC>::Contains(const CPair<K, V> & value) const
{
    size_t hashCode = m_pImpl->keyHash.HashCode(value.first) % m_pImpl->hashTableSize;
    for (_Element * pElement = m_pImpl->pHashTable[hashCode]; pElement != NULL; pElement = pElement->pNextInChain)
    {
        if (m_pImpl->keyComparer.Equal(value.first, pElement->key) &&
            m_pImpl->valueComparer.Equal(value.second, pElement->value))
        {
            return true;
        }
    }
    return false;
}

template <class K, class V, class KH, class KC, class VC>
CIterator<CPair<K, V>> CDictionary<K, V, KH, KC, VC>::GetElements() const
{
    return _PairIterator(new _PairIteratorImpl(*this));
}

//////////
//  CCollection<CPair<K, V>>
template <class K, class V, class KH, class KC, class VC>
void CDictionary<K, V, KH, KC, VC>::RemoveAll()
{
    _EnsureNonsharedImpl();

    for (size_t i = 0; i < m_pImpl->hashTableSize; i++)
    {
        while (m_pImpl->pHashTable[i] != NULL)
        {
            _Element * pElement = m_pImpl->pHashTable[i];
            m_pImpl->pHashTable[i] = pElement->pNextInChain;
            delete pElement;
        }
    }
    m_pImpl->numberOfElements = 0;
}

template <class K, class V, class KH, class KC, class VC>
bool CDictionary<K, V, KH, KC, VC>::Add(const CPair<K, V> & value)
{
    _EnsureNonsharedImpl();

    //  Does mapping already exist for the "key" ?
    size_t hashCode = m_pImpl->keyHash.HashCode(value.first) % m_pImpl->hashTableSize;
    for (_Element * pElement = m_pImpl->pHashTable[hashCode]; pElement != NULL; pElement = pElement->pNextInChain)
    {
        if (m_pImpl->keyComparer.Equal(value.first, pElement->key))
        {   //  Yes - replace the value if necessary
            if (m_pImpl->valueComparer.Equal(value.second, pElement->value))
            {   //  key already mapped to a proper value
                return false;
            }
            else
            {   //  key already mapped to a different value
                pElement->value = value.second;
                return true;
            }
        }
    }
    //  Need a new mapping
    m_pImpl->pHashTable[hashCode] = new _Element(value.first, value.second, m_pImpl->pHashTable[hashCode]);
    m_pImpl->numberOfElements++;
    _ExpandHashTable();
    return true;
}

template <class K, class V, class KH, class KC, class VC>
size_t CDictionary<K, V, KH, KC, VC>::Remove(const CPair<K, V> & value)
{
    size_t hashCode = m_pImpl->keyHash.HashCode(value.first) % m_pImpl->hashTableSize;
    for (_Element ** ppElement = &(m_pImpl->pHashTable[hashCode]); (*ppElement) != NULL; ppElement = &((*ppElement)->pNextInChain))
    {
        if (m_pImpl->keyComparer.Equal(value.first, (*ppElement)->key) &&
            m_pImpl->valueComparer.Equal(value.second, (*ppElement)->value))
        {   //  Remove this mapping
            _Element * pElement = (*ppElement);
            (*ppElement) = pElement->pNextInChain;
            delete pElement;
            m_pImpl->numberOfElements--;
            return 1;
        }
    }
    return 0;
}

//////////
//  Operations
template <class K, class V, class KH, class KC, class VC>
bool CDictionary<K, V, KH, KC, VC>::ContainsKey(const K & key) const
{
    size_t hashCode = m_pImpl->keyHash.HashCode(key) % m_pImpl->hashTableSize;
    for (_Element * pElement = m_pImpl->pHashTable[hashCode]; pElement != NULL; pElement = pElement->pNextInChain)
    {
        if (m_pImpl->keyComparer.Equal(key, pElement->key))
        {
            return true;
        }
    }
    return false;
}

template <class K, class V, class KH, class KC, class VC>
bool CDictionary<K, V, KH, KC, VC>::ContainsValue(const V & value) const
{
    for (size_t i = 0; i < m_pImpl->hashTableSize; i++)
    {
        for (_Element * pElement = m_pImpl->pHashTable[i]; pElement != NULL; pElement = pElement->pNextInChain)
        {
            if (m_pImpl->valueComparer.Equal(value, pElement->value))
            {
                return true;
            }
        }
    }
    return false;
}

template <class K, class V, class KH, class KC, class VC>
V CDictionary<K, V, KH, KC, VC>::GetAt(const K & key, const V & defaultValue)
{
    size_t hashCode = m_pImpl->keyHash.HashCode(key) % m_pImpl->hashTableSize;
    for (_Element * pElement = m_pImpl->pHashTable[hashCode]; pElement != NULL; pElement = pElement->pNextInChain)
    {
        if (m_pImpl->keyComparer.Equal(key, pElement->key))
        {
            return pElement->value;
        }
    }
    return defaultValue;
}

template <class K, class V, class KH, class KC, class VC>
bool CDictionary<K, V, KH, KC, VC>::RemoveKey(const K & key)
{
    size_t hashCode = m_pImpl->keyHash.HashCode(key) % m_pImpl->hashTableSize;
    for (_Element ** ppElement = &(m_pImpl->pHashTable[hashCode]); (*ppElement) != NULL; ppElement = &((*ppElement)->pNextInChain))
    {
        if (m_pImpl->keyComparer.Equal(key, (*ppElement)->key))
        {   //  Remove this mapping
            _Element * pElement = (*ppElement);
            (*ppElement) = pElement->pNextInChain;
            delete pElement;
            m_pImpl->numberOfElements--;
            return true;
        }
    }
    return false;
}

//////////
//  Implementation helpers
template <class K, class V, class KH, class KC, class VC>
void CDictionary<K, V, KH, KC, VC>::_EnsureNonsharedImpl()
{
    if (m_pImpl->referenceCount > 1)
    {   //  Need a new _Impl just for this dictionary
        _Impl * pPrivateImpl = new _Impl(m_pImpl->hashTableSize);
        for (size_t i = 0; i < m_pImpl->hashTableSize; i++)
        {
            for (_Element * pElement = m_pImpl->pHashTable[i]; pElement != NULL; pElement = pElement->pNextInChain)
            {   //  Element's hash code is the same for the "private" _Impl
                pPrivateImpl->pHashTable[i] = new _Element(pElement->key, pElement->value, pPrivateImpl->pHashTable[i]);
            }
        }
        pPrivateImpl->numberOfElements = m_pImpl->numberOfElements;
        //  Use new "private" impl for this dictionary only
        m_pImpl->referenceCount--;
        m_pImpl = pPrivateImpl;
        m_pImpl->referenceCount++;
        ASSERT(m_pImpl->referenceCount == 1);
    }
}

template <class K, class V, class KH, class KC, class VC>
void CDictionary<K, V, KH, KC, VC>::_ExpandHashTable()
{
    if (m_pImpl->numberOfElements * 4 > m_pImpl->hashTableSize * 3)
    {   //  > 75% full - allocate new hash table...
        size_t newHashTableSize = m_pImpl->hashTableSize * 3 / 2 + 1;
        ASSERT(newHashTableSize > m_pImpl->hashTableSize);
        _Element ** pNewHashTable = new _Element * [newHashTableSize];
        for (size_t i = 0; i < newHashTableSize; i++)
        {
            pNewHashTable[i] = NULL;
        }
        //  ...move elements there...
        for (size_t i = 0; i < m_pImpl->hashTableSize; i++)
        {
            while (m_pImpl->pHashTable[i] != NULL)
            {
                _Element * pElement = m_pImpl->pHashTable[i];
                m_pImpl->pHashTable[i] = pElement->pNextInChain;
                size_t newHashCode = m_pImpl->keyHash.HashCode(pElement->key) % newHashTableSize;
                pElement->pNextInChain = pNewHashTable[newHashCode];
                pNewHashTable[newHashCode] = pElement;
            }
        }
        //  ...and replace current hash table with the new one
        delete[] m_pImpl->pHashTable;
        m_pImpl->pHashTable = pNewHashTable;
        m_pImpl->hashTableSize = newHashTableSize;
    }
}

END_TT3_NAMESPACE
//  End of Tt3.Util/Dictionary.hpp
