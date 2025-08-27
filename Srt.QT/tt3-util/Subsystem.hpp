//
//  tt3-util/Subsystem.hpp - tt3 Subsystems framework
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

namespace tt3::util
{
    //  A generic "subsystem" is a group of components that
    //  share the same purpose or area of functionality
    class TT3_UTIL_PUBLIC ISubsystem
    {
        //////////
        //  This is an interface
    protected:
        ISubsystem() = default;
        virtual ~ISubsystem() = default;

        //////////
        //  Operations
    public:
        //  The mnemonic identifier of this subsystem
        virtual QString     mnemonic() const = 0;

        //  The user-readable display name of this subsystem
        //  for the current default locale.
        virtual QString     displayName() const = 0;

        //  The short (1 line) description of this subsystem
        //  for the current default locale.
        virtual QString     description() const = 0;

        //  The small (16x16) icon representing this subsystem.
        virtual QIcon       smallIcon() const;

        //  The large (32x32) icon representing this subsystem.
        virtual QIcon       largeIcon() const;
    };

    //  The standard (predefined) subsystems
    class TT3_UTIL_PUBLIC StandardSubsystems final
    {
        UTILITY_CLASS(StandardSubsystems)

        //////////
        //  Members
    public:
        //  The "Applications" subsystems (i.e. .EXE files)
        class TT3_UTIL_PUBLIC Applications final : public virtual ISubsystem
        {
            DECLARE_SINGLETON(Applications);

            //////////
            //  ISubsystem
        public:
            virtual QString     mnemonic() const override;
            virtual QString     displayName() const override;
            virtual QString     description() const override;
        };

        //  The "Storage" subsystems (data storage)
        class TT3_UTIL_PUBLIC Storage final : public virtual ISubsystem
        {
            DECLARE_SINGLETON(Storage);

            //////////
            //  ISubsystem
        public:
            virtual QString     mnemonic() const override;
            virtual QString     displayName() const override;
            virtual QString     description() const override;
        };

        //  The "GUI" subsystems (user interface)
        class TT3_UTIL_PUBLIC Gui final : public virtual ISubsystem
        {
            DECLARE_SINGLETON(Gui);

            //////////
            //  ISubsystem
        public:
            virtual QString     mnemonic() const override;
            virtual QString     displayName() const override;
            virtual QString     description() const override;
        };

        //  The "Reporting" subsystems (quick/full reports)
        class TT3_UTIL_PUBLIC Reporting final : public virtual ISubsystem
        {
            DECLARE_SINGLETON(Reporting);

            //////////
            //  ISubsystem
        public:
            virtual QString     mnemonic() const override;
            virtual QString     displayName() const override;
            virtual QString     description() const override;
        };

        //  The "Utility" subsystems (e.g. support libraries)
        class TT3_UTIL_PUBLIC Utility final : public virtual ISubsystem
        {
            DECLARE_SINGLETON(Utility);

            //////////
            //  ISubsystem
        public:
            virtual QString     mnemonic() const override;
            virtual QString     displayName() const override;
            virtual QString     description() const override;
        };
    };

    //  The manager of known Subsystems
    class TT3_UTIL_PUBLIC SubsystemManager final
    {
        UTILITY_CLASS(SubsystemManager)

        //////////
        //  Operations
    public:
        //  Returns the set of all registered subsystems.
        static QSet<ISubsystem*>    allSubsystems();

        //  Registers the specified Subsystem; returns true
        //  on success, false on failure.
        static bool         registerSubsystem(ISubsystem * subsystem);

        //  Finds a registered Subsystem by its mnemonic;
        //  returns nullptr if not found.
        static ISubsystem * findSubsystem(const QString & mnemonic);

        //////////
        //  Implementation
    private:
        static tt3::util::Mutex _guard;
        static QMap<QString, ISubsystem*>   _registry;  //  key = mnemonic

        //  Helpers
        static void         _registerStandardSubsystems();
    };
}

//  End of tt3-util/Subsystem.hpp
