//
//  tt3-help/ContentType.hpp - The content type specification
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

namespace tt3::help
{
    class TT3_HELP_PUBLIC IContentType
    {
        //////////
        //  This is an interface
    protected:
        /// \brief
        ///     The empty (interface) constructor.
        IContentType() = default;

        /// \brief
        ///     The empty (interface) destructor.
        virtual ~IContentType() = default;

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the mnemonic identifier of this content type.
        /// \return
        ///     The mnemonic identifier of this content type.
        virtual auto    mnemonic(
            ) const -> tt3::util::Mnemonic = 0;

        /// \brief
        ///     Returns the user-readable display name of this content type.
        /// \return
        ///     The user-readable display name of this content type.
        virtual QString displayName() const = 0;

        /// \brief
        ///     Returns the small (16x16) icon representing this content type.
        /// \return
        ///     The small (16x16) icon representing this content type.
        virtual QIcon   smallIcon() const = 0;

        /// \brief
        ///     Returns the large (32x32) icon representing this content type.
        /// \return
        ///     The large (32x32) icon representing this content type.
        virtual QIcon   largeIcon() const = 0;
    };

    class TT3_HELP_PUBLIC StandardContentTypes final
    {
        UTILITY_CLASS(StandardContentTypes)

        //////////
        //  Content types
    };

    /// \class ContentTypeManager tt3-db-api/API.hpp
    /// \brief The manager of known content types.
    class TT3_HELP_PUBLIC ContentTypeManager final
    {
        UTILITY_CLASS(ContentTypeManager)

        //////////
        //  Operations
    public:
        /// \brief
        ///     Registers the specified content type; returns
        ///     true on success, false on failure.
        /// \param contentType
        ///     The content type to register.
        /// \return
        ///     True on success, false on failure.
        ///     Repeated registration of the same content type
        ///     is considered as a "success".
        static bool     registerContentType(
                                IContentType * contentType
                            );

        /// \brief
        ///     Finds a registered content type by its mnemonic.
        /// \param mnemonic
        ///     The mnemonic to look for.
        /// \return
        ///     The registered content type with the required
        ///     mnemonic; nullptr if not found.
        static auto     findContentType(
                                const tt3::util::Mnemonic & mnemonic
                            ) -> IContentType *;

        /// \brief
        ///     Returns the set of all registered content types.
        /// \return
        ///     Rhe set of all registered content types.
        static auto     allContentTypes(
                            ) -> ContentTypes;

        //////////
        //  Implementation
    private:
        struct _Impl;

        //  Helpers
        static _Impl *  _impl();
    };
}

//  End of tt3-help/ContentType.hpp
