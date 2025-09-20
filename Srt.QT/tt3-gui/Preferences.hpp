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
    class TT3_GUI_PUBLIC PreferencesEditor;

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
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Preferences)

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
        virtual Mnemonic        mnemonic() const = 0;

        /// \brief
        ///     Returns the user-readable display name of this
        ///     preferences node.
        /// \return
        ///     The user-readable display name of this preferences.
        ///     node for the current default locale.
        virtual QString         displayName() const = 0;

        /// \brief
        ///     Returns the immediate parent node of this Preferences node.
        /// \return
        ///     The immediate parent node of this Preferences node
        ///     or null if this is one of the root nodes.
        virtual Preferences *   parent() const = 0;

        /// \brief
        ///     Returns the set of immediate child nodes of this
        ///     Preferences node.
        /// \return
        ///     The set of immediate child nodes of this
        ///     Preferences node.
        virtual QSet<Preferences*>  children() const;

        /// \brief
        ///     The relative order of this Preferences node
        ///     within its parent.
        /// \details
        ///     Lower-order nodes come before higher-order nodes;
        ///     children with the same order are sorted by display name).
        /// \return
        ///     The relative order of this Preferences node
        ///     within its parent.
        virtual int             order() const { return INT_MAX; }

        /// \brief
        ///     Creates a new editor widget for this preferences node.
        /// \return
        ///     Creates a new editor widget for this preferences node,
        ///     or nullptr if this node is not editable. The returned
        ///     editor widget will be re-parented as necessary.
        virtual PreferencesEditor * createEditor() = 0;
    };

    /// \class PreferencesManager tt3-gui/API.hpp
    /// \brief The manager of known Preferences.
    class TT3_GUI_PUBLIC PreferencesManager final
    {
        UTILITY_CLASS(PreferencesManager)

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the set of all registered Preferences, whether
        ///     root, leaf or intermediate.
        /// \return
        ///     The set of all registered Preferences.
        static QSet<Preferences*>   allPreferences();

        /// \brief
        ///     Returns the set of all registered Preferemces that have no parent.
        /// \return
        ///     The set of all registered Preferemces that have no parent.
        static QSet<Preferences*>   rootPreferences();

        /// \brief
        ///     Registers the specified {references (along with its parents).
        /// \param preferences
        ///     The preferences node to register.
        /// \return
        ///     True if preferences (and all its parents) have
        ///     registered successfully, else false. A repeated
        ///     registration of the same preferences is treated as
        ///     a success.
        static bool                 registerPreferences(Preferences * preferences);

        /// \brief
        ///     Finds a registered Preferences by its mnemonic.
        /// \param mnemonic
        ///     The mnemonic to look for.
        /// \return
        ///     The registered Preferences with the required
        ///     mnemonic or if not found.
        static Preferences *        findPreferences(const tt3::util::Mnemonic & mnemonic);

        //////////
        //  Implementation
    private:
        struct _Impl;

        //  Helpers
        static _Impl *          _impl();
    };

    /// \class GeneralPreferences tt3-gui/API.hpp
    /// \brief The "/General" preferences.
    class TT3_GUI_PUBLIC GeneralPreferences final
        :   public Preferences
    {
        DECLARE_SINGLETON(GeneralPreferences)

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
        DECLARE_SINGLETON(GeneralAppearancePreferences)

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
        DECLARE_SINGLETON(GeneralStartupPreferences)

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
        DECLARE_SINGLETON(GeneralDialogsPreferences)

        //////////
        //  Preferences
    public:
        virtual Mnemonic        mnemonic() const override;
        virtual QString         displayName() const override;
        virtual Preferences *   parent() const override;
        virtual int             order() const { return 5; }
        virtual PreferencesEditor * createEditor() override;
    };
}

//  End of tt3-gui/Preferences.hpp

