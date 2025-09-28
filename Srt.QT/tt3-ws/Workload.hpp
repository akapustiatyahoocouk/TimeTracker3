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
    class TT3_WS_PUBLIC WorkloadImpl final
        :   public ObjectImpl
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(WorkloadImpl)

        friend class WorkspaceImpl;
        friend std::shared_ptr<WorkloadImpl>;
        friend class ActivityImpl;
        friend class PublicTaskImpl;

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
        //  Implementation
    private:
        tt3::db::api::IWorkload *const  _dataWorkload;    //  counts as "reference"

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
Q_DECLARE_METATYPE(tt3::ws::WorkloadImpl)
Q_DECLARE_METATYPE(tt3::ws::Workload)

//  End of tt3-ws/Workload.hpp
