//
//  tt3-db-api/Classes.hpp - forward declarations and typedefs
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
    class TT3_DB_API_PUBLIC IDatabaseType;
    class TT3_DB_API_PUBLIC IDatabaseAddress;
    class TT3_DB_API_PUBLIC IDatabase;
    class TT3_DB_API_PUBLIC ChangeNotifier;
    class TT3_DB_API_PUBLIC OID;    //  TODO define? Kill off?
    class TT3_DB_API_PUBLIC IObject;
    class TT3_DB_API_PUBLIC IPrincipal;
    class TT3_DB_API_PUBLIC IUser;
    class TT3_DB_API_PUBLIC IAccount;

    using DatabaseTypes = QSet<IDatabaseType*>;
    using DatabaseAddresses= QSet<IDatabaseAddress*>;
    using Objects = QSet<IObject*>;
    using Users = QSet<IUser*>;
    using Accounts = QSet<IAccount*>;

    class TT3_DB_API_PUBLIC DatabaseException;
}

//  End of tt3-db-api/Classes.hpp
