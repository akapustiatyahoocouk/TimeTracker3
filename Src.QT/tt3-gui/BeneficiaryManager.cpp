//
//  tt3-gui/BeneficiaryManager.cpp - tt3::gui::BeneficiaryManager class implementation
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
#include "ui_BeneficiaryManager.h"
using namespace tt3::gui;

namespace tt3::gui
{
    extern CurrentTheme theCurrentTheme;
    extern CurrentCredentials theCurrentCredentials;
    extern CurrentWorkspace theCurrentWorkspace;
}

//////////
//  Construction/destruction
BeneficiaryManager::BeneficiaryManager(
        QWidget * parent
    ) : QWidget(parent),
        //  Implementation
        _workspace(theCurrentWorkspace),
        _credentials(theCurrentCredentials),
        //  Controls
        _ui(new Ui::BeneficiaryManager)
{
    _ui->setupUi(this);
    _decorations = TreeWidgetDecorations(_ui->beneficiariesTreeWidget);
    _applyCurrentLocale();

    //  Theme change means widget decorations change
    connect(&theCurrentTheme,
            &CurrentTheme::changed,
            this,
            &BeneficiaryManager::_currentThemeChanged,
            Qt::ConnectionType::QueuedConnection);

    //  Locale change requires UI translation
    connect(&tt3::util::theCurrentLocale,
            &tt3::util::CurrentLocale::changed,
            this,
            &BeneficiaryManager::_currentLocaleChanged,
            Qt::ConnectionType::QueuedConnection);

    //  Must listen to delayed refresh requests
    connect(this,
            &BeneficiaryManager::refreshRequested,
            this,
            &BeneficiaryManager::_refreshRequested,
            Qt::ConnectionType::QueuedConnection);

    //  Start listening for change notifications
    //  on the currently "viewed" Workspace
    _startListeningToWorkspaceChanges();
}

BeneficiaryManager::~BeneficiaryManager()
{
    _stopListeningToWorkspaceChanges();
    delete _ui;
}

//////////
//  Operaions
tt3::ws::Workspace BeneficiaryManager::workspace() const
{
    return _workspace;
}

void BeneficiaryManager::setWorkspace(tt3::ws::Workspace workspace)
{
    if (workspace != _workspace)
    {
        _stopListeningToWorkspaceChanges();
        _workspace = workspace;
        _startListeningToWorkspaceChanges();
        requestRefresh();
    }
}

tt3::ws::Credentials BeneficiaryManager::credentials() const
{
    return _credentials;
}

void BeneficiaryManager::setCredentials(const tt3::ws::Credentials & credentials)
{
    if (credentials != _credentials)
    {
        _credentials = credentials;
        requestRefresh();
    }
}

void BeneficiaryManager::refresh()
{
    static const QIcon viewBeneficiaryIcon(":/tt3-gui/Resources/Images/Actions/ViewBeneficiaryLarge.png");
    static const QIcon modifyBeneficiaryIcon(":/tt3-gui/Resources/Images/Actions/ModifyBeneficiaryLarge.png");
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(BeneficiaryManager));

    //  We don't want a refresh() to trigger a recursive refresh()!
    if (auto _ = RefreshGuard(_refreshUnderway)) //  Don't recurse!
    {
        try
        {
            if (_workspace == nullptr || !_credentials.isValid() ||
                !_workspace->isOpen() ||
                !_workspace->canAccess(_credentials))   //  may throw
            {   //  Nothing to show...
                _clearAndDisableAllControls();
                return;
            }
        }
        catch (const tt3::util::Exception & ex)
        {   //  OOPS! No point in proceesing.
            qCritical() << ex;
            _clearAndDisableAllControls();
            return;
        }

        //  Otherwise some controls are always enabled...
        _ui->filterLabel->setEnabled(true);
        _ui->filterLineEdit->setEnabled(true);
        _ui->beneficiariesTreeWidget->setEnabled(true);

        //  ...while others are enabled based on current
        //  selection and permissions granted by Credentials
        _WorkspaceModel workspaceModel =
            _createWorkspaceModel(_workspace, _credentials, _decorations);
        QString filter = _ui->filterLineEdit->text().trimmed();
        if (!filter.isEmpty())
        {
            _filterItems(workspaceModel, filter, _decorations);
        }
        _refreshWorkspaceTree(_ui->beneficiariesTreeWidget, workspaceModel);

        tt3::ws::Beneficiary currentBeneficiary = _currentBeneficiary();
        bool readOnly = _workspace->isReadOnly();
        try
        {
            _ui->createBeneficiaryPushButton->setEnabled(
                !readOnly &&
                _workspace->grantsAny(  //  may throw
                    _credentials,
                    tt3::ws::Capability::Administrator |
                        tt3::ws::Capability::ManageBeneficiaries));
        }
        catch (const tt3::util::Exception & ex)
        {   //  OOPS! Log & disable
            qCritical() << ex;
            _ui->createBeneficiaryPushButton->setEnabled(false);
        }
        _ui->modifyBeneficiaryPushButton->setEnabled(
            currentBeneficiary != nullptr);
        try
        {
            _ui->destroyBeneficiaryPushButton->setEnabled(
                !readOnly &&
                currentBeneficiary != nullptr &&
                currentBeneficiary->canDestroy(_credentials));    //  may throw
        }
        catch (const tt3::util::Exception & ex)
        {   //  OOPS! Log & disable
            qCritical() << ex;
            _ui->destroyBeneficiaryPushButton->setEnabled(false);
        }

        //  Some buttons need to be adjusted for ReadOnoly mode
        try
        {
            if (currentBeneficiary != nullptr &&
                !currentBeneficiary->workspace()->isReadOnly() &&
                currentBeneficiary->canModify(_credentials))  //  may throw
            {   //  RW
                _ui->modifyBeneficiaryPushButton->setIcon(modifyBeneficiaryIcon);
                _ui->modifyBeneficiaryPushButton->setText(
                    rr.string(RID(ModifyBeneficiaryPushButton)));
            }
            else
            {   //  RO
                _ui->modifyBeneficiaryPushButton->setIcon(viewBeneficiaryIcon);
                _ui->modifyBeneficiaryPushButton->setText(
                    rr.string(RID(ViewBeneficiaryPushButton)));
            }
        }
        catch (const tt3::util::Exception & ex)
        {   //  OOPS! Log & simulate RO
            qCritical() << ex;
            _ui->modifyBeneficiaryPushButton->setIcon(viewBeneficiaryIcon);
            _ui->modifyBeneficiaryPushButton->setText(
                rr.string(RID(ViewBeneficiaryPushButton)));
        }
    }
}

void BeneficiaryManager::requestRefresh()
{
    emit refreshRequested();
}

//////////
//  View model
auto BeneficiaryManager::_createWorkspaceModel(
        tt3::ws::Workspace workspace,
        const tt3::ws::Credentials & credentials,
        const TreeWidgetDecorations & decorations
    ) -> _WorkspaceModel
{
    auto workspaceModel = std::make_shared<_WorkspaceModelImpl>();
    try
    {
        for (const auto & beneficiary : workspace->beneficiaries(credentials))    //  may throw
        {
            workspaceModel->beneficiaryModels.append(
                _createBeneficiaryModel(beneficiary, credentials, decorations));
        }
        std::sort(
            workspaceModel->beneficiaryModels.begin(),
            workspaceModel->beneficiaryModels.end(),
            [&](auto a, auto b)
            { return a->text < b->text; });
    }
    catch (const tt3::util::Exception & ex)
    {
        qCritical() << ex;
        workspaceModel->beneficiaryModels.clear();
    }
    return workspaceModel;
}

auto BeneficiaryManager::_createBeneficiaryModel(
        tt3::ws::Beneficiary beneficiary,
        const tt3::ws::Credentials & credentials,
        const TreeWidgetDecorations & decorations
    ) -> _BeneficiaryModel
{
    static const QIcon errorIcon(":/tt3-gui/Resources/Images/Misc/ErrorSmall.png");

    auto beneficiaryModel = std::make_shared<_BeneficiaryModelImpl>(beneficiary);
    try
    {
        beneficiaryModel->text = beneficiary->displayName(credentials);
        beneficiaryModel->icon = beneficiary->type()->smallIcon();
        beneficiaryModel->brush = decorations.itemForeground;
        beneficiaryModel->font = decorations.itemFont;
        beneficiaryModel->tooltip = beneficiary->description(credentials).trimmed();
    }
    catch (const tt3::util::Exception & ex)
    {
        qCritical() << ex;
        beneficiaryModel->text = ex.errorMessage();
        beneficiaryModel->icon = errorIcon;
        beneficiaryModel->font = decorations.itemFont;
        beneficiaryModel->brush = decorations.errorItemForeground;
        beneficiaryModel->tooltip = ex.errorMessage();
    }
    return beneficiaryModel;
}

void BeneficiaryManager::_filterItems(
        _WorkspaceModel workspaceModel,
        const QString & filter,
        const TreeWidgetDecorations & decorations
    )
{
    Q_ASSERT(!filter.isEmpty());

    for (qsizetype i = workspaceModel->beneficiaryModels.size() - 1; i >= 0; i--)
    {
        _BeneficiaryModel beneficiaryModel = workspaceModel->beneficiaryModels[i];
        if (beneficiaryModel->text.indexOf(filter, 0, Qt::CaseInsensitive) != -1)
        {   //  Item matches the filter - mark it as a match
            beneficiaryModel->brush = decorations.filterMatchItemForeground;
        }
        else
        {   //  Item does not match the filter
            workspaceModel->beneficiaryModels.removeAt(i);
        }
    }
}

void BeneficiaryManager::_refreshWorkspaceTree(
        QTreeWidget * beneficiariesTreeWidget,
        _WorkspaceModel workspaceModel
    )
{
    Q_ASSERT(beneficiariesTreeWidget != nullptr);
    Q_ASSERT(workspaceModel != nullptr);

    //  Make sure the "beneficiaries" tree contains
    //  a proper number of root (Beneficiary) items...
    while (beneficiariesTreeWidget->topLevelItemCount() < workspaceModel->beneficiaryModels.size())
    {   //  Too few root (Beneficiary) items
        beneficiariesTreeWidget->addTopLevelItem(new QTreeWidgetItem());
    }
    while (beneficiariesTreeWidget->topLevelItemCount() > workspaceModel->beneficiaryModels.size())
    {   //  Too many root (Beneficiary) items
        delete beneficiariesTreeWidget->takeTopLevelItem(
            beneficiariesTreeWidget->topLevelItemCount() - 1);
    }
    //  ...and that each top-level item represents
    //  a proper Beneficiary and has proper children
    for (int i = 0; i < workspaceModel->beneficiaryModels.size(); i++)
    {
        _refreshBeneficiaryItem(
            beneficiariesTreeWidget->topLevelItem(i),
            workspaceModel->beneficiaryModels[i]);
    }
}

void BeneficiaryManager::_refreshBeneficiaryItem(
    QTreeWidgetItem * beneficiaryItem,
    _BeneficiaryModel beneficiaryModel
    )
{
    Q_ASSERT(beneficiaryItem != nullptr);
    Q_ASSERT(beneficiaryModel != nullptr);

    //  Refresh Beneficiary item properties
    beneficiaryItem->setText(0, beneficiaryModel->text);
    beneficiaryItem->setIcon(0, beneficiaryModel->icon);
    beneficiaryItem->setForeground(0, beneficiaryModel->brush);
    beneficiaryItem->setFont(0, beneficiaryModel->font);
    beneficiaryItem->setToolTip(0, beneficiaryModel->tooltip);
    beneficiaryItem->setData(0, Qt::ItemDataRole::UserRole, QVariant::fromValue(beneficiaryModel->beneficiary));
    //  There will be no further children
    Q_ASSERT(beneficiaryItem->childCount() == 0);
}

//////////
//  Implementation helpers
tt3::ws::Beneficiary BeneficiaryManager::_currentBeneficiary()
{
    QTreeWidgetItem * item = _ui->beneficiariesTreeWidget->currentItem();
    return (item != nullptr) ?
               item->data(0, Qt::ItemDataRole::UserRole).value<tt3::ws::Beneficiary>() :
               nullptr;
}

void BeneficiaryManager::_setCurrentBeneficiary(tt3::ws::Beneficiary beneficiary)
{
    for (int i = 0; i < _ui->beneficiariesTreeWidget->topLevelItemCount(); i++)
    {
        QTreeWidgetItem * beneficiaryItem = _ui->beneficiariesTreeWidget->topLevelItem(i);
        if (beneficiary == beneficiaryItem->data(0, Qt::ItemDataRole::UserRole).value<tt3::ws::Beneficiary>())
        {   //  This one!
            _ui->beneficiariesTreeWidget->setCurrentItem(beneficiaryItem);
            return;
        }
    }
}

void BeneficiaryManager::_startListeningToWorkspaceChanges()
{
    if (_workspace != nullptr)
    {
        connect(_workspace.get(),
                &tt3::ws::WorkspaceImpl::workspaceClosed,
                this,
                &BeneficiaryManager::_workspaceClosed,
                Qt::ConnectionType::QueuedConnection);
        connect(_workspace.get(),
                &tt3::ws::WorkspaceImpl::objectCreated,
                this,
                &BeneficiaryManager::_objectCreated,
                Qt::ConnectionType::QueuedConnection);
        connect(_workspace.get(),
                &tt3::ws::WorkspaceImpl::objectDestroyed,
                this,
                &BeneficiaryManager::_objectDestroyed,
                Qt::ConnectionType::QueuedConnection);
        connect(_workspace.get(),
                &tt3::ws::WorkspaceImpl::objectModified,
                this,
                &BeneficiaryManager::_objectModified,
                Qt::ConnectionType::QueuedConnection);
    }
}

void BeneficiaryManager::_stopListeningToWorkspaceChanges()
{
    if (_workspace != nullptr)
    {
        disconnect(_workspace.get(),
                   &tt3::ws::WorkspaceImpl::workspaceClosed,
                   this,
                   &BeneficiaryManager::_workspaceClosed);
        disconnect(_workspace.get(),
                   &tt3::ws::WorkspaceImpl::objectCreated,
                   this,
                   &BeneficiaryManager::_objectCreated);
        disconnect(_workspace.get(),
                   &tt3::ws::WorkspaceImpl::objectDestroyed,
                   this,
                   &BeneficiaryManager::_objectDestroyed);
        disconnect(_workspace.get(),
                   &tt3::ws::WorkspaceImpl::objectModified,
                   this,
                   &BeneficiaryManager::_objectModified);
    }
}

void BeneficiaryManager::_clearAndDisableAllControls()
{
    _ui->beneficiariesTreeWidget->clear();
    _ui->filterLineEdit->setText("");
    _ui->filterLabel->setEnabled(false);
    _ui->filterLineEdit->setEnabled(false);
    _ui->beneficiariesTreeWidget->setEnabled(false);
    _ui->createBeneficiaryPushButton->setEnabled(false);
    _ui->modifyBeneficiaryPushButton->setEnabled(false);
    _ui->destroyBeneficiaryPushButton->setEnabled(false);
}

void BeneficiaryManager::_applyCurrentLocale()
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(BeneficiaryManager));

    _ui->filterLabel->setText(
        rr.string(RID(FilterLabel)));
    _ui->createBeneficiaryPushButton->setText(
        rr.string(RID(CreateBeneficiaryPushButton)));
    _ui->modifyBeneficiaryPushButton->setText(
        rr.string(RID(ModifyBeneficiaryPushButton)));
    _ui->destroyBeneficiaryPushButton->setText(
        rr.string(RID(DestroyBeneficiaryPushButton)));
    refresh();
}

//////////
//  Signal handlers
void BeneficiaryManager::_currentThemeChanged(ITheme *, ITheme *)
{
    _ui->beneficiariesTreeWidget->clear();
    _decorations = TreeWidgetDecorations(_ui->beneficiariesTreeWidget);
    requestRefresh();
}

void BeneficiaryManager::_currentLocaleChanged(QLocale, QLocale)
{
    _applyCurrentLocale();
    refresh();
}

void BeneficiaryManager::_beneficiariesTreeWidgetCurrentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)
{
    requestRefresh();
}

void BeneficiaryManager::_beneficiariesTreeWidgetCustomContextMenuRequested(QPoint p)
{
    //  [re-]create the popup menu
    _beneficiariesTreeContextMenu.reset(new QMenu());
    QAction * createBeneficiaryAction =
        _beneficiariesTreeContextMenu->addAction(
            _ui->createBeneficiaryPushButton->icon(),
            _ui->createBeneficiaryPushButton->text());
    QAction * modifyBeneficiaryAction =
        _beneficiariesTreeContextMenu->addAction(
            _ui->modifyBeneficiaryPushButton->icon(),
            _ui->modifyBeneficiaryPushButton->text());
    QAction * destroyBeneficiaryAction =
        _beneficiariesTreeContextMenu->addAction(
            _ui->destroyBeneficiaryPushButton->icon(),
            _ui->destroyBeneficiaryPushButton->text());
    //  Adjust menu item states
    createBeneficiaryAction->setEnabled(_ui->createBeneficiaryPushButton->isEnabled());
    modifyBeneficiaryAction->setEnabled(_ui->modifyBeneficiaryPushButton->isEnabled());
    destroyBeneficiaryAction->setEnabled(_ui->destroyBeneficiaryPushButton->isEnabled());
    //  Set up signal handling
    connect(createBeneficiaryAction,
            &QAction::triggered,
            this,
            &BeneficiaryManager::_createBeneficiaryPushButtonClicked);
    connect(modifyBeneficiaryAction,
            &QAction::triggered,
            this,
            &BeneficiaryManager::_modifyBeneficiaryPushButtonClicked);
    connect(destroyBeneficiaryAction,
            &QAction::triggered,
            this,
            &BeneficiaryManager::_destroyBeneficiaryPushButtonClicked);
    //  Go!
    _beneficiariesTreeContextMenu->popup(_ui->beneficiariesTreeWidget->mapToGlobal(p));
}

void BeneficiaryManager::_createBeneficiaryPushButtonClicked()
{
    try
    {
        CreateBeneficiaryDialog dlg(this, _workspace, _credentials);   //  may throw
        if (dlg.doModal() == CreateBeneficiaryDialog::Result::Ok)
        {   //  Beneficiary created
            refresh();  //  must refresh NOW
            _setCurrentBeneficiary(dlg.createdBeneficiary());
        }
    }
    catch (const tt3::util::Exception & ex)
    {
        qCritical() << ex;
        tt3::gui::ErrorDialog::show(this, ex);
    }
}

void BeneficiaryManager::_modifyBeneficiaryPushButtonClicked()
{
    if (auto beneficiary = _currentBeneficiary())
    {
        try
        {
            ModifyBeneficiaryDialog dlg(this, beneficiary, _credentials); //  may throw
            if (dlg.doModal() == ModifyBeneficiaryDialog::Result::Ok)
            {   //  Beneficiary modified - its position in the activity types tree may have changed
                refresh();  //  must refresh NOW
                _setCurrentBeneficiary(beneficiary);
            }
        }
        catch (const tt3::util::Exception & ex)
        {
            qCritical() << ex;
            ErrorDialog::show(this, ex);
            requestRefresh();
        }
    }
}

void BeneficiaryManager::_destroyBeneficiaryPushButtonClicked()
{
    if (auto beneficiary = _currentBeneficiary())
    {
        try
        {
            DestroyBeneficiaryDialog dlg(this, beneficiary, _credentials); //  may throw
            if (dlg.doModal() == DestroyBeneficiaryDialog::Result::Ok)
            {   //  Destroyed
                requestRefresh();
            }
        }
        catch (const tt3::util::Exception & ex)
        {
            qCritical() << ex;
            ErrorDialog::show(this, ex);
            requestRefresh();
        }
    }
}

void BeneficiaryManager::_filterLineEditTextChanged(QString)
{
    requestRefresh();
}

void BeneficiaryManager::_workspaceClosed(tt3::ws::WorkspaceClosedNotification /*notification*/)
{
    requestRefresh();
}

void BeneficiaryManager::_objectCreated(tt3::ws::ObjectCreatedNotification /*notification*/)
{
    requestRefresh();
}

void BeneficiaryManager::_objectDestroyed(tt3::ws::ObjectDestroyedNotification /*notification*/)
{
    requestRefresh();
}

void BeneficiaryManager::_objectModified(tt3::ws::ObjectModifiedNotification /*notification*/)
{
    requestRefresh();
}

void BeneficiaryManager::_refreshRequested()
{
    refresh();
}

//  End of tt3-gui/BeneficiaryManager.cpp
