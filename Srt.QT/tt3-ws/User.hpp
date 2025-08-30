//
//  tt3-ws/User.hpp - A named user
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
    //  A named user
    class TT3_WS_PUBLIC UserImpl : public PrincipalImpl
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(UserImpl)

        friend class WorkspaceImpl;
        friend std::shared_ptr<UserImpl>;

        //////////
        //  Construction/destruction
    private:
        UserImpl(Workspace workspace, tt3::db::api::IUser * dataUser);
        virtual ~UserImpl();

        //////////
        //  Operations (properties)
    public:
        //  Returns/sets the "real name" of this User.
        //  Throws WorkspaceException if an error occurs.
        QString             realName(const Credentials & credentials) const throws(WorkspaceException);
        void                setRealName(const Credentials & credentials, const QString & realName) throws(WorkspaceException);

        //  Returns/sets the "inactivity timeout" of this User.
        //  This is the timeout after which, if the user does nothing,
        //  the "current" Activity is automatically stopped.
        //  Throws WorkspaceException if an error occurs.
        InactivityTimeout   inactivityTimeout(const Credentials & credentials) const throws(WorkspaceException);
        void                setInactivityTimeout(const Credentials & credentials, const InactivityTimeout & inactivityTimeout) throws(WorkspaceException);

        //  Returns/sets the preferred UI locale  of this User.
        //  Throws WorkspaceException if an error occurs.
        UiLocale            uiLocale(const Credentials & credentials) const throws(WorkspaceException);
        void                setUiLocale(const Credentials & credentials, const UiLocale & uiLocale) throws(WorkspaceException);

        //////////
        //  Operations (associations)
    public:
        //  Returns the set of all Accounts of this User.
        //  Throws WorkspaceException if an error occurs.
        Accounts            accounts(const Credentials & credentials) const throws(WorkspaceException);

        //////////
        //  Operations (life cycle)
    public:
        //  Creates a new Account for this User.
        //  All Accounts in a database must have different lohins.
        //  Throws WorkspaceException if an error occurs.
        Account             createAccount(
                                bool enabled, const QStringList & emailAddresses,
                                const QString & login, const QString & password,
                                Capabilities capabilities) throws(WorkspaceException);

        //////////
        //  Implementation
    private:
        tt3::db::api::IUser *const  _dataUser;    //  counts as "reference"

        //  Access control
        virtual bool    _canRead(const Credentials & credentials) const throws(WorkspaceException) override;
        virtual bool    _canModify(const Credentials & credentials) const throws(WorkspaceException) override;
        virtual bool    _canDestroy(const Credentials & credentials) const throws(WorkspaceException) override;
    };
}

//  Enable objects and object pointers for QVariant
Q_DECLARE_METATYPE(tt3::ws::UserImpl)
Q_DECLARE_METATYPE(tt3::ws::User)

//  End of tt3-ws/User.hpp
