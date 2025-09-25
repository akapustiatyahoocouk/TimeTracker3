//
//  tt3-ws/PublicActivity.hpp - A public activity
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
    /// \class PublicActivityImpl tt3-ws/API.hpp
    /// \brief A public activity.
    class TT3_WS_PUBLIC PublicActivityImpl
        :   public virtual ActivityImpl
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(PublicActivityImpl)

        friend class WorkspaceImpl;
        friend std::shared_ptr<PublicActivityImpl>;
        friend class PublicTaskImpl;

        //////////
        //  Construction/destruction - from friends only
    private:
        PublicActivityImpl(
                Workspace workspace,
                tt3::db::api::IPublicActivity * dataPublicActivity
            );
        virtual ~PublicActivityImpl();

        //////////
        //  Implementation
    private:
        tt3::db::api::IPublicActivity *const    _dataPublicActivity;  //  counts as "reference"

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
Q_DECLARE_METATYPE(tt3::ws::PublicActivityImpl)
Q_DECLARE_METATYPE(tt3::ws::PublicActivity)

//  End of tt3-ws/PublicActivity.hpp
