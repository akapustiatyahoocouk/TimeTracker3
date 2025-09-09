//
//  tt3-db-xml/Classes.hpp - forward declarations and typedefs
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

namespace tt3::db::xml
{
    class TT3_DB_XML_PUBLIC DatabaseType;
    class TT3_DB_XML_PUBLIC DatabaseAddress;
    class TT3_DB_XML_PUBLIC Database;

    class TT3_DB_XML_PUBLIC Object;
    class TT3_DB_XML_PUBLIC Principal;
    class TT3_DB_XML_PUBLIC User;
    class TT3_DB_XML_PUBLIC Account;
    class TT3_DB_XML_PUBLIC ActivityType;
    class TT3_DB_XML_PUBLIC Activity;

    using Users = QSet<User*>;
    using Accounts = QSet<Account*>;
    using ActivityTypes = QSet<ActivityType*>;
    using Activities = QSet<Activity*>;
}

//  End of tt3-db-xml/Classes.hpp
