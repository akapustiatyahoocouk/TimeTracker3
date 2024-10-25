//
//  Tt3.UnitTests/TestCharacterSetConsistency.cpp
//  
//  A test for CCharacterSet's encoding and decoding of tectual data
//
//////////
#include "Tt3.UnitTests/API.hpp"
#include <iostream>
#include <fstream>
BEGIN_TT3_NAMESPACE

void TestCharacterSetConsistency(const CString& characterSetName) throws(CTestException*)
{
    CCharacterSet* pCharacterSet = CCharacterSet::Find(characterSetName);
    if (pCharacterSet == NULL)
    {
        throw new CNoSuchItemError(_T("character set"), characterSetName);
    }
    TestCharacterSetConsistency(pCharacterSet);
}

void TestCharacterSetConsistency(CCharacterSet* pCharacterSet) throws(CTestException*)
{
    ASSERT(pCharacterSet != NULL);

    const size_t CharacterDataSize = 1024 * 1024; //  characters
    const size_t MaxEncodingLength = 8;
    const size_t BinaryDataSize = CharacterDataSize * MaxEncodingLength;   //  just in case

    //  Prepare random character data
    std::unique_ptr<CCharacterSet::Encoder> pEncoder{ pCharacterSet->CreateEncoder() };
    std::unique_ptr<TCHAR[]> characterData{ new TCHAR[CharacterDataSize] };
    for (int i = 0; i < CharacterDataSize; )
    {
        TCHAR c = (TCHAR)rand();
        int encodingLength = pEncoder->GetEncodingLength(c);
        if (encodingLength > 0)
        {
            ASSERT((size_t)encodingLength <= MaxEncodingLength);
            characterData[i++] = c;
        }
    }

    //  Encode character data to binary
    class BinaryData : public IDataDestination<BYTE>
    {
    public:
        BinaryData() {}

        virtual bool Send(const BYTE& item) override
        {
            binaryData[actualBinaryDataSize++] = item;
            return true;
        }

        std::unique_ptr<BYTE[]> binaryData{ new BYTE[BinaryDataSize] };
        size_t actualBinaryDataSize = 0;
    };
    BinaryData binaryData;
    size_t charsConsumed = pEncoder->Encode(CString(characterData.get(), CharacterDataSize),
                                            CharacterDataSize, binaryData);
    if (charsConsumed  != CharacterDataSize)
    {
        throw new CTestException("Attempted to encode " + ToString(CharacterDataSize) +
            " characters, but " + ToString(charsConsumed) + " characters were consumed");
    }

    //  Decode binary data back into textual data
    std::unique_ptr<CCharacterSet::Decoder> pDecoder{ pCharacterSet->CreateDecoder() };
    class CharacterData : public IDataDestination<TCHAR>
    {
    public:
        CharacterData() {}

        virtual bool Send(const TCHAR& item) override
        {
            characterData[actualCharacterDataSize++] = item;
            return true;
        }

        std::unique_ptr<TCHAR[]> characterData{ new TCHAR[BinaryDataSize] }; //  just in case...
        size_t actualCharacterDataSize = 0;
    };
    CharacterData decodedCharacterData;
    size_t bytesConsumed = pDecoder->Decode(binaryData.binaryData.get(),
                                            binaryData.actualBinaryDataSize,
                                            decodedCharacterData);
    if (bytesConsumed != binaryData.actualBinaryDataSize)
    {
        throw new CTestException("Attempted to decode " + ToString(binaryData.actualBinaryDataSize) +
            " bytes, but " + ToString(bytesConsumed) + " bytes were consumed");
    }
    if (decodedCharacterData.actualCharacterDataSize != CharacterDataSize)
    {
        throw new CTestException("Attempted to decode " + ToString(CharacterDataSize) +
            " characters, but " + ToString(decodedCharacterData.actualCharacterDataSize) + " characters were decoded");
    }
    if (memcmp(characterData.get(), decodedCharacterData.characterData.get(), sizeof(TCHAR) * CharacterDataSize) != 0)
    {
        throw new CTestException("Decoding an encoding character block produced different result from the original");
    }
}

END_TT3_NAMESPACE
//  End of Tt3.UnitTests/TestCharacterSetConsistency.cpp
