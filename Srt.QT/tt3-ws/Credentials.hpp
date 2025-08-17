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
        //////////
        //  Constats
    public:
        //  The invalid credentials
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
        bool            operator == (const Credentials & op2) const
        {
            return _login == op2._login && _password == op2._password;
        }

        bool            operator != (const Credentials & op2) const
        {
            return _login != op2._login || _password != op2._password;
        }

        //////////
        //  Operations
    public:
        //  TODO document
        bool            isValid() const { return !_login.isEmpty(); }
        QString         login() const { return _login; }

        //////////
        //  Implementation
    private:
        QString         _login;
        QString         _password;  //  always "" if _login == ""
    };

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
        ~CurrentCredentials();
        \
        //////////
        //  Operators
    public:
        void                operator = (const Credentials & credentials);
                            operator const Credentials &() const;

        //////////
        //  Operations
    public:
        //  TODO document
        void                swap(Credentials & other);

        bool                isValid() const { return _currentCredentials.isValid(); }
        QString             login() const { return _currentCredentials.login(); }


        //////////
        //  Signals
    signals:
        void                currentCredentialsChanged();

        //////////
        //  Implementation
    private:
        static int          _instanceCount; //  ...to disallow a second instance
        static Credentials  _currentCredentials;
    };

#if defined(TT3_WS_LIBRARY)
    //  Building tt3-ws
#else
    //  Building tt3-ws client
    Q_DECL_IMPORT CurrentCredentials theCurrentCredentials;
#endif
}

inline size_t qHash(const tt3::ws::Credentials & key, size_t seed)
{
    return qHash(key.login(), seed);
}

//  End of tt3-ws/Credentials.hpp
