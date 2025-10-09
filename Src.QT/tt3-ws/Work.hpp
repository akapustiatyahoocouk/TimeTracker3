//
//  tt3-ws/Work.hpp - A work item
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
    /// \class WorkImpl tt3-ws/API.hpp
    /// \brief A work item.
    class TT3_WS_PUBLIC WorkImpl final
        :   public ObjectImpl
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(WorkImpl)

        friend class WorkspaceImpl;
        friend std::shared_ptr<WorkImpl>;

        //////////
        //  Construction/destruction - from friends only
    private:
        WorkImpl(
                Workspace workspace,
                tt3::db::api::IWork * dataWork
            );
        virtual ~WorkImpl();

        //////////
        //  Operations (properties)
    public:

        //////////
        //  Operations (properties)
    public:
        /// \brief
        ///     Returns the UTC date+time when this unit of work
        ///     was started.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \return
        ///     The UTC date+time when this unit of work
        ///     was started.
        /// \exception WorkspaceException
        ///     If an error occurs.
        auto        startedAt(
                            const tt3::ws::Credentials & credentials
                        ) const -> QDateTime;

        /// \brief
        ///     Returns the UTC date+time when this unit of work
        ///     was finished.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \return
        ///     The UTC date+time when this unit of work
        ///     was finisheds.
        /// \exception WorkspaceException
        ///     If an error occurs.
        auto        finishedAt(
                            const tt3::ws::Credentials & credentials
                        ) const -> QDateTime;

        //////////
        //  Operations (associations)
    public:
        /// \brief
        ///     Returns the Account who has logged this unit of work.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \return
        ///     The Account who has logged this unit of work.
        /// \exception WorkspaceException
        ///     If an error occurs.
        auto        account(
                            const tt3::ws::Credentials & credentials
                        ) const -> Account;

        /// \brief
        ///     Returns the Activity against which this unit of work was logged.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \return
        ///     The Activity against which this unit of work was logged.
        /// \exception WorkspaceException
        ///     If an error occurs.
        auto        activity(
                            const tt3::ws::Credentials & credentials
                        ) const -> Activity;

        //////////
        //  Implementation
    private:
        tt3::db::api::IWork *const  _dataWork;    //  counts as "reference"

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
Q_DECLARE_METATYPE(tt3::ws::WorkImpl)
Q_DECLARE_METATYPE(tt3::ws::Work)

//  End of tt3-ws/Work.hpp
