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
                for (QString emailAddress : emailAddresses)
                {
                    if (!isValidEmailAddress(emailAddress))
                    {
                        return false;
                    }
                }
                return QSet<QString>(emailAddresses.begin(), emailAddresses.end()).size() == emailAddresses.size();
            }
        };
        /// \brief
        ///     Returns the validator for Principals.
        /// \return
        ///     The validator for Principals.
        virtual auto    principal(
                            ) const
                            -> IPrincipalValidator * = 0;

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
                            ) const
                            -> IUserValidator * = 0;

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
                            ) const
                            -> IAccountValidator * = 0;

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
                            ) const
                            -> IActivityTypeValidator * = 0;

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

            //////////
            //  Operations
        public:
        };
        /// \brief
        ///     Returns the validator for PublicActivities.
        /// \return
        ///     The validator for PublicActivities.
        virtual auto    publicActivity(
            ) const -> IPublicActivityValidator * = 0;
    };

    /// \class DefaultValidator tt3-db-api/API.hpp
    /// \brief
    ///     The "default" validator, normally suitable
    ///     for all database [types].
    class TT3_DB_API_PUBLIC DefaultValidator final
        :   public virtual IValidator
    {
        DECLARE_SINGLETON(DefaultValidator)

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

        //  Per-type validators
    private:
        class TT3_DB_API_PUBLIC _PrincipalValidator final :
            public virtual IPrincipalValidator
        {
            DECLARE_SINGLETON(_PrincipalValidator)

            //////////
            //  IPrincipalValidator
        public:
            virtual bool    isValidEmailAddress(
                                    const QString & emailAddress
                                ) override;
        };

        class TT3_DB_API_PUBLIC _UserValidator final :
            public virtual IUserValidator
        {
            DECLARE_SINGLETON(_UserValidator)

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
            DECLARE_SINGLETON(_AccountValidator)

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

        class TT3_DB_API_PUBLIC _ActivityTypeValidator final :
            public virtual IActivityTypeValidator
        {
            DECLARE_SINGLETON(_ActivityTypeValidator)

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
            :   public virtual IActivityValidator
        {
            DECLARE_SINGLETON(_ActivityValidator)

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

        class TT3_DB_API_PUBLIC _PublicActivityValidator final
            :   public virtual IPublicActivityValidator
        {
            DECLARE_SINGLETON(_PublicActivityValidator)

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
    };
}

//  End of tt3-db-api/Validator.hpp
