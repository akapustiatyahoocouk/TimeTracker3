//
//  tt3-ws/WorkspaceAddressImpl.hpp - "Workspace address" ADT support
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
    /// \class WorkspaceAddressImpl tt3-ws/API.hpp
    /// \brief
    ///     A workspace address uniquely specifies the
    ///     location of a workspace of a certain type.
    /// \details
    ///     In addition, a WorkspaceAddress instance can
    ///     be "invalid", guaranteed not to refer to
    ///     any existing workspace.
    class TT3_WS_PUBLIC WorkspaceAddressImpl final
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(WorkspaceAddressImpl)

        friend class WorkspaceTypeImpl;
        friend std::shared_ptr<WorkspaceAddressImpl>;
        friend class WorkspaceImpl;

        //////////
        //  Construction/destruction
    private:
        WorkspaceAddressImpl(WorkspaceType workspaceType, tt3::db::api::IDatabaseAddress * databaseAddress);
        ~WorkspaceAddressImpl();

        //////////
        //  Operations (general)
    public:
        /// \brief
        ///     Returns the workspace type to which this workspace
        ///     address belongs.
        /// \return
        ///     The workspace type to which this workspace address
        ///     belongs or nullptr is this workspace address is invalid.
        WorkspaceType   workspaceType() const;

        /// \brief
        ///     Returns the user-readable form of this workspace address.
        /// \return
        ///     The user-readable form of this workspace address.
        QString         displayForm() const;

        /// \brief
        ///     Returns the external (re-parsable) form of this
        ///     workspace address.
        /// \return
        ///     The external (re-parsable) form of this workspace   address.
        QString         externalForm() const;

        //////////
        //  Implementation
    private:
        const WorkspaceType _workspaceType; //  never nullptr
        tt3::db::api::IDatabaseAddress *const   _databaseAddress;   //  counts as "reference", never nullptr
    };
}

//  Formatting/parsing
namespace tt3::util
{
    template <> TT3_WS_PUBLIC
    QString toString<tt3::ws::WorkspaceAddress>(
            const tt3::ws::WorkspaceAddress & value
        );

    template <> TT3_WS_PUBLIC
    QString toString<tt3::ws::WorkspaceAddressesList>(
            const tt3::ws::WorkspaceAddressesList & value
        );

    template <> TT3_WS_PUBLIC
    auto fromString<tt3::ws::WorkspaceAddress>(
            const QString & s,
            qsizetype & scan
        ) -> tt3::ws::WorkspaceAddress;

    template <> TT3_WS_PUBLIC
    auto fromString<tt3::ws::WorkspaceAddressesList>(
            const QString & s,
            qsizetype & scan
        ) -> tt3::ws::WorkspaceAddressesList;
}

//  End of tt3-ws/WorkspaceAddressImpl.hpp
