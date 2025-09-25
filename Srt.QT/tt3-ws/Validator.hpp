//
//  tt3-ws/Validator.hpp - property validation API
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
    /// \class Validator tt3-ws/API.hpp
    /// \brief
    ///     An agent that, for a given workspace [type]
    ///     can decide whether a specific value is valid
    ///     for a specific property of a specific object.
    class TT3_WS_PUBLIC Validator final
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Validator)

        friend class WorkspaceTypeImpl;

        //////////
        //  Construction/destruction
    private:
        explicit Validator(
                tt3::db::api::IValidator * databaseValidator
            ) : _databaseValidator(databaseValidator)
        {
            Q_ASSERT(_databaseValidator != nullptr);
        }
        ~Validator() = default;

        //////////
        //  We can piggyback on some DB API types.
    public:
        /// \brief The validator for Principals.
        using Principal = tt3::db::api::IValidator::IPrincipalValidator;
        /// \brief The validator for Users.
        using User = tt3::db::api::IValidator::IUserValidator;
        /// \brief The validator for Accounts.
        using Account = tt3::db::api::IValidator::IAccountValidator;
        /// \brief The validator for ActivityTypes.
        using ActivityType = tt3::db::api::IValidator::IActivityTypeValidator;
        /// \brief The validator for Activities.
        using Activity = tt3::db::api::IValidator::IActivityValidator;
        /// \brief The validator for PublicActivities.
        using PublicActivity = tt3::db::api::IValidator::IPublicActivityValidator;
        /// \brief The validator for PrivateActivities.
        using PrivateActivity = tt3::db::api::IValidator::IPrivateActivityValidator;
        /// \brief The validator for Tasks.
        using Task = tt3::db::api::IValidator::ITaskValidator;
        /// \brief The validator for PublicTasks.
        using PublicTask = tt3::db::api::IValidator::IPublicTaskValidator;
        /// \brief The validator for PrivateTasks.
        using PrivateTask = tt3::db::api::IValidator::IPrivateTaskValidator;

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the validator for Principals.
        /// \return
        ///     The validator for Principals.
        Principal * principal() const { return _databaseValidator->principal(); }

        /// \brief
        ///     Returns the validator for Users.
        /// \return
        ///     The validator for Users.
        User *      user() const { return _databaseValidator->user(); }

        /// \brief
        ///     Returns the validator for Accounts.
        /// \return
        ///     The validator for Accounts.
        Account *   account() const { return _databaseValidator->account(); }

        /// \brief
        ///     Returns the validator for ActivityTypes.
        /// \return
        ///     The validator for ActivityTypes.
        ActivityType *  activityType() const { return _databaseValidator->activityType(); }

        /// \brief
        ///     Returns the validator for Activities.
        /// \return
        ///     The validator for Activities.
        Activity *      activity() const { return _databaseValidator->activity(); }

        /// \brief
        ///     Returns the validator for PublicActivities.
        /// \return
        ///     The validator for PublicActivities.
        PublicActivity *publicActivity() const { return _databaseValidator->publicActivity(); }

        /// \brief
        ///     Returns the validator for PrivateActivities.
        /// \return
        ///     The validator for PrivateActivities.
        PrivateActivity*privateActivity() const { return _databaseValidator->privateActivity(); }

        /// \brief
        ///     Returns the validator for Tasks.
        /// \return
        ///     The validator for Tasks.
        Task *          task() const { return _databaseValidator->task(); }

        /// \brief
        ///     Returns the validator for PublicTasks.
        /// \return
        ///     The validator for PublicTasks.
        PublicTask *    publicTask() const { return _databaseValidator->publicTask(); }

        /// \brief
        ///     Returns the validator for PrivateTasks.
        /// \return
        ///     The validator for PrivateTasks.
        PrivateTask *   privateTask() const { return _databaseValidator->privateTask(); }

        //////////
        //  Implementation
    private:
        tt3::db::api::IValidator *const _databaseValidator;
    };
}

//  End of tt3-ws/Validator.hpp
