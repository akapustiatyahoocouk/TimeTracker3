//
//  Tt3.UnitTests/TestFileInputStreamPerformanceAsBytes.cpp
//  
//  A test for the CFileInputStream performance (as bytes)
//
//////////
#include "Tt3.UnitTests/API.hpp"
#include <iostream>
BEGIN_TT3_NAMESPACE

void TestFileInputStreamPerformanceAsBytes() throws(CTestException*)
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
        CFileInputStream fis{ CString(tempFileName) };
        for (size_t i = 0; i < RandomDataSize; i++)
        {
            if (fis.Read() == -1)
            {   //  OOPS!
                fis.Close();
                remove(tempFileName);
                throw new CTestException("Attempting to read " + ToString(RandomDataSize) +
                                         " bytes, but only " + ToString(i) +
                                         " bytes were read");
            }
        }
        fis.Close();
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
//  End of Tt3.UnitTests/TestFileInputStreamPerformanceAsBytes.cpp
