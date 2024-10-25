//
//  Tt3.UnitTests/TestFileInputStreamConsistency.cpp
//  
//  A test for the binary input stream consistency
//
//////////
#include "Tt3.UnitTests/API.hpp"
#include <iostream>
BEGIN_TT3_NAMESPACE

void TestFileInputStreamConsistency() throws(CTestException*)
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
    CFileInputStream fis1{ CString(tempFileName) };
    for (size_t scan = 0; scan < RandomDataSize; )
    {
        size_t chunkSize = min(RandomChunkSize, RandomDataSize - scan);
        memset(randomChunkCopy, 0, RandomChunkSize);
        size_t bytesRead = fis1.Read(randomChunkCopy, chunkSize);
        if (bytesRead != chunkSize)
        {
            fis1.Close();
            remove(tempFileName);
            throw new CTestException("Read " + ToString(bytesRead) +
                                     " bytes, " + ToString(chunkSize) + " bytes were expected");
        }
        if (memcmp(randomChunk, randomChunkCopy, chunkSize) != 0)
        {
            fis1.Close();
            remove(tempFileName);
            throw new CTestException("Read back wrong data");
        }
        scan += chunkSize;
    }
    if (fis1.Read() != -1)
    {
        fis1.Close();
        remove(tempFileName);
        throw new CTestException("Read back data where EOF should have been");
    }
    fis1.Close();

    //  Re-read data byte-by-byte
    CFileInputStream fis2{ CString(tempFileName) };
    for (size_t i = 0; i < RandomDataSize; i++)
    {
        int c3 = fis2.Read();
        if (c3 != randomChunk[i % RandomChunkSize])
        {
            fis2.Close();
            remove(tempFileName);
            throw new CTestException("Read back wrong data");
        }
    }
    if (fis2.Read() != -1)
    {
        fis2.Close();
        remove(tempFileName);
        throw new CTestException("Read back data where EOF should have been");
    }
    fis2.Close();
    remove(tempFileName);
}

END_TT3_NAMESPACE
//  End of Tt3.UnitTests/TestFileInputStreamConsistency.cpp
