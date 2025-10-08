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

    //  Theme change means widget decorations change
    connect(&theCurrentTheme,
            &CurrentTheme::changed,
            this,
            &BeneficiaryManager::_currentThemeChanged,
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

    //  We don't want a refresh() to trigger a recursive refresh()!
    static bool refreshUnderway = false;
    RefreshGuard refreshGuard(refreshUnderway);
    if (refreshGuard)   //  Don't recurse!
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
            qCritical() << ex.errorMessage();
            _clearAndDisableAllControls();
            return;
        }

        //  Otherwise some controls are always enabled...
        _ui->filterLabel->setEnabled(true);
        _ui->filterLineEdit->setEnabled(true);
        _ui->beneficiariesTreeWidget->setEnabled(true);

        //  ...while others are enabled based on current
        //  selection and permissions granted by Credentials
        _WorkspaceModel workspaceModel = _createWorkspaceModel();
        if (!_ui->filterLineEdit->text().trimmed().isEmpty())
        {
            _filterItems(workspaceModel);
        }
        _refreshWorkspaceTree(workspaceModel);

        tt3::ws::Beneficiary selectedBeneficiary = _selectedBeneficiary();
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
            qCritical() << ex.errorMessage();
            _ui->createBeneficiaryPushButton->setEnabled(false);
        }
        _ui->modifyBeneficiaryPushButton->setEnabled(
            selectedBeneficiary != nullptr);
        try
        {
            _ui->destroyBeneficiaryPushButton->setEnabled(
                !readOnly &&
                selectedBeneficiary != nullptr &&
                selectedBeneficiary->canDestroy(_credentials));    //  may throw
        }
        catch (const tt3::util::Exception & ex)
        {   //  OOPS! Log & disable
            qCritical() << ex.errorMessage();
            _ui->destroyBeneficiaryPushButton->setEnabled(false);
        }

        //  Some buttons need to be adjusted for ReadOnoly mode
        try
        {
            if (selectedBeneficiary != nullptr &&
                !selectedBeneficiary->workspace()->isReadOnly() &&
                selectedBeneficiary->canModify(_credentials))  //  may throw
            {   //  RW
                _ui->modifyBeneficiaryPushButton->setIcon(modifyBeneficiaryIcon);
                _ui->modifyBeneficiaryPushButton->setText("Modify beneficiary");
            }
            else
            {   //  RO
                _ui->modifyBeneficiaryPushButton->setIcon(viewBeneficiaryIcon);
                _ui->modifyBeneficiaryPushButton->setText("View beneficiary");
            }
        }
        catch (const tt3::util::Exception & ex)
        {   //  OOPS! Log & simulate RO
            qCritical() << ex.errorMessage();
            _ui->modifyBeneficiaryPushButton->setIcon(viewBeneficiaryIcon);
            _ui->modifyBeneficiaryPushButton->setText("View beneficiary");
        }
    }
}

void BeneficiaryManager::requestRefresh()
{
    emit refreshRequested();
}

//////////
//  View model
BeneficiaryManager::_WorkspaceModel BeneficiaryManager::_createWorkspaceModel()
{
    _WorkspaceModel workspaceModel { new _WorkspaceModelImpl() };
    try
    {
        for (tt3::ws::Beneficiary beneficiary : _workspace->beneficiaries(_credentials))    //  may throw
        {
            workspaceModel->beneficiaryModels.append(_createBeneficiaryModel(beneficiary));
        }
        std::sort(workspaceModel->beneficiaryModels.begin(),
                  workspaceModel->beneficiaryModels.end(),
                  [&](auto a, auto b)
                  { return a->text < b->text; });
    }
    catch (const tt3::util::Exception & ex)
    {
        qCritical() << ex.errorMessage();
        workspaceModel->beneficiaryModels.clear();
    }
    return workspaceModel;
}

BeneficiaryManager::_BeneficiaryModel BeneficiaryManager::_createBeneficiaryModel(tt3::ws::Beneficiary beneficiary)
{
    static const QIcon errorIcon(":/tt3-gui/Resources/Images/Misc/ErrorSmall.png");

    _BeneficiaryModel beneficiaryModel { new _BeneficiaryModelImpl(beneficiary) };
    try
    {
        beneficiaryModel->text = beneficiary->displayName(_credentials);
        beneficiaryModel->icon = beneficiary->type()->smallIcon();
        beneficiaryModel->brush = _decorations.itemForeground;
        beneficiaryModel->font = _decorations.itemFont;
        beneficiaryModel->tooltip = beneficiary->description(_credentials).trimmed();
    }
    catch (const tt3::util::Exception & ex)
    {
        qCritical() << ex.errorMessage();
        beneficiaryModel->text = ex.errorMessage();
        beneficiaryModel->icon = errorIcon;
        beneficiaryModel->font = _decorations.itemFont;
        beneficiaryModel->brush = _decorations.errorItemForeground;
        beneficiaryModel->tooltip = ex.errorMessage();
    }
    return beneficiaryModel;
}

void BeneficiaryManager::_filterItems(_WorkspaceModel workspaceModel)
{
    QString filter = _ui->filterLineEdit->text().trimmed();
    Q_ASSERT(!filter.isEmpty());

    for (qsizetype i = workspaceModel->beneficiaryModels.size() - 1; i >= 0; i--)
    {
        _BeneficiaryModel beneficiaryModel = workspaceModel->beneficiaryModels[i];
        if (beneficiaryModel->text.indexOf(filter, 0, Qt::CaseInsensitive) != -1)
        {   //  Item matches the filter - mark it as a match
            beneficiaryModel->brush = _decorations.filterMatchItemForeground;
        }
        else
        {   //  Item does not match the filter
            workspaceModel->beneficiaryModels.removeAt(i);
        }
    }
}

void BeneficiaryManager::_refreshWorkspaceTree(
    _WorkspaceModel workspaceModel
    )
{
    Q_ASSERT(_workspace != nullptr);
    Q_ASSERT(_credentials.isValid());

    //  Make sure the "beneficiaries" tree contains
    //  a proper number of root (Beneficiary) items...
    while (_ui->beneficiariesTreeWidget->topLevelItemCount() < workspaceModel->beneficiaryModels.size())
    {   //  Too few root (Beneficiary) items
        _ui->beneficiariesTreeWidget->addTopLevelItem(new QTreeWidgetItem());
    }
    while (_ui->beneficiariesTreeWidget->topLevelItemCount() > workspaceModel->beneficiaryModels.size())
    {   //  Too many root (Beneficiary) items
        delete _ui->beneficiariesTreeWidget->takeTopLevelItem(
            _ui->beneficiariesTreeWidget->topLevelItemCount() - 1);
    }
    //  ...and that each top-level item represents
    //  a proper Beneficiary and has proper children
    for (int i = 0; i < workspaceModel->beneficiaryModels.size(); i++)
    {
        _refreshBeneficiaryItem(
            _ui->beneficiariesTreeWidget->topLevelItem(i),
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
    Q_ASSERT(_credentials.isValid());

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
tt3::ws::Beneficiary BeneficiaryManager::_selectedBeneficiary()
{
    QTreeWidgetItem * item = _ui->beneficiariesTreeWidget->currentItem();
    return (item != nullptr) ?
               item->data(0, Qt::ItemDataRole::UserRole).value<tt3::ws::Beneficiary>() :
               nullptr;
}

void BeneficiaryManager::_setSelectedBeneficiary(tt3::ws::Beneficiary beneficiary)
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
    _ui->filterLabel->setEnabled(false);
    _ui->filterLineEdit->setEnabled(false);
    _ui->beneficiariesTreeWidget->setEnabled(false);
    _ui->createBeneficiaryPushButton->setEnabled(false);
    _ui->modifyBeneficiaryPushButton->setEnabled(false);
    _ui->destroyBeneficiaryPushButton->setEnabled(false);
}

//////////
//  Signal handlers
void BeneficiaryManager::_currentThemeChanged(ITheme *, ITheme *)
{
    _ui->beneficiariesTreeWidget->clear();
    _decorations = TreeWidgetDecorations(_ui->beneficiariesTreeWidget);
    requestRefresh();
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
            _setSelectedBeneficiary(dlg.createdBeneficiary());
        }
    }
    catch (const tt3::util::Exception & ex)
    {
        tt3::gui::ErrorDialog::show(this, ex);
    }
}

void BeneficiaryManager::_modifyBeneficiaryPushButtonClicked()
{
    ErrorDialog::show(this, "Not yet implemented");
    /*  TODO uncomment
    if (auto Beneficiary = _selectedBeneficiary())
    {
        try
        {
            ModifyBeneficiaryDialog dlg(this, Beneficiary, _credentials); //  may throw
            if (dlg.doModal() == ModifyBeneficiaryDialog::Result::Ok)
            {   //  Beneficiary modified - its position in the activity types tree may have changed
                refresh();  //  must refresh NOW
                _setSelectedBeneficiary(Beneficiary);
            }
        }
        catch (const tt3::util::Exception & ex)
        {
            ErrorDialog::show(this, ex);
            requestRefresh();
        }
    }
    */
}

void BeneficiaryManager::_destroyBeneficiaryPushButtonClicked()
{
    ErrorDialog::show(this, "Not yet implemented");
    /*  TODO uncomment
    if (auto Beneficiary = _selectedBeneficiary())
    {
        try
        {
            DestroyBeneficiaryDialog dlg(this, Beneficiary, _credentials); //  may throw
            if (dlg.doModal() == DestroyBeneficiaryDialog::Result::Ok)
            {   //  Destroyed
                requestRefresh();
            }
        }
        catch (const tt3::util::Exception & ex)
        {
            ErrorDialog::show(this, ex);
            requestRefresh();
        }
    }
    */
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
