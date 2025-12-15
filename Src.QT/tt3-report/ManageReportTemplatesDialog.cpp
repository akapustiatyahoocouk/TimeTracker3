//
//  tt3-report/ManageReportTemplatesDialog.cpp - tt3::report::ManageReportTemplatesDialog class implementation
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
#include "ui_ManageReportTemplatesDialog.h"
using namespace tt3::report;

//////////
//  Construction/destruction
ManageReportTemplatesDialog::ManageReportTemplatesDialog(
        QWidget * parent
    ) : QDialog(parent),
        _ui(new Ui::ManageReportTemplatesDialog)
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(ManageReportTemplatesDialog));

    _ui->setupUi(this);
    setWindowTitle(rr.string(RID(Title)));

    //  Set static control values
    _ui->templatesGroupBox->setTitle(
        rr.string(RID(TemplatesGroupBox)));
    _ui->previewGroupBox->setTitle(
        rr.string(RID(PreviewGroupBox)));
    _ui->exportPushButton->setText(
        rr.string(RID(ExportPushButton)));
    _ui->importPushButton->setText(
        rr.string(RID(ImportPushButton)));
    _ui->removePushButton->setText(
        rr.string(RID(RemovePushButton)));

    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setText(rr.string(RID(OkPushButton)));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/OkSmall.png"));

    //  Create static tree widget items
    _predefinedReportsItem = new QTreeWidgetItem();
    _customReportsItem = new QTreeWidgetItem();
    _predefinedReportsItem->setText(
        0,
        rr.string(RID(PredefinedReportTemplates)));
    _customReportsItem->setText(
        0,
        rr.string(RID(CustomReportTemplates)));
    _ui->templatesTreeWidget->addTopLevelItem(_predefinedReportsItem);
    _ui->templatesTreeWidget->addTopLevelItem(_customReportsItem);

    //  Adjust controls
    _ui->splitter->setSizes(QList<int>{100, 1000});
    _refresh(); //  Must populate templates tree NOW
    _ui->templatesTreeWidget->expandAll();

    //  Handle refresh requests
    connect(this,
            &ManageReportTemplatesDialog::refreshRequested,
            this,
            &ManageReportTemplatesDialog::_refreshRequested,
            Qt::ConnectionType::QueuedConnection);
    connect(this,
            &ManageReportTemplatesDialog::previewAvailable,
            this,
            &ManageReportTemplatesDialog::_previewAvailable,
            Qt::ConnectionType::QueuedConnection);
    //  Done...
    _ui->templatesTreeWidget->setFocus();
    //  ...but need to make almost-fullscreen - report
    //  preview are LARGE
    if (QScreen *screen = QGuiApplication::primaryScreen())
    {
        QRect r = screen->availableGeometry();
        int l = 32;
        int t = 64;
        int w = r.width() - 2 * l;
        int h = r.height() - 2 * t;
        this->setGeometry(l, t, w, h);
    }
}

ManageReportTemplatesDialog::~ManageReportTemplatesDialog()
{
    //  Need to kill & delete all preview generators
    for (auto previewGenerator : _previewGenerators)
    {
        previewGenerator->terminate();
        previewGenerator->wait();
        delete previewGenerator;
    }
    //  Remove any temporary preview files created
    for (auto previewFileName : _previewFileNames)
    {
        QFile(previewFileName).remove();
    }
    //  Cleanup & done
    delete _ui;
}

//////////
//  Operations
void ManageReportTemplatesDialog::doModal()
{
    exec();
}

//////////
//  Implementation helpers
void ManageReportTemplatesDialog::_refresh()
{
    static const QIcon openFolderIcon(":/tt3-report/Resources/Images/Misc/OpenFolderSmall.png");
    static const QIcon closedFolderIcon(":/tt3-report/Resources/Images/Misc/ClosedFolderSmall.png");
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(ManageReportTemplatesDialog));

    //  We don't want a refresh() to trigger a recursive refresh()!
    if (auto _ = tt3::gui::RefreshGuard(_refreshUnderway)) //  Don't recurse!
    {
        _refreshReportTemplateItems(_predefinedReportsItem);
        _refreshReportTemplateItems(_customReportsItem);

        //  Adjust folder icons
        _predefinedReportsItem->setIcon(0,
            (_predefinedReportsItem->isExpanded() &&
             _predefinedReportsItem->childCount() > 0)?
                openFolderIcon :
                closedFolderIcon);
        _customReportsItem->setIcon(0,
            (_customReportsItem->isExpanded() &&
             _customReportsItem->childCount() > 0) ?
                openFolderIcon :
                closedFolderIcon);

        //  Adjust action button availability
        _ui->exportPushButton->setEnabled(
            _selectedReportTemplate() != nullptr);
        _ui->importPushButton->setEnabled(true);
        _ui->removePushButton->setEnabled(
            dynamic_cast<CustomReportTemplate*>(_selectedReportTemplate()) != nullptr);

        //  Display preview
        _ui->previewWebEngineView->page()->setBackgroundColor(
            tt3::gui::LabelDecorations().background);
        QString emptyStyle =
            "color: " + tt3::util::toString(tt3::gui::LabelDecorations().foreground) + ";" +
            "background-color: " + tt3::util::toString(tt3::gui::LabelDecorations().background) + ";" +
            "text-align: center; ";
        auto reportTemplate = _selectedReportTemplate();
        if (reportTemplate == nullptr)
        {
            _ui->previewGroupBox->setTitle(
                rr.string(RID(PreviewGroupBox.NotAvailable)));
            _ui->previewWebEngineView->setHtml(
                "<p style=\"" + emptyStyle + "\">-</p>");
        }
        else if (_previews.contains(reportTemplate))
        {
            _ui->previewGroupBox->setTitle(
                rr.string(RID(PreviewGroupBox.Available),
                          reportTemplate->displayName()));
            _ui->previewWebEngineView->setHtml(
                _previews[reportTemplate]);
        }
        else if (_previewGenerators.contains(reportTemplate) &&
                 _previewGenerators[reportTemplate]->isRunning())
        {
            _ui->previewGroupBox->setTitle(
                rr.string(RID(PreviewGroupBox.Available),
                          reportTemplate->displayName()));
            _ui->previewWebEngineView->setHtml(
                "<p style=\"" + emptyStyle + "\">" +
                rr.string(RID(GeneratingPreview)) +
                "</p>");
        }
        else
        {   //  Need a new preview generator
            QTemporaryFile  previewFile;
            if (previewFile.open())
            {   //  Go!
                previewFile.close();
                _previewFileNames.insert(previewFile.fileName());
                auto previewGenerator =
                    new _PreviewGenerator(this, reportTemplate, previewFile.fileName());
                _previewGenerators[reportTemplate] = previewGenerator;
                previewGenerator->start();
            }
            else
            {   //  OOPS! No go! Show error message instead.
                _previews[reportTemplate] =
                    "<p style=\"" + emptyStyle + "\">" +
                    previewFile.errorString().toHtmlEscaped() +
                    "</p>";
            }
            //  Refresh ASAP
            emit refreshRequested();
        }
    }
}

void ManageReportTemplatesDialog::_refreshReportTemplateItems(QTreeWidgetItem * parentItem)
{
    QList<IReportTemplate*> reportTemplates;
    if (parentItem == _predefinedReportsItem)
    {
        for (auto reportTemplate :
             ReportTemplateManager::allReportTemplates())
        {
            if (dynamic_cast<BasicReportTemplate*>(reportTemplate))
            {
                reportTemplates.append(reportTemplate);
            }
        }
    }
    else if (parentItem == _customReportsItem)
    {
        for (auto reportTemplate :
             ReportTemplateManager::allReportTemplates())
        {
            if (dynamic_cast<CustomReportTemplate*>(reportTemplate))
            {
                reportTemplates.append(reportTemplate);
            }
        }
    }
    std::sort(
        reportTemplates.begin(),
        reportTemplates.end(),
        [](auto a, auto b)
        {
            return a->displayName() < b->displayName();
        });
    //  Make sure the parent item has a proper number
    //  of child (report template) items...
    while (parentItem->childCount() < reportTemplates.size())
    {   //  Too few child (ReportTemplate) items
        parentItem->addChild(new QTreeWidgetItem());
    }
    while (parentItem->childCount() > reportTemplates.size())
    {   //  Too many child (ReportTemplate) items
        delete parentItem->takeChild(
                parentItem->childCount() - 1);
    }
    //  ...and that each child item represents
    //  a proper ReportTemplate
    for (int i = 0; i < reportTemplates.size(); i++)
    {
        auto reportTemplateItem = parentItem->child(i);
        auto reportTemplate = reportTemplates[i];
        reportTemplateItem->setText(0, reportTemplate->displayName());
        reportTemplateItem->setIcon(0, reportTemplate->smallIcon());
        reportTemplateItem->setData(0, Qt::ItemDataRole::UserRole, QVariant::fromValue(reportTemplate));
    }
}

auto ManageReportTemplatesDialog::_selectedReportTemplate() -> IReportTemplate *
{
    QTreeWidgetItem * item = _ui->templatesTreeWidget->currentItem();
    return (item != nullptr && item->parent() != nullptr) ?
                item->data(0, Qt::ItemDataRole::UserRole).value<IReportTemplate*>() :
                nullptr;
}

void ManageReportTemplatesDialog::_setSelectedReportTemplate(
        IReportTemplate * reportTemplate
    )
{
    for (int i = 0; i < _ui->templatesTreeWidget->topLevelItemCount(); i++)
    {
        auto categoryItem = _ui->templatesTreeWidget->topLevelItem(i);
        for (int j = 0; j < categoryItem->childCount(); j++)
        {
            auto reportTemplateItem = categoryItem->child(j);
            auto rt = reportTemplateItem->data(0, Qt::ItemDataRole::UserRole).value<IReportTemplate*>();
            if (rt == reportTemplate)
            {   //  This one!
                _ui->templatesTreeWidget->setCurrentItem(reportTemplateItem);
                return;
            }
        }
    }
}

//////////
//  Signal handlers:
void ManageReportTemplatesDialog::_templateTreeWidgetCurrentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)
{
    _refresh();
}

void ManageReportTemplatesDialog::_templateTreeWidgetItemExpanded(QTreeWidgetItem*)
{
    _refresh();
}

void ManageReportTemplatesDialog::_templateTreeWidgetItemCollapsed(QTreeWidgetItem*)
{
    _refresh();
}

void ManageReportTemplatesDialog::_exportPushButtonClicked()
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(ManageReportTemplatesDialog));

    if (auto reportTemplate = _selectedReportTemplate())
    {
        QString path =
            QFileDialog::getSaveFileName(
                this,
                rr.string(RID(ExportReportTemplate)),
                /*dir =*/ QString(),
                rr.string(RID(ReportTemplateFilter), IReportTemplate::PreferredExtension));
        if (path.isEmpty())
        {   //  User has cancelled
            return;
        }
        //  Do it!
        QFile file(path);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream out(&file);
            out << reportTemplate->toXmlString();
            out.flush();
            file.close();
            //  Show "report template exported" confirmation popup
            //  to provide user with visible action feedback
            tt3::gui::MessageDialog::show(
                this,
                rr.string(RID(ExportCompletedTitle)),
                rr.string(RID(ExportCompletedMessage),
                          reportTemplate->displayName(),
                          path));
        }
        else
        {   //  OOPS!
            tt3::gui::ErrorDialog::show(this, file.errorString());
        }
    }
}

void ManageReportTemplatesDialog::_importPushButtonClicked()
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(ManageReportTemplatesDialog));

    QString path =
        QFileDialog::getOpenFileName(
            this,
            rr.string(RID(ImportReportTemplate)),
            /*dir =*/ QString(),
            rr.string(RID(ReportTemplateFilter), IReportTemplate::PreferredExtension));
    if (path.isEmpty())
    {   //  User has cancelled
        return;
    }
    //  Load file as text (assuming it's XML)...
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {   //  OOPS!
        throw CustomReportException(path + ": " + file.errorString());
    }
    QTextStream in(&file);
    QString xml = in.readAll();
    file.close();
    //  Create report template from XML
    auto reportTemplate =
        std::make_unique<CustomReportTemplate>(xml);    //  may throw
    //  No duplication!
    if (auto rt =
        ReportTemplateManager::findReportTemplate(reportTemplate->mnemonic()))
    {   //  OOPS!
        throw ReportTemplateAlreadyExistsException(
            rt->mnemonic(),
            rt->displayName());

    }
    //  Register, record & release
    if (ReportTemplateManager::registerReportTemplate(reportTemplate.get()))
    {   //  Success - record, refresh, select & release
        auto known = Component::Settings::instance()->knownCustomReportTemplates.value();
        known.append(
            KnownCustomReportTemplate(
                reportTemplate->mnemonic(),
                reportTemplate->displayName(),
                path));
        Component::Settings::instance()->knownCustomReportTemplates = known;
        _refresh();
        _setSelectedReportTemplate(reportTemplate.release());
    }
}

void ManageReportTemplatesDialog::_removePushButtonClicked()
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(ManageReportTemplatesDialog));

    if (auto customReportTemplate =
        dynamic_cast<CustomReportTemplate*>(_selectedReportTemplate()))
    {   //  Confirm...
        tt3::gui::AskYesNoDialog dlg(
            this,
            QIcon(":/tt3-report/Resources/Images/Actions/RemoveSmall.png"),
            rr.string(RID(RemoveReportTemplateTitle)),
            rr.string(RID(RemoveReportTemplatePrompt),
                      customReportTemplate->displayName(),
                      customReportTemplate->mnemonic()));
        if (dlg.doModal() != tt3::gui::AskYesNoDialog::Result::Yes)
        {
            return;
        }
        //  ...forget...
        auto known = Component::Settings::instance()->knownCustomReportTemplates.value();
        for (auto kcrt : KnownCustomReportTemplates(known)) //  shallow clone
        {
            if (kcrt.mnemonic == customReportTemplate->mnemonic())
            {
                known.removeOne(kcrt);
            }
        }
        Component::Settings::instance()->knownCustomReportTemplates = known;
        //  ...and remove
        ReportTemplateManager::unregisterReportTemplate(customReportTemplate);
        _refresh();
        delete customReportTemplate;
    }
}

void ManageReportTemplatesDialog::_refreshRequested()
{
    _refresh();
}

void ManageReportTemplatesDialog::_previewAvailable(IReportTemplate * reportTemplate, QString html)
{
    _previews[reportTemplate] = html;
}

//////////
//  ManageReportTemplatesDialog::_PreviewGenerator
ManageReportTemplatesDialog::_PreviewGenerator::_PreviewGenerator(
        ManageReportTemplatesDialog * dialog,
        IReportTemplate * reportTemplate,
        const QString & htmlFileName
    ) : _dialog(dialog),
        _reportTemplate(reportTemplate),
        _htmlFileName(htmlFileName)
{
}

ManageReportTemplatesDialog::_PreviewGenerator::~_PreviewGenerator()
{
}

void ManageReportTemplatesDialog::_PreviewGenerator::run()
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(ManageReportTemplatesDialog));

    //  Generate a "preview" report with initial content
    //  by asking the report template to create one
    std::unique_ptr<Report> report
        { _reportTemplate->createNewReport() };

    //  Find/create the "body" section
    ReportSection * bodySection = nullptr;
    for (auto section : report->sections())
    {
        if (section->style() != nullptr &&
            section->style()->name() == ISectionStyle::BodyStyleName)
        {   //  This one!
            bodySection = section;
            break;
        }
    }
    if (bodySection == nullptr)
    {
        bodySection =
            report->createSection(
                "1",
                _reportTemplate->findSectionStyle(
                    ISectionStyle::BodyStyleName));
    }

    //  Populate the report body
    bodySection
        ->createParagraph(
            _reportTemplate->findParagraphStyle(
                IParagraphStyle::Heading1StyleName))
        ->createText(rr.string(RID(PreviewReport.Chapter1)));

    ReportText * topText =
        bodySection
            ->createParagraph()
            ->createText(rr.string(RID(PreviewReport.Paragraph1)));
    ReportAnchor * anchor =
        topText->createAnchor();

    ReportList * list =
        bodySection->createList();
    list->createItem()
        ->createParagraph()
        ->createText(rr.string(RID(PreviewReport.ListItem1)));
    list->createItem()
        ->createParagraph()
        ->createText(rr.string(RID(PreviewReport.ListItem2)));
    list->createItem()
        ->createParagraph()
        ->createText(rr.string(RID(PreviewReport.ListItem3)));

    bodySection
        ->createParagraph(
            _reportTemplate->findParagraphStyle(
                IParagraphStyle::Heading2StyleName))
        ->createText(rr.string(RID(PreviewReport.Chapter11)));
    bodySection
        ->createParagraph()
        ->createText(rr.string(RID(PreviewReport.Paragraph2)));

    bodySection
        ->createParagraph(
            _reportTemplate->findParagraphStyle(
                IParagraphStyle::Heading2StyleName))
        ->createText(rr.string(RID(PreviewReport.Chapter12)));
    bodySection
        ->createParagraph()
        ->createText(rr.string(RID(PreviewReport.Paragraph3)));

    bodySection
        ->createParagraph(
            _reportTemplate->findParagraphStyle(
                IParagraphStyle::Heading1StyleName))
        ->createText(rr.string(RID(PreviewReport.Chapter2)));
    bodySection
        ->createParagraph()
        ->createText(rr.string(RID(PreviewReport.Paragraph4)));

    ReportTable * table =
        bodySection->createTable();
    table
        ->createCell(
            0, 0, 2, 1,
            report->reportTemplate()->findTableCellStyle(ITableCellStyle::HeadingStyleName))
        ->createParagraph()
        ->createText(rr.string(RID(PreviewReport.TableHeading)));
    table
        ->createCell(
            0, 1, 1, 1,
            report->reportTemplate()->findTableCellStyle(ITableCellStyle::DefaultStyleName))
        ->createParagraph()
        ->createText(rr.string(RID(PreviewReport.TableCell11)));
    table
        ->createCell(1, 1, 1, 1)
        ->createParagraph()
        ->createText(rr.string(RID(PreviewReport.TableCell12)));
    table
        ->createCell(
            0, 2, 1, 1,
            report->reportTemplate()->findTableCellStyle(ITableCellStyle::DefaultStyleName))
        ->createParagraph()
        ->createText(rr.string(RID(PreviewReport.TableCell21)));
    table
        ->createCell(
            1, 2, 1, 1,
            report->reportTemplate()->findTableCellStyle(ITableCellStyle::DefaultStyleName))
        ->createParagraph()
        ->createText(rr.string(RID(PreviewReport.TableCell22)));

    bodySection
        ->createParagraph()
        ->createText(rr.string(RID(PreviewReport.Link1)))
        ->createInternalLink(anchor);

    //  ...save as HTML...
    QString html;
    try
    {
        HtmlReportFormat::instance()->saveReport(report.get(), _htmlFileName); //  may throw!
        //  ...and load HTML as a string
        QFile file(_htmlFileName);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream in(&file);
            html = in.readAll();
            file.close();
            //  Adjust colors to the current UI mode.
            //  IMPORETANT: The below code uses internal
            //  knowledge about how the HTML Reporrt Format
            //  generates the CSS. However, the knowledge
            //  is still encapsulated within the tt3-report
            //  component.
            tt3::gui::LabelDecorations decorations;
            html =
                html.replace(
                    " color: initial;",
                    " color: " + tt3::util::toString(decorations.foreground) + ";");
            html =
                html.replace(
                    " background-color: initial;",
                    " background-color: " + tt3::util::toString(decorations.background) + ";");
        }
        else
        {
            html = _htmlFileName + ": " +  file.errorString();
        }

    }
    catch (const tt3::util::Exception & ex)
    {
        html = ex.errorMessage();
    }

    //  ...and we're done
    emit _dialog->previewAvailable(_reportTemplate, html);
    emit _dialog->refreshRequested();
}

//  End of tt3-report/ManageReportTemplatesDialog.cpp
