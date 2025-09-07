//
//  tt3-db-api/Beneficiary.hpp - a beneficiary
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
    //  A beneficiary
    class TT3_DB_API_PUBLIC IBeneficiary :
        public virtual IObject
    {
        //////////
        //  This is an interface
    protected:
        IBeneficiary() = default;
        virtual ~IBeneficiary() = default;

        //////////
        //  IObject
    public:
        virtual auto    type() const
                            -> IObjectType * override
        {
            return ObjectTypes::Beneficiary::instance();
        }

        //////////
        //  Operations (properties)
    public:
        //  Returns/sets the user-readable display name of
        //  this beneficiary.
        //  Throws DatabaseException if an error occurs.
        virtual auto    displayName(
                            ) const
                            throws(DatabaseException)
                            -> QString = 0;
        virtual void    setDisplayName(
                                const QString & displayName
                            )
                            throws(DatabaseException) = 0;

        //  Returns/sets the multi-line description of this
        //  beneficiary, with lines separated by a newline
        //  character '\n'.
        //  Throws DatabaseException if an error occurs.
        virtual auto    description(
                            ) const
                            throws(DatabaseException)
                            -> QString = 0;
        virtual void    setDescription(
                                const QString & description
                            )
                            throws(DatabaseException) = 0;

        //////////
        //  Operations (associations)
    public:
        //  Returns/sets the set of all workloads associated with
        //  this beneficiary.
        //  Throws DatabaseException if an error occurs.
        virtual auto    workloads(
                            ) const
                            throws(DatabaseException)
                            -> Workloads = 0;
        virtual void    setWorkloads(
                                const Workloads & workloads
                            )
                            throws(DatabaseException) = 0;

        //  Adds/removes the specified Workload to/from the
        //  set of Workloads associated with this Beneficiary;
        //  has no effect if already there/not there.
        //  Throws DatabaseException if an error occurs.
        virtual void    addWorkload(
                                IWorkload * workload
                            )
                            throws(DatabaseException) = 0;
        virtual void    removeWorkload(
                                IWorkload * workload
                            )
                            throws(DatabaseException) = 0;
    };
}

//  End of tt3-db-api/Beneficiary.hpp
