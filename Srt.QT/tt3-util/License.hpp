//
//  tt3-util/License.hpp - tt3 Licenses framework
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
    //  A generic "License" is a legal displamer
    //  of the rights and responsibilities of a product user
    class TT3_UTIL_PUBLIC ILicense
    {
        //////////
        //  This is an interface
    protected:
        ILicense() = default;
        virtual ~ILicense() = default;

        //////////
        //  Operations
    public:
        //  The mnemonic identifier of this license
        virtual QString     mnemonic() const = 0;

        //  The user-readable display name of this license
        //  for the current default locale.
        virtual QString     displayName() const = 0;

        //  The short (1 line) description of this license
        //  for the current default locale.
        virtual QString     description() const = 0;

        //  The small (16x16) icon representing this license.
        virtual QIcon       smallIcon() const;

        //  The large (32x32) icon representing this license.
        virtual QIcon       largeIcon() const;

        //  The user-readable textual content of this license for
        //  the current default locale in the fowm of an HTML document.
        virtual QString     contentAsHtml() const = 0;
    };

    class TT3_UTIL_PUBLIC Gpl3Licene final : public virtual ILicense
    {
        DECLARE_SINGLETON(Gpl3Licene)

        //////////
        //  ILicense
    public:
        virtual QString     mnemonic() const override;
        virtual QString     displayName() const override;
        virtual QString     description() const override;
        virtual QString     contentAsHtml() const override;

        //////////
        //  Implementation
    private:
        mutable tt3::util::Mutex        _contentsGuard;
        mutable QMap<QLocale, QString>  _contentsAsHtml;
    };

    //////////
    //  The manager of known Licenses
    class TT3_UTIL_PUBLIC LicenseManager final
    {
        UTILITY_CLASS(LicenseManager)

        //////////
        //  Operations
    public:
        //  Returns the set of all registered licenses.
        static QSet<ILicense*>  allLicenses();

        //  Registers the specified License; returns true
        //  on success, false on failure.
        static bool         registerLicense(ILicense * license);

        //  Finds a registered License by its mnemonic;
        //  returns nullptr if not found.
        static ILicense *   findLicense(const QString & mnemonic);

        //////////
        //  Implementation
    private:
        static tt3::util::Mutex _guard;
        static QMap<QString, ILicense*> _registry;  //  key = mnemonic

        //  Helpers
        static void         _registerStandardLicenses();
    };
}

//  End of tt3-util/License.hpp
