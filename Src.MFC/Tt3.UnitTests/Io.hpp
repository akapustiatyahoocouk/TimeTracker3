//
//  Tt3.UnitTests/Io.hpp
//  
//  Tt3 UnitTests: Tests for Tt3.Util Ui.hpp facilities
//
//////////
BEGIN_TT3_NAMESPACE

void TestFileOutputStreamConsistency() throws(CTestException*);
void TestFileInputStreamConsistency() throws(CTestException*);
void TestBufferedOutputStreamConsistency() throws(CTestException*);
void TestBufferedInputStreamConsistency() throws(CTestException*);
void TestBinaryFileWriterConsistency() throws(CTestException*);
void TestBinaryFileReaderConsistency() throws(CTestException*);

void TestBinaryCrtOutputPerformanceAsChunks() throws(CTestException*);
void TestBinaryCppOutputPerformanceAsChunks() throws(CTestException*);
void TestFileOutputStreamPerformanceAsChunks() throws(CTestException*);
void TestBufferedOutputStreamPerformanceAsChunks() throws(CTestException*);
void TestBinaryFileWriterPerformanceAsChunks() throws(CTestException*);

void TestBinaryCrtOutputPerformanceAsBytes() throws(CTestException*);
void TestBinaryCppOutputPerformanceAsBytes() throws(CTestException*);
void TestFileOutputStreamPerformanceAsBytes() throws(CTestException*);
void TestBufferedOutputStreamPerformanceAsBytes() throws(CTestException*);
void TestBinaryFileWriterPerformanceAsBytes() throws(CTestException*);

void TestBinaryCrtInputPerformanceAsChunks() throws(CTestException*);
void TestBinaryCppInputPerformanceAsChunks() throws(CTestException*);
void TestFileInputStreamPerformanceAsChunks() throws(CTestException*);
void TestBufferedInputStreamPerformanceAsChunks() throws(CTestException*);
void TestBinaryFileReaderPerformanceAsChunks() throws(CTestException*);

void TestBinaryCrtInputPerformanceAsBytes() throws(CTestException*);
void TestBinaryCppInputPerformanceAsBytes() throws(CTestException*);
void TestFileInputStreamPerformanceAsBytes() throws(CTestException*);
void TestBufferedInputStreamPerformanceAsBytes() throws(CTestException*);
void TestBinaryFileReaderPerformanceAsBytes() throws(CTestException*);

void TestTextOutputStreamConsistency() throws(CTestException*);
void TestTextInputStreamConsistency() throws(CTestException*);

END_TT3_NAMESPACE
//  End of Tt3.UnitTests/Io.hpp
