//
//  tt3-gui/HelpClient.cpp - HelpClient class implementation
//  TOO translate via resourcess
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
#include "tt3-gui/API.hpp"
using namespace tt3::gui;

namespace tt3::gui
{
    extern CurrentSkin theCurrentSkin;
}

struct HelpClient::_Impl
{
    _Impl()
    {
        //  Listen to help site builder events
        QObject::connect(
            &siteBuilder,
            &tt3::help::HelpSiteBuilder::siteBuildingStarted,
            &siteBuilder,
            [&]()
            {
                progressWindow = new HelpBuilderProgressWindow();
                progressWindow->setVisible(true);
            },
            Qt::QueuedConnection);
        QObject::connect(
            &siteBuilder,
            &tt3::help::HelpSiteBuilder::siteBuildingProgress,
            &siteBuilder,
            [&](QString context, QString action)
            {
                progressWindow->reportProgress(context, action);
            },
            Qt::QueuedConnection);
        QObject::connect(
            &siteBuilder,
            &tt3::help::HelpSiteBuilder::siteBuildingCompleted,
            &siteBuilder,
            [&](bool /*success*/)
            {
                progressWindow->setVisible(false);
                delete progressWindow;
                progressWindow = nullptr;
            },
            Qt::QueuedConnection);
        QObject::connect(
            &siteBuilder,
            &tt3::help::HelpSiteBuilder::siteBuildingError,
            &siteBuilder,
            [](QString errorMessage)
            {
                ErrorDialog::show(theCurrentSkin->dialogParent(), errorMessage);
            },
            Qt::QueuedConnection);
    }

    tt3::help::HelpSiteBuilder siteBuilder;
    HelpBuilderProgressWindow * progressWindow = nullptr;
};

//////////
//  Operations
void HelpClient::showContents()
{
    Q_ASSERT(QThread::currentThread()->eventDispatcher() != nullptr);

    _Impl * impl = _impl();

    bool rebuild = !Component::Settings::instance()->cacheHelpCollection;
    if (!impl->siteBuilder.buildHelpSite(rebuild))
    {   //  OOPS! Can't proceed
        return;
    }
    //  Otherwise, open the content.htm for
    //  the current/default locale
    _openHelpFile("/content.htm");
}

void HelpClient::showIndex()
{
    Q_ASSERT(QThread::currentThread()->eventDispatcher() != nullptr);
    throw tt3::util::NotImplementedError();
}

void HelpClient::showSearch()
{
    Q_ASSERT(QThread::currentThread()->eventDispatcher() != nullptr);
    throw tt3::util::NotImplementedError();
}

void HelpClient::showTopic(const QString & /*topic*/)
{
    Q_ASSERT(QThread::currentThread()->eventDispatcher() != nullptr);
    throw tt3::util::NotImplementedError();
}

//////////
//  Implementation helpers
HelpClient::_Impl * HelpClient::_impl()
{
    static _Impl impl;
    return &impl;
}

bool HelpClient::_openHelpFile(const QString & fileName)
{
    _Impl * impl = _impl();
    QDir helpDir(impl->siteBuilder.helpSiteDirectory());
    QString contentFilePath1 =
        helpDir.filePath(tt3::util::toString(QLocale()) + fileName);
    QString contentFilePath2 =
        helpDir.filePath(tt3::util::toString(Component::Resources::instance()->baseLocale()) + fileName);
    if (QFile(contentFilePath1).exists())
    {
        if (!QDesktopServices::openUrl(QUrl::fromLocalFile(contentFilePath1)))
        {
            ErrorDialog::show("Could not open " + contentFilePath1);
            return false;
        }
        return true;
    }
    else if (QFile(contentFilePath2).exists())
    {
        if (!QDesktopServices::openUrl(QUrl::fromLocalFile(contentFilePath2)))
        {
            ErrorDialog::show("Could not open " + contentFilePath2);
            return false;
        }
        return true;
    }
    else
    {
        ErrorDialog::show("Don't know how to open help content");
        return false;
    }
}

//  End of tt3-gui/HelpClient.cpp
