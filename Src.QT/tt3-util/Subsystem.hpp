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
    /// \class ISubsystem tt3-util/API.hpp
    /// \brief
    ///     A generic "subsystem" is a group of components that
    ///     share the same purpose or area of functionality.
    class TT3_UTIL_PUBLIC ISubsystem
    {
        //////////
        //  This is an interface
    protected:
        /// \brief
        ///     The empty [interface] constructor.
        ISubsystem() = default;

        /// \brief
        ///     The empty [interface] destructor.
        virtual ~ISubsystem() = default;

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the mnemonic identifier of this subsystem,
        /// \return
        ///     The mnemonic identifier of this subsystem,
        virtual Mnemonic    mnemonic() const = 0;

        /// \brief
        ///     Returns the user-readable display name of this subsystem.
        /// \return
        ///     The user-readable display name of this subsystem
        ///     for the current default locale.
        virtual QString     displayName() const = 0;

        /// \brief
        ///     Returns the short (1 line) description of this subsystem.
        /// \return
        ///     The short (1 line) description of this subsystem
        ///     for the current default locale.
        virtual QString     description() const = 0;

        /// \brief
        ///     Returns the small (16x16) icon representing this subsystem.
        /// \return
        ///     The small (16x16) icon representing this subsystem.
        virtual QIcon       smallIcon() const;

        /// \brief
        ///     Returns the large (32x32) icon representing this subsystem.
        /// \return
        ///     The large (32x32) icon representing this subsystem.
        virtual QIcon       largeIcon() const;

        /// \brief
        ///     Returns the set of all registered components that
        ///     belong to this subsystem.
        /// \return
        ///     The set of all registered components that belong
        ///     to this subsystem.
        virtual Components  components() const;
    };

    /// \class StandardSubsystems tt3-util/API.hpp
    /// \brief The standard (predefined) subsystems.
    class TT3_UTIL_PUBLIC StandardSubsystems final
    {
        TT3_UTILITY_CLASS(StandardSubsystems)

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the set of all standard Subsystems.
        /// \return
        ///     The set of all standard Subsystems.
        static Subsystems   all();

        //////////
        //  Members
    public:
        /// \class Applications tt3-util/API.hpp
        /// \brief The "Applications" subsystems (EXE files).
        class TT3_UTIL_PUBLIC Applications final
            :   public virtual ISubsystem
        {
            TT3_DECLARE_SINGLETON(Applications);

            //////////
            //  ISubsystem
        public:
            virtual Mnemonic    mnemonic() const override;
            virtual QString     displayName() const override;
            virtual QString     description() const override;
        };

        /// \class Storage tt3-util/API.hpp
        /// \brief The "Storage" subsystems (data storage).
        class TT3_UTIL_PUBLIC Storage final
            :   public virtual ISubsystem
        {
            TT3_DECLARE_SINGLETON(Storage);

            //////////
            //  ISubsystem
        public:
            virtual Mnemonic    mnemonic() const override;
            virtual QString     displayName() const override;
            virtual QString     description() const override;
        };

        /// \class Gui tt3-util/API.hpp
        /// \brief The "GUI" subsystems (user interface).
        class TT3_UTIL_PUBLIC Gui final
            :   public virtual ISubsystem
        {
            TT3_DECLARE_SINGLETON(Gui);

            //////////
            //  ISubsystem
        public:
            virtual Mnemonic    mnemonic() const override;
            virtual QString     displayName() const override;
            virtual QString     description() const override;
        };

        /// \class Reporting tt3-util/API.hpp
        /// \brief The "Reporting" subsystems (quick/full reports).
        class TT3_UTIL_PUBLIC Reporting final
            :   public virtual ISubsystem
        {
            TT3_DECLARE_SINGLETON(Reporting);

            //////////
            //  ISubsystem
        public:
            virtual Mnemonic    mnemonic() const override;
            virtual QString     displayName() const override;
            virtual QString     description() const override;
        };

        /// \class Utility tt3-util/API.hpp
        /// \brief The "Utility" subsystems (support libraries).
        class TT3_UTIL_PUBLIC Utility final
            :   public virtual ISubsystem
        {
            TT3_DECLARE_SINGLETON(Utility);

            //////////
            //  ISubsystem
        public:
            virtual Mnemonic    mnemonic() const override;
            virtual QString     displayName() const override;
            virtual QString     description() const override;
        };
    };

    /// \class SubsystemManager tt3-util/API.hpp
    /// \brief The manager of known Subsystems.
    class TT3_UTIL_PUBLIC SubsystemManager final
    {
        TT3_UTILITY_CLASS(SubsystemManager)

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the set of all registered subsystems.
        /// \return
        ///     The set of all registered subsystems.
        static Subsystems   allSubsystems();

        /// \brief
        ///     Registers the specified Subsystem.
        /// \param subsystem
        ///     The subsystem to register.
        /// \return
        ///     True on success, false on failure. Repeated
        ///     registration of the same subsystem is treated
        ///     as a success.
        static bool         registerSubsystem(ISubsystem * subsystem);

        /// \brief
        ///     Finds a registered Subsystem by its mnemonic.
        /// \param mnemonic
        ///     The mnemonic to lok for.
        /// \return
        ///     The registered subsystem with the required mnemonic;
        ///     nullptr if not found.
        static ISubsystem * findSubsystem(const Mnemonic & mnemonic);

        //////////
        //  Implementation
    private:
        struct _Impl;

        //  Helpers
        static _Impl *  _impl();
    };
}

//  End of tt3-util/Subsystem.hpp
