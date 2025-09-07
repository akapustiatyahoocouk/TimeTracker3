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
    //  An activity type
    class TT3_WS_PUBLIC ActivityTypeImpl final :
        public ObjectImpl
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ActivityTypeImpl)

        friend class WorkspaceImpl;
        friend std::shared_ptr<ActivityTypeImpl>;

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
        //  Returns/sets the user-readable display name of
        //  this activity type.
        //  Throws WorkspaceException if an error occurs.
        auto        displayName(
                            const Credentials & credentials
                        ) const
                        throws(WorkspaceException)
                        -> QString;
        void        setDisplayName(
                            const Credentials & credentials,
                            const QString & displayName
                        )
                        throws(WorkspaceException);


        //  Returns/sets the multi-line description of this activity
        //  type, with lines separated by a newline character '\n'.
        //  Throws WorkspaceException if an error occurs.
        auto        description(
                            const Credentials & credentials
                        ) const
                        throws(WorkspaceException)
                        -> QString;
        void        setDescription(
                            const Credentials & credentials,
                            const QString & description
                        )
                        throws(WorkspaceException);

        //////////
        //  Operations (associations)
    public:
        //  The set of all activities assigned to this activity type.
        //  Throws WorkspaceException if an error occurs.
        auto        activities(
                            const Credentials & credentials
                        ) const
                        throws(WorkspaceException)
                        -> Activities;

        //////////
        //  Implementation
    private:
        tt3::db::api::IActivityType *const  _dataActivityType;    //  counts as "reference"

        //  Access control - throw WorkspaceException in DB error
        virtual bool    _canRead(
                                const Credentials & credentials
                            ) const override;
        virtual bool    _canModify(
                                const Credentials & credentials
                            ) const override;
        virtual bool    _canDestroy(
                                const Credentials & credentials
                            ) const override;
    };
}

//  Enable objects and object pointers for QVariant
Q_DECLARE_METATYPE(tt3::ws::ActivityTypeImpl)
Q_DECLARE_METATYPE(tt3::ws::ActivityType)

//  End of tt3-ws/ActivityType.hpp
