//
//  Tt3.UnitTests/Main.cpp
//  
//  Tt3 UnitTests top-level login
//
//////////
#include "Tt3.UnitTests/API.hpp"
#include <iostream>
USING_TT3_NAMESPACE

#define TEST2(call, testName)                                       \
    try                                                             \
    {                                                               \
        std::wcout << (const TCHAR*)testName << L"...";             \
        FILETIME ft1, ft2;                                          \
        GetSystemTimeAsFileTime(&ft1);                              \
        call;                                                       \
        GetSystemTimeAsFileTime(&ft2);                              \
                                                                    \
        ULARGE_INTEGER i1, i2;                                      \
        i1.LowPart = ft1.dwLowDateTime;                             \
        i1.HighPart = ft1.dwHighDateTime;                           \
        i2.LowPart = ft2.dwLowDateTime;                             \
        i2.HighPart = ft2.dwHighDateTime;                           \
        int64_t ns = (i2.QuadPart - i1.QuadPart) * 100;             \
        std::wcout << L"OK, took " << ((double)ns / 1000000000) << L" seconds\n"; \
    }                                                               \
    catch (CException* pEx)                                         \
    {                                                               \
        TCHAR errorMessage[256];                                    \
        pEx->GetErrorMessage(errorMessage, sizeof(errorMessage) / sizeof(errorMessage[0])); \
        std::wcout << L"ERROR! " << errorMessage << L"\n";          \
        pEx->Delete();                                              \
    }                                                               \
    catch (...)                                                     \
    {                                                               \
        std::wcout << L"Unknown ERROR\n";                           \
    }

#define TEST(call) TEST2(call, #call)

int main()
{
    //  Character sets
    //for (CCharacterSet* pCharacterSet : CCharacterSet::All)
    //{
    //    for (CString name : pCharacterSet->GetAllNames())
    //    {
    //        CString testName = _T("TestCharacterSetConsistency(") + name + _T(")");
    //        TEST2(TestCharacterSetConsistency(name), testName);
    //    }
    //}
    //for (CCharacterSet* pCharacterSet : CCharacterSet::All)
    //{
    //    for (CString name : pCharacterSet->GetAllNames())
    //    {
    //        CString testName = _T("TestCharacterSetPerformance(") + name + _T(")");
    //        TEST2(TestCharacterSetPerformance(name), testName);
    //    }
    //}

    //  Binary I/O
    //TEST(TestFileOutputStreamConsistency());
    //TEST(TestFileInputStreamConsistency());
    //TEST(TestBufferedOutputStreamConsistency());
    //TEST(TestBufferedInputStreamConsistency());
    //TEST(TestBinaryFileWriterConsistency());
    //TEST(TestBinaryFileReaderConsistency());

    //TEST(TestBinaryCrtOutputPerformanceAsChunks());
    //TEST(TestBinaryCppOutputPerformanceAsChunks());
    //TEST(TestFileOutputStreamPerformanceAsChunks());
    //TEST(TestBufferedOutputStreamPerformanceAsChunks());
    //TEST(TestBinaryFileWriterPerformanceAsChunks());

    //TEST(TestBinaryCrtOutputPerformanceAsBytes());
    //TEST(TestBinaryCppOutputPerformanceAsBytes());
    //TEST(TestFileOutputStreamPerformanceAsBytes());
    //TEST(TestBufferedOutputStreamPerformanceAsBytes());
    //TEST(TestBinaryFileWriterPerformanceAsBytes());

    //TEST(TestBinaryCrtInputPerformanceAsChunks());
    //TEST(TestBinaryCppInputPerformanceAsChunks());
    //TEST(TestFileInputStreamPerformanceAsChunks());
    //TEST(TestBufferedInputStreamPerformanceAsChunks());
    //TEST(TestBinaryFileReaderPerformanceAsChunks());

    //TEST(TestBinaryCrtInputPerformanceAsBytes());
    //TEST(TestBinaryCppInputPerformanceAsBytes());
    //TEST(TestFileInputStreamPerformanceAsBytes());
    //TEST(TestBufferedInputStreamPerformanceAsBytes());
    //TEST(TestBinaryFileReaderPerformanceAsBytes());

    //  Text I/O
    //TEST(TestTextOutputStreamConsistency());
    //TEST(TestTextInputStreamConsistency());
}

//  End of Tt3.UnitTests/Main.cpp
