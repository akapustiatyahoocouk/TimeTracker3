//
//  Tt3.Util/Sha1.cpp
//  
//  CSha1 class implementation
//
//////////
#include "Tt3.Util/API.hpp"
USING_TT3_NAMESPACE

//////////
//  Operations
CSha1::Hash CSha1::CalculateHash(const void* pData, size_t dataSize)
{
    _DigestBuilder digestBuilder;
    digestBuilder.Digest(pData, dataSize);
    return digestBuilder.GetDigest();
}

CSha1::Hash CSha1::CalculateHash(const CString& data)
{
    //  Build ISO-8851-1 data bytes
    std::unique_ptr<BYTE[]> bytes{ new BYTE[data.GetLength()] };
    for (int i = 0; i < data.GetLength(); i++)
    {
        bytes[i] = BYTE(data[i]);
    }
    //  Go!
    return CalculateHash(bytes.get(), size_t(data.GetLength()));
}

CString CSha1::CalculateHashString(const void* pData, size_t dataSize)
{
    return _HashToString(CalculateHash(pData, dataSize));
}

CString CSha1::CalculateHashString(const CString& data)
{
    return _HashToString(CalculateHash(data));
}

//////////
//  Implementation helpers
CString CSha1::_HashToString(const Hash& hash)
{
    CString result;
    for (size_t i = 0; i < hash.GetSize(); i++)
    {
        TCHAR hh[3];
        wprintf(hh, _T("%02X"), hash[i]);
        result += hh;
    }
    return result;
}

//////////
//  CSha1::_DigestBuilder
void CSha1::_DigestBuilder::Reset()
{
    m_LengthLo = 0;
    m_LengthHi = 0;
    m_MessageBlockIndex = 0;

    m_H[0] = 0x67452301;
    m_H[1] = 0xEFCDAB89;
    m_H[2] = 0x98BADCFE;
    m_H[3] = 0x10325476;
    m_H[4] = 0xC3D2E1F0;

    m_Result.RemoveAll();
}

void CSha1::_DigestBuilder::Update(const void* data, size_t dataSize)
{
    ASSERT(m_Result.IsEmpty());
    ASSERT(data != nullptr);
    const BYTE* bytes = static_cast<const BYTE*>(data);

    if (dataSize == 0)
    {
        return;
    }

    while (dataSize-- != 0)
    {
        m_MessageBlock[m_MessageBlockIndex++] = *bytes;

        m_LengthLo += 8;
        m_LengthLo &= 0xFFFFFFFF;               // Force it to 32 bits
        if (m_LengthLo == 0)
        {
            m_LengthHi++;
        }

        if (m_MessageBlockIndex == 64)
        {
            _ProcessMessageBlock();
        }

        bytes++;
    }
}

void CSha1::_DigestBuilder::Digest(const void* data, size_t dataSize)
{
    Update(data, dataSize); //  ...to consume the last data

    _PadMessage();
    m_Result.SetSize(20);
    for (size_t i = 0; i < 5; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            m_Result[4 * i + j] = static_cast<BYTE>(m_H[i] >> ((3 - j) * 8));
        }
    }
}

CSha1::Hash CSha1::_DigestBuilder::GetDigest() const
{
    ASSERT(!m_Result.IsEmpty());
    return m_Result;
}

void CSha1::_DigestBuilder::_ProcessMessageBlock()
{
    static const uint32_t K[] =
    {   // Constants defined for SHA-1
        0x5A827999,
        0x6ED9EBA1,
        0x8F1BBCDC,
        0xCA62C1D6
    };

    uint32_t temp;             //  Temporary word value
    uint32_t W[80];            //  Word sequence
    uint32_t A, B, C, D, E;    //  Word buffers

    //  Initialize the first 16 words in the array W
    for (int t = 0; t < 16; t++)
    {
        W[t] = static_cast<uint32_t>(m_MessageBlock[t * 4]) << 24;
        W[t] |= static_cast<uint32_t>(m_MessageBlock[t * 4 + 1]) << 16;
        W[t] |= static_cast<uint32_t>(m_MessageBlock[t * 4 + 2]) << 8;
        W[t] |= static_cast<uint32_t>(m_MessageBlock[t * 4 + 3]);
    }

    for (int t = 16; t < 80; t++)
    {
        W[t] = _CircularShift(1, W[t - 3] ^ W[t - 8] ^ W[t - 14] ^ W[t - 16]);
    }

    A = m_H[0];
    B = m_H[1];
    C = m_H[2];
    D = m_H[3];
    E = m_H[4];

    for (int t = 0; t < 20; t++)
    {
        temp = _CircularShift(5, A) + ((B & C) | ((~B) & D)) + E + W[t] + K[0];
        temp &= 0xFFFFFFFF;
        E = D;
        D = C;
        C = _CircularShift(30, B);
        B = A;
        A = temp;
    }

    for (int t = 20; t < 40; t++)
    {
        temp = _CircularShift(5, A) + (B ^ C ^ D) + E + W[t] + K[1];
        temp &= 0xFFFFFFFF;
        E = D;
        D = C;
        C = _CircularShift(30, B);
        B = A;
        A = temp;
    }

    for (int t = 40; t < 60; t++)
    {
        temp = _CircularShift(5, A) +
            ((B & C) | (B & D) | (C & D)) + E + W[t] + K[2];
        temp &= 0xFFFFFFFF;
        E = D;
        D = C;
        C = _CircularShift(30, B);
        B = A;
        A = temp;
    }

    for (int t = 60; t < 80; t++)
    {
        temp = _CircularShift(5, A) + (B ^ C ^ D) + E + W[t] + K[3];
        temp &= 0xFFFFFFFF;
        E = D;
        D = C;
        C = _CircularShift(30, B);
        B = A;
        A = temp;
    }

    m_H[0] = (m_H[0] + A) & 0xFFFFFFFF;
    m_H[1] = (m_H[1] + B) & 0xFFFFFFFF;
    m_H[2] = (m_H[2] + C) & 0xFFFFFFFF;
    m_H[3] = (m_H[3] + D) & 0xFFFFFFFF;
    m_H[4] = (m_H[4] + E) & 0xFFFFFFFF;

    m_MessageBlockIndex = 0;
}

void CSha1::_DigestBuilder::_PadMessage()
{
    //  Check to see if the current message block is too small to hold
    //  the initial padding bits and length.  If so, we will pad the
    //  block, process it, and then continue padding into a second block.
    if (m_MessageBlockIndex > 55)
    {
        m_MessageBlock[m_MessageBlockIndex++] = 0x80;
        while (m_MessageBlockIndex < 64)
        {
            m_MessageBlock[m_MessageBlockIndex++] = 0;
        }

        _ProcessMessageBlock();

        while (m_MessageBlockIndex < 56)
        {
            m_MessageBlock[m_MessageBlockIndex++] = 0;
        }
    }
    else
    {
        m_MessageBlock[m_MessageBlockIndex++] = 0x80;
        while (m_MessageBlockIndex < 56)
        {
            m_MessageBlock[m_MessageBlockIndex++] = 0;
        }
    }

    //  Store the message length as the last 8 octets
    m_MessageBlock[56] = static_cast<BYTE>(m_LengthHi >> 24);
    m_MessageBlock[57] = static_cast<BYTE>(m_LengthHi >> 16);
    m_MessageBlock[58] = static_cast<BYTE>(m_LengthHi >> 8);
    m_MessageBlock[59] = static_cast<BYTE>(m_LengthHi);
    m_MessageBlock[60] = static_cast<BYTE>(m_LengthLo >> 24);
    m_MessageBlock[61] = static_cast<BYTE>(m_LengthLo >> 16);
    m_MessageBlock[62] = static_cast<BYTE>(m_LengthLo >> 8);
    m_MessageBlock[63] = static_cast<BYTE>(m_LengthLo);

    _ProcessMessageBlock();
}

//  End of Tt3.Util/Sha1.cpp
