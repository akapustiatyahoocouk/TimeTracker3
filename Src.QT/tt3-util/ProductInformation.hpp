//
//  tt3-util/ProductInformation.hpp - TT3 product information
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
    /// \class ProductInformation tt3-util/API.hpp
    /// \brief The product information provider.
    class TT3_UTIL_PUBLIC ProductInformation final
    {
        UTILITY_CLASS(ProductInformation)

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the internal name of the TT3 product.
        /// \return
        ///     The internal name of the TT3 product.
        static QString          applicationName();

        /// \brief
        ///     Returns yhe version of the TT3 product.
        /// \return
        ///     The version of the TT3 product.
        static QVersionNumber   applicationVersion();

        /// \brief
        ///     Returns the short (1 line) user-readable copyright message of the TT3 product.
        /// \return
        ///     The short (1 line) user-readable copyright message of the TT3 product.
        static QString          applicationCopyright();

        /// \brief
        ///     Returns the build number of the TT3 product.
        /// \return
        ///     The build number of the TT3 product.
        static QString          applicationBuildNumber();

        /// \brief
        ///     Returns the user-readable name of the TT3 product.
        /// \return
        ///     The user-readable name of the TT3 product.
        static QString          applicationDisplayName();

        /// \brief
        ///     Returns the TT3 organization name.
        /// \return
        ///     The TT3 organization name.
        static QString          organizationName();

        /// \brief
        ///     Returns the TT3 organization domain.
        /// \return
        ///     The TT3 organization domain.
        static QString          organizationDomain();

        /// \brief
        ///     Returns the user-readable credits message of the TT3 product.
        /// \return
        ///     The user-readable credits message of the TT3 product.
        static QString          credits();
    };
}

//  End of tt3-util/ProductInformation.hpp
