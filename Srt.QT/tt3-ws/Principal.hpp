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
    class TT3_WS_PUBLIC PrincipalImpl : public ObjectImpl
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(PrincipalImpl)

        //////////
        //  Construction/destruction
    private:
        PrincipalImpl(Workspace workspace, tt3::db::api::IPrincipal * dataPrincipal);
        virtual ~PrincipalImpl();

        //////////
        //  Operations (properties)
    public:
        //  Checks/sets whether this Principal is "enabled". "Disabled"
        //  Principals do not exist as far as access control is concerned.
        //  Throws WorkspaceException if an error occurs.
        bool            enabled(const Credentials & credentials) const throws(WorkspaceException);
        void            setEnabled(const Credentials & credentials, bool enabled) throws(WorkspaceException);

        //  Returns/sets the list of e-mail addresses
        //  associated with this Principal.
        //  Throws WorkspaceException if an error occurs.
        QStringList     emailAddresses(const Credentials & credentials) const throws(WorkspaceException);
        void            setEmailAddresses(const Credentials & credentials, const QStringList & emailAddresses) throws(WorkspaceException);

        //////////
        //  Implementation
    private:
        tt3::db::api::IPrincipal *const _dataPrincipal;    //  counts as "refrence"
    };
}

//  End of tt3-ws/Principal.hpp
