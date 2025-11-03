//
//  tt3-gui/DestroyBeneficiaryDialog.cpp - tt3::gui::DestroyBeneficiaryDialog class implementation
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

//////////
//  Construction/destrution
DestroyBeneficiaryDialog::DestroyBeneficiaryDialog(
        ::QWidget * parent,
        tt3::ws::Beneficiary beneficiary,
        const tt3::ws::Credentials & credentials
    ) : AskYesNoDialog(
            parent,
            QIcon(":/tt3-gui/Resources/Images/Actions/DestroyBeneficiaryLarge.png"),
            Component::Resources::instance()->string(
                RSID(DestroyBeneficiaryDialog),
                RID(Title)),
            _prompt(beneficiary, credentials)),
        _beneficiary(beneficiary),
        _credentials(credentials)
{
}

//////////
//  Operations
auto DestroyBeneficiaryDialog::doModal(
    ) -> Result
{
    return (AskYesNoDialog::doModal() == AskYesNoDialog::Result::Yes) ?
               Result::Ok :
               Result::Cancel;
}

//////////
//  Implementation helpers
QString DestroyBeneficiaryDialog::_prompt(
        tt3::ws::Beneficiary beneficiary,
        const tt3::ws::Credentials & credentials
    )
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(DestroyBeneficiaryDialog));

    QString result =
        rr.string(
            RID(Prompt),
            beneficiary->displayName(credentials)); //  may throw
    try
    {
        qsizetype projectsCount = 0, workStreamsCount = 0, worksCount = 0;
        tt3::ws::Events events;
        int64_t worksDurationMs = 0;
        _collectDestructionClosure(
            beneficiary,
            credentials,
            projectsCount,
            workStreamsCount,
            worksCount,
            events,
            worksDurationMs);
        if (projectsCount > 0 || workStreamsCount > 0)
        {
            result +=
                rr.string(
                    RID(ProjectsAndWorkStreams),
                    projectsCount,
                    workStreamsCount);
        }
        if (worksCount > 0 && events.size() > 0)
        {
            result +=
                rr.string(
                    RID(DetailsWE),
                    worksCount,
                    (worksDurationMs + 59999) / 60000,
                    events.size());
        }
        else if (worksCount > 0)
        {
            result +=
                rr.string(
                    RID(DetailsW),
                    worksCount,
                    (worksDurationMs + 59999) / 60000);
        }
        else if (events.size() > 0)
        {
            result +=
                rr.string(
                    RID(DetailsE),
                    events.size());
        }
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Log, but suppress
        qCritical() << ex;
    }
    //  Done
    return result;
}

void DestroyBeneficiaryDialog::_collectDestructionClosure(
        tt3::ws::Beneficiary beneficiary,
        const tt3::ws::Credentials & credentials,
        qsizetype & projectsCount,
        qsizetype & workStreamsCount,
        qsizetype & worksCount,
        tt3::ws::Events & events,
        int64_t & worksDurationMs
    )
{
    for (auto workload : beneficiary->workloads(credentials))
    {
        if (auto project =
            std::dynamic_pointer_cast<tt3::ws::ProjectImpl>(workload))
        {
            _collectDestructionClosure(
                project,
                credentials,
                projectsCount,
                worksCount,
                events,
                worksDurationMs);
        }
        else if (auto workStream =
                 std::dynamic_pointer_cast<tt3::ws::WorkStreamImpl>(workload))
        {
            _collectDestructionClosure(
                workStream,
                credentials,
                workStreamsCount,
                worksCount,
                events,
                worksDurationMs);
        }
    }
}

void DestroyBeneficiaryDialog::_collectDestructionClosure(
        tt3::ws::Project project,
        const tt3::ws::Credentials & credentials,
        qsizetype & projectsCount,
        qsizetype & worksCount,
        tt3::ws::Events & events,
        int64_t & worksDurationMs
    )
{   //  Measure this item...
    projectsCount++;
    for (tt3::ws::Activity activity : project->contributingActivities(credentials))    //  may throw
    {
        for (tt3::ws::Work work : activity->works(credentials)) //  may throw
        {
            worksCount++;
            worksDurationMs += work->startedAt(credentials).msecsTo(work->finishedAt(credentials)); //  may throw
        }
        //  Some events may be related to more tha one
        //  activity, so DON'T count them twice
        events += activity->events(credentials);
    }
    //  ...and children, recursively
    for (tt3::ws::Project child : project->children(credentials))
    {
        _collectDestructionClosure(
            child,
            credentials,
            projectsCount,
            worksCount,
            events,
            worksDurationMs);
    }
}

void DestroyBeneficiaryDialog::_collectDestructionClosure(
        tt3::ws::WorkStream workStream,
        const tt3::ws::Credentials & credentials,
        qsizetype & workStreamsCount,
        qsizetype & worksCount,
        tt3::ws::Events & events,
        int64_t & worksDurationMs
    )
{
    workStreamsCount++;
    for (tt3::ws::Activity activity : workStream->contributingActivities(credentials))    //  may throw
    {
        for (tt3::ws::Work work : activity->works(credentials)) //  may throw
        {
            worksCount++;
            worksDurationMs += work->startedAt(credentials).msecsTo(work->finishedAt(credentials)); //  may throw
        }
        //  Some events may be related to more tha one
        //  activity, so DON'T count them twice
        events += activity->events(credentials);
    }
}

//////////
//  Signal handlers
void DestroyBeneficiaryDialog::accept()
{
    try
    {
        _beneficiary->destroy(_credentials);
        AskYesNoDialog::accept();
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! We cannot cast "this" to "QDialog*" directly
        qCritical() << ex;
        ErrorDialog::show(meAsParent(), ex);
    }
}

//  End of tt3-gui/DestroyBeneficiaryDialog.cpp
