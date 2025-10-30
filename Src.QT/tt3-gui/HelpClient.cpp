//
//  tt3-gui/HelpClient.cpp - HelpClient class implementation
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
#include "tt3-gui/API.hpp"
using namespace tt3::gui;

struct HelpClient::_Impl
{
    _Impl()
    {
        //  Listen to help site builder events
        QObject::connect(
            &siteBuilder,
            &HelpSiteBuilder::siteBuildingStarted,
            &siteBuilder,
            [&]()
            {
                progressWindow = new HelpBuilderProgressWindow();
                progressWindow->setVisible(true);
            },
            Qt::QueuedConnection);
        QObject::connect(
            &siteBuilder,
            &HelpSiteBuilder::siteBuildingProgress,
            &siteBuilder,
            [&](QString context, QString action)
            {
                progressWindow->reportProgress(context, action);
            },
            Qt::QueuedConnection);
        QObject::connect(
            &siteBuilder,
            &HelpSiteBuilder::siteBuildingCompleted,
            &siteBuilder,
            [&](bool success)
            {
                progressWindow->setVisible(false);
                delete progressWindow;
                progressWindow = nullptr;
            },
            Qt::QueuedConnection);
        QObject::connect(
            &siteBuilder,
            &HelpSiteBuilder::siteBuildingError,
            &siteBuilder,
            [](QString errorMessage)
            {
                ErrorDialog::show(QApplication::activeWindow(), errorMessage);
            },
            Qt::QueuedConnection);
    }

    HelpSiteBuilder siteBuilder;
    HelpBuilderProgressWindow * progressWindow = nullptr;
};

//////////
//  Operations
void HelpClient::showContents()
{
    _Impl * impl = _impl();

    if (!impl->siteBuilder.buildHelpSite())
    {   //  OOPS! Can't proceed
        return;
    }
    //  TODO finish the implementation
}

void HelpClient::showIndex()
{
    throw tt3::util::NotImplementedError();
}

void HelpClient::showSearch()
{
    throw tt3::util::NotImplementedError();
}

void HelpClient::showTopic(const QString & /*topic*/)
{
    throw tt3::util::NotImplementedError();
}

//////////
//  Implementation helpers
HelpClient::_Impl * HelpClient::_impl()
{
    static _Impl impl;
    return &impl;
}

//  End of tt3-gui/HelpClient.cpp
