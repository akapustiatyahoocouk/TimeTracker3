//
//  tt3-report/ReportTemplateManagerTool.hpp - tt3-report Template Manager tool
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

namespace tt3::report
{
    /// \class ReportTemplateManagerTool tt3-report/API.hpp
    /// \brief The "Report templatte manager" tool.
    class TT3_REPORT_PUBLIC ReportTemplateManagerTool final
        :   public virtual tt3::util::ITool
    {
        TT3_DECLARE_SINGLETON(ReportTemplateManagerTool)

        //////////
        //  tt3::util::ITool
    public:
        virtual Mnemonic    mnemonic() const override;
        virtual QString     displayName() const override;
        virtual QString     description() const override;
        virtual QIcon       smallIcon() const override;
        virtual QIcon       largeIcon() const override;
        virtual bool        isEnabled() const override;
        virtual void        run(QWidget * parent) override;
    };
}

//  End of tt3-report/ReportTemplateManagerTool.hpp
