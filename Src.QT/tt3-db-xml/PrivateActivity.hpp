//
//  tt3-db-xml/PrivateActivity.hpp - a private activity
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
    /// \class PrivateActivity tt3-db-xml/API.hpp
    /// \brief A private activity in an XML file database.
    class TT3_DB_XML_PUBLIC PrivateActivity
        :   public virtual Activity,
            public virtual tt3::db::api::IPrivateActivity
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(PrivateActivity)

        friend class Database;
        friend class User;
        friend class PrivateTask;

        //////////
        //  Construction/destruction (from DB type only)
    private:
        PrivateActivity(User * owner, tt3::db::api::Oid oid);
        virtual ~PrivateActivity();

        //////////
        //  tt3::db::api::IPrivateActivity (associations)
    public:
        virtual auto    owner(
                            ) const -> tt3::db::api::IUser * override;

        //////////
        //  Implementation
    private:
        //  Associations
        User *          _owner; //  never nullptr

        //  Helpers
        virtual bool    _siblingExists(const QString & displayName) const override;
        virtual void    _makeDead() override;

        //////////
        //  Serialization
    private:
        virtual void    _serializeProperties(
                                QDomElement & objectElement
                            ) const override;
        virtual void    _serializeAggregations(
                                QDomElement & objectElement
                            ) const override;
        virtual void    _serializeAssociations(
                                QDomElement & objectElement
                            ) const override;

        virtual void    _deserializeProperties(
                                const QDomElement & objectElement
                            ) override; //  throws tt3::util::ParseException)
        virtual void    _deserializeAggregations(
                                const QDomElement & objectElement
                            ) override; //  throws tt3::util::ParseException)
        virtual void    _deserializeAssociations(
                                const QDomElement & objectElement
                            ) override;  //  throws tt3::util::ParseException

        //////////
        //  Validation
    private:
        virtual void    _validate(  //  throws tt3::db::api::DatabaseException
                                Objects & validatedObjects
                            ) override;
    };
}

//  End of tt3-db-xml/PrivateActivity.hpp
