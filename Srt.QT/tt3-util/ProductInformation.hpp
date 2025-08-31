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
    //  The product information provider
    class TT3_UTIL_PUBLIC ProductInformation final
    {
        UTILITY_CLASS(ProductInformation)

        //////////
        //  Operations
    public:
        //  The internal name of the TT3 product.
        static QString          applicationName();

        //  The version of the TT3 product.
        static QVersionNumber   applicationVersion();

        //  The short (1 line) user-readable copyright message of the TT3 product.
        static QString          applicationCopyright();

        //  The build number of the TT3 product.
        static QString          applicationBuildNumber();

        //  The user-readable name of the TT3 product.
        static QString          applicationDisplayName();

        //  The TT3 organization name.
        static QString          organizationName();

        //  The TT3 organization domain.
        static QString          organizationDomain();

        //  The user-readable credits message of the TT3 product.
        static QString          credits();
    };
}

//  End of tt3-util/ProductInformation.hpp
