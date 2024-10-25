//
//  Tt3.UnitTests/TestBufferedOutputStreamConsistency.cpp
//  
//  A test for the buffered output stream consistency
//
//////////
#include "Tt3.UnitTests/API.hpp"
#include <iostream>
BEGIN_TT3_NAMESPACE

void TestBufferedOutputStreamConsistency() throws(CTestException*)
{
    const int RandomDataSize = 1024 * 1024 * 16;
    const size_t RandomChunkSize = 4095;
    BYTE randomChunk[RandomChunkSize];

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

    //  Write random data to the raw binary file in chunks...
    CFileOutputStream fos1{ CString(tempFileName) };
    CBufferedOutputStream bos1(&fos1);
    for (size_t scan = 0; scan < RandomDataSize; )
    {
        size_t chunkSize = min(RandomChunkSize, RandomDataSize - scan);
        bos1.Write(randomChunk, chunkSize);
        scan += chunkSize;
    }
    bos1.Close();
    fos1.Close();
    //  ...and re-read the random data using CRT I/O and compare with the result
    FILE* f1 = fopen(tempFileName, "rb");
    if (f1 == NULL)
    {   //  OOPS!
        remove(tempFileName);
        throw new CTestException("Could not open " + CString(tempFileName) + " for reading");
    }
    int c1;
    for (size_t i = 0; i < RandomDataSize; i++)
    {
        c1 = fgetc(f1);
        if (c1 != randomChunk[i % RandomChunkSize])
        {   //  OOPS!
            fclose(f1);
            remove(tempFileName);
            throw new CTestException("ERROR!Reads a byte " + ToString(c1) +
                                     " at offset " + ToString(i) +
                                     ", expected " + ToString(randomChunk[i % RandomChunkSize]));
        }
    }
    if ((c1 = fgetc(f1)) != EOF)
    {   //  OOPS! Should have been no more data!
        fclose(f1);
        remove(tempFileName);
        throw new CTestException("Reads a byte " + ToString(c1) + " where EOF should have been");
    }
    //  Done - remove the random data file
    fclose(f1);
    remove(tempFileName);

    //  Write random data to the raw binary file in bytes...
    CFileOutputStream fos2{ CString(tempFileName) };
    CBufferedOutputStream bos2(&fos2);
    for (size_t i = 0; i < RandomDataSize; i++)
    {
        bos2.Write(randomChunk[i % RandomChunkSize]);
    }
    bos2.Close();
    fos2.Close();
    //  ...and re-read the random data using CRT I/O and compare with the result
    FILE* f2 = fopen(tempFileName, "rb");
    if (f2 == NULL)
    {   //  OOPS!
        remove(tempFileName);
        throw new CTestException("Could not open " + CString(tempFileName) + " for reading");
    }
    int c2;
    for (size_t i = 0; i < RandomDataSize; i++)
    {
        c2 = fgetc(f2);
        if (c2 != randomChunk[i % RandomChunkSize])
        {   //  OOPS!
            fclose(f2);
            remove(tempFileName);
            throw new CTestException("Reads a byte " + ToString(c2) +
                                     " at offset " + ToString(i) +
                                     ", expected " + ToString(randomChunk[i % RandomChunkSize]));
        }
    }
    if ((c2 = fgetc(f2)) != EOF)
    {   //  OOPS! Should have been no more data!
        fclose(f2);
        remove(tempFileName);
        throw new CTestException("Reads a byte " + ToString(c2) + " where EOF should have been");
    }
    //  Done - remove the random data file
    fclose(f2);
    remove(tempFileName);
}

END_TT3_NAMESPACE
//  End of Tt3.UnitTests/TestBufferedOutputStreamConsistency.cpp
