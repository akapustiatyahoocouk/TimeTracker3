//
//  tt3-util/ResourceFactory.hpp - Resource factory framework
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
    //  A "resource factory" is an agent that provides
    //  localized variants of the resources it manages.
    //  Concrete classes implementing this interface
    //  will normally be singletons.
    class TT3_UTIL_PUBLIC IResourceFactory
    {
        //////////
        //  This is an interface
    protected:
        IResourceFactory() = default;
        virtual ~IResourceFactory() = default;

        //////////
        //  Operations
    public:
        //  The name of this resource factory (e.g. the name
        //  of the underlying resaource file, etc.)
        virtual QString     name() const = 0;

        //  The locale that is guaranteed to be supported
        //  by this resource factory.
        virtual QLocale     baseLocale() const = 0;

        //  The set of all locales supported by this
        //  resource factory.
        //  The resulting set is guaranteed to inclide the
        //  locale returned by the baseLocale() above.
        virtual Locales     supportedLocales() const = 0;

        //  Returns the string resource with the specified section
        //  ID and resource ID for the current default locale.
        //  C++ - staly escapes (e.g. '\n', etc.) in resource strings
        //  (but not the section?resource IDs!) are recognized and
        //  replaced with the corresponding control characters.
        //  Throws MissingResourceException (or a subclass thereof)
        //  if an error occurs.
        virtual QString     string(const ResourceSectionId & sectionId, const ResourceId & resourceId) const = 0;

        //  Returns the string resource with the specified section
        //  ID and resource ID for the current default locale.
        //  Within the resource:
        //  *   The placeholder strings {0}, {1}, etc. are replaced
        //      with the corresponding parameter.
        //  *   The escape sequences {{ and }} become single
        //      characters { and }.
        //  Throws MissingResourceException (or a subclass thereof)
        //  if an error occurs.
        virtual QString     string(const ResourceSectionId & sectionId, const ResourceId & resourceId, const QStringList & params) const;
        virtual QString     string(const ResourceSectionId & sectionId, const ResourceId & resourceId, const QString & param1) const;
        virtual QString     string(const ResourceSectionId & sectionId, const ResourceId & resourceId, const QString & param1, const QString & param2) const;
        virtual QString     string(const ResourceSectionId & sectionId, const ResourceId & resourceId, const QString & param1, const QString & param2, const QString & param3) const;

        template <class P1>
        QString             string(const ResourceSectionId & sectionId, const ResourceId & resourceId, const P1 & param1) const
        {
            return string(sectionId, resourceId, toString(param1));
        }

        template <class P1, class P2>
        QString             string(const ResourceSectionId & sectionId, const ResourceId & resourceId, const P1 & param1, const P2 & param2) const
        {
            return string(sectionId, resourceId,
                          toString(param1), toString(param2));
        }

        template <class P1, class P2, class P3>
        QString             string(const ResourceSectionId & sectionId, const ResourceId & resourceId, const P1 & param1, const P2 & param2, const P3 & param3) const
        {
            return string(sectionId, resourceId,
                          toString(param1), toString(param2), toString(param3));
        }
    };

    //  A resource factory that loads localized resources
    //  from a set of related "<base>_<ll>_<CC>.res" files.
    //  Which files are found becomes the "set of supported
    //  locales" for the FileResourceFactory; the default
    //  base locale is "en_GB".
    class TT3_UTIL_PUBLIC FileResourceFactory : public virtual IResourceFactory
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(FileResourceFactory)

        //////////
        //  Construction/destruction
    protected:
        /// \brief
        ///     The class constructor.
        /// \param baseFileName
        ///     The "base" name of the resource file.
        ///     Locale-specific variants will be constructed automatically
        ///     by appending the language and countrycodes to the base file
        ///     name just before the extension (e.g. "x.txt" becomes e.g.
        explicit FileResourceFactory(
                const QString & baseFileName
            );

        /// \brief
        ///     The class destructor.
        virtual ~FileResourceFactory();

        //////////
        //  IResourceFactory
    public:
        using tt3::util::IResourceFactory::string;

        virtual QString     name() const override;
        virtual QLocale     baseLocale() const override;
        virtual Locales     supportedLocales() const override;
        virtual QString     string(const ResourceSectionId & sectionId, const ResourceId & resourceId) const override;

        //////////
        //  Implementation
    private:
        const QString   _baseFileName;
        bool            _loaded = false;
        Locales         _supportedLocales;
        QMap<QString, QString>  _stringResources;   //  key == <ll>_<CC>[<section>]<resource>

        //  Helpers
        void            _loadResources();
        void            _loadResourceFile(const QLocale & locale, const QString & fileName);
        static QString  _key(const QLocale & locale, const ResourceSectionId & sectionId, const ResourceId & resourceId);
        static QString  _key(const ResourceSectionId & sectionId, const ResourceId & resourceId);
        static int      _xdigit(const QChar & c);
        static QString  _unescape(const QString & s);   //  throws ParseException on error
    };
}

//  End of tt3-util/ResourceFactory.hpp
