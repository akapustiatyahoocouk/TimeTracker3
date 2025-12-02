//
//  tt3-help/ContentLoader.hpp - The content acquisition agent
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

namespace tt3::help
{
    /// \class IContentLoader tt3-help/API.hpp
    /// \brief The generic agent loading help content.
    class TT3_HELP_PUBLIC IContentLoader
    {
        friend class SimpleHelpTopic;

        //////////
        //  This is an interface
    protected:
        /// \brief
        ///     The empty (interface) constructor.
        IContentLoader() = default;

        /// \brief
        ///     The empty (interface) destructor.
        virtual ~IContentLoader() = default;

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the URL from which the content will be loaded.
        /// \return
        ///     The URL from which the content will be loaded.
        virtual QUrl        contentUrl() const = 0;

        /// \brief
        ///     Loads the full content.
        /// \return
        ///     The full content, as loaded from the relevant URL.
        /// \exception HelpException
        ///     If an error occurs.
        virtual QByteArray  loadContent() = 0;
    };

    /// \class LocalFileContentLoader tt3-help/API.hpp
    /// \brief The agent loading help content from local file system.
    class TT3_HELP_PUBLIC LocalFileContentLoader final
        :   public virtual IContentLoader
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(LocalFileContentLoader)

        //////////
        //  Constants
    public:
        /// \brief
        ///     The "protocol" prefix of a "local file" URL.
        static inline const QString Protocol = "file:";

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Costructs the content loader.
        /// \param path
        ///     The local filesystem path to the content file.
        explicit LocalFileContentLoader(const QString & path);

        /// \brief
        ///     The class destuctor.
        virtual ~LocalFileContentLoader();

        //////////
        //  IContentLoader
    public:
        virtual QUrl        contentUrl() const override;
        virtual QByteArray  loadContent() override;

        //////////
        //  Implementation
    private:
        QString         _path;  //  full path to local file
    };

    /// \class ContentLoaderFactory tt3-help/API.hpp
    /// \brief The factory for help content loaders.
    class TT3_HELP_PUBLIC ContentLoaderFactory final
    {
        TT3_UTILITY_CLASS(ContentLoaderFactory)

        //////////
        //  Operations
    public:
        /// \brief
        ///     Creates a ContentLoader for the specififed URL.
        /// \param url
        ///     The URL to create ContentLoader for.
        /// \return
        ///     A new ContentLoader for the specififed URL.
        /// \exception HelpException
        ///     If an error occurs (e.g. URL not recognized, etc.)
        static IContentLoader * createContentLoader(const QUrl & url);
    };
}

//  End of tt3-help/ContentLoader.hpp
