//
//  tt3-ws/Account.hpp - A login account
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
    //  A login account
    class TT3_WS_PUBLIC AccountImpl : public PrincipalImpl
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(AccountImpl)

        friend class WorkspaceImpl;
        friend std::shared_ptr<AccountImpl>;

        //////////
        //  Construction/destruction
    private:
        AccountImpl(Workspace workspace, tt3::db::api::IAccount * dataAccount);
        virtual ~AccountImpl();

        //////////
        //  Operations (properties)
    public:
        //  Returns/sets the login identifier of this account.
        //  Throws WorkspaceException if an error occurs.
        QString         login(const Credentials & credentials) const throws(WorkspaceException);
        void            setLogin(const Credentials & credentials, const QString & login) throws(WorkspaceException);

        //  Returns the SHA-1 hash of rhis account's password
        //  as an uppercase hexstring.
        //  Throws WorkspaceException if an error occurs.
        QString         passwordHash(const Credentials & credentials) const throws(WorkspaceException);

        //  Sets the password of this account.
        //  Throws WorkspaceException if an error occurs.
        void            setPassword(const Credentials & credentials, const QString & password) throws(WorkspaceException);

        //  Returns/sets the capabilities of this account.
        //  Throws WorkspaceException if an error occurs.
        Capabilities    capabilities(const Credentials & credentials) const throws(WorkspaceException);
        void            setCapabilities(const Credentials & credentials, Capabilities capabilities) throws(WorkspaceException);

        //////////
        //  Operations (associations)
    public:
        //  Returns the user to whom this account belongs.
        //  Throws WorkspaceException if an error occurs.
        User            user(const Credentials & credentials) const throws(WorkspaceException);

        //////////
        //  Implementation
    private:
        tt3::db::api::IAccount *const   _dataAccount;    //  counts as "reference"

        //  Access control
        virtual bool    _canRead(const Credentials & credentials) const throws(WorkspaceException) override;
        virtual bool    _canModify(const Credentials & credentials) const throws(WorkspaceException) override;
        virtual bool    _canDestroy(const Credentials & credentials) const throws(WorkspaceException) override;
        virtual bool    _destroyingLosesAccess() const throws(WorkspaceException) override;
    };
}

//  Enable objects and object pointers for QVariant
Q_DECLARE_METATYPE(tt3::ws::AccountImpl)
Q_DECLARE_METATYPE(tt3::ws::Account)

//  End of tt3-ws/Account.hpp
