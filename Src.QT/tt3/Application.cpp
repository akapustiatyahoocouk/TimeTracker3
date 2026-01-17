//
//  tt3/Application.cpp - tt3::Application class implementation
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
#include "tt3/API.hpp"
using namespace tt3;
#if defined(Q_OS_WINDOWS)
    #include <windows.h>
    #pragma comment(lib, "user32.lib")
#elif defined(Q_OS_LINUX)
    #include <X11/extensions/scrnsaver.h>
#endif

//////////
//  Construction/destruction
Application::Application(int & argc, char ** argv)
    :   QApplication(argc, argv),
        _stateActivityChecker(this)
{
    setApplicationName(tt3::util::ProductInformation::applicationName());
    setApplicationVersion(tt3::util::ProductInformation::applicationVersion().toString());
    //  Don't setApplicationDisplayName() - will mess up dialog titles!
    setOrganizationName(tt3::util::ProductInformation::organizationName());
    setOrganizationDomain(tt3::util::ProductInformation::organizationDomain());

    setQuitOnLastWindowClosed(false);

    //  Prepare to handle system shutdown
    tt3::util::SystemShutdownHandler::addShutdownHook(_systemShutdownHook, this);
    tt3::util::SystemShutdownHandler::activate();

    //  Start checking for runaway activities
    connect(&_stateActivityChecker,
            &QTimer::timeout,
            this,
            &Application::_staleActivityCheckerTimeout);
}

Application::~Application()
{
    //  Stop handling system shutdown
    tt3::util::SystemShutdownHandler::deactivate();
    tt3::util::SystemShutdownHandler::removeShutdownHook(_systemShutdownHook, this);
}

//////////
//  QGuiApplication
bool Application::notify(QObject * receiver, QEvent * event)
{
    static Component::Resources *const resources = Component::Resources::instance();   //  idempotent

    try
    {
        return QApplication::notify(receiver, event);
    }
    catch (const tt3::util::Exception & ex)
    {
        qCritical() << ex;
        auto parent =
            (tt3::gui::theCurrentSkin != nullptr) ?
                tt3::gui::theCurrentSkin->mainWindow() :
                nullptr;
        if (!tt3::util::SystemShutdownHandler::isShutdownInProgress())
        {   //  Suppress dialog if system shutdown is in progress
            tt3::gui::ErrorDialog::show(parent, ex);
        }
        return false;
    }
    catch (const tt3::util::Error & ex)
    {
        qCritical() << ex;
        auto parent =
            (tt3::gui::theCurrentSkin != nullptr) ?
                tt3::gui::theCurrentSkin->mainWindow() :
                nullptr;
        if (!tt3::util::SystemShutdownHandler::isShutdownInProgress())
        {   //  Suppress dialog if system shutdown is in progress
            tt3::gui::ErrorDialog::show(parent, ex);
        }
        return false;
    }
    catch (const std::exception & ex)
    {   //  Just in case
        qCritical() << ex.what();
        auto parent =
            (tt3::gui::theCurrentSkin != nullptr) ?
                tt3::gui::theCurrentSkin->mainWindow() :
                nullptr;
        if (!tt3::util::SystemShutdownHandler::isShutdownInProgress())
        {   //  Suppress dialog if system shutdown is in progress
            tt3::gui::ErrorDialog::show(parent, ex.what());
        }
        return false;
    }
    catch (...)
    {
        qCritical() << resources->string(RSID(Errors), RID(UncaughtException));
        return false;
    }
}

//////////
//  QApplication
int Application::exec()
{
    try
    {
        _initialize();
        int exitCode = QApplication::exec();    //  may throw
        _cleanup();
        return exitCode;
    }
    catch (...)
    {   //  OOPS! Cleanup & re-throw.
        _cleanup();
        throw;
    }
}

//////////
//  Implementation helpers
void Application::_prepareForLogging()
{
    _logFileNameBase =
        QDir(QStandardPaths::writableLocation(QStandardPaths::TempLocation))
            .filePath("tt3-!.log");
    qInfo() << "_logFileNameBase = " + _logFileNameBase;
    qInstallMessageHandler(_logMessageOutput);
}

void Application::_selectActiveStyle()
{
    tt3::gui::IStyle * initialStyle =
        tt3::gui::StyleManager::find(tt3::gui::Component::Settings::instance()->activeStyle);
    if (initialStyle == nullptr)
    {   //  Keep the default one
        if (QApplication::style() != nullptr)
        {   //  Use that one
            QString styleName = QApplication::style()->name().toLower();
            initialStyle = tt3::gui::StyleManager::find(tt3::util::Mnemonic(styleName));
        }
    }
    if (initialStyle != nullptr)
    {
        tt3::gui::theCurrentStyle = initialStyle;
    }
}

void Application::_selectActiveTheme()
{
    tt3::gui::ITheme * initialTheme =
        tt3::gui::ThemeManager::find(tt3::gui::Component::Settings::instance()->activeTheme);
    tt3::gui::theCurrentTheme =
        (initialTheme != nullptr) ?
            initialTheme :
            tt3::gui::StandardThemes::System::instance();
}

void Application::_selectActiveSkin()
{
    static Component::Resources *const resources = Component::Resources::instance();   //  idempotent

    tt3::gui::ISkin * initialSkin =
        tt3::gui::SkinManager::find(tt3::gui::Component::Settings::instance()->activeSkin);
    //  Use a default skin ?
    for (tt3::gui::ISkin * skin : tt3::gui::SkinManager::all())
    {
        if (skin->isDefault() && initialSkin == nullptr)
        {
            initialSkin = skin;
            break;
        }
    }
    //  Use any available skin ?
    for (tt3::gui::ISkin * skin : tt3::gui::SkinManager::all())
    {
        if (initialSkin == nullptr)
        {
            initialSkin = skin;
            break;
        }
    }
    //  A skin MUST be selected
    if (initialSkin == nullptr)
    {
        tt3::gui::ErrorDialog::show(
            resources->string(RSID(Errors), RID(NoSkinsDefined)));
        ::exit(EXIT_FAILURE);
    }
    tt3::gui::theCurrentSkin = initialSkin;
    initialSkin->activate();
    //  If the skin was selected "by default", update the settings
    tt3::gui::Component::Settings::instance()->activeSkin = initialSkin->mnemonic();
}

void Application::_initialize()
{
    if (_initialized)
    {   //  Nothing to do
        return;
    }
    _initialized = true;

    //  Core (preloaded) components can be initialzed
    //  NOW - we need these settings for SplashScreen
    //  localization. ComponentManager takes care of
    //  not initializing the same Component more than once.
    tt3::util::ComponentManager::initializeComponents();
    tt3::util::ComponentManager::loadComponentSettings();
    tt3::util::theCurrentLocale = tt3::gui::Component::Settings::instance()->uiLocale;
    //  In case theme/style were one of the tt3-gui predefined ones...
    _selectActiveStyle();
    _selectActiveTheme();

    //  Show splash screen unless disabled
    tt3::gui::SplashScreen splashScreen;
    splashScreen.showStartupProgress("", "", 0.0);
    if (tt3::gui::Component::Settings::instance()->useSplashScreen)
    {
        splashScreen.show();
    }
    QDateTime closeSplashScreenAt =
        QDateTime::currentDateTimeUtc().addMSecs(tt3::gui::SplashScreen::PreferredStartupDurationMs);

    _prepareForLogging();

    QPixmap pm;
    pm.load(":/tt3/Resources/Images/Misc/Tt3Large.png");
    QIcon ic(pm);
    QGuiApplication::setWindowIcon(ic);
    //QGuiApplication::setApplicationName(tt3::util::ProductInformation::applicationName());
    //QGuiApplication::setApplicationVersion(tt3::util::ProductInformation::applicationVersion().toString());
    //QGuiApplication::setApplicationDisplayName(tt3::util::ProductInformation::applicationDisplayName());

    //  Optional components must be discovered dynamially
    tt3::util::ComponentManager::discoverComponents(
        [&](auto a, auto c, auto r)
        {
            splashScreen.showStartupProgress(a, c, r * 0.5f);
        });
    tt3::util::ComponentManager::initializeComponents(
        [&](auto a, auto c, auto r)
        {
            splashScreen.showStartupProgress(a, c, 0.5f + r * 0.5f);
        });
    tt3::util::ComponentManager::loadComponentSettings();

    //  Close splash screen BEFORE UI (skin) is activated
    if (splashScreen.isVisible())
    {   //  No point in delays if invisible
        while (QDateTime::currentDateTimeUtc() < closeSplashScreenAt)
        {
            QCoreApplication::processEvents();
        }
    }
    splashScreen.hide();

    _selectActiveStyle();
    _selectActiveTheme();
    _selectActiveSkin();

    //  Perform initial login
    tt3::gui::LoginDialog loginDialog(
        tt3::gui::theCurrentSkin->mainWindow(),
        tt3::gui::Component::Settings::instance()->rememberLastLogin ?
            tt3::gui::Component::Settings::instance()->lastLogin.value() :
            "");
    if (loginDialog.doModal() != tt3::gui::LoginDialog::Result::Ok)
    {   //  No need to cleanup - nothing has
        //  chnaged in application's settings
        splashScreen.hide();
        ::exit(0);
    }
    tt3::gui::theCurrentCredentials = loginDialog.credentials();

    //  Can we open the last workspace now ?
    if (tt3::gui::Component::Settings::instance()->loadLastWorkspaceAtStartup)
    {
        tt3::ws::WorkspaceAddressesList mru =
            tt3::ws::Component::Settings::instance()->recentWorkspaces;
        if (mru.size() != 0)
        {
            try
            {
                tt3::ws::Workspace workspace =
                    mru[0]->workspaceType()->openWorkspace(mru[0], tt3::ws::OpenMode::Default);
                if (workspace->canAccess(tt3::gui::theCurrentCredentials))
                {
                    tt3::gui::theCurrentWorkspace.swap(workspace);
                    _adjustUiLocale();  //  in case logged-in User has custom UI locale
                }
                else
                {
                    workspace->close();
                }
            }
            catch (const tt3::util::Exception & ex)
            {   //  OOPS! Report
                qCritical() << ex;
                tt3::gui::ErrorDialog::show(tt3::gui::theCurrentSkin->mainWindow(), ex);
            }
        }
    }

    //  Look out for stale activities
    _stateActivityChecker.start(60 * 1000); //  once every minute

    //  The "current locale" may need to be adjusted:
    //  1.  If the "current workspace" changes.
    //  2.  If the "current credentials" change.
    //  3.  If the User currently logged in is modified.
    //  4.  If the "current UI locale" global setting changes.
    //  This is because Users can specify UI locale explicitly.
    connect(&tt3::gui::theCurrentWorkspace,
            &tt3::gui::CurrentWorkspace::changed,
            this,
            &Application::_currentWorkspaceChanged,
            Qt::ConnectionType::QueuedConnection);
    connect(&tt3::gui::theCurrentCredentials,
            &tt3::gui::CurrentCredentials::changed,
            this,
            &Application::_currentCredentialsChanged,
            Qt::ConnectionType::QueuedConnection);
    connect(&tt3::gui::Component::Settings::instance()->uiLocale,
            &tt3::util::AbstractSetting::valueChanged,
            this,
            &Application::_uiLocaleSettingValueChanged,
            Qt::ConnectionType::QueuedConnection);
    if (tt3::gui::theCurrentWorkspace != nullptr)
    {
        connect(tt3::gui::theCurrentWorkspace.operator ->().get(),
                &tt3::ws::WorkspaceImpl::objectModified,
                this,
                &Application::_workspaceObjectModified,
                Qt::ConnectionType::QueuedConnection);
    }
}

void Application::_cleanup()
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(Application));

    if (!_initialized)
    {   //  Nothing to do
        return;
    }
    _initialized = false;

    //  Stop auto-adjusting the "current" locale
    disconnect(&tt3::gui::theCurrentWorkspace,
               &tt3::gui::CurrentWorkspace::changed,
               this,
               &Application::_currentWorkspaceChanged);
    disconnect(&tt3::gui::theCurrentCredentials,
               &tt3::gui::CurrentCredentials::changed,
               this,
               &Application::_currentCredentialsChanged);
    disconnect(&tt3::gui::Component::Settings::instance()->uiLocale,
               &tt3::util::AbstractSetting::valueChanged,
               this,
               &Application::_uiLocaleSettingValueChanged);
    if (tt3::gui::theCurrentWorkspace != nullptr)
    {
        disconnect(tt3::gui::theCurrentWorkspace.operator ->().get(),
                   &tt3::ws::WorkspaceImpl::objectModified,
                   this,
                   &Application::_workspaceObjectModified);
    }

    //  Stop look after stale activities
    _stateActivityChecker.stop();

    //  If there is a "current activity", record & stop it.
    try
    {
        tt3::gui::theCurrentActivity.replaceWith(nullptr);
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Report!
        qCritical() << ex;
        if (!tt3::util::SystemShutdownHandler::isShutdownInProgress())
        {   //  No modal popups during system shutdown
            tt3::gui::ErrorDialog::show(ex);
        }
    }

    //  If there's a "current" workspace, close it
    tt3::ws::Workspace currentWorkspace;
    tt3::gui::theCurrentWorkspace.swap(currentWorkspace);
    if (currentWorkspace != nullptr)
    {
        try
        {
            currentWorkspace->close();
        }
        catch (const tt3::util::Exception & ex)
        {   //  OOPS! Report!
            qCritical() << ex;
            if (!tt3::util::SystemShutdownHandler::isShutdownInProgress())
            {   //  No modal popups during system shutdown
                tt3::gui::ErrorDialog::show(ex);
            }
        }
    }

    //  There MUST be a "current" skin, so deactivate it
    Q_ASSERT(tt3::gui::theCurrentSkin != nullptr);
    tt3::gui::theCurrentSkin->deactivate();
    tt3::gui::theCurrentSkin = nullptr;

    //  Deinitialize all Components
    tt3::util::ComponentManager::saveComponentSettings();
    tt3::util::ComponentManager::deinitializeComponents();
}

void Application::_adjustUiLocale()
{
    //  If there IS a "current workspace" and the "current
    //  credentials" grant access to it AND the correesponding
    //  User specified a custom UI locale, then use that UI locale
    try
    {
        if (tt3::gui::theCurrentWorkspace != nullptr)   //  may throw
        {
            tt3::ws::Credentials credentials = tt3::gui::theCurrentCredentials;
            auto user = tt3::gui::theCurrentWorkspace->login(credentials)->user(credentials);    //  may throw
            auto uiLocale = user->uiLocale(credentials);    //  may throw
            if (uiLocale.has_value())
            {   //  Use this one!
                tt3::util::theCurrentLocale = uiLocale.value();
                return;
            }
        }
    }
    catch (...)
    {   //  OOPS! Suppress, though Suppress
    }
    //  Otherwise fall back to the persistent setting
    tt3::util::theCurrentLocale =
        tt3::gui::Component::Settings::instance()->uiLocale;
}

void Application::_systemShutdownHook(void * cbData)
{
    qInfo() << "Application::_systemShutdownHook() called";
    //  Must cleanup on system shutdown.
    //  This specifically saves the "current activity"
    //  and closes the "current workspace", flushing all changes
    auto app = static_cast<Application*>(cbData);
    app->_cleanup();
}

//////////
//  Logging
void Application::_logMessageOutput(
        QtMsgType type,
        const QMessageLogContext & context,
        const QString & msg
    )
{
    QByteArray localMsg = msg.toLocal8Bit();
    QString txt;

    //  Determine message type and format accordingly
    switch (type)
    {
        case QtDebugMsg:
            txt = QString("DEBUG: %1").arg(localMsg.constData());
            break;
        case QtInfoMsg:
            txt = QString("INFO: %1").arg(localMsg.constData());
            break;
        case QtWarningMsg:
            txt = QString("WARNING: %1").arg(localMsg.constData());
            break;
        case QtCriticalMsg:
            txt = QString("CRITICAL: %1").arg(localMsg.constData());
            break;
        case QtFatalMsg:
            txt = QString("FATAL: %1").arg(localMsg.constData());
            break;
        default:    //  Be defensive!
            txt = QString("UNEXPECTED: %1").arg(localMsg.constData());
            break;
    }

    // Add timestamp and context (file/line number) to the log message
    QString contextInfo = QString(" (%1:%2, %3)").arg(context.file).arg(context.line).arg(context.function);
    QString currentDateTime = QDateTime::currentDateTimeUtc().toString("yyyy-MM-dd hh:mm:ss") + " UTC";
    //QString finalMessage = QString("%1 %2 %3\n").arg(currentDateTime, txt, contextInfo);
    QString finalMessage = QString("%1 %2\n").arg(currentDateTime, txt);

    //  Open the log file and write the message.
    //  Opening/closing the log file on every message
    //  is SLOW, but we don't expect many of those
    QFile logFile(_logFileNameBase.replace("!", QDateTime::currentDateTimeUtc().toString("yyyy-MM-dd")));
    if (logFile.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        QTextStream logStream(&logFile);
        logStream << finalMessage;
        logStream.flush();
        logFile.close();
    }

    //  Echo to stdout
    QString stdoutMessage = "\n" + txt + "\nAT " + contextInfo + "\n";
#ifdef Q_OS_WINDOWS
    OutputDebugStringW(stdoutMessage.toStdWString().c_str());
#else   //  Linux, etc.
    QTextStream(stdout) << stdoutMessage;
#endif
}

//////////
//  Signal handlers
void Application::_staleActivityCheckerTimeout()
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(Application));

    tt3::ws::Activity activity = tt3::gui::theCurrentActivity;
    if (activity == nullptr)
    {   //  Nothing to check
        return;
    }
#if defined(Q_OS_WINDOWS)
    LASTINPUTINFO lii;
    lii.cbSize = sizeof(LASTINPUTINFO);
    if (!GetLastInputInfo(&lii))
    {   //  OOPS! Can't proceed
        return;
    }
    DWORD currentTickCount = GetTickCount();
    DWORD idleTimeMs =
        (lii.dwTime <= currentTickCount) ?
            (currentTickCount - lii.dwTime) :
            currentTickCount;
#elif defined(Q_OS_LINUX)
    unsigned long idleTimeMs = 0;
    if (Display * display = XOpenDisplay(NULL))
    {
        if (XScreenSaverInfo * info = XScreenSaverAllocInfo())
        {
            if (XScreenSaverQueryInfo(display, DefaultRootWindow(display), info))
            {
                idleTimeMs = info->idle;
            }   //  TODO else wayland ?
            XFree(info);
        }
        XCloseDisplay(display);
    }
#else
    #error Unsupported OS
#endif
    try
    {
        tt3::ws::Credentials credentials = tt3::gui::theCurrentCredentials;
        auto account = activity->workspace()->login(credentials);
        auto timeout = activity->timeout(credentials);
        qint64 overdueMs = 0;   //  Activity timeout is not treated as overdue
        if (!timeout.has_value())
        {   //  Go to the User for defaults...
            timeout = account->user(credentials)->inactivityTimeout(credentials);
            if (timeout.has_value())
            {   //  ...but count the User's timeout towards
                //  overdue time
                overdueMs = timeout.value().asMinutes() * 60 * 1000;
            }
        }
        if (timeout.has_value() &&
            int(idleTimeMs / (60 * 1000)) >= timeout.value().asMinutes())
        {   //  Must stop the "current" activity...
            tt3::gui::theCurrentActivity.quietlyReplaceWith(nullptr, overdueMs);
            //  ...and log an Event about that
            int h = int(idleTimeMs / (60 * 60 * 1000));
            int m = int(idleTimeMs / (60 * 1000) % 60);
            QString summary =
                (h != 0) ?
                    rr.string(RID(StaleActivityStoppedHM),
                              activity->type()->displayName(),
                              activity->displayName(credentials),
                              h, m) :
                    rr.string(RID(StaleActivityStoppedH),
                              activity->type()->displayName(),
                              activity->displayName(credentials),
                              m);
            account->createEvent(   //  may throw
                credentials,
                QDateTime::currentDateTimeUtc(),
                summary,
                tt3::ws::Activities{activity});
            return;
        }

    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Log, but otherwise ignore
        qCritical() << ex;
    }
    catch (...)
    {   //  Don't let anhything through
    }
}

void Application::_currentWorkspaceChanged(
        tt3::ws::Workspace before,
        tt3::ws::Workspace after
    )
{
    if (before != nullptr)
    {   //  Stop listening to object changes
        disconnect(before.get(),
                   &tt3::ws::WorkspaceImpl::objectModified,
                   this,
                   &Application::_workspaceObjectModified);
    }
    if (after != nullptr)
    {   //  Start listening to object changes
        connect(after.get(),
                &tt3::ws::WorkspaceImpl::objectModified,
                this,
                &Application::_workspaceObjectModified,
                Qt::ConnectionType::QueuedConnection);
    }
    _adjustUiLocale();
}

void Application::_currentCredentialsChanged(
        tt3::ws::Credentials /*before*/,
        tt3::ws::Credentials /*after*/
    )
{
    _adjustUiLocale();
}

void Application::_workspaceObjectModified(
        tt3::ws::ObjectModifiedNotification notification
    )
{
    if (notification.objectType() == tt3::ws::ObjectTypes::User::instance())
    {   //  User's "UI locale" may have changed
        _adjustUiLocale();
    }
}

void Application::_uiLocaleSettingValueChanged()
{
    _adjustUiLocale();
}

//  End of tt3/Application.cpp
