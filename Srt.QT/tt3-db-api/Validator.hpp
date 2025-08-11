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
            //  TODO document
            virtual bool    isValidEmailAddress(const QString & emailAddress) = 0;
        };
        virtual IPrincipalValidator *   principal() const = 0;

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
            //  TODO document
            virtual bool    isValidRealName(const QString & realName) = 0;
            virtual bool    isValidInactivityTimeout(const tt3::util::TimeSpan & inactivityTimeout) = 0;
        };
        virtual IUserValidator *    user() const = 0;

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
            //  TODO document
            virtual bool    isValidLogin(const QString & login) = 0;
            virtual bool    isValidPassword(const QString & password) = 0;
        };
        virtual IAccountValidator * accountl() const = 0;
    };

    //  The "default" validator, normally suitable
    //  for all database [types]
    class TT3_DB_API_PUBLIC DefaultValidator final : public virtual IValidator
    {
        DECLARE_SINGLETON(DefaultValidator)

        //////////
        //  IValidator
    public:
        virtual IPrincipalValidator *   principal() const override;
        virtual IUserValidator *        user() const override;
        virtual IAccountValidator *     accountl() const override;

        //////////
        //  Implementatiob
    private:
        //  Helpers
        static bool     _isValidEmailAddress(const QString & emailAddress);
        static bool     _isValidName(const QString & name, int maxLength);

        //  Per-type validators
    private:
        class TT3_DB_API_PUBLIC _PrincipalValidator final : public virtual IPrincipalValidator
        {
            DECLARE_SINGLETON(_PrincipalValidator)

            //////////
            //  IPrincipalValidator
        public:
            virtual bool    isValidEmailAddress(const QString & emailAddress) override;
        };

        class TT3_DB_API_PUBLIC _UserValidator final : public virtual IUserValidator
        {
            DECLARE_SINGLETON(_UserValidator)

            //////////
            //  IPrincipalValidator
        public:
            virtual bool    isValidEmailAddress(const QString & emailAddress) override;

            //////////
            //  IUserValidator
        public:
            virtual bool    isValidRealName(const QString & realName) override;
            virtual bool    isValidInactivityTimeout(const tt3::util::TimeSpan & inactivityTimeout) override;
        };

        class TT3_DB_API_PUBLIC _AccountValidator final : public virtual IAccountValidator
        {
            DECLARE_SINGLETON(_AccountValidator)

            //////////
            //  IPrincipalValidator
        public:
            virtual bool    isValidEmailAddress(const QString & emailAddress) override;

            //////////
            //  IAccountValidator
        public:
            virtual bool    isValidLogin(const QString & login) override;
            virtual bool    isValidPassword(const QString & password) override;
        };
    };
}

//  End of tt3-db-api/Validator.hpp
