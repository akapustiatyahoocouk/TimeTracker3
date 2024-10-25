//
//  Tt3.Util/Io.hpp
//  
//  Tt3 I/O framework
//
//////////
BEGIN_TT3_NAMESPACE

//////////
//  I/O interfaces

//  A generic I/O resource
class TT3_UTIL_PUBLIC IIo
{
    //////////
    //  Types
public:
    typedef INT_PTR Handle;   //  the native I/O handle of the underlying platform; HANDLE actually
    static const Handle InvalidHandle = (Handle)INVALID_HANDLE_VALUE;   //  a handle guaranteed to be invalid

    //////////
    //  This is an interface
public:
    virtual ~IIo() = default;

    //////////
    //  Operations
public:
    //  Returns the native I/O handle underlying this I/O resource.
    virtual Handle      GetNativeHandle() const = 0;

    //  Checks if this I/O resource is currently "open" (i.e. can be used).
    virtual bool        IsOpen() const = 0;

    //  Closes this I/O resource; has no effect if already closed.
    //  Throws CIoException * if an error occurs.
    virtual void        Close() throws(CIoException *) = 0;

    //  Returns the "context name" of this I/O resource.
    //  This can be a file name, a socket address, etc. - anything 
    //  that identifies this I/O resource more or less uniquely.
    virtual CString     GetContextName() const = 0;

    //////////
    //  Helpers for derived classes
public:
    //  Retrieves an error message for the specified Win32 error
    //  code (as e.g. retrieved by GetLastError()).
    static CString      GetWin32ErrorMessage(DWORD errorCode);
};

//  A generic input resource
class TT3_UTIL_PUBLIC IInput : public virtual IIo
{
    //////////
    //  Operations
public:
    //  Checks if this input resource is ready to provide at least one item
    //  Throws CIoException * if an error occurs.
    virtual bool        IsReady() throws(CIoException *) = 0;
};

//  A generic output resource
class TT3_UTIL_PUBLIC IOutput : public virtual IIo
{
    //////////
    //  Operations
public:
    //  Flushes all of this output resource's cached data.
    //  Throws CIoException * if an error occurs.
    virtual void        Flush() throws(CIoException *) = 0;
};

//  A raw binary input resource
class TT3_UTIL_PUBLIC IBinaryInput : public virtual IInput
{
    //////////
    //  Operations
public:
    //  Attempts to read a single byte from this input resource.
    //  Returns the byte read or -1 if EOF has been reached.
    //  Throws CIoException * if an error occurs.
    virtual int         Read() throws(CIoException *) = 0;
    
    //  Attempts to read up to maxBytesToRead bytes from this input resource.
    //  Returns the number of byte read or 0 if EOF has been reached.
    //  Throws CIoException * if an error occurs.
    virtual size_t      Read(void * pData, size_t maxBytesToRead) throws(CIoException *) = 0;
};

//  A text input resource
class TT3_UTIL_PUBLIC ITextInput : public virtual IInput
{
    //////////
    //  Operations
public:
    //  Attempts to read a single character from this input resource.
    //  Returns the character read or -1 if EOF has been reached.
    //  Throws CIoException * if an error occurs.
    virtual int         Read() throws(CIoException *) = 0;

    //  Attempts to read up to maxCharsToRead characters from this input resource.
    //  Returns the number of characters read or 0 if EOF has been reached.
    //  Throws CIoException * if an error occurs.
    virtual size_t      Read(TCHAR * pChars, size_t maxCharsToRead) throws(CIoException *) = 0;
};

//  A raw binary output resource
class TT3_UTIL_PUBLIC IBinaryOutput : public virtual IOutput
{
    //////////
    //  Operations
public:
    //  Writes a single byte to this output resource.
    //  Throws CIoException * if an error occurs.
    virtual void        Write(BYTE b) throws(CIoException *) = 0;

    //  Writes bytesToWrite bytes to this output resource.
    //  Throws CIoException * if an error occurs.
    virtual void        Write(const void * pData, size_t bytesToWrite) throws(CIoException *) = 0;
};

//  A text output resource
class TT3_UTIL_PUBLIC ITextOutput : public virtual IOutput
{
    //////////
    //  Operations
public:
    //  Writes a single character to this output resource.
    //  Throws CIoException * if an error occurs.
    virtual void        Write(TCHAR c) throws(CIoException *) = 0;

    //  Writes charsToWrite characters to this output resource.
    //  Throws CIoException * if an error occurs.
    virtual void        Write(const TCHAR * pChars, size_t charsToWrite) throws(CIoException *) = 0;
};

//////////
//  File I/O

//  An unbuffered stream that reads from a raw binary file
class TT3_UTIL_PUBLIC CFileInputStream : public virtual IBinaryInput
{
    CANNOT_ASSIGN_OR_COPY_CONSTRUCT(CFileInputStream)

    //////////
    //  Construction/destruction
public:
    explicit CFileInputStream(const CString & fileName) throws(CIoException *);
    virtual ~CFileInputStream();

    //////////
    //  IIo
public:
    virtual Handle      GetNativeHandle() const override;
    virtual bool        IsOpen() const override;
    virtual void        Close() throws(CIoException *) override;
    virtual CString     GetContextName() const override;

    //////////
    //  IInput
public:
    virtual bool        IsReady() throws(CIoException *) override;

    //////////
    //  IBinaryInput
public:
    virtual int         Read() throws(CIoException *) override;
    virtual size_t      Read(void * pData, size_t maxBytesToRead) throws(CIoException *) override;

    //////////
    //  Implementation
private:
    CString             m_FileName;
    HANDLE              m_Handle;   //  INVALID_HANDLE_VALUE when closed
};

//  An unbuffered stream that writes to a raw binary file
class TT3_UTIL_PUBLIC CFileOutputStream : public virtual IBinaryOutput
{
    CANNOT_ASSIGN_OR_COPY_CONSTRUCT(CFileOutputStream)

    //////////
    //  Types
public:
    //  The mode in which the file is open for output
    enum class OpenMode
    {
        CreateAlways = CREATE_ALWAYS,       //  create if not there, truncate if there
        CreateNew = CREATE_NEW,             //  create if not there, error if there
        OpenAlways = OPEN_ALWAYS,           //  create if not there, open for appending if there
        OpenExisting = OPEN_EXISTING,       //  error if not there, open for appending if there
        TruncateExisting = TRUNCATE_EXISTING//  error if not there, truncate if there
    };

    //////////
    //  Construction/destruction
public:
    explicit CFileOutputStream(const CString & fileName, OpenMode openMode = OpenMode::CreateAlways) throws(CIoException *);
    virtual ~CFileOutputStream();

    //////////
    //  IIo
public:
    virtual Handle      GetNativeHandle() const override;
    virtual bool        IsOpen() const override;
    virtual void        Close() throws(CIoException *) override;
    virtual CString     GetContextName() const override;

    //////////
    //  IOutput
public:
    virtual void        Flush() throws(CIoException *) override;

    //////////
    //  IBinaryOutput
public:
    virtual void        Write(BYTE b) throws(CIoException *) override;
    virtual void        Write(const void * pData, size_t bytesToWrite) throws(CIoException *) override;

    //////////
    //  Implementation
private:
    CString             m_FileName;
    HANDLE              m_Handle;   //  INVALID_HANDLE_VALUE when closed
};

//////////
//  Buffering

//  A binary stream that performs buffering when reading 
//  raw binary data from the specified "source" stream
class TT3_UTIL_PUBLIC CBufferedInputStream : public virtual IBinaryInput
{
    CANNOT_ASSIGN_OR_COPY_CONSTRUCT(CBufferedInputStream)

    //////////
    //  Constants
public:
    static const size_t DefaultBufferSize = 8192;

    //////////
    //  Construction/destruction
public:
    explicit CBufferedInputStream(IBinaryInput * pSource, size_t bufferSize = DefaultBufferSize);
    virtual ~CBufferedInputStream();

    //////////
    //  IIo
public:
    virtual Handle      GetNativeHandle() const override;
    virtual bool        IsOpen() const override;
    virtual void        Close() throws(CIoException *) override;
    virtual CString     GetContextName() const override;

    //////////
    //  IInput
public:
    virtual bool        IsReady() throws(CIoException *) override;

    //////////
    //  IBinaryInput
public:
    virtual int         Read() throws(CIoException *) override;
    virtual size_t      Read(void * pData, size_t maxBytesToRead) throws(CIoException *) override;

    //////////
    //  Implementation
private:
    IBinaryInput *      m_pSource; //  //  NULL == stream closed
    CString             m_ContextName;
    size_t              m_BufferSize;
    BYTE *              m_pBuffer;
    size_t              m_DataSize; //  number of bytes currently in buffer; 0 <= m_DataSize <= m_BufferSize
    size_t              m_Scan;     //  0 <= m_Scan <= m_DataSize

    //  Helpers
    void                _RefillBuffer() throws(CIoException *);
};

//  A binary stream that performs buffering when writing 
//  raw binary data to the specified "destination" stream
class TT3_UTIL_PUBLIC CBufferedOutputStream : public virtual IBinaryOutput
{
    CANNOT_ASSIGN_OR_COPY_CONSTRUCT(CBufferedOutputStream)

    //////////
    //  Constants
public:
    static const size_t DefaultBufferSize = 8192;

    //////////
    //  Construction/destruction
public:
    explicit CBufferedOutputStream(IBinaryOutput * pDestination, size_t bufferSize = DefaultBufferSize);
    virtual ~CBufferedOutputStream();

    //////////
    //  IIo
public:
    virtual Handle      GetNativeHandle() const override;
    virtual bool        IsOpen() const override;
    virtual void        Close() throws(CIoException *) override;
    virtual CString     GetContextName() const override;

    //////////
    //  IOutput
public:
    virtual void        Flush() throws(CIoException *) override;

    //////////
    //  IBinaryOutput
public:
    virtual void        Write(BYTE b) throws(CIoException *) override;
    virtual void        Write(const void * pData, size_t bytesToWrite) throws(CIoException *) override;

    //////////
    //  Implementation
private:
    IBinaryOutput *     m_pDestination; //  NULL == stream closed
    CString             m_ContextName;
    size_t              m_BufferSize;
    BYTE *              m_pBuffer;
    size_t              m_DataSize; //  number of bytes currently in buffer, 0 <= m_DataSize <= m_BufferSize

    //  Helpers
    void                _FlushBuffer() throws(CIoException *);
};

//  A buffered stream that reads from a raw binary file
class TT3_UTIL_PUBLIC CBinaryFileReader : public virtual IBinaryInput
{
    CANNOT_ASSIGN_OR_COPY_CONSTRUCT(CBinaryFileReader)

    //////////
    //  Construction/destruction
public:
    explicit CBinaryFileReader(const CString& fileName) throws(CIoException*)
        : m_Fis(fileName), m_Bis(&m_Fis) {}
    CBinaryFileReader(const CString& fileName, size_t bufferSize) throws(CIoException*)
        : m_Fis(fileName), m_Bis(&m_Fis, bufferSize) {}
    virtual ~CBinaryFileReader() {}

    //////////
    //  IIo
public:
    virtual Handle      GetNativeHandle() const override { return m_Bis.GetNativeHandle(); }
    virtual bool        IsOpen() const override { return m_Bis.IsOpen(); }
    virtual void        Close() throws(CIoException*) override { m_Bis.Close(); m_Fis.Close(); }
    virtual CString     GetContextName() const override { return m_Bis.GetContextName(); }

    //////////
    //  IInput
public:
    virtual bool        IsReady() throws(CIoException*) override { return m_Bis.IsReady(); }

    //////////
    //  IBinaryInput
public:
    virtual int         Read() throws(CIoException*) override { return m_Bis.Read(); }
    virtual size_t      Read(void* pData, size_t maxBytesToRead) throws(CIoException*) override { return m_Bis.Read(pData, maxBytesToRead); }

    //////////
    //  Implementation
private:
    CFileInputStream    m_Fis;
    CBufferedInputStream    m_Bis;
};

//  A buffered stream that writes to a raw binary file
class TT3_UTIL_PUBLIC CBinaryFileWriter : public virtual IBinaryOutput
{
    CANNOT_ASSIGN_OR_COPY_CONSTRUCT(CBinaryFileWriter)

    //////////
    //  Types
public:
    //  The mode in which the file is open for output
    typedef CFileOutputStream::OpenMode OpenMode;

    //////////
    //  Construction/destruction
public:
    explicit CBinaryFileWriter(const CString& fileName, OpenMode openMode = OpenMode::CreateAlways) throws(CIoException*)
        :   m_Fos(fileName, openMode), m_Bos(&m_Fos) {}
    CBinaryFileWriter(const CString& fileName, size_t bufferSize, OpenMode openMode = OpenMode::CreateAlways) throws(CIoException*)
        :   m_Fos(fileName, openMode), m_Bos(&m_Fos, bufferSize) {}
    virtual ~CBinaryFileWriter() {}

    //////////
    //  IIo
public:
    virtual Handle      GetNativeHandle() const override { return m_Bos.GetNativeHandle(); }
    virtual bool        IsOpen() const override { return m_Bos.IsOpen(); }
    virtual void        Close() throws(CIoException*) override { m_Bos.Close(); m_Fos.Close(); }
    virtual CString     GetContextName() const override { return m_Bos.GetContextName(); }

    //////////
    //  IOutput
public:
    virtual void        Flush() throws(CIoException*) override { m_Bos.Flush(); m_Fos.Flush(); }

    //////////
    //  IBinaryOutput
public:
    virtual void        Write(BYTE b) throws(CIoException*) override { m_Bos.Write(b); }
    virtual void        Write(const void* pData, size_t bytesToWrite) throws(CIoException*) override { m_Bos.Write(pData, bytesToWrite); }

    //////////
    //  Implementation
private:
    CFileOutputStream   m_Fos;
    CBufferedOutputStream   m_Bos;
};

//////////
//  Text/binary bridge

//  A text stream that converts binary data obtained from the
//  specified source to textual
class TT3_UTIL_PUBLIC CTextInputStream : public virtual ITextInput
{
    CANNOT_ASSIGN_OR_COPY_CONSTRUCT(CTextInputStream)

    //////////
    //  Construction/destruction
public:
    CTextInputStream(IBinaryInput * pSource, const CString & characterSetName) throws(CIoException *);
    explicit CTextInputStream(IBinaryInput * pSource, CCharacterSet * pCharacterSet = CCharacterSet::GetSystem()) throws(CIoException *);
    virtual ~CTextInputStream();

    //////////
    //  IIo
public:
    virtual Handle      GetNativeHandle() const override;
    virtual bool        IsOpen() const override;
    virtual void        Close() throws(CIoException *) override;
    virtual CString     GetContextName() const override;

    //////////
    //  IInput
public:
    virtual bool        IsReady() throws(CIoException *) override;

    //////////
    //  ITextInput
public:
    virtual int         Read() throws(CIoException *) override;
    virtual size_t      Read(TCHAR * pChars, size_t maxCharsToRead) throws(CIoException *) override;

    //////////
    //  Implementation
private:
    IBinaryInput *      m_pSource; //  //  NULL == stream closed
    CString             m_ContextName;
    CCharacterSet::Decoder *    m_pDecoder;

    BYTE                m_PrereadBytes[16];
    size_t              m_NumPrereadBytes = 0;

    TCHAR               m_PrereadChars[16];
    size_t              m_NumPrereadChars = 0;

    //  Helpers
    int                 _ReadRaw() throws(CIoException *);
};

//  A text stream that converts textual data to binary and sends
//  that binary data to the specified destination
class TT3_UTIL_PUBLIC CTextOutputStream : public virtual ITextOutput
{
    CANNOT_ASSIGN_OR_COPY_CONSTRUCT(CTextOutputStream)
        
    friend class CTextInputStream;

    //////////
    //  Constants
public:
    static const size_t DefaultBufferSize = 8192;

    //////////
    //  Construction/destruction
public:
    CTextOutputStream(IBinaryOutput* pDestination, const char* pCharacterSetName) throws(CIoException*);
    CTextOutputStream(IBinaryOutput* pDestination, const CString& characterSetName) throws(CIoException*);
    explicit CTextOutputStream(IBinaryOutput * pDestination, CCharacterSet * pCharacterSet = CCharacterSet::GetSystem());
    virtual ~CTextOutputStream();

    //////////
    //  IIo
public:
    virtual Handle      GetNativeHandle() const override;
    virtual bool        IsOpen() const override;
    virtual void        Close() throws(CIoException *) override;
    virtual CString     GetContextName() const override;

    //////////
    //  IOutput
public:
    virtual void        Flush() throws(CIoException *) override;

    //////////
    //  ITextOutput
public:
    virtual void        Write(TCHAR c) throws(CIoException *) override;
    virtual void        Write(const TCHAR * pChars, size_t charsToWrite) throws(CIoException *) override;

    //////////
    //  Implementation
private:
    IBinaryOutput *     m_pDestination; //  NULL == stream closed
    CString             m_ContextName;
    CCharacterSet::Encoder * m_pEncoder;

    static const size_t _EncodeBufferSize = 256;
    BYTE                m_EncodedBytes[_EncodeBufferSize];
    size_t              m_NumEncodedBytes; //  0 <= m_EncodedBytes <= _EncodeBufferSize

    static const TCHAR * const   m_pEoln;

    class _ByteDestination : public IDataDestination<BYTE>
    {
        //////////
        //  Construction/destruction
    public:
        explicit _ByteDestination(CTextOutputStream* pTos) : m_pTos(pTos) {}

        //////////
        //  IDataDestination<BYTE>
    public:
        virtual bool    Send(const BYTE& item) override
        {
            if (m_pTos->m_NumEncodedBytes == _EncodeBufferSize)
            {   //  Encode buffer full
                m_pTos->_FlushEncodedBytes();
            }
            ASSERT(m_pTos->m_NumEncodedBytes < _EncodeBufferSize);
            m_pTos->m_EncodedBytes[m_pTos->m_NumEncodedBytes++] = item;
            return true;
        }

        //////////
        //  Implementation
    private:
        CTextOutputStream* m_pTos;
    };
    _ByteDestination    m_ByteDestination;

    //  Helpers
    void                _WriteRaw(TCHAR c) throws(CIoException *);
    void                _FlushEncodedBytes() throws(CIoException *);
};

//////////
//  Text file handling

//  A buffered stream that writes to a raw binary file
class TT3_UTIL_PUBLIC CTextFileWriter : public virtual ITextOutput
{
    CANNOT_ASSIGN_OR_COPY_CONSTRUCT(CTextFileWriter)

    //////////
    //  Types
public:
    //  The mode in which the file is open for output
    typedef CFileOutputStream::OpenMode OpenMode;

    //////////
    //  Construction/destruction
public:
    explicit CTextFileWriter(const CString& fileName, OpenMode openMode = OpenMode::CreateAlways) throws(CIoException*)
        : m_Fos(fileName, openMode), m_Bos(&m_Fos), m_Tos(&m_Bos) {}
    CTextFileWriter(const CString& fileName, size_t bufferSize, OpenMode openMode = OpenMode::CreateAlways) throws(CIoException*)
        : m_Fos(fileName, openMode), m_Bos(&m_Fos, bufferSize), m_Tos(&m_Bos) {}
    CTextFileWriter(const CString& fileName, const CString& characterSetName, OpenMode openMode = OpenMode::CreateAlways) throws(CIoException*)
        : m_Fos(fileName, openMode), m_Bos(&m_Fos), m_Tos(&m_Bos) {}
    CTextFileWriter(const CString& fileName, const CString& characterSetName, size_t bufferSize, OpenMode openMode = OpenMode::CreateAlways) throws(CIoException*)
        : m_Fos(fileName, openMode), m_Bos(&m_Fos, bufferSize), m_Tos(&m_Bos) {}
    CTextFileWriter(const CString& fileName, CCharacterSet* pCharacterSet, OpenMode openMode = OpenMode::CreateAlways) throws(CIoException*)
        : m_Fos(fileName, openMode), m_Bos(&m_Fos), m_Tos(&m_Bos) {}
    CTextFileWriter(const CString& fileName, CCharacterSet* pCharacterSet, size_t bufferSize, OpenMode openMode = OpenMode::CreateAlways) throws(CIoException*)
        : m_Fos(fileName, openMode), m_Bos(&m_Fos, bufferSize), m_Tos(&m_Bos) {}
    virtual ~CTextFileWriter() {}

    //////////
    //  IIo
public:
    virtual Handle      GetNativeHandle() const override { return m_Tos.GetNativeHandle(); }
    virtual bool        IsOpen() const override { return m_Tos.IsOpen(); }
    virtual void        Close() throws(CIoException*) override { m_Tos.Close();  m_Bos.Close(); m_Fos.Close(); }
    virtual CString     GetContextName() const override { return m_Tos.GetContextName(); }

    //////////
    //  IOutput
public:
    virtual void        Flush() throws(CIoException*) override { m_Tos.Flush();  m_Bos.Flush(); m_Fos.Flush(); }

    //////////
    //  ITextOutput
public:
    virtual void        Write(TCHAR c) throws(CIoException*) override { m_Tos.Write(c); }
    virtual void        Write(const TCHAR* pChars, size_t charsToWrite) throws(CIoException*) override { m_Tos.Write(pChars, charsToWrite); }

    //////////
    //  Implementation
private:
    CFileOutputStream   m_Fos;
    CBufferedOutputStream   m_Bos;
    CTextOutputStream   m_Tos;
};

//////////
//  Exceptions

//  An exception thrown when an I/O service fails
class TT3_UTIL_PUBLIC CIoException : public CCheckedException
{
    //////////
    //  Construction/destruction/assignment
public:
    explicit CIoException(const CString & errorMessage) : CCheckedException(errorMessage) {}
};

//  An exception thrown when an EOF occurs unexpectedly on input
class TT3_UTIL_PUBLIC CEofException : public CIoException
{
    //////////
    //  Construction/destruction/assignment
public:
    CEofException() : CIoException(_T("Unexpected EOF")) {}
};

//  An exception thrown when a file that is expected to exist is not there
class TT3_UTIL_PUBLIC CFileNotFoundException : public CIoException
{
    //////////
    //  Construction/destruction/assignment
public:
    explicit CFileNotFoundException(const CString & fileName) : CIoException(_T("'") + fileName + _T("' not found")) {}
};

END_TT3_NAMESPACE
//  End of Tt3.Util/Io.hpp
