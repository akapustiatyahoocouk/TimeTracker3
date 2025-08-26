//
//  tt3-db-api/ObjectType.hpp - a database object type
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
    //  singletons defined in ObjectTypes.
    class TT3_DB_API_PUBLIC IObjectType
    {
        //////////
        //  This is an interface
    protected:
        IObjectType() = default;
        virtual ~IObjectType() = default;

        //////////
        //  Operations
    public:
        //  The mnemonic identifier of this database object type
        virtual QString mnemonic() const = 0;

        //  The user-readable display name of this database object
        //  type for the current default locale.
        virtual QString displayName() const = 0;

        //  The small (16x16) icon representing this database object type
        virtual QIcon   smallIcon() const = 0;

        //  The large (32x32) icon representing this database object type
        virtual QIcon   largeIcon() const = 0;
    };

    //  Standard database object types
    class TT3_DB_API_PUBLIC ObjectTypes final
    {
        UTILITY_CLASS(ObjectTypes)

        //////////
        //  Database object types
    public:
        //  The type of the "User" objects.
        class TT3_DB_API_PUBLIC User final : public virtual IObjectType
        {
            DECLARE_SINGLETON(User)

            //////////
            //  IObjectType
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

        //  The type of the "Account" objects.
        class TT3_DB_API_PUBLIC Account final : public virtual IObjectType
        {
            DECLARE_SINGLETON(Account)

            //////////
            //  IObjectType
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

//  End of tt3-db-api/ObjectType.hpp
