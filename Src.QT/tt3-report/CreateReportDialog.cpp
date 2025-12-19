//
//  tt3-report/CreateReportDialog.cpp - tt3::report::CreateReportDialog class implementation
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
#include "tt3-report/API.hpp"
#include "ui_CreateReportDialog.h"
using namespace tt3::report;

//////////
//  Construction/destruction
CreateReportDialog::CreateReportDialog(
        QWidget * parent,
        tt3::ws::Workspace workspace,
        const tt3::ws::ReportCredentials & credentials,
        IReportType * reportType
    ) : QDialog(parent),
        //  Implementation
        _workspace(workspace),
        _credentials(credentials),
        //  Controls
        _ui(new Ui::CreateReportDialog)
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(CreateReportDialog));

    _ui->setupUi(this);
    setWindowTitle(rr.string(RID(Title)));

    //  Populate "report type" combo box
    auto reportTypes = tt3::report::ReportTypeManager::allReportTypes();
    QList<tt3::report::IReportType*> reportTypesList(reportTypes.cbegin(), reportTypes.cend());
    std::sort(
        reportTypesList.begin(),
        reportTypesList.end(),
        [](auto a, auto b)
        {
            return tt3::util::NaturalStringOrder::less(a->displayName(), b->displayName());
        });
    for (auto reportType : reportTypesList)
    {
        _ui->reportTypeComboBox->addItem(
            reportType->smallIcon(),
            reportType->displayName(),
            QVariant::fromValue(reportType));
    }

    //  Populate "report format" combo box
    auto reportFormats = tt3::report::ReportFormatManager::allReportFormats();
    QList<tt3::report::IReportFormat*> reportFormatsList(reportFormats.cbegin(), reportFormats.cend());
    std::sort(
        reportFormatsList.begin(),
        reportFormatsList.end(),
        [](auto a, auto b)
        {
            return tt3::util::NaturalStringOrder::less(a->displayName(), b->displayName());
        });
    for (auto reportFormat : reportFormatsList)
    {
        _ui->reportFormatComboBox->addItem(
            reportFormat->smallIcon(),
            reportFormat->displayName(),
            QVariant::fromValue(reportFormat));
    }

    //  Populate "report template" combo box
    auto reportTemplates = tt3::report::ReportTemplateManager::allReportTemplates();
    QList<tt3::report::IReportTemplate*> reportTemplatesList(reportTemplates.cbegin(), reportTemplates.cend());
    std::sort(
        reportTemplatesList.begin(),
        reportTemplatesList.end(),
        [](auto a, auto b)
        {
            return tt3::util::NaturalStringOrder::less(a->displayName(), b->displayName());
        });
    for (auto reportTemplate : reportTemplatesList)
    {
        _ui->reportTemplateComboBox->addItem(
            reportTemplate->smallIcon(),
            reportTemplate->displayName(),
            QVariant::fromValue(reportTemplate));
    }

    //  Set static control values
    _ui->reportAndDestinationGroupBox->setTitle(
        rr.string(RID(ReportAndDestinationGroupBox)));
    _ui->reportTypeLabel->setText(
        rr.string(RID(ReportTypeLabel)));
    _ui->reportFormatLabel->setText(
        rr.string(RID(ReportFormatLabel)));
    _ui->reportTemplateLabel->setText(
        rr.string(RID(ReportTemplateLabel)));
    _ui->destinationLabel->setText(
        rr.string(RID(DestinationLabel)));
    _ui->browsePushButton->setText(
        rr.string(RID(BrowsePushButton)));
    _ui->configuratiokGroupBox->setTitle(
        rr.string(RID(ConfiguratiokGroupBox)));

    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setText(rr.string(RID(OkPushButton)));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setIcon(QIcon(":/tt3-report/Resources/Images/Actions/OkSmall.png"));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Cancel)->
        setText(rr.string(RID(CancelPushButton)));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Cancel)->
        setIcon(QIcon(":/tt3-report/Resources/Images/Actions/CancelSmall.png"));

    //  Create editor widgets for each report type...
    for (auto reportType : ReportTypeManager::allReportTypes())
    {
        if (auto editor = reportType->createConfigurationEditor(_ui->configuratiokGroupBox))
        {
            _configurationEditors[reportType] = editor; //  editor may be nullptr!
            editor->loadControlValues();
            //  Listen to value change events
            //  We need to connect() by name (old-style)
            //  because we privately inherit from QDialog
            connect(editor,
                    SIGNAL(controlValueChanged()),
                    this,
                    SLOT(_editorControlValueChanged()));
        }
    }
    //  ...and add them to the dialog
    _configuratiokGroupBoxLayout = new QStackedLayout();
    for (auto editor : _configurationEditors.values())
    {
        _configuratiokGroupBoxLayout->addWidget(editor);
    }
    _ui->configuratiokGroupBox->setLayout(_configuratiokGroupBoxLayout);

    //  Adjust controls
    //  Remember last used report type/format/template
    _setSelectedReportType(
        ReportTypeManager::findReportType(
            Component::Settings::instance()->lastUsedReportType));
    _setSelectedReportFormat(
        ReportFormatManager::findReportFormat(
            Component::Settings::instance()->lastUsedReportFormat));
    _setSelectedReportTemplate(
        ReportTemplateManager::findReportTemplate(
            Component::Settings::instance()->lastUsedReportTemplate));
    if (reportType != nullptr)
    {   //  Specified explicitly
        _setSelectedReportType(reportType);
    }
    _ui->reportTypeComboBox->setEnabled(
        _ui->reportTypeComboBox->count() > 0);
    _ui->reportFormatComboBox->setEnabled(
        _ui->reportFormatComboBox->count() > 0);
    _ui->destinationLineEdit->setEnabled(
        _selectedReportFormat() != nullptr);

    //  Done
    _refresh();
    adjustSize();
}

CreateReportDialog::~CreateReportDialog()
{
    delete _ui;
}

//////////
//  Operations
auto CreateReportDialog::doModal(
    ) -> Result
{
    return Result(this->exec());
}

//////////
//  Implementation helpers
IReportType * CreateReportDialog::_selectedReportType() const
{
    if (_ui->reportTypeComboBox->currentIndex() != -1)
    {   //  There IS a selection
        return _ui->reportTypeComboBox->currentData().value<IReportType*>();
    }
    return nullptr;
}

void CreateReportDialog::_setSelectedReportType(IReportType * reportType)
{
    for (int i = 0; i < _ui->reportTypeComboBox->count(); i++)
    {
        if (_ui->reportTypeComboBox->itemData(i).value<IReportType*>() == reportType)
        {   //  This one!
            _ui->reportTypeComboBox->setCurrentIndex(i);
            break;
        }
    }
}

IReportFormat * CreateReportDialog::_selectedReportFormat() const
{
    if (_ui->reportFormatComboBox->currentIndex() != -1)
    {   //  There IS a selection
        return _ui->reportFormatComboBox->currentData().value<IReportFormat*>();
    }
    return nullptr;
}

void CreateReportDialog::_setSelectedReportFormat(IReportFormat * reportFormat)
{
    for (int i = 0; i < _ui->reportFormatComboBox->count(); i++)
    {
        if (_ui->reportFormatComboBox->itemData(i).value<IReportFormat*>() == reportFormat)
        {   //  This one!
            _ui->reportFormatComboBox->setCurrentIndex(i);
            break;
        }
    }
}

IReportTemplate * CreateReportDialog::_selectedReportTemplate() const
{
    if (_ui->reportTemplateComboBox->currentIndex() != -1)
    {   //  There IS a selection
        return _ui->reportTemplateComboBox->currentData().value<IReportTemplate*>();
    }
    return nullptr;
}

void CreateReportDialog::_setSelectedReportTemplate(IReportTemplate * reportTemplate)
{
    for (int i = 0; i < _ui->reportTemplateComboBox->count(); i++)
    {
        if (_ui->reportTemplateComboBox->itemData(i).value<IReportTemplate*>() == reportTemplate)
        {   //  This one!
            _ui->reportTemplateComboBox->setCurrentIndex(i);
            break;
        }
    }
}

void CreateReportDialog::_refresh()
{
    //  Make sure the correct configuration editor
    //  is visible
    if (auto reportType = _selectedReportType())
    {
        if (_configurationEditors.contains(reportType))
        {   //  Editor exists - make it visible
            _configuratiokGroupBoxLayout->setCurrentWidget(_configurationEditors[reportType]);
            _ui->configuratiokGroupBox->setVisible(true);
        }
        else
        {   //  No editor to show
            _ui->configuratiokGroupBox->setVisible(false);
        }
    }
    else
    {   //  No editor to show
        _ui->configuratiokGroupBox->setVisible(false);
    }

    //  Refresh action buttons
    _ui->browsePushButton->setEnabled(
        _selectedReportFormat() != nullptr);
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->setEnabled(
        _selectedReportType() != nullptr &&
        _selectedReportFormat() != nullptr &&
        !_ui->destinationLineEdit->text().trimmed().isEmpty() &&
        (_configurationEditors[_selectedReportType()] == nullptr ||
         _configurationEditors[_selectedReportType()]->isValid()));
}

//////////
//  Signal handlers
void CreateReportDialog::_reportTypeComboBoxCurrentIndexChanged(int)
{
    _refresh();
}

void CreateReportDialog::_reportFormatComboBoxCurrentIndexChanged(int)
{
    _ui->destinationLineEdit->setText("");
    _refresh();
}

void CreateReportDialog::_browsePushButtonClicked()
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(CreateReportDialog));

    auto reportFormat = _selectedReportFormat();
    Q_ASSERT(reportFormat != nullptr);

    QString path =
        QFileDialog::getSaveFileName(
            this,
            rr.string(RID(SelectDestinationTitle)),
            /*dir =*/ QString(),
            rr.string(RID(SelectDestinationFilter),
                      reportFormat->displayName(),
                      reportFormat->preferredExtension()));
    if (path.isEmpty())
    {
        return;
    }
    //  On e.g. Linux we may need to auto-add the extension
    if (QFileInfo(path).suffix().isEmpty())
    {
        path += reportFormat->preferredExtension();
    }
    //  Apply
    _ui->destinationLineEdit->setText(path);
    _refresh();
}

void CreateReportDialog::_editorControlValueChanged()
{
    _refresh();
}

void CreateReportDialog::accept()
{
    _reportType = _selectedReportType();
    _reportFormat = _selectedReportFormat();
    _reportTemplate = _selectedReportTemplate();
    _reportDestination = _ui->destinationLineEdit->text().trimmed();
    Q_ASSERT(_reportType != nullptr &&
             _reportFormat != nullptr &&
             _reportTemplate != nullptr);
    Component::Settings::instance()->lastUsedReportType = _reportType->mnemonic();
    Component::Settings::instance()->lastUsedReportFormat = _reportFormat->mnemonic();
    Component::Settings::instance()->lastUsedReportTemplate = _reportTemplate->mnemonic();

    //  Save editor states before proceeding
    for (auto editor : _configurationEditors.values())
    {
        editor->saveControlValues();
    }

    //  Go!
    try
    {
        ReportProgressDialog dlg(this, _reportType, _reportDestination);
        dlg.show();
        std::unique_ptr<Report> report
        {
            _reportType->generateReport(    //  may throw
                _workspace,
                _credentials,
                nullptr,    //  TODO report configuration
                _reportTemplate,
                [&](float ratioCompleted)
                {
                    dlg.reportGenerationProgress(ratioCompleted);
                    if (dlg.cancelRequested())
                    {
                        throw _CancelRequest();
                    }
                })
        };
        dlg.reportGenerationProgress(1.0f);
        if (report != nullptr)
        {   //  Be defensive in release mode
            _reportFormat->saveReport(
                report.get(),
                _reportDestination,
                [&](float ratioCompleted)
                {
                    dlg.reportSaveProgress(ratioCompleted);
                    if (dlg.cancelRequested())
                    {
                        throw _CancelRequest();
                    }
                });
            dlg.reportSaveProgress(1.0f);
        }
        //  All done
        dlg.hide();
    }
    catch (const _CancelRequest &)
    {   //  Report generation was cancelled half-way
        tt3::gui::MessageDialog::show(
            this,
            Component::Resources::instance()->string(
                RSID(ReportCancelledDialog),
                RID(Title)),
            Component::Resources::instance()->string(
                RSID(ReportCancelledDialog),
                RID(Message)));
        //  Don't leave half-written file behind
        QFile(_reportDestination).remove(); //  ignore errors
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS!
        qCritical() << ex;
        tt3::gui::ErrorDialog::show(this, ex);
        return;
    }
    //  At this point we know the report has been
    //  generated and saved successfully
    //  TODO Ask whether to open report/report directory
    //  Close this dialog anyhow
    done(int(Result::Ok));
}

void CreateReportDialog::reject()
{
    done(int(Result::Cancel));
}

//  End of tt3-report/CreateReportDialog.cpp
