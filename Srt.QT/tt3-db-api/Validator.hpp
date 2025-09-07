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
    //  An agent that, for a givel database [typr]
    //  can decide whether a specific value is valid
    //  for a s[ecific property of a specific object
    class TT3_DB_API_PUBLIC IValidator
    {
        //////////
        //  This is an interface
    protected:
        IValidator() = default;
        virtual ~IValidator() = default;

        //////////
        //  Operations
    public:
        //  The validator for a Principal
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
            //  Checks whether the specified string is a valid e-0maiol address.
            virtual bool    isValidEmailAddress(
                                    const QString & emailAddress
                                ) = 0;

            //  Checks whether the specified list of strings is a valid e-0maiol addresses list.
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
        virtual auto    principal(
                            ) const
                            -> IPrincipalValidator * = 0;

        //  The validator for a User
        class TT3_DB_API_PUBLIC IUserValidator : public virtual IPrincipalValidator
        {
            //////////
            //  This is an interface
        protected:
            IUserValidator() = default;
            virtual ~IUserValidator() = default;

            //////////
            //  Operations
        public:
            //  Checks whether the specified string is a valid "real name" for a User.
            virtual bool    isValidRealName(
                                    const QString & realName
                                ) = 0;

            //  Checks whether the specified value is a valid "inactivity timeout" for a User.
            virtual bool    isValidInactivityTimeout(
                                    const InactivityTimeout & inactivityTimeout
                                ) = 0;

            //  Checks whether the specified value is a valid "UI locale" for a User.
            virtual bool    isValidUiLocale(
                                    const UiLocale & uiLocale
                                ) = 0;
        };
        virtual auto    user(
                            ) const
                            -> IUserValidator * = 0;

        //  The validator for an Account
        class TT3_DB_API_PUBLIC IAccountValidator : public virtual IPrincipalValidator
        {
            //////////
            //  This is an interface
        protected:
            IAccountValidator() = default;
            virtual ~IAccountValidator() = default;

            //////////
            //  Operations
        public:
            //  Checks whether the specified string is a valid "login" for an Account.
            virtual bool    isValidLogin(
                                    const QString & login
                                ) = 0;

            //  Checks whether the specified string is a valid "password" for an Account.
            virtual bool    isValidPassword(
                                    const QString & password
                                ) = 0;
        };
        virtual auto    account(
                            ) const
                            -> IAccountValidator * = 0;

        //  The validator for an ActivityType
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
            //  Checks whether the specified string is a valid
            //  "display name" for an ActivityType.
            virtual bool    isValidDisplayName(
                                    const QString & login
                                ) = 0;

            //  Checks whether the specified string is a valid
            //  "description" for an ActivityType.
            virtual bool    isValidDescription(
                                    const QString & description
                                ) = 0;
        };
        virtual auto    activityType(
                            ) const
                            -> IActivityTypeValidator * = 0;
    };

    //  The "default" validator, normally suitable
    //  for all database [types]
    class TT3_DB_API_PUBLIC DefaultValidator final :
        public virtual IValidator
    {
        DECLARE_SINGLETON(DefaultValidator)

        //////////
        //  IValidator
    public:
        virtual auto    principal(
                            ) const
                        -> IPrincipalValidator * override;
        virtual auto    user(
                            ) const
                            -> IUserValidator * override;
        virtual auto    account(
                            ) const
                            -> IAccountValidator * override;
        virtual auto    activityType(
                            ) const
                            -> IActivityTypeValidator * override;

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

        class TT3_DB_API_PUBLIC _AccountValidator final : public virtual IAccountValidator
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
                                    const QString & login
                                ) override;
            virtual bool    isValidDescription(
                                    const QString & description
                                ) override;
        };
    };
}

//  End of tt3-db-api/Validator.hpp
