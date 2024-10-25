//
//  Tt3.UnitTests/TestBinaryCrtInputPerformanceAsChunks.cpp
//  
//  A test for the binary CRT input performance (as chunks)
//
//////////
#include "Tt3.UnitTests/API.hpp"
#include <iostream>
BEGIN_TT3_NAMESPACE

void TestBinaryCrtInputPerformanceAsChunks() throws(CTestException*)
{
    const size_t RandomDataSize = 1024 * 1024 * 1024;
    const size_t RandomChunkSize = 1024;
    BYTE randomChunk[RandomChunkSize];
    const size_t Iterations = 16;

    //  Prepare the random data for the test
    for (size_t i = 0; i < RandomChunkSize; i++)
    {
        randomChunk[i] = (BYTE)rand();
    }
    char tempFileName[MAX_PATH];
    if (::GetTempFileNameA(".", "tmp", 0, tempFileName) == 0)
    {   //  OOPS!
        throw new CTestException("Could not create a temp file name");
    }
    FILE* f = fopen(tempFileName, "wb");
    if (f == NULL)
    {   //  OOPS!
        throw new CTestException("Could not open " + CString(tempFileName) + " for writing");
    }
    for (size_t scan = 0; scan < RandomDataSize; )
    {
        size_t chunkSize = min(RandomChunkSize, RandomDataSize - scan);
        if (fwrite(randomChunk, 1, chunkSize, f) != chunkSize)
        {
            fclose(f);
            remove(tempFileName);
            throw new CTestException("Error writing to " + CString(tempFileName));
        }
        scan += chunkSize;
    }
    fclose(f);

    //  Read test file in chunks
    FILETIME ft1;
    GetSystemTimeAsFileTime(&ft1);
    for (size_t iteration = 0; iteration < Iterations; iteration++)
    {   
        FILE* f1 = fopen(tempFileName, "rb");
        if (f1 == NULL)
        {   //  OOPS!
            remove(tempFileName);
            throw new CTestException("Could not open " + CString(tempFileName) + " for reading");
        }
        for (size_t scan = 0; scan < RandomDataSize; )
        {
            size_t chunkSize = min(RandomChunkSize, RandomDataSize - scan);
            size_t bytesRead = fread(randomChunk, sizeof(char), chunkSize, f1);
            if (bytesRead != chunkSize)
            {   //  OOPS!
                fclose(f1);
                remove(tempFileName);
                throw new CTestException("Attempting to read " + ToString(chunkSize) +
                                         " bytes, but only " + ToString(bytesRead) +
                                         " bytes were read");
            }
            scan += chunkSize;
        }
        fclose(f1);
    }
    FILETIME ft2;
    GetSystemTimeAsFileTime(&ft2);
    remove(tempFileName);

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
//  End of Tt3.UnitTests/TestBinaryCrtInputPerformanceAsChunks.cpp
