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
    //////////
    //  The login credentials
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
        //  The invalid credentials (with empty login)
        static const Credentials Invalid;

        //////////
        //  Construction/destruction/assignment
    public:
        Credentials() = default;
        Credentials(const QString & login, const QString & password);
        ~Credentials() = default;
        //  Default copy constructor and assignment are both OK

        //////////
        //  Operators
    public:
        //  Two credentials are equal if all their details are equal
        bool            operator == (const Credentials & op2) const;
        bool            operator != (const Credentials & op2) const;

        //  We want to use Credentials as a QSet/QMap keys
        bool            operator <  (const Credentials & op2) const;
        bool            operator <= (const Credentials & op2) const;
        bool            operator >  (const Credentials & op2) const;
        bool            operator >= (const Credentials & op2) const;

        //////////
        //  Operations
    public:
        //  Checks whether these Credentials are "valid",
        //  (i.e. the "login" os not empty.
        bool            isValid() const { return !_login.isEmpty(); }

        //  The login identifier of these Credentials.
        //  For invalid Credentials this is an empty string.
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

    //////////
    //  The accessor for a "current" credentials.
    class TT3_WS_PUBLIC CurrentCredentials final : public QObject
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(CurrentCredentials)

        //////////
        //  Construction/destruction
    public:
        CurrentCredentials();
        virtual ~CurrentCredentials();
        \
        //////////
        //  Operators
    public:
        void                operator = (const Credentials & credentials);
                            operator const Credentials &() const;

        //////////
        //  Operations
    public:
        //  Checks whether current Credentials are "valid",
        //  (i.e. the "login" is not empty.
        bool                isValid() const;

        //  The login identifier of the current Credentials.
        //  For invalid Credentials this is an empty string.
        QString             login() const;

        //////////
        //  Signals
    signals:
        //  Emitted after the current credentials have changed.
        void                changed(Credentials before, Credentials after);

        //////////
        //  Implementation
    private:
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
