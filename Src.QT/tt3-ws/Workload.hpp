//
//  tt3-ws/Workload.hpp - A generic workload
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
    /// \class WorkloadImpl tt3-ws/API.hpp
    /// \brief A generic workload [implementation].
    class TT3_WS_PUBLIC WorkloadImpl
        :   public ObjectImpl
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(WorkloadImpl)

        friend class WorkspaceImpl;
        friend std::shared_ptr<WorkloadImpl>;
        friend class UserImpl;
        friend class ActivityImpl;
        friend class PublicTaskImpl;
        friend class PrivateTaskImpl;
        friend class ProjectImpl;
        friend class WorkStreamImpl;
        friend class BeneficiaryImpl;

        //////////
        //  Construction/destruction - from friends only
    private:
        WorkloadImpl(
                Workspace workspace,
                tt3::db::api::IWorkload * dataWorkload
            );
        virtual ~WorkloadImpl();

        //////////
        //  Operations (properties)
    public:
        /// \brief
        ///     Returns the user-readable display name of this workload.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \return
        ///     The user-readable display name of this workload.
        /// \exception WorkspaceException
        ///     If an error occurs.
        QString     displayName(
                            const Credentials & credentials
                        ) const;

        /// \brief
        ///     Sets the user-readable display name of this workload.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \param displayName
        ///     The new user-readable display name for this workload.
        /// \exception WorkspaceException
        ///     If an error occurs.
        void        setDisplayName(
                            const Credentials & credentials,
                            const QString & displayName
                        );


        /// \brief
        ///     Returns the multi-line description of this workload.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \return
        ///     The multi-line description of this activity
        ///     type, with lines separated by a newline character '\n'.
        /// \exception WorkspaceException
        ///     If an error occurs.
        QString     description(
                            const Credentials & credentials
                        ) const;

        /// \brief
        ///     Sets the multi-line description of this workload.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \param description
        ///     The new multi-line description for this workload,
        ///     with lines separated by a newline character '\n'.
        /// \exception WorkspaceException
        ///     If an error occurs.
        void        setDescription(
                            const Credentials & credentials,
                            const QString & description
                        );

        //////////
        //  Operations (associations)
    public:
        /// \brief
        ///     Returns the set of Activities associated with this Workload.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \return
        ///     The set of Activities associated with this Workload.
        /// \exception WorkspaceException
        ///     If an error occurs.
        auto        contributingActivities(
                            const Credentials & credentials
                        ) const -> Activities;

        /// \brief
        ///     Returns the Beneficiaries of this Workload.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \return
        ///     The Beneficiaries of this Workload.
        /// \exception WorkspaceException
        ///     If an error occurs.
        auto        beneficiaries(
                            const Credentials & credentials
                        ) const -> Beneficiaries;

        /// \brief
        ///     Sets the Beneficiaries of this Workload.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \param beneficiaries
        ///     The new Beneficiaries for this Workload.
        /// \exception WorkspaceException
        ///     If an error occurs.
        void        setBeneficiaries(
                            const Credentials & credentials,
                            const Beneficiaries & beneficiaries
                        );

        /// \brief
        ///     Adds the specified Beneficiary to the set
        ///     of Beneficiaries associated with this Workload.
        /// \details
        ///     Has no effect if already there.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \param beneficiary
        ///     The beneficiary to add.
        /// \exception WorkspaceException
        ///     If an error occurs.
        void        addBeneficiary(
                            const Credentials & credentials,
                            Beneficiary beneficiary
                        );

        /// \brief
        ///     Removes the specified Beneficiary from the set
        ///     of Beneficiaries associated with this Workload.
        /// \details
        ///     Has no effect if already not there.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \param beneficiary
        ///     The beneficiary to remove.
        /// \exception WorkspaceException
        ///     If an error occurs.
        void        removeBeneficiary(
                            const Credentials & credentials,
                            Beneficiary beneficiary
                        );

        /// \brief
        ///     Returns the set of Users assigned to this Workload.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \return
        ///     The set of Users assigned to this Workload.
        /// \exception WorkspaceException
        ///     If an error occurs.
        auto        assignedUsers(
                            const Credentials & credentials
                        ) const -> Users;

        /// \brief
        ///     Sets the Users assigned to this Workload.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \param users
        ///     The ne set of Users to assign to this Workload.
        /// \exception WorkspaceException
        ///     If an error occurs.
        void        setAssignedUsers(
                            const Credentials & credentials,
                            const Users & users
                        );

        /// \brief
        ///     Adds the specified User to the  set of Users
        ///     assigned to this Workload;
        /// \details
        ///     Has no effect if already there there.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \param user
        ///     The user to add.
        /// \exception WorkspaceException
        ///     If an error occurs.
        void        addAssignedUser(
                            const Credentials & credentials,
                            User user
                        );

        /// \brief
        ///     Removes the specified User from the  set of Users
        ///     assigned to this Workload;
        /// \details
        ///     Has no effect if already not there.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \param user
        ///     The user to remove.
        /// \exception WorkspaceException
        ///     If an error occurs.
        void        removeAssignedUser(
                            const Credentials & credentials,
                            User user
                        );

        //////////
        //  Implementation
    private:
        tt3::db::api::IWorkload *const  _dataWorkload;    //  counts as "reference"
    };
}

//  Enable objects and object pointers for QVariant
Q_DECLARE_METATYPE(tt3::ws::WorkloadImpl)
Q_DECLARE_METATYPE(tt3::ws::Workload)

//  End of tt3-ws/Workload.hpp
