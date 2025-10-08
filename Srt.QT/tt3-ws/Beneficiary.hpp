//
//  tt3-ws/Beneficiary.hpp - A beneficiary
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
    /// \class BeneficiaryImpl tt3-ws/API.hpp
    /// \brief A beneficiary.
    class TT3_WS_PUBLIC BeneficiaryImpl final
        :   public ObjectImpl
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(BeneficiaryImpl)

        friend class WorkspaceImpl;
        friend std::shared_ptr<BeneficiaryImpl>;

        //////////
        //  Construction/destruction - from friends only
    private:
        BeneficiaryImpl(
                Workspace workspace,
                tt3::db::api::IBeneficiary * dataBeneficiary
            );
        virtual ~BeneficiaryImpl();

        //////////
        //  Operations (properties)
    public:
        /// \brief
        ///     Returns the user-readable display name of this c.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \return
        ///     The user-readable display name of this Beneficiary.
        /// \exception WorkspaceException
        ///     If an error occurs.
        QString     displayName(
                            const Credentials & credentials
                        ) const;

        /// \brief
        ///     Sets the user-readable display name of this Beneficiary.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \param displayName
        ///     The new user-readable display name for this Beneficiary.
        /// \exception WorkspaceException
        ///     If an error occurs.
        void        setDisplayName(
                                const Credentials & credentials,
                                const QString & displayName
                            );


        /// \brief
        ///     Returns the multi-line description of this Beneficiary.
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
        ///     Sets the multi-line description of this Beneficiary.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \param description
        ///     The new multi-line description for this activity
        ///     type, with lines separated by a newline character '\n'.
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
        ///     Returns the set of all workloads associated with
        ///     this Beneficiary.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \return
        ///     The set of all workloads associated with
        ///     this Beneficiary.
        /// \exception WorkspaceException
        ///     If an error occurs.
        auto        workloads(
                            const Credentials & credentials
                        ) const -> Workloads;

        /// \brief
        ///     Sets the set of all workloads associated with
        ///     this Beneficiary.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \param workloads
        ///     The new set of all workloads to associate with
        ///     this Beneficiary.
        /// \exception WorkspaceException
        ///     If an error occurs.
        void        setWorkloads(
                            const Credentials & credentials,
                            const Workloads & workloads
                        );

        /// \brief
        ///     Adds the specified Workload to the set of
        ///     Workloads associated with this Beneficiary;
        /// \details
        ///     Has no effect if already there.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \param workload
        ///     The workload to add.
        /// \exception WorkspaceException
        ///     If an error occurs.
        void        addWorkload(
                            const Credentials & credentials,
                            Workload workload
                        );

        /// \brief
        ///     Removes the specified Workload from the set of
        ///     Workloads associated with this Beneficiary;
        /// \details
        ///     Has no effect if already not there.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \param workload
        ///     The workload to remove.
        /// \exception WorkspaceException
        ///     If an error occurs.
        void        removeWorkload(
                            const Credentials & credentials,
                            Workload workload
                        );

        //////////
        //  Implementation
    private:
        tt3::db::api::IBeneficiary *const  _dataBeneficiary;    //  counts as "reference"

        //  Access control
        virtual bool    _canRead(       //  throws WorkspaceException
                                const Credentials & credentials
                            ) const override;
        virtual bool    _canModify(     //  throws WorkspaceException
                                const Credentials & credentials
                            ) const override;
        virtual bool    _canDestroy(    //  throws WorkspaceException
                                const Credentials & credentials
                            ) const override;
    };
}

//  Enable objects and object pointers for QVariant
Q_DECLARE_METATYPE(tt3::ws::BeneficiaryImpl)
Q_DECLARE_METATYPE(tt3::ws::Beneficiary)

//  End of tt3-ws/Beneficiary.hpp
