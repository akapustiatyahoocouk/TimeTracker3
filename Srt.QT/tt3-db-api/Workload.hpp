//
//  tt3-db-api/Workload.hpp - a workload that can be contributed to
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
    //  A workload that can be contributed to
    class TT3_DB_API_PUBLIC IWorkload : public virtual IObject
    {
        //////////
        //  This is an interface
    protected:
        IWorkload() = default;
        virtual ~IWorkload() = default;

        //////////
        //  Operations (properties)
    public:
        //  Returns/sets the user-readable display name of
        //  this Workload.
        //  Throws DatabaseException if an error occurs.
        virtual auto    displayName(
                            ) const
                            throws(DatabaseException)
                            -> QString = 0;
        virtual void    setDisplayName(
                                const QString & displayName
                            )
                            throws(DatabaseException) = 0;

        //  Returns/sets the multi-line description of this Workload,
        //  with lines separated by a newline character '\n'.
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
        //  Returns the set of Activities associated with this Workload.
        //  Throws DatabaseException if an error occurs.
        virtual auto    contributingActivities(
                            ) const
                            throws(DatabaseException)
                            -> Activities = 0;

        //  Returns/sets the Beneficiaries of this Workload.
        //  Throws DatabaseException if an error occurs.
        virtual auto    beneficiaries(
                            ) const
                            throws(DatabaseException)
                            -> Beneficiaries = 0;
        virtual void    setBeneficiaries(
                                const Beneficiaries & beneficiaries
                            )
                            throws(DatabaseException) = 0;

        //  Adds/removes the specified Beneficiary to/from the
        //  set of Beneficiaries associated with this Workload;
        //  has no effect if already there/not there.
        //  Throws DatabaseException if an error occurs.
        virtual void    addBeneficiary(
                                IBeneficiary * beneficiary
                            )
                            throws(DatabaseException) = 0;
        virtual void    removeBeneficiary(
                                IBeneficiary * beneficiary
                            )
                            throws(DatabaseException) = 0;

        //  Returns/sets the Users assigned to this  Workload.
        //  Throws DatabaseException if an error occurs.
        virtual auto    assignedUsers(
                            ) const
                            throws(DatabaseException)
                            -> Users = 0;
        virtual void    setAssignedUsers(
                                const Users & users
                            ) throws(DatabaseException) = 0;

        //  Adds/removes the specified User to/from the
        //  set of Users assigned to this Workload;
        //  has no effect if already there/not there.
        //  Throws DatabaseException if an error occurs.
        virtual void    addAssignedUser(
                                IUser * user
                            )
                            throws(DatabaseException) = 0;
        virtual void    removeAssignedUser(
                                IUser * user
                            )
                            throws(DatabaseException) = 0;
    };
}

//  End of tt3-db-api/Workload.hpp
