//
//  Tt3.Util/CharacterSet.cpp
//  
//  CCharacterSet class implementation
//
//////////
#include "Tt3.Util/API.hpp"
USING_TT3_NAMESPACE

//////////
//  Registry
CCharacterSet::AllCollection CCharacterSet::All;

size_t CCharacterSet::AllCollection::GetSize() const
{
    return _Registry::GetInstance()->m_CharacterSetsList.GetSize();
}

bool CCharacterSet::AllCollection::Contains(const T& value) const
{
    return _Registry::GetInstance()->m_CharacterSetsList.Contains(value);
}

CIterator<CCharacterSet::AllCollection::T> CCharacterSet::AllCollection::GetElements() const
{
    return _Registry::GetInstance()->m_CharacterSetsList.GetElements();
}

CCharacterSet* CCharacterSet::GetSystem()
{
    return CUtf8CharacterSet::GetInstance();
}

CCharacterSet* CCharacterSet::Find(const char* pName)
{
    ASSERT(pName != NULL);
    return _Registry::GetInstance()->FindCharacterSet(CString(pName));
}

CCharacterSet* CCharacterSet::Find(const CString& name)
{
    return _Registry::GetInstance()->FindCharacterSet(name);
}

//////////
//  CCharacterSet::_Registry
IMPLEMENT_SINGLETON(CCharacterSet::_Registry)

CCharacterSet::_Registry::_Registry() 
{
    RegisterCharacterSet(CIso88591CharacterSet::GetInstance());
    RegisterCharacterSet(CUtf8CharacterSet::GetInstance());
}

CCharacterSet::_Registry::~_Registry() {}

void CCharacterSet::_Registry::RegisterCharacterSet(CCharacterSet* pCharacterSet)
{
    ASSERT(pCharacterSet != NULL);
    ASSERT(pCharacterSet->GetAllNames().Contains(pCharacterSet->GetCanonicalName()));
    ASSERT(pCharacterSet->GetAllNames().Contains(pCharacterSet->GetPreferredName()));

    for (CString key : pCharacterSet->GetAllNames())
    {
        key.MakeLower();
        ASSERT(!m_CharacterSetsMap.ContainsKey(key));
        m_CharacterSetsMap[key] = pCharacterSet;
    }
    m_CharacterSetsList.Add(pCharacterSet);
}

CCharacterSet* CCharacterSet::_Registry::FindCharacterSet(const CString& name)
{
    CString key = name;
    key.MakeLower();
    return m_CharacterSetsMap.GetAt(key, NULL);
}

CCharacterSetList CCharacterSet::_Registry::GetAllCharacterSets()
{
    return CCharacterSetList(m_CharacterSetsMap.Values);
}

//  End of Tt3.Util/CharacterSet.cpp
