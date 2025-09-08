//
//  tt3-ws/Principal.hpp - A generic access control principal
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

namespace tt3::ws
{
    //  A generic access control principal
    class TT3_WS_PUBLIC PrincipalImpl :
        public ObjectImpl
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(PrincipalImpl)

        friend class UserImpl;
        friend class AccountImpl;

        //////////
        //  Construction/destruction - from friends only
    private:
        PrincipalImpl(Workspace workspace, tt3::db::api::IPrincipal * dataPrincipal);
        virtual ~PrincipalImpl();

        //////////
        //  Operations (properties)
    public:
        /// \brief
        ///     Checks whether this Principal is "enabled".
        /// \details
        ///     "Disabled" Principals do not exist as far as
        ///     access control is concerned.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \return
        ///     True if this Principal is "enabled", false
        ///     if "disabled".
        /// \exception WorkspaceException
        ///     If an error occurs.
        bool        enabled(
                            const Credentials & credentials
                        ) const;

        /// \brief
        ///     Enables or disables this Principal is "enabled".
        /// \details
        ///     "Disabled" Principals do not exist as far as
        ///     access control is concerned.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \param enabled
        ///     True to make this Principal "enabled", false
        ///     to make it "disabled".
        /// \exception WorkspaceException
        ///     If an error occurs.
        void        setEnabled(
                            const Credentials & credentials,
                            bool enabled
                        );

        /// \brief
        ///     Returns the list of e-mail addresses
        ///     associated with this Principal.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \return
        ///     The list of e-mail addresses associated
        ///     with this Principal.
        /// \exception WorkspaceException
        ///     If an error occurs.
        auto        emailAddresses(
                            const Credentials & credentials
                        ) const -> QStringList;

        /// \brief
        ///     Sets the list of e-mail addresses
        ///     associated with this Principal.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \param emailAddresses
        ///     The new list of e-mail addresses to associate
        ///     with this Principal.
        /// \exception WorkspaceException
        ///     If an error occurs.
        void        setEmailAddresses(
                            const Credentials & credentials,
                            const QStringList & emailAddresses
                        );

        //////////
        //  Implementation
    private:
        tt3::db::api::IPrincipal *const _dataPrincipal;    //  counts as "reference"
    };
}

//  Enable objects and object pointers for QVariant
Q_DECLARE_METATYPE(tt3::ws::PrincipalImpl)
Q_DECLARE_METATYPE(tt3::ws::Principal)

//  End of tt3-ws/Principal.hpp
