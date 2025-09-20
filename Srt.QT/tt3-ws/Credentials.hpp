//
//  tt3-ws/Credentials.hpp - Login credentials
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
#include "tt3-ws/API.hpp"

namespace tt3::ws
{
    /// \class Credentials tt3-ws/API.hpp
    /// \brief The login credentials.
    class TT3_WS_PUBLIC Credentials final
    {
        friend class WorkspaceTypeImpl;
        friend class WorkspaceImpl;
        friend class PrincipalImpl;
        friend class UserImpl;
        friend class AccountImpl;

        //////////
        //  Constats
    public:
        /// \brief
        ///     The invalid credentials (with empty login).
        static const Credentials Invalid;

        //////////
        //  Construction/destruction/assignment
    public:
        /// \brief
        ///     Constructs invalid credentials (with empty login).
        Credentials() = default;

        /// \brief
        ///     Constructs the credentials.
        /// \param login
        ///     The login identifier.
        /// \param password
        ///     The password.
        Credentials(
                const QString & login,
                const QString & password
            );

        /// \brief
        ///     The class destructor.
        ~Credentials() = default;

        //  Default copy constructor and assignment are both OK

        //////////
        //  Operators
    public:
        /// \brief
        ///     Checks two Credentials for equality.
        /// \details
        ///     Two credentials are equal if all their details are equal.
        /// \param op2
        ///     The Credentials to compare this Credentials yo.
        /// \return
        ///     True if thw two Credentials are equal, else false.
        bool            operator == (const Credentials & op2) const;

        /// \brief
        ///     Checks two Credentials for inequality.
        /// \details
        ///     Two credentials are equal if all their details are equal.
        /// \param op2
        ///     The Credentials to compare this Credentials yo.
        /// \return
        ///     False if thw two Credentials are equal, else true.
        bool            operator != (const Credentials & op2) const;

        /// \brief
        ///     Checks two Credentials for order.
        /// \param op2
        ///     The 2nd Credentials to compare this Credentials to.
        /// \return
        ///     True if this Credentials is lexicographically
        ///     "less than" the 2nd C5redentials, else false.
        bool            operator <  (const Credentials & op2) const;

        /// \brief
        ///     Checks two Credentials for order.
        /// \param op2
        ///     The 2nd Credentials to compare this Credentials to.
        /// \return
        ///     True if this Credentials is lexicographically
        ///     "less than or equal to" the 2nd C5redentials, else false.
        bool            operator <= (const Credentials & op2) const;

        /// \brief
        ///     Checks two Credentials for order.
        /// \param op2
        ///     The 2nd Credentials to compare this Credentials to.
        /// \return
        ///     True if this Credentials is lexicographically
        ///     "greater than" the 2nd C5redentials, else false.
        bool            operator >  (const Credentials & op2) const;

        /// \brief
        ///     Checks two Credentials for order.
        /// \param op2
        ///     The 2nd Credentials to compare this Credentials to.
        /// \return
        ///     True if this Credentials is lexicographically
        ///     "greater than or equal to" the 2nd C5redentials, else false.
        bool            operator >= (const Credentials & op2) const;

        //////////
        //  Operations
    public:
        /// \brief
        ///     Checks whether these Credentials are "valid"
        ///     (its the "login" is not empty.
        /// \return
        ///     True if this Credentials are "vaolid" else false.
        bool            isValid() const { return !_login.isEmpty(); }

        /// \brief
        ///     Returns the login identifier of these Credentials.
        /// \return
        ///     The login identifier of these Credentials; empty
        ///     string for "invalid" Credentials.
        QString         login() const { return _login; }

        //////////
        //  Implementation
    private:
        QString         _login;
        QString         _password;  //  always "" if _login == ""
    };

    TT3_WS_PUBLIC inline size_t qHash(const tt3::ws::Credentials & key, size_t seed)
    {
        return qHash(key.login(), seed);
    }

    /// \class CurrentCredentials tt3-ws/API.hpp
    /// \brief The accessor for a "current" credentials.
    class TT3_WS_PUBLIC CurrentCredentials final
        :   public QObject
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(CurrentCredentials)

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     The class constructor.
        CurrentCredentials();

        /// \brief
        ///     The class destructor.
        virtual ~CurrentCredentials();
        \
        //////////
        //  Operators
    public:
        /// \brief
        ///     Replaces the "current" Credentials.
        /// \param credentials
        ///     The Credentials to replace the "current" Credentials with.
        void                operator = (const Credentials & credentials);

        /// \brief
        ///     Returns the "current" credentials.
        /// \return
        ///     The "current" credentials.
                            operator const Credentials &() const;

        //////////
        //  Operations
    public:
        /// \brief
        ///     Checks whether current Credentials are "valid"
        ///     (its "login" is not empty.
        /// \return
        ///     True if "current" Credentials are valid, else false.
        bool                isValid() const;

        /// \brief
        ///     Returns the login identifier of the "current" Credentials.
        /// \return
        ///     The login identifier of the "current" Credentials
        ///     or an empty string if the "current" Credentials are invalid.
        QString             login() const;

        //////////
        //  Signals
    signals:
        /// \brief
        ///     Emitted after the current credentials have changed.
        /// \param before
        ///     The "current" Credentials before the change.
        /// \param after
        ///     The "current" Credentials after the change.
        void                changed(Credentials before, Credentials after);

        //////////
        //  Implementation
    private:
        //  TODO use the "struct _Impl" trick
        static std::atomic<int>     _instanceCount; //  ...to disallow a second instance
        mutable tt3::util::Mutex    _currentCredentialsGuard;
        Credentials                 _currentCredentials;
    };

#if defined(TT3_WS_LIBRARY)
    //  Building tt3-ws
#else
    //  Building tt3-ws client
    Q_DECL_IMPORT CurrentCredentials theCurrentCredentials;
#endif
}

//  End of tt3-ws/Credentials.hpp
