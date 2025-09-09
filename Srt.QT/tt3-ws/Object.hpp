//
//  tt3-ws/Object.hpp - A generic workspace object
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
    /// A generic workspace object.
    class TT3_WS_PUBLIC ObjectImpl
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ObjectImpl)

        friend class WorkspaceImpl;
        friend class PrincipalImpl;
        friend class UserImpl;
        friend class AccountImpl;
        friend class ActivityTypeImpl;

        //////////
        //  Construction/destruction - from frinds only
    private:
        ObjectImpl(Workspace workspace, tt3::db::api::IObject * dataObject);
        virtual ~ObjectImpl();

        //////////
        //  Operations (general)
    public:
        /// Returns the type of this workspace object.
        /// Can be safely obtained for both live and dead objects.
        ///
        /// \return
        ///     The type of this workspace object
        auto        type(
                        ) const -> ObjectType *;

        /// Returns the workspace where the corresponding object
        /// resides (if live) or used to reside (if dead).
        ///
        /// \return
        ///     The workspace where this object resides or used to reside.
        auto        workspace(
                        ) const -> Workspace;

        /// Returns the OID of the corresponding object.
        /// Can be safely obtained for both live and dead objects.
        ///
        /// \return
        ///     The OID of the corresponding object
        Oid         oid() const;

        /// True if this instance represents an existing data
        /// object residing in a database, else false.
        bool        isLive() const;

        //////////
        //  Operations (life cycle)
    public:
        /// \brief
        ///     Destroys this object, delete-cascading
        ///     as necessary.
        /// \details
        ///     The instance (and all other delete-cascaded
        ///     instances) is marked as "reoresenting a dead
        ///     object" and, therefore, unusable.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \exception WorkspaceException
        ///     If an error occurs.
        void        destroy(
                            const Credentials & credentials
                        );

        //////////
        //  Operations (access control)
    public:
        /// \brief
        ///     Checks whether the specifid credentials allow the
        ///     "read" operations to be performed on this object.
        /// \details
        ///     These are operations that examine the object's state.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \return
        ///     True the specifid credentials allow the "read"
        ///     operations to be performed on this object, else false.
        /// \exception WorkspaceException
        ///     If an error occurs.
        bool        canRead(
                            const Credentials & credentials
                        ) const;

        /// \brief
        ///     Checks whether the specifid credentials allow the
        ///     "modify" operations to be performed on this object.
        /// \details
        ///     These are operations that modify the object's state.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \return
        ///     True the specifid credentials allow the "modify"
        ///     operations to be performed on this object, else false.
        /// \exception WorkspaceException
        ///     If an error occurs.
        bool        canModify(
                            const Credentials & credentials
                        ) const;

        /// \brief
        ///     Checks whether the specifid credentials allow the
        ///     "destroy" operations to be performed on this object.
        /// \details
        ///     These are operations that destroy the object.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \return
        ///     True the specifid credentials allow the "destroy"
        ///     operations to be performed on this object, else false.
        /// \exception WorkspaceException
        ///     If an error occurs.
        bool        canDestroy(
                            const Credentials & credentials
                        ) const;

        //////////
        //  Implementation
    private:
        const Workspace _workspace;
        tt3::db::api::IObject *const    _dataObject;    //  counts as "reference"

        //  Helpers
        void            _ensureLive(    //  throws WorkspaceException
                            ) const;

        //  Access control
        virtual bool    _canRead(       //  throws WorkspaceException
                                const Credentials & credentials
                            ) const = 0;
        virtual bool    _canModify(     //  throws WorkspaceException
                                const Credentials & credentials
                            ) const = 0;
        virtual bool    _canDestroy(    //  throws WorkspaceException
                                const Credentials & credentials
                            ) const = 0;
        virtual bool    _destroyingLosesAccess(
                            ) const { return false; }
    };
}

//  Enable objects and object pointers for QVariant
Q_DECLARE_METATYPE(tt3::ws::ObjectImpl)
Q_DECLARE_METATYPE(tt3::ws::Object)

//  End of tt3-ws/Object.hpp
