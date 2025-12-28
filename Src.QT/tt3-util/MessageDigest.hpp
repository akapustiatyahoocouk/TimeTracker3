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

namespace tt3::util
{
    /// \class IMessageDigest tt3-util/API.hpp
    /// \brief
    ///     A generic "message digest" - an algorithm that creates
    ///     a fixed-size "digest" for an arbitrary size message.
    class TT3_UTIL_PUBLIC IMessageDigest
    {
        //////////
        //  Types
    public:
        /// \class Builder tt3-util/API.hpp
        /// \brief The "builder" of a message digest.
        /// \details
        ///     Once created, a Builder is fed the message bytes
        ///     (all at once or piecemeal), after which it is "finalised".
        ///     Once "finalised", the digest bytes become available.
        class TT3_UTIL_PUBLIC Builder
        {
            TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Builder)

            //////////
            //  Construction/destruction
        public:
            /// \brief
            ///     The class constructor.
            Builder() = default;

            /// \brief
            ///     The class destructor.
            virtual ~Builder() = default;

            //////////
            //  Operations
        public:
            /// \brief
            ///     Returns the message digest algorithm implemented by
            ///     this Builder.
            /// \return
            ///     The message digest algorithm implemented by this Builder.
            virtual IMessageDigest * messageDigest() const = 0;

            /// \brief
            ///     Resets this Builder to its initial state, ready
            ///     to digest the next message
            virtual void        reset() = 0;

            /// \brief
            ///     Modifies the state of this builder by processing
            ///     the specified bytes.
            /// \param data
            ///     The data bytes to digest.
            /// \param numBytes
            ///     The number of data bytes to digest.
            virtual void        digestFragment(const void * data, size_t numBytes) = 0;

            /// \brief
            ///     Modifies the state of this builder by processing
            ///     the specified bytes.
            /// \details
            ///     Uses UTF-8 to encode string as bytes before digesting them.
            /// \param s
            ///     The string to digest.
            virtual void        digestFragment(const QString & s);

            /// \brief
            ///     Finalises the state of this builder and calculates
            ///     the "final" message digest.
            virtual void        finalise() = 0;

            /// \brief
            ///     Returns the "final" message digest; "finalise()"s
            ///     first if necessary.
            /// \return
            ///     The "final" message digest as a byte array.
            virtual QByteArray  digestAsBytes() = 0;

            /// \brief
            ///     Returns the "final" message digest; "finalise()"s
            ///     first if necessary.
            /// \return
            ///     The "final" message digest as an uppercase hex string.
            virtual QString     digestAsString();
        };

        //////////
        //  This is an interface
    protected:
        /// \brief
        ///     The empty [interface] constructor.
        IMessageDigest() = default;

        /// \brief
        ///     The empty [interface] destructor.
        virtual ~IMessageDigest() = default;

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the mnemonic identifier of this message digest algorithm.
        /// \return
        ///     The mnemonic identifier of this message digest algorithm.
        virtual Mnemonic        mnemonic() const = 0;

        /// \brief
        ///     Returns the user-readable display name of this
        ///     message digest algorithm.
        /// \return
        ///     The user-readable display name of this message
        ///     digest algorithm for the current default locale.
        virtual QString         displayName() const = 0;

        /// \brief
        ///     Creates a new Builder that implements this MessageDigest.
        /// \details
        ///     The caller is responsible for destroying the Builder when done.
        /// \return
        ///     A new Builder that implements this MessageDigest.
        virtual Builder *       createBuilder() = 0;
    };

    /// \class StandardMessageDigests tt3-util/API.hpp
    /// \brief The standard (predefined) MESSAGE DIGESTS.
    class TT3_UTIL_PUBLIC StandardMessageDigests final
    {
        TT3_UTILITY_CLASS(StandardMessageDigests)

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the set of all standard message digests.
        /// \return
        ///     The set of all standard message digests.
        static MessageDigests   all();

        //////////
        //  Members
    public:
        /// \class Sha1 tt3-util/API.hpp
        /// \brief SHA-1 message digest.
        class TT3_UTIL_PUBLIC Sha1 final
            :   public virtual IMessageDigest
        {
            TT3_DECLARE_SINGLETON(Sha1)

            //////////
            //  IMessageDigest
        public:
            virtual Mnemonic        mnemonic() const override;
            virtual QString         displayName() const override;
            virtual Builder *       createBuilder() override;

            //////////
            //  Implementation
        private:
            class TT3_UTIL_PUBLIC _Builder final
                :   public Builder
            {
                TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(_Builder)

                //////////
                //  Construction/destruction
            public:
                _Builder();
                virtual ~_Builder();

                //////////
                //  Builder
            public:
                using Builder::digestFragment;

                virtual IMessageDigest *messageDigest() const override;
                virtual void        reset() override;
                virtual void        digestFragment(const void * data, size_t numBytes) override;
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

                QByteArray          _result;    //  empty unless finalized

                //  Helpers
                void                _processMessageBlock();
                void                _padMessage();
                inline uint32_t     _circularShift(int bits, uint32_t word) { return (word << bits) | (word >> (32 - bits)); }
            };
        };
    };

    /// \class MessageDigestManager tt3-util/API.hpp
    /// \brief The manager of known message ditest algorithms.
    class TT3_UTIL_PUBLIC MessageDigestManager final
    {
        TT3_UTILITY_CLASS(MessageDigestManager)

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the set of all registered message
        ///     digest algorithms.
        /// \return
        ///     The set of all registered message digest algorithms.
        static auto     all() -> MessageDigests;

        /// \brief
        ///     Registers the specified message digest algorithm.
        /// \param messageDigest
        ///     The message digest algorithm to register.
        /// \return
        ///     True on success, false on failure. Repeated registration
        ///     of the same message digest algorithm is treated as a success.
        static bool     register(IMessageDigest * messageDigest);

        /// \brief
        ///     Finds a registered message digest algorithm by its mnemonic.
        /// \param mnemonic
        ///     The mnemonic to look for.
        /// \return
        ///     The message digest algorithm with the required
        ///     mnemonic; nullptr if not found.
        static auto     find(const Mnemonic & mnemonic) -> IMessageDigest *;

        //////////
        //  Implementation
    private:
        struct _Impl;

        //  Helpers
        static _Impl *  _impl();
    };
}

//  End of tt3-util/MessageDigest.hpp
