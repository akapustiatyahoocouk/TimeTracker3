//
//  tt3-db-api/Account.hpp - a login account
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
    /// \class IAccount tt3-db-api/API.hpp
    /// \brief A login account.
    class TT3_DB_API_PUBLIC IAccount :
        public virtual IPrincipal
    {
        //////////
        //  This is an interface
    protected:
        IAccount() = default;
        virtual ~IAccount() = default;

        //////////
        //  IObject
    public:
        virtual auto    type(
            ) const -> IObjectType * override
        {
            return ObjectTypes::Account::instance();
        }

        //////////
        //  Operations (properties)
    public:
        /// \brief
        ///     Returns the login identifier of this account.
        /// \return
        ///     The login identifier of this account.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual QString login() const = 0;

        /// \brief
        ///     Sets the login identifier of this account.
        /// \param login
        ///     The new login identifier for this account.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual void    setLogin(
                                const QString & login
                            ) = 0;

        /// \brief
        ///     Returns the SHA-1 hash of rhis account's password.
        /// \return
        ///     The SHA-1 hash of rhis account's password
        ///     as an uppercase hexstring.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual QString passwordHash() const = 0;

        /// \brief
        ///     Sets the password of this account.
        /// \param password
        ///     The new password for this Account.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual void    setPassword(
                                const QString & password
                            ) = 0;

        /// \brief
        ///     Returns the capabilities of this account.
        /// \return
        ///     The capabilities of this account.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual auto    capabilities(
                            ) const -> Capabilities = 0;

        /// \brief
        ///     Sets the capabilities of this account.
        /// \param capabilities
        ///     The new capabilities for this account.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual void    setCapabilities(
                                Capabilities capabilities
                            ) = 0;

        //////////
        //  Operations (associations)
    public:
        /// \brief
        ///     Returns the user to whom this account belongs.
        /// \return
        ///     The user to whom this account belongs.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual auto    user(
                            ) const -> IUser * = 0;

        /// \brief
        ///     Returns the list of "quick pick" activities
        ///     associated with this account.
        /// \details
        ///     This is the list of activities that can be quickly
        ///     chosen via the UI.
        /// \return
        ///     The list of "quick pick" activities
        ///     associated with this account.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual auto    quickPicksList(
                            ) const -> QList<IActivity*> = 0;

        /// \brief
        ///     Sets the list of "quick pick" activities
        ///     associated with this account.
        /// \details
        ///     This is the list of activities that can be quickly
        ///     chosen via the UI.
        /// \param quickPicksList
        ///     The new list of "quick pick" activities
        ///     associated with this account.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual void    setQuickPicksList(
                                const QList<IActivity*> & quickPicksList
                            ) = 0;

        /// \brief
        ///     Returns the set of all Works logged by this Account.
        /// \return
        ///     The set of all Works logged by this Account.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual auto    works(
                            ) const -> Works = 0;

        /// \brief
        ///     Returns the set of all Works logged by this Account
        ///     that fall, fully or partially, within the given UTC
        ///     date+time range.
        /// \param from
        ///     The UTC date+time when the range begins (inclusive).
        /// \param to
        ///     The UTC date+time when the range ends (inclusive).
        /// \return
        ///     The set of all Works logged by this Account.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual auto    works(
                                const QDateTime & from,
                                const QDateTime & to
                            ) const -> Works = 0;

        /// \brief
        ///     Returns the set of all Events logged by this Account.
        /// \return
        ///     The set of all Events logged by this Account.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual auto    events(
                            ) const -> Events = 0;

        /// \brief
        ///     Returns the set of all Events logged by this Account
        ///     that fall within the given UTC date+time range.
        /// \param from
        ///     The UTC date+time when the range begins (inclusive).
        /// \param to
        ///     The UTC date+time when the range ends (inclusive).
        /// \return
        ///     The set of all Events logged by this Account.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual auto    events(
                                const QDateTime & from,
                                const QDateTime & to
                            ) const -> Events = 0;

        //////////
        //  Operations (life cycle)
    public:
        /// \brief
        ///     Creates a new unit of Work for this Account against
        ///     the specified activity.
        /// \param startedAt
        ///     The UTC date+time when this unit of work has started.
        /// \param finishedAt
        ///     The UTC date+time when this unit of work has finished.
        /// \param activity
        ///     The activity associated with this unit of work;
        ///     cannot be nullptr.
        /// \return
        ///     The newly created Work.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual auto    createWork(
                                const QDateTime & startedAt,
                                const QDateTime & finishedAt,
                                IActivity * activity
                            ) -> IWork * = 0;

        /// \brief
        ///     Creates a new Event for this Account against
        ///     the specified activity.
        /// \param occurredAt
        ///     The UTC date+time when the event has occurred.
        /// \param summary
        ///     The brief 1-line summary for the new Event.
        /// \param activities
        ///     The set of activities associated with the new Event;
        ///     can be empty.
        /// \return
        ///     The newly created Event.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual auto    createEvent(
                                const QDateTime & occurredAt,
                                const QString & summary,
                                const Activities & activities
                            ) -> IEvent * = 0;
    };
}

//  End of tt3-db-api/Account.hpp
