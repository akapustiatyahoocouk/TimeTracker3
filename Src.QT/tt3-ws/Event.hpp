//
//  tt3-ws/Event.hpp - An event
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
    /// \class EventImpl tt3-ws/API.hpp
    /// \brief An event.
    class TT3_WS_PUBLIC EventImpl final
        :   public ObjectImpl
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(EventImpl)

        friend class WorkspaceImpl;
        friend std::shared_ptr<EventImpl>;
        friend class ActivityImpl;

        //////////
        //  Construction/destruction - from friends only
    private:
        EventImpl(
                Workspace workspace,
                tt3::db::api::IEvent * dataEvent
            );
        virtual ~EventImpl();

        //////////
        //  Operations (properties)
    public:
        /// \brief
        ///     Returns the UTC date+time when this event was logged.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \return
        ///     The UTC date+time when this event was logged.
        /// \exception WorkspaceException
        ///     If an error occurs.
        auto        occurredAt(
                            const tt3::ws::Credentials & credentials
                        ) const -> QDateTime;

        /// \brief
        ///     Returns the summary of this event, as entered by the user.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \return
        ///     The 1-line summary of this event, as entered by the user.
        /// \exception WorkspaceException
        ///     If an error occurs.
        QString     summary(
                            const tt3::ws::Credentials & credentials
                        ) const;

        //////////
        //  Operations (associations)
    public:
        /// \brief
        ///     Returns the Account who has logged this event.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \return
        ///     The Account who has logged this event.
        /// \exception WorkspaceException
        ///     If an error occurs.
        auto        account(
                            const tt3::ws::Credentials & credentials
                        ) const -> Account;

        /// \brief
        ///     Returns the set of Activities against which
        ///     this event was logged;.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \return
        ///     The set of Activities against which this event
        ///     was logged; can be empty.
        /// \exception WorkspaceException
        ///     If an error occurs.
        auto        activities(
                            const tt3::ws::Credentials & credentials
                        ) const -> Activities;

        //////////
        //  Implementation
    private:
        tt3::db::api::IEvent *const _dataEvent;    //  counts as "reference"

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
Q_DECLARE_METATYPE(tt3::ws::EventImpl)
Q_DECLARE_METATYPE(tt3::ws::Event)

//  End of tt3-ws/Event.hpp
