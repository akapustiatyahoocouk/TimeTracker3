//
//  Tt3.UnitTests/TestFileOutputStreamPerformanceAsBytes.cpp
//  
//  A test for the CFileOutputStream output performance (as bytes)
//
//////////
#include "Tt3.UnitTests/API.hpp"
#include <iostream>
BEGIN_TT3_NAMESPACE

void TestFileOutputStreamPerformanceAsBytes() throws(CTestException*)
{
    const size_t RandomDataSize = 1024 * 1024 * 4;
    const size_t RandomChunkSize = 1024;
    BYTE randomChunk[RandomChunkSize];
    const size_t Iterations = 4;

    //  Prepare the random data for the test
    for (size_t i = 0; i < RandomChunkSize; i++)
    {
        randomChunk[i] = (BYTE)rand();
    }

    FILETIME ft1;
    GetSystemTimeAsFileTime(&ft1);
    for (size_t iteration = 0; iteration < Iterations; iteration++)
    {
        //  Write random data to a file as one big chunk...
        char tempFileName[MAX_PATH];
        if (::GetTempFileNameA(".", "tmp", 0, tempFileName) == 0)
        {   //  OOPS!
            throw new CTestException("Could not create a temp file name");
        }
        CFileOutputStream f{ CString(tempFileName) };
        for (size_t i = 0; i < RandomDataSize; i++)
        {
            f.Write(randomChunk[i % RandomChunkSize]);
        }
        f.Close();
        remove(tempFileName);
    }
    FILETIME ft2;
    GetSystemTimeAsFileTime(&ft2);

    //  Determine and print data transfer speed
    ULARGE_INTEGER i1, i2;
    i1.LowPart = ft1.dwLowDateTime;
    i1.HighPart = ft1.dwHighDateTime;
    i2.LowPart = ft2.dwLowDateTime;
    i2.HighPart = ft2.dwHighDateTime;
    double ns = int64_t(i2.QuadPart - i1.QuadPart) * 100.0;
    double mb = (RandomDataSize / 1000000.0) * Iterations;
    std::cout << (mb * 1000000000.0 / ns) << " mb/sec, ";
}

END_TT3_NAMESPACE
//  End of Tt3.UnitTests/TestFileOutputStreamPerformanceAsBytes.cpp
