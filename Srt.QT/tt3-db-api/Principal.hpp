//
//  tt3-db-api/Principal.hpp - an access control principal
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

namespace tt3::db::api
{
    /// \class IPrincipal tt3-db-api/API.hpp
    /// \brief A generic access control principal.
    class TT3_DB_API_PUBLIC IPrincipal : public virtual IObject
    {
        //////////
        //  This is an interface
    protected:
        IPrincipal() = default;
        virtual ~IPrincipal() = default;

        //////////
        //  Operations (properties)
    public:
        /// \brief
        ///     Checks whether this Principal is "enabled".
        /// \details
        ///     "Disabled" Principals do not exist as far as
        ///     access control is concerned.
        /// \return
        ///     True if this Principal is "enabled", false if "disabled".
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual bool    enabled(
                            ) const = 0;

        /// \brief
        ///     Enables or disabled this Principal.
        /// \details
        ///     "Disabled" Principals do not exist as far as
        ///     access control is concerned.
        /// \param enabled
        ///     True to mark this Principal "enabled", false for "disabled".
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual void    setEnabled(
                                bool enabled
                            ) = 0;

        /// \brief
        ///     Returns the list of e-mail addresses associated with
        ///     this Principal.
        /// \return
        ///     The list of e-mail addresses of this Principal.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual auto    emailAddresses(
                            ) const -> QStringList = 0;

        /// \brief
        ///     Sets the list of e-mail addresses associated with
        ///     this Principal.
        /// \param emailAddresses
        ///     The new list of e-mail addresses for this Principal.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual void    setEmailAddresses(
                                const QStringList & emailAddresses
                            ) = 0;
    };
}

//  End of tt3-db-api/Principal.hpp
