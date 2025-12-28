//
//  tt3-util/Settings.cpp - Persistent settinga framework
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
#pragma once
#include "tt3-util/API.hpp"

namespace tt3::util
{
    class AbstractSetting;

    /// \class Settings tt3-util/API.hpp
    /// \brief A bunch of related settings.
    /// \details
    ///     Concrete subclasses will normally be singletons.
    class TT3_UTIL_PUBLIC Settings
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Settings)

        friend class AbstractSetting;

        //////////
        //  Construction/destruction
    protected:
        /// \brief
        ///     Constructs an initially empty binch of Settings.
        Settings() : _settings() {}

        /// \brief
        ///     The class destructor.
        virtual ~Settings() = default;

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the set of all settings in this bunch.
        /// \return
        ///     The set of all settings in this bunch.
        auto        settings(
                        ) const -> QSet<AbstractSetting*>;

        /// \brief
        ///     Finds a setting with the specified mnemonic in
        ///     this settings.
        /// \param mnemonic
        ///     The mnemonic to look for.
        /// \return
        ///     The setting with the required mnemonic in
        ///     this settings; returns nullptr if not found
        auto        findSetting(
                            const Mnemonic & mnemonic
                        ) const -> AbstractSetting *;

        //////////
        //  Implementation
    private:
        QMap<Mnemonic, AbstractSetting*>    _settings;
    };

    /// \class AbstractSetting tt3-util/API.hpp
    /// \brief A generic "settings" is a single value which is retained between runs.
    class TT3_UTIL_PUBLIC AbstractSetting
        :   public QObject
    {
        Q_OBJECT
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(AbstractSetting)

        //////////
        //  Construction/destruction
    protected:
        /// \brief
        ///     Constructs the setting.
        /// \param settings
        ///     The settings bunch to which this AbstractSetting belonga.
        /// \param mnemonic
        ///     The mnemonic identifier of the setting.
        /// \param changeRequiresRestart
        ///     True if changes to the setting's values require TT3
        ///     to be restarted, false if not.
        explicit AbstractSetting(
                Settings * settings,
                const Mnemonic & mnemonic,
                bool changeRequiresRestart
            ) : _mnemonic(mnemonic),
                _changeRequiresRestart(changeRequiresRestart)
        {
            Q_ASSERT(settings != nullptr);
            Q_ASSERT(!settings->_settings.contains(mnemonic));
            settings->_settings.insert(mnemonic, this);
        }

        /// \brief
        ///     The class destructor.
        virtual ~AbstractSetting() = default;

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the mnemonic identifier of this Setting.
        /// \return
        ///     The mnemonic identifier of this Setting.
        Mnemonic    mnemonic() const { return _mnemonic; }

        /// \brief
        ///     Checks whether changes to the value of this
        ///     Setting shall require application restart in
        ///     order to take effect.
        /// \return
        ///     True if changes to the value of this Setting
        ///     shall require application restart in order to
        ///     take effect, else false.
        bool        changeRequiresRestart() const { return _changeRequiresRestart; }

        /// \brief
        ///     Returns the string representation of this Setting' value.
        /// \return
        ///     The string representation of this Setting' value.
        virtual QString valueString(
                            ) const = 0;

        /// \brief
        ///     Returns the string representation of this Setting' value.
        /// \param valueString
        ///     The string representation of this Setting' new value.
        virtual void    setValueString(
                                const QString & valueString
                            ) = 0;

        //////////
        //  Signals
    signals:
        /// \brief
        ///     Emitted when a value of the setting changes.
        void        valueChanged();

        //////////
        //  Implementation
    private:
        const Mnemonic  _mnemonic;
        const bool      _changeRequiresRestart;
    };

    /// \class Setting tt3-util/API.hpp
    /// \brief A "settings" is a single value which is retained between runs.
    template <class T>
    class Setting final
        :   public AbstractSetting
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Setting)

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs the setting.
        /// \param settings
        ///     The settings bunch to which this Setting belonga.
        /// \param mnemonic
        ///     The mnemonic identifier of the setting.
        /// \param defaultValue
        ///     The default value of the setting.
        /// \param changeRequiresRestart
        ///     True if changes to the setting's values require TT3
        ///     to be restarted, false if not.
        Setting(Settings * settings,
                const Mnemonic & mnemonic,
                const T & defaultValue,
                bool changeRequiresRestart = false
            ) : AbstractSetting(settings, mnemonic, changeRequiresRestart),
                _defaultValue(defaultValue),
                _value(defaultValue),
                _valueAssigned(false) {}

        /// \brief
        ///     The class destructor.
        virtual ~Setting() = default;

        //////////
        //  Operators
    public:
        /// \brief
        ///     Returns the value of this Setting.
        /// \return
        ///     The value of this Setting (starts off as default value).
                    operator T() const { return value(); }

        /// \brief
        ///     Assigns a new value to this Setting.
        /// \param src
        ///     The new value to assign to this Setting.
        /// \return
        ///     The reference to this Setting.
        Setting<T> &operator = (const T & src) { setValue(src); return *this; }

        //////////
        //  AbstractSetting
    public:
        virtual QString valueString() const override
        {
            return toString<T>(value());
        }

        virtual void    setValueString(const QString & valueString) override
        {
            try
            {
                setValue(fromString<T>(valueString, _defaultValue));
            }
            catch (const ParseException & ex)
            {   //  OOPS! Log, but Ignore
                qCritical() << ex;
            }
        }

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the default value of this setting.
        /// \return
        ///     The default value of this setting.
        const T     defaultValue() const
        {
            return _defaultValue;
        }

        /// \brief
        ///     Returns the current value of this Setting.
        /// \return
        ///     The current value of this Setting; default value
        ///     if not assigned explicitly.
        T           value() const
        {
            return _valueAssigned ? _value : _defaultValue;
        }

        /// \brief
        ///     Sets the current value of this Setting.
        /// \param value
        ///     The new value for this Setting.
        void        setValue(const T & value)
        {
            if (value != this->value())
            {
                _value = value;
                _valueAssigned = true;
                emit valueChanged();
            }
        }

        //////////
        //  Implementation
    private:
        const T         _defaultValue;

        T               _value;
        bool            _valueAssigned;
    };
}

//  Macro needed for MOC-generated .cpp files
#define TT3_UTIL_SETTINGS_DEFINED

//  End of tt3-util/Settings.cpp
