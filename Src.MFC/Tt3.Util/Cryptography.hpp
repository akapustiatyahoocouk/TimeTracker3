//
//  Tt3.Util/Cryptography.hpp
//  
//  Tt3 Cryptography helpers
//
//////////
BEGIN_TT3_NAMESPACE

//  The SHA-1 hash calculator
class TT3_UTIL_PUBLIC CSha1
{
    UTILITY_CLASS(CSha1)

    //////////
    //  Types
public:
    typedef CVector<BYTE> Hash; //  The calculated "hash"

    //////////
    //  Operations
public:
    //  TODO document
    static Hash     CalculateHash(const void * pData, size_t dataSize);
    static Hash     CalculateHash(const CString & data);
    static CString  CalculateHashString(const void * pData, size_t dataSize);
    static CString  CalculateHashString(const CString & data);

    //////////
    //  Implementation
private:
    class _DigestBuilder final
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(_DigestBuilder)

        //////////
        //  Construction/destruction
    public:
        _DigestBuilder() { Reset(); }

        //////////
        //  Builder
    public:
        virtual void    Reset();
        virtual void    Update(const void* pData, size_t dataSize);
        virtual void    Digest(const void* pData, size_t dataSize);
        virtual Hash    GetDigest() const;

        //////////
        //  Implementation
    private:
        uint32_t        m_H[5];     //  Message digest buffers
        uint32_t        m_LengthLo; //  Message length in bits
        uint32_t        m_LengthHi; //  Message length in bits

        uint8_t         m_MessageBlock[64]; //  512-bit message blocks
        int             m_MessageBlockIndex;//  Index into message block array

        CVector<BYTE>   m_Result;   //  empty unless finalized

        //  Helpers
        void            _ProcessMessageBlock();
        void            _PadMessage();
        inline uint32_t _CircularShift(int bits, uint32_t word) { return (word << bits) | (word >> (32 - bits)); }
    };

    //  Helpers
    static CString  _HashToString(const Hash & hash);
};

END_TT3_NAMESPACE
//  End of Tt3.Util/Cryptography.hpp
