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
    //  A generic "settings" is a single value which is retained between runs
    class TT3_UTIL_PUBLIC AbstractSetting : public QObject
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(AbstractSetting)

        //////////
        //  Construction/destruction
    protected:
        explicit AbstractSetting(const Mnemonic & mnemonic, bool changeRequiresRestart)
            :   _mnemonic(mnemonic), _changeRequiresRestart(changeRequiresRestart) {}
        virtual ~AbstractSetting() = default;

        //////////
        //  Operations
    public:
        //  The mnemonic identifier of this Setting.
        Mnemonic        mnemonic() const { return _mnemonic; }

        //  True if changes to the value of this Setting
        //  shall require application restart in order to
        //  take effect, else false.
        bool            changeRequiresRestart() const { return _changeRequiresRestart; }

        //  Returns/sets the string representation of this Setting' value.
        virtual QString valueString() const = 0;
        virtual void    setValueString(const QString & valueString) = 0;

        //////////
        //  Signals
    signals:
        //  Emitted when a value of the setting changes
        void            valueChanged(AbstractSetting *);

        //////////
        //  Implementation
    private:
        const Mnemonic  _mnemonic;
        const bool      _changeRequiresRestart;
    };

    //  A "settings" is a single value which is retained between runs
    template <class T>
    class Setting final : public AbstractSetting
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Setting)

        //////////
        //  Construction/destruction
    public:
        Setting(const Mnemonic & mnemonic, const T & defaultValue, bool changeRequiresRestart = false)
            :   AbstractSetting(mnemonic, changeRequiresRestart),
                _defaultValue(defaultValue),
                _value(defaultValue),
                _valueLoaded(false),
                _needsSaving(false) {}
        ~Setting() = default;

        //////////
        //  Operators
    public:
                        operator T() const { return value(); }
        Setting<T> &    operator = (const T & src) { setValue(src); return *this; }

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
            catch (const ParseException &)
            {   //  Ignore
            }
        }

        //////////
        //  Operations
    public:
        //  The default value of this setting
        const T         defaultValue() const
        {
            return _defaultValue;
        }

        //  Returns the current value of this setting
        T               value() const
        {
            return _valueLoaded ? _value : _defaultValue;
        }

        //  Sets the current value of this setting
        void            setValue(const T & value)
        {
            if (value != this->value())
            {
                _value = value;
                _valueLoaded = true;
                _needsSaving = true;
                emit valueChanged(this);
            }
        }

        //////////
        //  Implementation
    private:
        const T         _defaultValue;

        T               _value;
        bool            _valueLoaded;
        bool            _needsSaving;
    };

    //////////
    //  A bunch of related settings.
    //  Concrete subclasses will normally be singletons.
    class TT3_UTIL_PUBLIC Settings
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Settings)

        //////////
        //  Construction/destruction
    protected:
        Settings() = default;
        virtual ~Settings() = default;

        //////////
        //  Operations
    protected:
        //  A helper service for constructors of
        //  derived classes to populate the Settings
        //  with aggregated Setting<?> instances
        bool                addSetting(AbstractSetting * setting);
    public:
        //  A set of all settings in this bunch
        QSet<AbstractSetting*>  settings() const;

        //  Finds a setting with the specified mnemonic in
        //  this settings; returns nullptr if not found
        AbstractSetting *   findSetting(const Mnemonic & mnemonic) const;

        //////////
        //  Implementation
    private:
        QMap<Mnemonic, AbstractSetting*>    _settings;
    };
}

//  Macro needed for MOC-generated .cpp files
#define TT3_UTIL_SETTINGS_DEFINED

//  End of tt3-util/Settings.cpp
