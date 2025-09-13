//
//  tt3-db-api/ObjectType.hpp - a database object type
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
    /// \class IObjectType tt3-db-api/API.hpp
    /// \brief
    ///     Represents a type of a database object.
    /// \details
    ///     All predefined database object types are
    ///     singletons defined in ObjectTypes.
    class TT3_DB_API_PUBLIC IObjectType
    {
        //////////
        //  This is an interface
    protected:
        IObjectType() = default;
        virtual ~IObjectType() = default;

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the mnemonic identifier of this database object type.
        /// \return
        ///     The mnemonic identifier of this database object type.
        virtual auto    mnemonic() const
                            -> tt3::util::Mnemonic = 0;

        /// \brief
        ///     Returns the user-readable display name of this database
        ///     object type for the current default locale.
        /// \return
        ///     The user-readable display name of this database object type.
        virtual QString displayName() const = 0;

        /// \brief
        ///     Returns the small (16x16) icon representing this database object type.
        /// \return
        ///     The small (16x16) icon representing this database object type.
        virtual QIcon   smallIcon() const = 0;

        /// \brief
        ///     Returns the large (32x32) icon representing this database object type.
        /// \return
        ///     The large (32x32) icon representing this database object type.
        virtual QIcon   largeIcon() const = 0;
    };

    /// \class ObjectTypes tt3-db-api/API.hpp
    /// \brief Standard database object types.
    class TT3_DB_API_PUBLIC ObjectTypes final
    {
        UTILITY_CLASS(ObjectTypes)

        //////////
        //  Database object types
    public:
        //  The type of the "User" objects.
        class TT3_DB_API_PUBLIC User final : public virtual IObjectType
        {
            DECLARE_SINGLETON(User)

            //////////
            //  IObjectType
        public:
            virtual auto    mnemonic() const
                                -> tt3::util::Mnemonic override;
            virtual QString displayName() const override;
            virtual QIcon   smallIcon() const override;
            virtual QIcon   largeIcon() const override;
        };

        /// \class Account tt3-db-api/API.hpp
        /// \brief The type of the "Account" objects.
        class TT3_DB_API_PUBLIC Account final : public virtual IObjectType
        {
            DECLARE_SINGLETON(Account)

            //////////
            //  IObjectType
        public:
            virtual auto    mnemonic() const
                                -> tt3::util::Mnemonic override;
            virtual QString displayName() const override;
            virtual QIcon   smallIcon() const override;
            virtual QIcon   largeIcon() const override;
        };

        /// \class ActivityType tt3-db-api/API.hpp
        /// \brief The type of the "ActivityType" objects.
        class TT3_DB_API_PUBLIC ActivityType final : public virtual IObjectType
        {
            DECLARE_SINGLETON(ActivityType)

            //////////
            //  IObjectType
        public:
            virtual auto    mnemonic() const
                                -> tt3::util::Mnemonic override;
            virtual QString displayName() const override;
            virtual QIcon   smallIcon() const override;
            virtual QIcon   largeIcon() const override;
        };

        //  The type of the "PublicActivity" objects.
        class TT3_DB_API_PUBLIC PublicActivity final : public virtual IObjectType
        {
            DECLARE_SINGLETON(PublicActivity)

            //////////
            //  IObjectType
        public:
            virtual auto    mnemonic() const
                                -> tt3::util::Mnemonic override;
            virtual QString displayName() const override;
            virtual QIcon   smallIcon() const override;
            virtual QIcon   largeIcon() const override;
        };

        //  The type of the "PublicTask" objects.
        class TT3_DB_API_PUBLIC PublicTask final : public virtual IObjectType
        {
            DECLARE_SINGLETON(PublicTask)

            //////////
            //  IObjectType
        public:
            virtual auto    mnemonic() const
                                -> tt3::util::Mnemonic override;
            virtual QString displayName() const override;
            virtual QIcon   smallIcon() const override;
            virtual QIcon   largeIcon() const override;
        };

        //  The type of the "PrivateActivity" objects.
        class TT3_DB_API_PUBLIC PrivateActivity final : public virtual IObjectType
        {
            DECLARE_SINGLETON(PrivateActivity)

            //////////
            //  IObjectType
        public:
            virtual auto    mnemonic() const
                                -> tt3::util::Mnemonic override;
            virtual QString displayName() const override;
            virtual QIcon   smallIcon() const override;
            virtual QIcon   largeIcon() const override;
        };

        //  The type of the "PrivateTask" objects.
        class TT3_DB_API_PUBLIC PrivateTask final : public virtual IObjectType
        {
            DECLARE_SINGLETON(PrivateTask)

            //////////
            //  IObjectType
        public:
            virtual auto    mnemonic() const
                                -> tt3::util::Mnemonic override;
            virtual QString displayName() const override;
            virtual QIcon   smallIcon() const override;
            virtual QIcon   largeIcon() const override;
        };

        //  The type of the "Work" objects.
        class TT3_DB_API_PUBLIC Work final : public virtual IObjectType
        {
            DECLARE_SINGLETON(Work)

            //////////
            //  IObjectType
        public:
            virtual auto    mnemonic() const
                                -> tt3::util::Mnemonic override;
            virtual QString displayName() const override;
            virtual QIcon   smallIcon() const override;
            virtual QIcon   largeIcon() const override;
        };

        //  The type of the "Event" objects.
        class TT3_DB_API_PUBLIC Event final : public virtual IObjectType
        {
            DECLARE_SINGLETON(Event)

            //////////
            //  IObjectType
        public:
            virtual auto    mnemonic() const
                                -> tt3::util::Mnemonic override;
            virtual QString displayName() const override;
            virtual QIcon   smallIcon() const override;
            virtual QIcon   largeIcon() const override;
        };

        //  The type of the "Project" objects.
        class TT3_DB_API_PUBLIC Project final : public virtual IObjectType
        {
            DECLARE_SINGLETON(Project)

            //////////
            //  IObjectType
        public:
            virtual auto    mnemonic() const
                                -> tt3::util::Mnemonic override;
            virtual QString displayName() const override;
            virtual QIcon   smallIcon() const override;
            virtual QIcon   largeIcon() const override;
        };

        //  The type of the "WorkStream" objects.
        class TT3_DB_API_PUBLIC WorkStream final :
        public virtual IObjectType
        {
            DECLARE_SINGLETON(WorkStream)

            //////////
            //  IObjectType
        public:
            virtual auto    mnemonic() const
                                -> tt3::util::Mnemonic override;
            virtual QString displayName() const override;
            virtual QIcon   smallIcon() const override;
            virtual QIcon   largeIcon() const override;
        };

        //  The type of the "Beneficiary" objects.
        class TT3_DB_API_PUBLIC Beneficiary final : public virtual IObjectType
        {
            DECLARE_SINGLETON(Beneficiary)

            //////////
            //  IObjectType
        public:
            virtual auto    mnemonic() const
                                -> tt3::util::Mnemonic override;
            virtual QString displayName() const override;
            virtual QIcon   smallIcon() const override;
            virtual QIcon   largeIcon() const override;
        };
    };
}

//  End of tt3-db-api/ObjectType.hpp
