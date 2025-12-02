//
//  tt3-ws/ActivityType.hpp - An activity type
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
    /// \class ActivityTypeImpl tt3-ws/API.hpp
    /// \brief An activity type.
    class TT3_WS_PUBLIC ActivityTypeImpl final
        :   public ObjectImpl
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ActivityTypeImpl)

        friend class WorkspaceImpl;
        friend std::shared_ptr<ActivityTypeImpl>;
        friend class UserImpl;
        friend class ActivityImpl;
        friend class PublicTaskImpl;
        friend class PrivateTaskImpl;

        //////////
        //  Construction/destruction - from friends only
    private:
        ActivityTypeImpl(
                Workspace workspace,
                tt3::db::api::IActivityType * dataActivityType
            );
        virtual ~ActivityTypeImpl();

        //////////
        //  Operations (properties)
    public:
        /// \brief
        ///     Returns the user-readable display name of this activity type.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \return
        ///     The user-readable display name of this activity type.
        /// \exception WorkspaceException
        ///     If an error occurs.
        QString     displayName(
                            const Credentials & credentials
                        ) const;

        /// \brief
        ///     Sets the user-readable display name of this activity type.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \param displayName
        ///     The new user-readable display name for this activity type.
        /// \exception WorkspaceException
        ///     If an error occurs.
        void        setDisplayName(
                            const Credentials & credentials,
                            const QString & displayName
                        );


        /// \brief
        ///     Returns the multi-line description of this activity type.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \return
        ///     The multi-line description of this activity
        ///     type, with lines separated by a newline character '\n'.
        /// \exception WorkspaceException
        ///     If an error occurs.
        QString     description(
                            const Credentials & credentials
                        ) const;

        /// \brief
        ///     Sets the multi-line description of this activity type.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \param description
        ///     The new multi-line description for this activity
        ///     type, with lines separated by a newline character '\n'.
        /// \exception WorkspaceException
        ///     If an error occurs.
        void        setDescription(
                            const Credentials & credentials,
                            const QString & description
                        );

        //////////
        //  Operations (associations)
    public:
        /// \brief
        ///     Returns the set of all activities assigned to this activity type.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \return
        ///     The set of all activities assigned to this activity type.
        /// \exception WorkspaceException
        ///     If an error occurs.
        auto        activities(
                            const Credentials & credentials
                        ) const -> Activities;

        //////////
        //  Implementation
    private:
        tt3::db::api::IActivityType *const  _dataActivityType;    //  counts as "reference"

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
Q_DECLARE_METATYPE(tt3::ws::ActivityTypeImpl)
Q_DECLARE_METATYPE(tt3::ws::ActivityType)

//  End of tt3-ws/ActivityType.hpp
