//
//  Tt3.UnitTests/TestCharacterSetPerformance.cpp
//  
//  A test for CCharacterSet's speed of encoding and decoding of textual data
//
//////////
#include "Tt3.UnitTests/API.hpp"
#include <iostream>
#include <fstream>
BEGIN_TT3_NAMESPACE

void TestCharacterSetPerformance(const CString& characterSetName) throws(CTestException*)
{
    CCharacterSet* pCharacterSet = CCharacterSet::Find(characterSetName);
    if (pCharacterSet == NULL)
    {
        throw new CNoSuchItemError(_T("character set"), characterSetName);
    }
    TestCharacterSetPerformance(pCharacterSet);
}

void TestCharacterSetPerformance(CCharacterSet* pCharacterSet) throws(CTestException*)
{
    ASSERT(pCharacterSet != NULL);

    const size_t CharacterDataSize = 1024 * 1024; //  characters
    const size_t MaxEncodingLength = 8;
    const size_t BinaryDataSize = CharacterDataSize * MaxEncodingLength;   //  just in case
    const int Iterations = 100;

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

        void Reset() { actualBinaryDataSize = 0; }

        std::unique_ptr<BYTE[]> binaryData{ new BYTE[BinaryDataSize] };
        size_t actualBinaryDataSize = 0;
    };
    BinaryData binaryData;

    FILETIME ft1;
    GetSystemTimeAsFileTime(&ft1);
    for (int iteration = 0; iteration < Iterations; iteration++)
    {
        binaryData.Reset();
        size_t charsConsumed = pEncoder->Encode(CString(characterData.get(), CharacterDataSize),
            CharacterDataSize, binaryData);
    }
    std::unique_ptr<CCharacterSet::Decoder> pDecoder{ pCharacterSet->CreateDecoder() };
    FILETIME ft2;
    GetSystemTimeAsFileTime(&ft2);

    //  Decode binary data back into textual data
    class CharacterData : public IDataDestination<TCHAR>
    {
    public:
        CharacterData() {}

        virtual bool Send(const TCHAR& item) override
        {
            characterData[actualCharacterDataSize++] = item;
            return true;
        }

        void Reset() { actualCharacterDataSize = 0; }

        std::unique_ptr<TCHAR[]> characterData{ new TCHAR[BinaryDataSize] }; //  just in case...
        size_t actualCharacterDataSize = 0;
    };
    CharacterData decodedCharacterData;
    for (int iteration = 0; iteration < Iterations; iteration++)
    {
        decodedCharacterData.Reset();
        size_t bytesConsumed = pDecoder->Decode(binaryData.binaryData.get(),
            binaryData.actualBinaryDataSize,
            decodedCharacterData);
    }
    FILETIME ft3;
    GetSystemTimeAsFileTime(&ft3);

    //  Determine and print data transfer speed
    ULARGE_INTEGER i1, i2, i3;
    i1.LowPart = ft1.dwLowDateTime;
    i1.HighPart = ft1.dwHighDateTime;
    i2.LowPart = ft2.dwLowDateTime;
    i2.HighPart = ft2.dwHighDateTime;
    i3.LowPart = ft3.dwLowDateTime;
    i3.HighPart = ft3.dwHighDateTime;
    double ns1 = int64_t(i2.QuadPart - i1.QuadPart) * 100.0;
    double ns2 = int64_t(i3.QuadPart - i2.QuadPart) * 100.0;
    double mc1 = (CharacterDataSize / 1000000.0) * Iterations;
    double mc2 = (CharacterDataSize / 1000000.0) * Iterations;
    std::cout << (mc1 * 1000000000.0 / ns1) << " mchars/sec encode, "
              << (mc2 * 1000000000.0 / ns2) << " mchars/sec decode,";
}

END_TT3_NAMESPACE
//  End of Tt3.UnitTests/TestCharacterSetPerformance.cpp
