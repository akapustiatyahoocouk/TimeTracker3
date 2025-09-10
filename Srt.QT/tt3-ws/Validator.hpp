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
    //  An agent that, for a given workspace [type]
    //  can decide whether a specific value is valid
    //  for a specific property of a specific object
    class TT3_WS_PUBLIC Validator final
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Validator)

        friend class WorkspaceTypeImpl;

        //////////
        //  Construction/destruction
    private:
        explicit Validator(tt3::db::api::IValidator * databaseValidator)
            :   _databaseValidator(databaseValidator) { Q_ASSERT(_databaseValidator != nullptr); }
        ~Validator() = default;

        //////////
        //  Operations
    public:
        //  We can piggyback on some DB API types.
        using Principal = tt3::db::api::IValidator::IPrincipalValidator;
        using User = tt3::db::api::IValidator::IUserValidator;
        using Account = tt3::db::api::IValidator::IAccountValidator;
        using ActivityType = tt3::db::api::IValidator::IActivityTypeValidator;

        //  The validator for Principals.
        Principal * principal() const { return _databaseValidator->principal(); }

        //  The validator for Users.
        User *      user() const { return _databaseValidator->user(); }

        //  The validator for Accounts.
        Account *   account() const { return _databaseValidator->account(); }

        //  The validator for ActivityTypes.
        ActivityType *  activityType() const { return _databaseValidator->activityType(); }

        //////////
        //  Implementation
    private:
        tt3::db::api::IValidator *const _databaseValidator;
    };
}

//  End of tt3-ws/Validator.hpp
