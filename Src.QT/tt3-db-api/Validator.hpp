//
//  tt3-db-api/Validator.hpp - property validation API
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
    /// \class IValidator tt3-db-api/API.hpp
    /// \brief
    ///     An agent that, for a givel database [type]
    ///     can decide whether a specific value is valid
    ///     for a specific property of a specific object.
    class TT3_DB_API_PUBLIC IValidator
    {
        //////////
        //  This is an interface
    protected:
        IValidator() = default;
        virtual ~IValidator() = default;

        //////////
        //  Validators
    public:
        /// \class IPrincipalValidator tt3-db-api/API.hpp
        /// \brief The validator for a Principal.
        class TT3_DB_API_PUBLIC IPrincipalValidator
        {
            //////////
            //  This is an interface
        protected:
            IPrincipalValidator() = default;
            virtual ~IPrincipalValidator() = default;

            //////////
            //  Operations
        public:
            /// \brief
            ///     Checks whether the specified string is a valid
            ///     e-mail address.
            /// \param emailAddress
            ///     The string to check.
            /// \return
            ///     True if the specified string is a valid e-mail
            ///     address, else false.
            virtual bool    isValidEmailAddress(
                                    const QString & emailAddress
                                ) = 0;

            /// \brief
            ///     Checks whether the specified list of strings
            ///     is a valid e-mail addresses list.
            /// \param emailAddresses
            ///     The list of strings to check.
            /// \return
            ///     True if the specified list of strings is a
            ///     valid e-mail addresses list, else false.
            virtual bool    isValidEmailAddresses(
                                    const QStringList & emailAddresses
                                )
            {
                for (const QString & emailAddress : emailAddresses)
                {
                    if (!isValidEmailAddress(emailAddress))
                    {
                        return false;
                    }
                }
                return QSet<QString>(emailAddresses.cbegin(), emailAddresses.cend()).size() == emailAddresses.size();
            }
        };
        /// \brief
        ///     Returns the validator for Principals.
        /// \return
        ///     The validator for Principals.
        virtual auto    principal(
                            ) const -> IPrincipalValidator * = 0;

        /// \class IUserValidator tt3-db-api/API.hpp
        /// \brief The validator for a User.
        class TT3_DB_API_PUBLIC IUserValidator
            :   public virtual IPrincipalValidator
        {
            //////////
            //  This is an interface
        protected:
            IUserValidator() = default;
            virtual ~IUserValidator() = default;

            //////////
            //  Operations
        public:
            /// \brief
            ///     Checks whether the specified string is a
            ///     valid "real name" for a User.
            /// \param realName
            ///     The string to check.
            /// \return
            ///     True the specified string is a valid
            ///     "real name" for a User, else false.
            virtual bool    isValidRealName(
                                    const QString & realName
                                ) = 0;

            /// \brief
            ///     Checks whether the specified value is a valid
            ///     "inactivity timeout" for a User.
            /// \param inactivityTimeout
            ///     The value to check.
            /// \return
            ///     True if the specified value is a valid "inactivity
            ///     timeout" for a User, else false.
            virtual bool    isValidInactivityTimeout(
                                    const InactivityTimeout & inactivityTimeout
                                ) = 0;

            /// \brief
            ///     Checks whether the specified value is a valid "UI locale"
            ///     for a User.
            /// \param uiLocale
            ///     The value to check.
            /// \return
            ///     True if the specified value is a valid "UI locale"
            ///     for a User, else false.
            virtual bool    isValidUiLocale(
                                    const UiLocale & uiLocale
                                ) = 0;
        };
        /// \brief
        ///     Returns the validator for Users.
        /// \return
        ///     The validator for Users.
        virtual auto    user(
                            ) const -> IUserValidator * = 0;

        /// \class IAccountValidator tt3-db-api/API.hpp
        /// \brief The validator for an Account.
        class TT3_DB_API_PUBLIC IAccountValidator
            :   public virtual IPrincipalValidator
        {
            //////////
            //  This is an interface
        protected:
            IAccountValidator() = default;
            virtual ~IAccountValidator() = default;

            //////////
            //  Operations
        public:
            /// \brief
            ///     Checks whether the specified string is a valid
            ///     "login" for an Account.
            /// \param login
            ///     The string to check.
            /// \return
            ///     True if the specified string is a valid "login"
            ///     for an Account, else false.
            virtual bool    isValidLogin(
                                    const QString & login
                                ) = 0;

            /// \brief
            ///     Checks whether the specified string is a valid
            ///     "password" for an Account.
            /// \param password
            ///     The string to check.
            /// \return
            ///     True if the specified string is a valid "password"
            ///     for an Account, else false.
            virtual bool    isValidPassword(
                                    const QString & password
                                ) = 0;
        };
        /// \brief
        ///     Returns the validator for Accounts.
        /// \return
        ///     The validator for Accounts.
        virtual auto    account(
                            ) const -> IAccountValidator * = 0;

        /// \class IActivityTypeValidator tt3-db-api/API.hpp
        /// \brief The validator for an ActivityType.
        class TT3_DB_API_PUBLIC IActivityTypeValidator
        {
            //////////
            //  This is an interface
        protected:
            IActivityTypeValidator() = default;
            virtual ~IActivityTypeValidator() = default;

            //////////
            //  Operations
        public:
            /// \brief
            ///     Checks whether the specified string is a valid
            ///     "display name" for an ActivityType.
            /// \param displayName
            ///     The string to check.
            /// \return
            ///     True if the specified string is a valid
            ///     "display name" for an ActivityType, else false.
            virtual bool    isValidDisplayName(
                                    const QString & displayName
                                ) = 0;

            /// \brief
            ///     Checks whether the specified string is a valid
            ///     "description" for an ActivityType.
            /// \param description
            ///     The string to check.
            /// \return
            ///     True if the specified string is a valid
            ///     "description" for an ActivityType, else false.
            virtual bool    isValidDescription(
                                    const QString & description
                                ) = 0;
        };
        /// \brief
        ///     Returns the validator for ActivityTypes.
        /// \return
        ///     The validator for ActivityTypes.
        virtual auto    activityType(
                            ) const -> IActivityTypeValidator * = 0;

        /// \class IActivityValidator tt3-db-api/API.hpp
        /// \brief The validator for an Activity.
        class TT3_DB_API_PUBLIC IActivityValidator
        {
            //////////
            //  This is an interface
        protected:
            IActivityValidator() = default;
            virtual ~IActivityValidator() = default;

            //////////
            //  Operations
        public:
            /// \brief
            ///     Checks whether the specified string is a valid
            ///     "display name" for an Activity.
            /// \param displayName
            ///     The string to check.
            /// \return
            ///     True if the specified string is a valid
            ///     "display name" for an Activity.
            virtual bool    isValidDisplayName(
                                    const QString & displayName
                                ) = 0;

            /// \brief
            ///     Checks whether the specified string is a valid
            ///     "description" for an Activity.
            /// \param description
            ///     The string to check.
            /// \return
            ///     True if the specified string is a valid
            ///     "description" for an Activity.
            virtual bool    isValidDescription(
                                    const QString & description
                                ) = 0;

            /// \brief
            ///     Checks whether the specified value is a valid
            ///     "timeout" for an Activity.
            /// \param timeout
            ///     The value to check.
            /// \return
            ///     True the specified value is a valid "timeout"
            ///     for an Activity, else false.
            virtual bool    isValidTimeout(
                                    const InactivityTimeout & timeout
                                ) = 0;
        };
        /// \brief
        ///     Returns the validator for Activities.
        /// \return
        ///     The validator for Activities.
        virtual auto    activity(
                            ) const -> IActivityValidator * = 0;

        /// \class IPublicActivityValidator tt3-db-api/API.hpp
        /// \brief The validator for a Public Activity.
        class TT3_DB_API_PUBLIC IPublicActivityValidator
            :   public virtual IActivityValidator
        {
            //////////
            //  This is an interface
        protected:
            IPublicActivityValidator() = default;
            virtual ~IPublicActivityValidator() = default;
        };
        /// \brief
        ///     Returns the validator for PublicActivities.
        /// \return
        ///     The validator for PublicActivities.
        virtual auto    publicActivity(
            ) const -> IPublicActivityValidator * = 0;

        /// \class IPrivateActivityValidator tt3-db-api/API.hpp
        /// \brief The validator for a Private Activity.
        class TT3_DB_API_PUBLIC IPrivateActivityValidator
            :   public virtual IActivityValidator
        {
            //////////
            //  This is an interface
        protected:
            IPrivateActivityValidator() = default;
            virtual ~IPrivateActivityValidator() = default;
        };
        /// \brief
        ///     Returns the validator for PrivateActivities.
        /// \return
        ///     The validator for PrivateActivities.
        virtual auto    privateActivity(
            ) const -> IPrivateActivityValidator * = 0;

        /// \class ITaskValidator tt3-db-api/API.hpp
        /// \brief The validator for a Task.
        class TT3_DB_API_PUBLIC ITaskValidator
            :   public virtual IActivityValidator
        {
            //////////
            //  This is an interface
        protected:
            ITaskValidator() = default;
            virtual ~ITaskValidator() = default;
        };
        /// \brief
        ///     Returns the validator for Tasks.
        /// \return
        ///     The validator for Tasks.
        virtual auto    task(
                            ) const -> ITaskValidator * = 0;

        /// \class IPublicTaskValidator tt3-db-api/API.hpp
        /// \brief The validator for a PublicTask.
        class TT3_DB_API_PUBLIC IPublicTaskValidator
            :   public virtual IPublicActivityValidator,
                public virtual ITaskValidator
        {
            //////////
            //  This is an interface
        protected:
            IPublicTaskValidator() = default;
            virtual ~IPublicTaskValidator() = default;
        };
        /// \brief
        ///     Returns the validator for PublicTasks.
        /// \return
        ///     The validator for PublicTasks.
        virtual auto    publicTask(
                            ) const -> IPublicTaskValidator * = 0;

        /// \class IPrivateTaskValidator tt3-db-api/API.hpp
        /// \brief The validator for a PrivateTask.
        class TT3_DB_API_PUBLIC IPrivateTaskValidator
            :   public virtual IPrivateActivityValidator,
                public virtual ITaskValidator
        {
            //////////
            //  This is an interface
        protected:
            IPrivateTaskValidator() = default;
            virtual ~IPrivateTaskValidator() = default;
        };
        /// \brief
        ///     Returns the validator for PrivateTasks.
        /// \return
        ///     The validator for PrivateTasks.
        virtual auto    privateTask(
                            ) const -> IPrivateTaskValidator * = 0;

        /// \class IWorkloadValidator tt3-db-api/API.hpp
        /// \brief The validator for a Workload.
        class TT3_DB_API_PUBLIC IWorkloadValidator
        {
            //////////
            //  This is an interface
        protected:
            IWorkloadValidator() = default;
            virtual ~IWorkloadValidator() = default;

            //////////
            //  Operations
        public:
            /// \brief
            ///     Checks whether the specified string is a valid
            ///     "display name" for a Workload.
            /// \param displayName
            ///     The string to check.
            /// \return
            ///     True if the specified string is a valid
            ///     "display name" for a Workload.
            virtual bool    isValidDisplayName(
                                    const QString & displayName
                                ) = 0;

            /// \brief
            ///     Checks whether the specified string is a valid
            ///     "description" for a Workload.
            /// \param description
            ///     The string to check.
            /// \return
            ///     True if the specified string is a valid
            ///     "description" for a Workload.
            virtual bool    isValidDescription(
                                    const QString & description
                                ) = 0;
        };
        /// \brief
        ///     Returns the validator for Workloads.
        /// \return
        ///     The validator for Workloads.
        virtual auto    workload(
            ) const -> IWorkloadValidator * = 0;

        /// \class IProjectValidator tt3-db-api/API.hpp
        /// \brief The validator for a Project.
        class TT3_DB_API_PUBLIC IProjectValidator
            : public virtual IWorkloadValidator
        {
            //////////
            //  This is an interface
        protected:
            IProjectValidator() = default;
            virtual ~IProjectValidator() = default;
        };
        /// \brief
        ///     Returns the validator for Projects.
        /// \return
        ///     The validator for Projects.
        virtual auto    project(
                            ) const -> IProjectValidator * = 0;

        /// \class IWorkStreamValidator tt3-db-api/API.hpp
        /// \brief The validator for a WorkStream.
        class TT3_DB_API_PUBLIC IWorkStreamValidator
            : public virtual IWorkloadValidator
        {
            //////////
            //  This is an interface
        protected:
            IWorkStreamValidator() = default;
            virtual ~IWorkStreamValidator() = default;
        };
        /// \brief
        ///     Returns the validator for WorkStreams.
        /// \return
        ///     The validator for WorkStreams.
        virtual auto    workStream(
                            ) const -> IWorkStreamValidator * = 0;

        /// \class IBeneficiaryValidator tt3-db-api/API.hpp
        /// \brief The validator for a Beneficiary.
        class TT3_DB_API_PUBLIC IBeneficiaryValidator
        {
            //////////
            //  This is an interface
        protected:
            IBeneficiaryValidator() = default;
            virtual ~IBeneficiaryValidator() = default;

            //////////
            //  Operations
        public:
            /// \brief
            ///     Checks whether the specified string is a valid
            ///     "display name" for a Beneficiary.
            /// \param displayName
            ///     The string to check.
            /// \return
            ///     True if the specified string is a valid
            ///     "display name" for a Beneficiary, else false.
            virtual bool    isValidDisplayName(
                                    const QString & displayName
                                ) = 0;

            /// \brief
            ///     Checks whether the specified string is a valid
            ///     "description" for a Beneficiary.
            /// \param description
            ///     The string to check.
            /// \return
            ///     True if the specified string is a valid
            ///     "description" for a Beneficiary, else false.
            virtual bool    isValidDescription(
                                    const QString & description
                                ) = 0;
        };
        /// \brief
        ///     Returns the validator for Beneficiaries.
        /// \return
        ///     The validator for Beneficiaries.
        virtual auto    beneficiary(
                                ) const -> IBeneficiaryValidator * = 0;

        /// \class IWorkValidator tt3-db-api/API.hpp
        /// \brief The validator for a Work.
        class TT3_DB_API_PUBLIC IWorkValidator
        {
            //////////
            //  This is an interface
        protected:
            IWorkValidator() = default;
            virtual ~IWorkValidator() = default;

            //////////
            //  Operations
        public:
            /// \brief
            ///     Checks whether the specified start/end date+time
            ///     is a valid start/at combination for a Work.
            /// \param startedAt
            ///     The UTC date+time when Work unit starts.
            /// \param finishedAt
            ///     The UTC date+time when Work unit finishes.
            /// \return
            ///     True the specified start/end date+time is a
            ///     valid start/at combination for a Work, else false.
            virtual bool    isValidStartedFinishedAt(
                                    const QDateTime & startedAt,
                                    const QDateTime & finishedAt
                                ) = 0;
        };
        /// \brief
        ///     Returns the validator for Works.
        /// \return
        ///     The validator for Works.
        virtual auto    work() const -> IWorkValidator * = 0;

        /// \class IEventValidator tt3-db-api/API.hpp
        /// \brief The validator for a Event.
        class TT3_DB_API_PUBLIC IEventValidator
        {
            //////////
            //  This is an interface
        protected:
            IEventValidator() = default;
            virtual ~IEventValidator() = default;

            //////////
            //  Operations
        public:
            /// \brief
            ///     Checks whether the specified date+time
            ///     is a valid occurrence time for an Work.
            /// \param occurredAt
            ///     The UTC date+time when the Event has occurred.
            /// \return
            ///     True the specified start/end date+time is a
            ///     valid start/at combination for an Work, else false.
            virtual bool    isValidOccurredAt(
                                    const QDateTime & occurredAt
                                ) = 0;

            /// \brief
            ///     Checks whether the specified string is a valid
            ///     "summary" for an Event.
            /// \param summary
            ///     The string to check.
            /// \return
            ///     True if the specified string is a valid
            ///     "summary" for an Event.
            virtual bool    isValidSummary(
                                    const QString & summary
                                ) = 0;
        };
        /// \brief
        ///     Returns the validator for Events.
        /// \return
        ///     The validator for Events.
        virtual auto    event(
            ) const -> IEventValidator * = 0;
    };

    /// \class DefaultValidator tt3-db-api/API.hpp
    /// \brief
    ///     The "default" validator, normally suitable
    ///     for all database [types].
    /// \details
    ///     It is recommended that concrete database types
    ///     adhere to object property validation rules
    ///     defined by the "default" validator.
    class TT3_DB_API_PUBLIC DefaultValidator final
        :   public virtual IValidator
    {
        TT3_DECLARE_SINGLETON(DefaultValidator)

        //////////
        //  IValidator
    public:
        virtual auto    principal(
                            ) const -> IPrincipalValidator * override;
        virtual auto    user(
                            ) const -> IUserValidator * override;
        virtual auto    account(
                            ) const -> IAccountValidator * override;
        virtual auto    activityType(
                            ) const -> IActivityTypeValidator * override;
        virtual auto    activity(
                            ) const -> IActivityValidator * override;
        virtual auto    publicActivity(
                            ) const -> IPublicActivityValidator * override;
        virtual auto    privateActivity(
                            ) const -> IPrivateActivityValidator * override;
        virtual auto    task(
                            ) const -> ITaskValidator * override;
        virtual auto    publicTask(
                            ) const -> IPublicTaskValidator * override;
        virtual auto    privateTask(
                            ) const -> IPrivateTaskValidator * override;
        virtual auto    workload(
                            ) const -> IWorkloadValidator * override;
        virtual auto    project(
                            ) const -> IProjectValidator * override;
        virtual auto    workStream(
                            ) const -> IWorkStreamValidator * override;
        virtual auto    beneficiary(
                            ) const -> IBeneficiaryValidator * override;
        virtual auto    work(
                            ) const -> IWorkValidator * override;
        virtual auto    event(
                            ) const -> IEventValidator * override;

        //////////
        //  Implementatiob
    private:
        //  Helpers
        static bool     _isValidEmailAddress(
                                const QString & emailAddress
                            );
        static bool     _isValidName(
                                const QString & name,
                                int maxLength
                            );
        static bool     _isValidDescription(
                                const QString & description,
                                int maxLength
                            );
        static bool     _isValidDateTime(
                                const QDateTime & dateTime
                            );

        //  Per-type validators
    private:
        class TT3_DB_API_PUBLIC _PrincipalValidator final
            :   public virtual IPrincipalValidator
        {
            TT3_DECLARE_SINGLETON(_PrincipalValidator)

            //////////
            //  IPrincipalValidator
        public:
            virtual bool    isValidEmailAddress(
                                    const QString & emailAddress
                                ) override;
        };

        class TT3_DB_API_PUBLIC _UserValidator final
            :   public virtual IUserValidator
        {
            TT3_DECLARE_SINGLETON(_UserValidator)

            //////////
            //  IPrincipalValidator
        public:
            virtual bool    isValidEmailAddress(
                                    const QString & emailAddress
                                ) override;

            //////////
            //  IUserValidator
        public:
            virtual bool    isValidRealName(
                                    const QString & realName
                                ) override;
            virtual bool    isValidInactivityTimeout(
                                    const InactivityTimeout & inactivityTimeout
                                ) override;
            virtual bool    isValidUiLocale(
                                    const UiLocale & uiLocale
                                ) override;
        };

        class TT3_DB_API_PUBLIC _AccountValidator final
            :   public virtual IAccountValidator
        {
            TT3_DECLARE_SINGLETON(_AccountValidator)

            //////////
            //  IPrincipalValidator
        public:
            virtual bool    isValidEmailAddress(
                                    const QString & emailAddress
                                ) override;

            //////////
            //  IAccountValidator
        public:
            virtual bool    isValidLogin(
                                    const QString & login
                                ) override;
            virtual bool    isValidPassword(
                                    const QString & password
                                ) override;
        };

        class TT3_DB_API_PUBLIC _ActivityTypeValidator final
            :   public virtual IActivityTypeValidator
        {
            TT3_DECLARE_SINGLETON(_ActivityTypeValidator)

            //////////
            //  IActivityTypeValidator
        public:
            virtual bool    isValidDisplayName(
                                    const QString & displayName
                                ) override;
            virtual bool    isValidDescription(
                                    const QString & description
                                ) override;
        };

        class TT3_DB_API_PUBLIC _ActivityValidator final
            :   public virtual IActivityValidator,
                public virtual IPublicActivityValidator,
                public virtual IPrivateActivityValidator,
                public virtual ITaskValidator,
                public virtual IPublicTaskValidator,
                public virtual IPrivateTaskValidator
        {
            TT3_DECLARE_SINGLETON(_ActivityValidator)

            //////////
            //  IActivityValidator
        public:
            virtual bool    isValidDisplayName(
                                    const QString & displayName
                                ) override;
            virtual bool    isValidDescription(
                                    const QString & description
                                ) override;
            virtual bool    isValidTimeout(
                                    const InactivityTimeout & timeout
                                ) override;
        };

        class TT3_DB_API_PUBLIC _WorkloadValidator final
            :   public virtual IWorkloadValidator,
                public virtual IProjectValidator,
                public virtual IWorkStreamValidator
        {
            TT3_DECLARE_SINGLETON(_WorkloadValidator)

            //////////
            //  IWorkloadValidator
        public:
            virtual bool    isValidDisplayName(
                                    const QString & displayName
                                ) override;
            virtual bool    isValidDescription(
                                    const QString & description
                                ) override;
        };

        class TT3_DB_API_PUBLIC _BeneficiaryValidator final
            :   public virtual IBeneficiaryValidator
        {
            TT3_DECLARE_SINGLETON(_BeneficiaryValidator)

            //////////
            //  IBeneficiaryValidator
        public:
            virtual bool    isValidDisplayName(
                                    const QString & displayName
                                ) override;
            virtual bool    isValidDescription(
                                    const QString & description
                                ) override;
        };

        class TT3_DB_API_PUBLIC _WorkValidator final
            :   public virtual IWorkValidator
        {
            TT3_DECLARE_SINGLETON(_WorkValidator)

            //////////
            //  IWorkValidator
        public:
            virtual bool    isValidStartedFinishedAt(
                                    const QDateTime & startedAt,
                                    const QDateTime & finishedAt
                                ) override;
        };

        class TT3_DB_API_PUBLIC _EventValidator final
            :   public virtual IEventValidator
        {
            TT3_DECLARE_SINGLETON(_EventValidator)

            //////////
            //  IEventValidator
        public:
            virtual bool    isValidOccurredAt(
                                    const QDateTime & occurredAt
                                ) override;
            virtual bool    isValidSummary(
                                    const QString & summary
                                ) override;
        };
    };
}

//  End of tt3-db-api/Validator.hpp
