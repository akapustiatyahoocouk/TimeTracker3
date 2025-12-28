//
//  tt3-tools-restore/Exceptions.hpp - tt3 Restore Exceptions
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

namespace tt3::tools::restore
{
    /// \class BackupFileCorruptException tt3-tools-restore/API.hpp
    /// \brief Thrown when a backup file is corrupt.
    class TT3_TOOLS_RESTORE_PUBLIC BackupFileCorruptException final
        :   public virtual tt3::util::Exception
    {
        //////////
        //  Types
    public:
        /// \brief A type alias to improve code readability.
        using Self = BackupFileCorruptException;

        //////////
        //  Construction/destruction/assignment
    public:
        /// \brief
        ///     Constructs the exception.
        /// \param fileName
        ///     The name of the corrupt backup file.
        explicit BackupFileCorruptException(
                const QString & fileName
            );

        //////////
        //  QException
    public:
        virtual Self *  clone() const override { return new Self(*this); }
        virtual void    raise() const override { throw *this; }

        //////////
        //  tt3::util::Exception
    public:
        virtual QString errorMessage() const override;

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the name of the corrupt backup file.
        /// \return
        ///     The name of the corrupt backup file.
        QString         fileName() const { return _fileName; }

        //////////
        //  Implementayion
    private:
        QString         _fileName;
    };
}

//  End of tt3-tools-restore/Exceptions.hpp
