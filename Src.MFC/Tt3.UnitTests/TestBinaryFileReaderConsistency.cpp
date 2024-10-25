//
//  Tt3.UnitTests/TestBinaryFileReaderConsistency.cpp
//  
//  A test for the binary file reader consistency
//
//////////
#include "Tt3.UnitTests/API.hpp"
#include <iostream>
BEGIN_TT3_NAMESPACE

void TestBinaryFileReaderConsistency() throws(CTestException*)
{
    const int RandomDataSize = 1024 * 1024 * 16;
    const size_t RandomChunkSize = 4095;
    BYTE randomChunk[RandomChunkSize];
    BYTE randomChunkCopy[RandomChunkSize];

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

    //  Write random data to the file using CRT facilities...
    FILE* f1 = fopen(tempFileName, "wb");
    if (f1 == NULL)
    {   //  OOPS!
        throw new CTestException("Could not open " + CString(tempFileName) + " for reading");
    }
    for (size_t i = 0; i < RandomDataSize; i++)
    {
        fputc(randomChunk[i % RandomChunkSize], f1);
    }
    fclose(f1);

    //  Re-read data in chunks
    CBinaryFileReader r1{ CString(tempFileName) };
    for (size_t scan = 0; scan < RandomDataSize; )
    {
        size_t chunkSize = min(RandomChunkSize, RandomDataSize - scan);
        memset(randomChunkCopy, 0, RandomChunkSize);
        size_t bytesRead = r1.Read(randomChunkCopy, chunkSize);
        if (bytesRead != chunkSize)
        {
            r1.Close();
            remove(tempFileName);
            throw new CTestException("Read " + ToString(bytesRead) + " bytes, " +
                ToString(chunkSize) + " bytes were expected");
        }
        if (memcmp(randomChunk, randomChunkCopy, chunkSize) != 0)
        {
            r1.Close();
            remove(tempFileName);
            throw new CTestException("Read back wrong data");
        }
        scan += chunkSize;
    }
    if (r1.Read() != -1)
    {
        r1.Close();
        remove(tempFileName);
        throw new CTestException("Read back data where EOF should have been");
    }
    r1.Close();

    //  Re-read data byte-by-byte
    CBinaryFileReader r2{ CString(tempFileName) };
    for (size_t i = 0; i < RandomDataSize; i++)
    {
        int c2 = r2.Read();
        if (c2 != randomChunk[i % RandomChunkSize])
        {
            r2.Close();
            remove(tempFileName);
            throw new CTestException("Read back wrong data");
        }
    }
    if (r2.Read() != -1)
    {
        r2.Close();
        remove(tempFileName);
        throw new CTestException("Read back data where EOF should have been");
    }
    r2.Close();
    remove(tempFileName);
}

END_TT3_NAMESPACE
//  End of Tt3.UnitTests/TestBinaryFileReaderConsistency.cpp
