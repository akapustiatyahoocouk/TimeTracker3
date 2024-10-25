//
//  Tt3.UnitTests/TestBinaryCppOutputPerformanceAsChunks.cpp
//  
//  A test for the binary C++ output performance (in chinks)
//
//////////
#include "Tt3.UnitTests/API.hpp"
#include <iostream>
#include <fstream>
BEGIN_TT3_NAMESPACE

void TestBinaryCppOutputPerformanceAsChunks() throws(CTestException*)
{
    const size_t RandomDataSize = 1024 * 1024 * 1024;
    const size_t ChunkSize = 1024;
    BYTE randomChunk[ChunkSize];
    const size_t Iterations = 4;

    //  Prepare the random data for the test
    for (size_t i = 0; i < ChunkSize; i++)
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
        std::fstream f(tempFileName, std::ios::out | std::ios::binary | std::ios::trunc);
        if (!f.is_open())
        {   //  OOPS!
            remove(tempFileName);
            throw new CTestException("Could not open " + CString(tempFileName) + " for writing");
        }
        for (size_t scan = 0; scan < RandomDataSize; )
        {
            size_t chunkSize = min(ChunkSize, RandomDataSize - scan);
            f.write((char*)randomChunk, chunkSize);
            scan += chunkSize;
        }
        f.close();
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
//  End of Tt3.UnitTests/TestBinaryCppOutputPerformanceAsChunks.cpp
