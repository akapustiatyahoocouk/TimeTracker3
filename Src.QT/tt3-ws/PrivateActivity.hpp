//
//  tt3-ws/PrivateActivity.hpp - A private activity
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
    /// \class PrivateActivityImpl tt3-ws/API.hpp
    /// \brief A private activity.
    class TT3_WS_PUBLIC PrivateActivityImpl
        :   public virtual ActivityImpl
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(PrivateActivityImpl)

        friend class WorkspaceImpl;
        friend std::shared_ptr<PrivateActivityImpl>;
        friend class PrivateTaskImpl;

        //////////
        //  Construction/destruction - from friends only
    private:
        PrivateActivityImpl(
                Workspace workspace,
                tt3::db::api::IPrivateActivity * dataPrivateActivity
            );
        virtual ~PrivateActivityImpl();

        //////////
        //  Operations (associations)
    public:
        /// \brief
        ///     Returns the user to whom this private activity belongs.
        /// \param credentials
        ///     The credentials of the service caller.
        /// \return
        ///     The user to whom this private activity belongs.
        /// \exception WorkspaceException
        ///     If an error occurs.
        auto        owner(
                            const Credentials & credentials
                        ) const -> User;

        //////////
        //  Implementation
    private:
        tt3::db::api::IPrivateActivity *const   _dataPrivateActivity;  //  counts as "reference"

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
Q_DECLARE_METATYPE(tt3::ws::PrivateActivityImpl)
Q_DECLARE_METATYPE(tt3::ws::PrivateActivity)

//  End of tt3-ws/PrivateActivity.hpp
