//
//  tt3-util/SystemShutdownHandler.cpp - tt3::util::SystemShutdownHandler class implementation
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
#include "tt3-util/API.hpp"
using namespace tt3::util;
#if defined(Q_OS_WINDOWS)
    #include <windows.h>
#elif defined(Q_OS_LINUX)
    #include <signal.h>
#else
    #error Unsupported platform
#endif

struct SystemShutdownHandler::_Impl
#if defined(Q_OS_WINDOWS)
    :   public QAbstractNativeEventFilter
#elif defined(Q_OS_LINUX)
#else
    #error Unsupported platform
#endif
{
    _Impl()
    {
    }

    ~_Impl()
    {   //  Singleton is destroyed
#if defined(Q_OS_WINDOWS)
        if (auto app =
            dynamic_cast<QApplication*>(QCoreApplication::instance()))
        {   //  GUI applications ONLY!
            app->removeNativeEventFilter(this);
        }
#elif defined(Q_OS_LINUX)
#else
    #error Unsupported platform
#endif
    }


    Mutex               guard;
    bool                active = false;
    bool                shutdownInProgress = false;
    //  Shutdown hooks + params - parallel lists
    QList<ShutdownHook> shutdownHooks;
    QList<void*>        shutdownHookParams;

#if defined(Q_OS_WINDOWS)
    //////////
    //  QAbstractNativeEventFilter
public:
    virtual bool    nativeEventFilter(
                            const QByteArray & eventType,
                            void * message,
                            qintptr * result
                    ) override
    {
        if (eventType == "windows_generic_MSG")
        {
            MSG * msg = static_cast<MSG*>(message);
            if (msg->message == WM_ENDSESSION)
            {
                qInfo() << "Received WM_ENDSESSION. Performing cleanup and quitting.";
                shutdownInProgress = true;
                //  Call all shutdown handlers ONCE
                auto hh = shutdownHooks;
                auto pp = shutdownHookParams;
                shutdownHooks.clear();
                shutdownHookParams.clear();
                Q_ASSERT(hh.size() == pp.size());
                for (int i = 0; i < hh.size(); i++)
                {
                    try
                    {
                        (*(hh[i]))(pp[i]);
                    }
                    catch (...)
                    {   //  OOPS! Must suppress!
                    }
                }
                //  Finish with the message
                qApp->quit();   //  ...to stop the event loop
                *result = 0;    //  ...return value doesn't matter much for WM_ENDSESSION
                return true;    //  ...event handled
            }
        }
        return false;   //  Let Qt handle other events
    }
#elif defined(Q_OS_LINUX)
#else
    #error Unsupported platform
#endif
};

//////////
//  Operations
void SystemShutdownHandler::addShutdownHook(
        ShutdownHook shutdownHook,
        void * cbData
    )
{
    Q_ASSERT(shutdownHook != nullptr);

    _Impl * impl = _impl();
    Lock _(impl->guard);

    removeShutdownHook(shutdownHook, cbData);   //  Mutex is recursive!
    impl->shutdownHooks.append(shutdownHook);
    impl->shutdownHookParams.append(cbData);
}

void SystemShutdownHandler::removeShutdownHook(
        ShutdownHook shutdownHook,
        void * cbData
    )
{
    Q_ASSERT(shutdownHook != nullptr);

    _Impl * impl = _impl();
    Lock _(impl->guard);

    for (int i = 0; i < impl->shutdownHooks.size(); i++)
    {
        if (impl->shutdownHooks[i] == shutdownHook &&
            impl->shutdownHookParams[i] == cbData)
        {   //  This one!
            impl->shutdownHooks.remove(i);
            impl->shutdownHookParams.remove(i);
            break;  //  We can't have more than one match!
        }
    }
}

void SystemShutdownHandler::activate()
{
    _Impl * impl = _impl();
    Lock _(impl->guard);

    if (!impl->active)
    {
#if defined(Q_OS_WINDOWS)
        if (auto app =
            dynamic_cast<QApplication*>(QCoreApplication::instance()))
        {   //  GUI applications ONLY!
            app->installNativeEventFilter(impl);
        }
#elif defined(Q_OS_LINUX)
        signal(SIGTERM, _sigtermHandler);
#else
    #error Unsupported platform
#endif
        impl->active = true;
    }
}

void SystemShutdownHandler::deactivate()
{
    _Impl * impl = _impl();
    Lock _(impl->guard);

    if (impl->active)
    {
#if defined(Q_OS_WINDOWS)
        if (auto app =
            dynamic_cast<QApplication*>(QCoreApplication::instance()))
        {   //  GUI applications ONLY!
            app->removeNativeEventFilter(impl);
        }
#elif defined(Q_OS_LINUX)
        signal(SIGTERM, SIG_DFL);
#else
    #error Unsupported platform
#endif
        impl->active = false;
    }
}

bool SystemShutdownHandler::isShutdownInProgress()
{
    _Impl * impl = _impl();
    Lock _(impl->guard);

    return impl->shutdownInProgress;
}

//////////
//  Implementation helpers
SystemShutdownHandler::_Impl * SystemShutdownHandler::_impl()
{
    static _Impl impl;
    return &impl;
}

//  Helpers
#ifdef Q_OS_LINUX
void SystemShutdownHandler::_sigtermHandler(int)
{
    _Impl * impl = _impl();

    qInfo() << "Received SIGTERM. Performing cleanup and quitting.";
    impl->shutdownInProgress = true;
    //  Call all shutdown handlers ONCE
    auto hh = impl->shutdownHooks;
    auto pp = impl->shutdownHookParams;
    impl->shutdownHooks.clear();
    impl->shutdownHookParams.clear();
    Q_ASSERT(hh.size() == pp.size());
    for (int i = 0; i < hh.size(); i++)
    {
        try
        {
            (*(hh[i]))(pp[i]);
        }
        catch (...)
        {   //  OOPS! Must suppress!
        }
    }
    //  Finish with the message
    qApp->quit();   //  ...to stop the event loop
}
#endif

//  End of tt3-util/SystemShutdownHandler.cpp
