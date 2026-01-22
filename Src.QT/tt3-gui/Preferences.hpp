//
//  tt3-gui/Preferences.hpp - Preferences framework
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

namespace tt3::gui
{
    class PreferencesEditor;

    /// \class Preferences tt3-gui/API.hpp
    /// \brief
    ///     The generic "preferences" - a node in the
    ///     hierarchy of editable preferences.
    /// \details
    ///     Normally the actual preferences in a given
    ///     Preferences node will be acked uo via Settings
    ///     of some components; Preferences just organize
    ///     these settings into a hierarchy of editable
    ///     setting groups.
    class TT3_GUI_PUBLIC Preferences
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Preferences)

        //////////
        //  Construction/destruction
    protected:
        /// \brief
        ///     Constructs a Preferences node.
        Preferences() = default;

        /// \brief
        ///     The class destructor.
        virtual ~Preferences() = default;

        //////////
        //  Operations
    public:
        /// \brief A type alias to improve code readability.
        using Mnemonic = tt3::util::Mnemonic;

        /// \brief
        ///     Returns the mnemonic identifier of this preferences node.
        /// \return
        ///     The mnemonic identifier of this preferences node.
        virtual Mnemonic mnemonic() const = 0;

        /// \brief
        ///     Returns the user-readable display name of this
        ///     preferences node.
        /// \return
        ///     The user-readable display name of this preferences.
        ///     node for the current default locale.
        virtual QString displayName() const = 0;

        /// \brief
        ///     Returns the immediate parent node of this Preferences node.
        /// \return
        ///     The immediate parent node of this Preferences node
        ///     or null if this is one of the root nodes.
        virtual auto    parent() const -> Preferences * = 0;

        /// \brief
        ///     Returns the set of immediate child nodes of this
        ///     Preferences node.
        /// \return
        ///     The set of immediate child nodes of this
        ///     Preferences node.
        virtual auto    children() const -> QSet<Preferences*>;

        /// \brief
        ///     The relative order of this Preferences node
        ///     within its parent.
        /// \details
        ///     Lower-order nodes come before higher-order nodes;
        ///     children with the same order are sorted by display name).
        /// \return
        ///     The relative order of this Preferences node
        ///     within its parent.
        virtual int     order() const { return INT_MAX; }

        /// \brief
        ///     Creates a new editor widget for this preferences node.
        /// \return
        ///     Creates a new editor widget for this preferences node,
        ///     or nullptr if this node is not editable. The returned
        ///     editor widget will be re-parented as necessary.
        virtual auto    createEditor() -> PreferencesEditor * = 0;
    };

    /// \class PreferencesManager tt3-gui/API.hpp
    /// \brief The manager of known Preferences.
    class TT3_GUI_PUBLIC PreferencesManager final
    {
        TT3_UTILITY_CLASS(PreferencesManager)

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the set of all registered Preferences, whether
        ///     root, leaf or intermediate.
        /// \return
        ///     The set of all registered Preferences.
        static auto     all() -> QSet<Preferences*>;

        /// \brief
        ///     Returns the set of all registered Preferemces that have no parent.
        /// \return
        ///     The set of all registered Preferemces that have no parent.
        static auto     roots() -> QSet<Preferences*>;

        /// \brief
        ///     Registers the specified {references (along with its parents).
        /// \param preferences
        ///     The preferences node to register.
        /// \return
        ///     True if preferences (and all its parents) have
        ///     registered successfully, else false. A repeated
        ///     registration of the same preferences is treated as
        ///     a success.
        static bool     register(Preferences * preferences);

        /// \brief
        ///     Un-registers the specified {references (along with its children).
        /// \param preferences
        ///     The preferences node to un-register.
        /// \return
        ///     True if preferences (and all its children) have
        ///     un-registered successfully, else false.
        static bool     unregister(Preferences * preferences);

        /// \brief
        ///     Finds a registered Preferences by its mnemonic.
        /// \param mnemonic
        ///     The mnemonic to look for.
        /// \return
        ///     The registered Preferences with the required
        ///     mnemonic or if not found.
        static auto     find(const tt3::util::Mnemonic & mnemonic) -> Preferences *;

        //////////
        //  Implementation
    private:
        struct _Impl;

        //  Helpers
        static _Impl *  _impl();
    };

    /// \class GeneralPreferences tt3-gui/API.hpp
    /// \brief The "/General" preferences.
    class TT3_GUI_PUBLIC GeneralPreferences final
        :   public Preferences
    {
        TT3_DECLARE_SINGLETON(GeneralPreferences)

        //////////
        //  Preferences
    public:
        virtual Mnemonic        mnemonic() const override;
        virtual QString         displayName() const override;
        virtual Preferences *   parent() const override;
        virtual int             order() const { return 0; }
        virtual PreferencesEditor * createEditor() override;
    };

    /// \class GeneralAppearancePreferences tt3-gui/API.hpp
    /// \brief The "/General/Appearance" preferences.
    class TT3_GUI_PUBLIC GeneralAppearancePreferences final
        :   public Preferences
    {
        TT3_DECLARE_SINGLETON(GeneralAppearancePreferences)

        //////////
        //  Preferences
    public:
        virtual Mnemonic        mnemonic() const override;
        virtual QString         displayName() const override;
        virtual Preferences *   parent() const override;
        virtual int             order() const { return 0; }
        virtual PreferencesEditor * createEditor() override;
    };

    /// \class GeneralStartupPreferences tt3-gui/API.hpp
    /// \brief The "/General/Startup" preferences.
    class TT3_GUI_PUBLIC GeneralStartupPreferences final
        :   public Preferences
    {
        TT3_DECLARE_SINGLETON(GeneralStartupPreferences)

        //////////
        //  Preferences
    public:
        virtual Mnemonic        mnemonic() const override;
        virtual QString         displayName() const override;
        virtual Preferences *   parent() const override;
        virtual int             order() const { return 10; }
        virtual PreferencesEditor * createEditor() override;
    };

    /// \class GeneralDialogsPreferences tt3-gui/API.hpp
    /// \brief The "/General/Dialogs" preferences.
    class TT3_GUI_PUBLIC GeneralDialogsPreferences final
        :   public Preferences
    {
        TT3_DECLARE_SINGLETON(GeneralDialogsPreferences)

        //////////
        //  Preferences
    public:
        virtual Mnemonic        mnemonic() const override;
        virtual QString         displayName() const override;
        virtual Preferences *   parent() const override;
        virtual int             order() const { return 5; }
        virtual PreferencesEditor * createEditor() override;
    };

    /// \class GeneralHelpPreferences tt3-gui/API.hpp
    /// \brief The "/General/Help" preferences.
    class TT3_GUI_PUBLIC GeneralHelpPreferences final
        :   public Preferences
    {
        TT3_DECLARE_SINGLETON(GeneralHelpPreferences)

        //////////
        //  Preferences
    public:
        virtual Mnemonic        mnemonic() const override;
        virtual QString         displayName() const override;
        virtual Preferences *   parent() const override;
        virtual int             order() const { return 15; }
        virtual PreferencesEditor * createEditor() override;
    };

    /// \class InterfacePreferences tt3-gui/API.hpp
    /// \brief The "/Interface" preferences.
    class TT3_GUI_PUBLIC InterfacePreferences final
        :   public Preferences
    {
        TT3_DECLARE_SINGLETON(InterfacePreferences)

        //////////
        //  Preferences
    public:
        virtual Mnemonic        mnemonic() const override;
        virtual QString         displayName() const override;
        virtual Preferences *   parent() const override;
        virtual int             order() const { return 100; }
        virtual PreferencesEditor * createEditor() override;
    };

    /// \class StoragePreferences tt3-gui/API.hpp
    /// \brief The "/Storage" preferences.
    class TT3_GUI_PUBLIC StoragePreferences final
        :   public Preferences
    {
        TT3_DECLARE_SINGLETON(StoragePreferences)

        //////////
        //  Preferences
    public:
        virtual Mnemonic        mnemonic() const override;
        virtual QString         displayName() const override;
        virtual Preferences *   parent() const override;
        virtual int             order() const { return 200; }
        virtual PreferencesEditor * createEditor() override;
    };
}

//  End of tt3-gui/Preferences.hpp
