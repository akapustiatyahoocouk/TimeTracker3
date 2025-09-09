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
    /// \class IWorkload "tt3-db-api/API.hpp"
    /// \brief A workload that can be contributed to.
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
        /// \brief
        ///     Returns the user-readable display name of this Workload.
        /// \return
        ///     The user-readable display name of this Workload.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual auto    displayName(
                            ) const -> QString = 0;

        /// \brief
        ///     Sets the user-readable display name of this Workload.
        /// \param displayName
        ///     The new user-readable display name for this Workload.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual void    setDisplayName(
                                const QString & displayName
                            ) = 0;

        /// \brief
        ///     Returns the multi-line description of this Workload.
        /// \return
        ///     The multi-line description of this Workload,
        ///     with lines separated by a newline character '\n'.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual auto    description(
                            ) const -> QString = 0;

        /// \brief
        ///     Sets the multi-line description of this Workload.
        /// \param description
        ///     The new multi-line description for this Workload,
        ///     with lines separated by a newline character '\n'.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual void    setDescription(
                                const QString & description
                            ) = 0;

        //////////
        //  Operations (associations)
    public:
        /// \brief
        ///     Returns the set of Activities associated with this Workload.
        /// \return
        ///     The set of Activities associated with this Workload.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual auto    contributingActivities(
                            ) const -> Activities = 0;

        /// \brief
        ///     Returns the Beneficiaries of this Workload.
        /// \return
        ///     The Beneficiaries of this Workload.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual auto    beneficiaries(
                            ) const -> Beneficiaries = 0;

        /// \brief
        ///     Sets the Beneficiaries of this Workload.
        /// \param beneficiaries
        ///     The new Beneficiaries for this Workload.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual void    setBeneficiaries(
                                const Beneficiaries & beneficiaries
                            ) = 0;

        /// \brief
        ///     Adds the specified Beneficiary to the set
        ///     of Beneficiaries associated with this Workload.
        /// \details
        ///     Has no effect if already there.
        /// \param beneficiary
        ///     The beneficiary to add.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual void    addBeneficiary(
                                IBeneficiary * beneficiary
                            ) = 0;

        /// \brief
        ///     Removes the specified Beneficiary from the set
        ///     of Beneficiaries associated with this Workload.
        /// \details
        ///     Has no effect if already not there.
        /// \param beneficiary
        ///     The beneficiary to remove.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual void    removeBeneficiary(
                                IBeneficiary * beneficiary
                            ) = 0;

        /// \brief
        ///     Returns the set of Users assigned to this Workload.
        /// \return
        ///     The set of Users assigned to this Workload.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual auto    assignedUsers(
                            ) const -> Users = 0;

        /// \brief
        ///     Sets the Users assigned to this Workload.
        /// \param users
        ///     The ne set of Users to assign to this Workload.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual void    setAssignedUsers(
                                const Users & users
                            ) = 0;

        /// \brief
        ///     Adds the specified User to the  set of Users
        ///     assigned to this Workload;
        /// \details
        ///     Has no effect if already there there.
        /// \param user
        ///     The user to add.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual void    addAssignedUser(
                                IUser * user
                            ) = 0;

        /// \brief
        ///     Removes the specified User from the  set of Users
        ///     assigned to this Workload;
        /// \details
        ///     Has no effect if already not there.
        /// \param user
        ///     The user to remove.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual void    removeAssignedUser(
                                IUser * user
                            ) = 0;
    };
}

//  End of tt3-db-api/Workload.hpp
