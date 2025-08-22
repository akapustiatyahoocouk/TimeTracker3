//
//  tt3-util/MessageDigest.hpp - Message digest framework
//
//  TimeTracker3
//  Copyright (C) 2026, Andrey Kapustin
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//////////

#pragma once

namespace tt3::util
{
    class TT3_UTIL_PUBLIC MessageDigest;
    using MessageDigests = QSet<MessageDigest*>;

    //////////
    //  A generic "message digest" - an algorithm that creates a fixed-size
    //  "digest" for an arbitrary size message
    class TT3_UTIL_PUBLIC MessageDigest
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(MessageDigest)

        //////////
        //  Types
    public:
        //  The "builder" of a message digest.
        //  Once created, a Builder is fed the message bytes (all at once or piecemeal),
        //  after which it is "finalised". Once "finalised", the digest bytes become available.
        class TT3_UTIL_PUBLIC Builder
        {
            CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Builder)

            //////////
            //  Construction/destruction
        public:
            Builder() = default;
            virtual ~Builder() = default;

            //////////
            //  Operations
        public:
            //  The MessageDigest implemented by this Builder.
            virtual MessageDigest * messageDigest() const = 0;

            //  Resets this Builder to its initial state, ready to digest the next message
            virtual void        reset() = 0;

            //  Modifies the state of this builder by processing the specified bytes.
            //  String version uses UTF-8 to encode string as bytes.
            virtual void        digest(const void * data, size_t numBytes) = 0;
            virtual void        digest(const QString & s);

            //  Finalises the state of this builder and calculates the "final" message digest.
            virtual void        finalise() = 0;

            //  Returns the "final" message digest; "finalise()"s first if necessary.
            virtual QByteArray  digestAsBytes() = 0;

            //  Same as "digestAsBytes()", but returns a string of [uppercase] hex digits, two per byte.
            virtual QString     digestAsString();
        };

        //////////
        //  Construction/destruction - from derived classes only
    protected:
        MessageDigest() = default;
        virtual ~MessageDigest() = default;

        //////////
        //  Operations
    public:
        //  The mnemonic identifier of this message digest method.
        virtual QString         mnemonic() const = 0;

        //  The user-readable display name of this message digest method.
        virtual QString         displayName() const = 0;

        //////////
        //  Operations
    public:
        //  Creates a new Builder that implements this MessageDigest.
        //  The caller is responsible for destroying the Builder when done.
        virtual Builder *       createBuilder() = 0;

        //////////
        //  Registry TODO move to separate class
    public:
        static MessageDigests   getAll();
        static MessageDigest *  findByMnemonic(const QString & mnemonic);
        static bool             registerMessageDigest(MessageDigest * messageDigest);
    private:
        static MessageDigests   _registry;
    };

    //////////
    //  SHA-1 message digest
    class TT3_UTIL_PUBLIC Sha1MessageDigest final : public MessageDigest
    {
        DECLARE_SINGLETON(Sha1MessageDigest)

        //////////
        //  MessageDigest
    public:
        virtual QString         mnemonic() const override;
        virtual QString         displayName() const override;

        //////////
        //  MessageDigest
    public:
        virtual Builder *       createBuilder() override;

        //////////
        //  Implementation
    private:
        class TT3_UTIL_PUBLIC _Builder : public Builder
        {
            CANNOT_ASSIGN_OR_COPY_CONSTRUCT(_Builder)

            //////////
            //  Construction/destruction
        public:
            _Builder();
            virtual ~_Builder();

            //////////
            //  Builder
        public:
            virtual MessageDigest * messageDigest() const override;
            virtual void        reset() override;
            virtual void        digest(const void * data, size_t numBytes) override;
            virtual void        finalise() override;
            virtual QByteArray  digestAsBytes() override;

            //////////
            //  Implementation
        private:
            bool                _finalised = false;

            uint32_t            _H[5];          //  Message digest buffers
            uint32_t            _lengthLo = 0;  //  Message length in bits
            uint32_t            _lengthHi = 0;  //  Message length in bits

            uint8_t             _messageBlock[64];      //  512-bit message blocks
            int                 _messageBlockIndex = 0; //  Index into message block array

            QByteArray          _result = {};   //  empty unless finalized

            //  Helpers
            void                _processMessageBlock();
            void                _padMessage();
            inline uint32_t     _circularShift(int bits, uint32_t word) { return (word << bits) | (word >> (32 - bits)); }
        };
    };
}

//  End of tt3-util/MessageDigest.hpp
