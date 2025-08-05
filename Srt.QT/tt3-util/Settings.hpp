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

namespace util
{
    class TT3_UTIL_PUBLIC AbstractSetting
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(AbstractSetting)

        //////////
        //  Construction/destruction
    public:
        AbstractSetting(const QString & mnemonic, const QString & displayName)
            :   _mnemonic(mnemonic), _displayName(displayName) {}
        virtual ~AbstractSetting() = default;

        //////////
        //  Operations
    public:
        //  TODO document
        QString         mnemonic() const { return _mnemonic; }
        QString         displayName() const { return _displayName; }

        virtual QString valueString() const = 0;
        virtual void    setValueString(const QString & valueString) = 0;

        //////////
        //  Implementation
    private:
        const QString   _mnemonic;
        const QString   _displayName;
    };

    //////////
    //  A "settings" is a single value which is retained between runs
    template <class T>
    class Setting final : public AbstractSetting
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Setting)

        //////////
        //  Construction/destruction
    public:
        Setting(const QString & mnemonic, const QString & displayName, const T & defaultValue)
            :   AbstractSetting(mnemonic, displayName),
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
        //  TODO document
        T               value() const
        {
            return _valueLoaded ? _value : _defaultValue;
        }

        void            setValue(const T & value)
        {
            if (value != this->value())
            {
                _value = value;
                _valueLoaded = true;
                _needsSaving = true;
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
    //  A bunch of related settings
    class TT3_UTIL_PUBLIC Settings
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Settings)

        //////////
        //  Construction/destruction
    public:
        Settings() = default;
        virtual ~Settings() = default;

        //////////
        //  Operations
    protected:
        bool                addSetting(AbstractSetting * setting);
    public:
        //  An unordered list of all settings in this bunch
        QList<AbstractSetting*> settings() const { return _settings.values(); }
        AbstractSetting *   findSetting(const QString & mnemonic) const;

        //////////
        //  Implementation
    private:
        QMap<QString, AbstractSetting*> _settings;
    };
}

//  End of tt3-util/Settings.cpp
