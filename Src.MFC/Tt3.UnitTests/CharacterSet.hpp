//
//  Tt3.UnitTests/CharacterSet.hpp
//  
//  Tt3 UnitTests: Tests for Tt3.Util CharacterSet.hpp facilities
//
//////////
BEGIN_TT3_NAMESPACE

void TestCharacterSetConsistency(const CString & characterSetName) throws(CTestException*);
void TestCharacterSetConsistency(CCharacterSet* pCharacterSet) throws(CTestException*);
void TestCharacterSetPerformance(const CString& characterSetName) throws(CTestException*);
void TestCharacterSetPerformance(CCharacterSet* pCharacterSet) throws(CTestException*);

END_TT3_NAMESPACE
//  End of Tt3.UnitTests/CharacterSet.hpp
