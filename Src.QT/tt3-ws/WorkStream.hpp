//
//  tt3-ws/WorkStream.hpp - A work stream
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
/// \class WorkStreamImpl tt3-ws/API.hpp
/// \brief A work stream.
class TT3_WS_PUBLIC WorkStreamImpl final
    :   public WorkloadImpl
{
    TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(WorkStreamImpl)

    friend class WorkspaceImpl;
    friend std::shared_ptr<WorkStreamImpl>;

    //////////
    //  Construction/destruction - from friends only
private:
    WorkStreamImpl(
            Workspace workspace,
            tt3::db::api::IWorkStream * dataWorkStream
        );
    virtual ~WorkStreamImpl();

    //////////
    //  Implementation
private:
    tt3::db::api::IWorkStream *const    _dataWorkStream;  //  counts as "reference"

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
Q_DECLARE_METATYPE(tt3::ws::WorkStreamImpl)
Q_DECLARE_METATYPE(tt3::ws::WorkStream)

//  End of tt3-ws/WorkStream.hpp
