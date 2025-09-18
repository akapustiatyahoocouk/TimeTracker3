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
    /// \class ILicense tt3-util/API.hpp
    /// \brief
    ///     A generic "License" is a legal disclamer of the
    ///     rights and responsibilities of a product user.
    class TT3_UTIL_PUBLIC ILicense
    {
        //////////
        //  This is an interface
    protected:
        /// \brief
        ///     The empty [interface] constructor.
        ILicense() = default;

        /// \brief
        ///     The empty [interface] destructor.
        virtual ~ILicense() = default;

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the mnemonic identifier of this license.
        /// \return
        ///     The mnemonic identifier of this license.
        virtual Mnemonic    mnemonic() const = 0;

        /// \brief
        ///     Returns the user-readable display name of this license.
        /// \return
        ///     The user-readable display name of this license
        ///     for the current default locale.
        virtual QString     displayName() const = 0;

        /// \brief
        ///     Returns the short (1 line) description of
        ///     this license.
        /// \return
        ///     The short (1 line) description of this license
        ///     for the current default locale.
        virtual QString     description() const = 0;

        /// \brief
        ///     Returns the small (16x16) icon representing this license.
        /// \return
        ///     The small (16x16) icon representing this license.
        virtual QIcon       smallIcon() const;

        /// \brief
        ///     Returns the large (32x32) icon representing this license.
        /// \return
        ///     The large (32x32) icon representing this license.
        virtual QIcon       largeIcon() const;

        /// \brief
        ///     Returns the user-readable textual content of this license
        ///     in the fowm of an HTML document.
        /// \return
        ///     The user-readable textual content of this license for
        ///     the current default locale in the fowm of an HTML document.
        virtual QString     contentAsHtml() const = 0;
    };

    /// \class StandardLicenses tt3-util/API.hpp
    /// \brief The standard (predefined) licenses.
    class TT3_UTIL_PUBLIC StandardLicenses final
    {
        UTILITY_CLASS(StandardLicenses)

        //////////
        //  Members
    public:
        /// \class Gpl3 tt3-util/PI.hpp
        /// \brief The GPLv3 license.
        class TT3_UTIL_PUBLIC Gpl3 final
            :   public virtual ILicense
        {
            DECLARE_SINGLETON(Gpl3)

            //////////
            //  ILicense
        public:
            virtual Mnemonic    mnemonic() const override;
            virtual QString     displayName() const override;
            virtual QString     description() const override;
            virtual QString     contentAsHtml() const override;

            //////////
            //  Implementation
        private:
            mutable Mutex       _contentsGuard;
            mutable QMap<QLocale, QString>  _contentsAsHtml;
        };
    };

    /// \class LicenseManager tt3-util/API.hpp
    /// \brief The manager of known Licenses.
    class TT3_UTIL_PUBLIC LicenseManager final
    {
        UTILITY_CLASS(LicenseManager)

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the set of all registered licenses.
        /// \return
        ///     The set of all registered licenses.
        static Licenses     allLicenses();

        /// \brief
        ///     Registers the specified License.
        /// \param license
        ///     The License to register.
        /// \return
        ///     True on success, false on failure. Repeated regiatration
        ///     of the same License is treated as a "success".
        static bool         registerLicense(ILicense * license);

        /// \brief
        ///     Finds a registered License by its mnemonic.
        /// \param mnemonic
        ///     The mnemonic to look for.
        /// \return
        ///     The registered License with the required mnemonic;
        ///     nullptr if not found.
        static ILicense *   findLicense(const Mnemonic & mnemonic);

        //////////
        //  Implementation
    private:
        struct _Impl;

        //  Helpers
        static _Impl *      _impl();
    };
}

//  End of tt3-util/License.hpp
