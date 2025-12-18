//
//  tt3-report/CreateReportDialog.cpp - tt3::report::CreateReportDialog class implementation
//  TODO localize via Resources
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
        IReportType * reportType
    ) : QDialog(parent),
        _ui(new Ui::CreateReportDialog)
{
    _ui->setupUi(this);

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

    //  Adjust controls
    //  TODO Remember last used report type/format
    if (reportType != nullptr)
    {   //  Specified explicitly
        _setSelectedReportType(reportType);
    }
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

//////////
//  Signal handlers
void CreateReportDialog::_reportTypeComboBoxCurrentIndexChanged(int)
{
}

void CreateReportDialog::_reportFormatComboBoxCurrentIndexChanged(int)
{
}

void CreateReportDialog::_browsePushButtonClicked()
{
}

void CreateReportDialog::accept()
{
    done(int(Result::Ok));
}

void CreateReportDialog::reject()
{
    done(int(Result::Cancel));
}

//  End of tt3-report/CreateReportDialog.cpp
