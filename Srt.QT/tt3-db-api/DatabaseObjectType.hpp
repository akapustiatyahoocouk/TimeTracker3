//
//  tt3-db-api/DatabaseObjectType.hpp - a database object type
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
    //  Represents a type of a database object.
    //  All predefined database object types are
    //  singletons defined in DatabaseObjectTypes.
    class TT3_DB_API_PUBLIC IDatabaseObjectType
    {
        //////////
        //  This is an interface
    protected:
        IDatabaseObjectType() = default;
        virtual ~IDatabaseObjectType() = default;

        //////////
        //  Operations
    public:
        //  The mnemonic identifier of this database object type
        virtual QString mnemonic() const = 0;

        //  The user-readable display name of this database object type
        virtual QString displayName() const = 0;

        //  The small (16x16) icon representing this database object type
        virtual QIcon   smallIcon() const = 0;

        //  The large (32x32) icon representing this database object type
        virtual QIcon   largeIcon() const = 0;
    };

    //  Standard database object types
    class TT3_DB_API_PUBLIC DatabaseObjectTypes final
    {
        UTILITY_CLASS(DatabaseObjectTypes)

        //////////
        //  Database object types
    public:
        class TT3_DB_API_PUBLIC User final : public virtual IDatabaseObjectType
        {
            DECLARE_SINGLETON(User)

            //////////
            //  IDatabaseObjectType
        public:
            virtual QString mnemonic() const override;
            virtual QString displayName() const override;
            virtual QIcon   smallIcon() const override;
            virtual QIcon   largeIcon() const override;

            //////////
            //  Implementation
        private:
            const QIcon     _smallIcon;
            const QIcon     _largeIcon;
        };

        class TT3_DB_API_PUBLIC Account final : public virtual IDatabaseObjectType
        {
            DECLARE_SINGLETON(Account)

            //////////
            //  IDatabaseObjectType
        public:
            virtual QString mnemonic() const override;
            virtual QString displayName() const override;
            virtual QIcon   smallIcon() const override;
            virtual QIcon   largeIcon() const override;

            //////////
            //  Implementation
        private:
            const QIcon     _smallIcon;
            const QIcon     _largeIcon;
        };
    };
}

//  End of tt3-db-api/DatabaseObjectType.hpp
