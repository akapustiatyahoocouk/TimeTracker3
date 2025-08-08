//
//  tt3-db-xml/DatabaseAddress.hpp - "XML file database address"
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
    //////////
    //  An address og an "XML fle database"is its full
    //  canonical path.
    class TT3_DB_XML_PUBLIC DatabaseAddress final
        :   public virtual tt3::db::api::IDatabaseAddress
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(DatabaseAddress)

        friend class DatabaseType;

        //////////
        //  Construction/destruction(from DB type only)
    private:
        explicit DatabaseAddress(const QString & path);
        virtual ~DatabaseAddress();

        //////////
        //  tt3::db::api::IDatabaseAddress (general)
    public:
        virtual tt3::db::api::IDatabaseType *   databaseType() const override;
        virtual QString         displayForm() const override;
        virtual QString         externalForm() const override;

        //////////
        //  tt3::db::api::IDatabaseAddress (reference counting)
    public:
        virtual State           state() const override;
        virtual int             referenceCount() const override;
        virtual void            addReference() override;
        virtual void            releaseReference() override;

        //////////
        //  Implementation
    private:
        QString                 _path;  //  always full path
        State                   _state = State::New;
        int                     _referenceCount = 0;

        //  Helpers
        void                    _checkState();
    };
}

//  End of tt3-db-api/DatabaseAddress.hpp
