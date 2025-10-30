//
//  tt3-ws/Account.hpp - A login account
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
    /// A login account.
    class TT3_WS_PUBLIC AccountImpl final
        :   public PrincipalImpl
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(AccountImpl)

        friend class WorkspaceImpl;
        friend std::shared_ptr<AccountImpl>;

        //////////
        //  Construction/destruction - from friends only
    private:
        AccountImpl(Workspace workspace, tt3::db::api::IAccount * dataAccount);
        virtual ~AccountImpl();

        //////////
        //  Operations (properties)
    public:
        /// \brief
        ///     Returns the login identifier of this account.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \return
        ///     The login identifier of this account.
        /// \exception WorkspaceException
        ///     If an error occurs.
        QString     login(
                            const Credentials & credentials
                        ) const;

        /// \brief
        ///     Sets the login identifier of this account.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \param login
        ///     The new login identifier for this account.
        ///     All Accounts in a Workspace must have different
        ///     login identifiers.
        /// \exception WorkspaceException
        ///     If an error occurs.
        void        setLogin(
                            const Credentials & credentials,
                            const QString & login
                        );

        /// \brief
        ///     Returns the SHA-1 hash of this account's password.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \return
        ///     The SHA-1 hash of this account's password
        ///     as an uppercase hexstring.
        /// \exception WorkspaceException
        ///     If an error occurs.
        QString     passwordHash(
                            const Credentials & credentials
                        ) const;

        /// \brief
        ///     Sets the password of this account.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \param password
        ///     The new password for this Account.
        /// \exception WorkspaceException
        ///     If an error occurs.
        void        setPassword(
                            const Credentials & credentials,
                            const QString & password
                        );

        /// \brief
        ///     Returns the capabilities of this account.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \return
        ///     The capabilities of this account.
        /// \exception WorkspaceException
        ///     If an error occurs.
        auto        capabilities(
                            const Credentials & credentials
                        ) const -> Capabilities;

        /// \brief
        ///     Sets the capabilities of this account.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \param capabilities
        ///     The new capabilities for this account.
        /// \exception WorkspaceException
        ///     If an error occurs.
        void        setCapabilities(
                            const Credentials & credentials,
                            Capabilities capabilities
                        );

        //////////
        //  Operations (associations)
    public:
        /// \brief
        ///     Returns the user to whom this account belongs.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \return
        ///     The user to whom this account belongs.
        /// \exception WorkspaceException
        ///     If an error occurs.
        auto        user(
                            const Credentials & credentials
                        ) const -> User;

        /// \brief
        ///     Returns the list of "quick pick" activities
        ///     associated with this account.
        /// \details
        ///     This is the list of activities that can be quickly
        ///     chosen via the UI.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \return
        ///     The list of "quick pick" activities
        ///     associated with this account.
        /// \exception WorkspaceException
        ///     If an error occurs.
        auto        quickPicksList(
                            const Credentials & credentials
                        ) const -> QList<Activity>;

        /// \brief
        ///     Sets the list of "quick pick" activities
        ///     associated with this account.
        /// \details
        ///     This is the list of activities that can be quickly
        ///     chosen via the UI.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \param quickPicksList
        ///     The new list of "quick pick" activities
        ///     associated with this account.
        /// \exception WorkspaceException
        ///     If an error occurs.
        void        setQuickPicksList(
                            const Credentials & credentials,
                            const QList<Activity> & quickPicksList
                        );

        /// \brief
        ///     Returns the set of all Works logged by this Account.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \return
        ///     The set of all Works logged by this Account.
        /// \exception WorkspaceException
        ///     If an error occurs.
        auto        works(
                            const Credentials & credentials
                        ) const -> Works;

        /// \brief
        ///     Returns the set of all Works logged by this Account
        ///     that fall, fully or partially, within the given UTC
        ///     date+time range.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \param from
        ///     The UTC date+time when the range begins (inclusive).
        /// \param to
        ///     The UTC date+time when the range ends (inclusive).
        /// \return
        ///     The set of all Works logged by this Account.
        /// \exception WorkspaceException
        ///     If an error occurs.
        auto        works(
                            const Credentials & credentials,
                            const QDateTime & from,
                            const QDateTime & to
                        ) const -> Works;

        /// \brief
        ///     Returns the set of all Events logged by this Account.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \return
        ///     The set of all Works logged by this Events.
        /// \exception WorkspaceException
        ///     If an error occurs.
        auto        events(
                            const Credentials & credentials
                        ) const -> Events;

        /// \brief
        ///     Returns the set of all Events logged by this Account
        ///     that fall within the given UTC date+time range.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \param from
        ///     The UTC date+time when the range begins (inclusive).
        /// \param to
        ///     The UTC date+time when the range ends (inclusive).
        /// \return
        ///     The set of all Events logged by this Account.
        /// \exception WorkspaceException
        ///     If an error occurs.
        auto        events(
                            const Credentials & credentials,
                            const QDateTime & from,
                            const QDateTime & to
                        ) const -> Events;

        //////////
        //  Operations (life cycle)
    public:
        /// \brief
        ///     Creates a new unit of Work for this Account against
        ///     the specified activity.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \param startedAt
        ///     The UTC date+time when this unit of work has started.
        /// \param finishedAt
        ///     The UTC date+time when this unit of work has finished.
        /// \param activity
        ///     The activity associated with this unit of work;
        ///     cannot be nullptr.
        /// \return
        ///     The newly created Work.
        /// \exception WorkspaceException
        ///     If an error occurs.
        auto        createWork(
                            const Credentials & credentials,
                            const QDateTime & startedAt,
                            const QDateTime & finishedAt,
                            Activity activity
                        ) -> Work;

        /// \brief
        ///     Creates a new Event for this Account against
        ///     the specified activity.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \param occurredAt
        ///     The UTC date+time when the event has occurred.
        /// \param summary
        ///     The brief 1-line summary for the new Event.
        /// \param activities
        ///     The set of activities associated with the new Event;
        ///     can be empty.
        /// \return
        ///     The newly created Event.
        /// \exception WorkspaceException
        ///     If an error occurs.
        auto        createEvent(
                            const Credentials & credentials,
                            const QDateTime & occurredAt,
                            const QString & summary,
                            const Activities & activities
                        ) -> Event;

        //////////
        //  Implementation
    private:
        tt3::db::api::IAccount *const   _dataAccount;    //  counts as "reference"

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
        virtual bool    _destroyingLosesAccess(
                            ) const override;
    };
}

//  Enable objects and object pointers for QVariant
Q_DECLARE_METATYPE(tt3::ws::AccountImpl)
Q_DECLARE_METATYPE(tt3::ws::Account)

//  End of tt3-ws/Account.hpp
