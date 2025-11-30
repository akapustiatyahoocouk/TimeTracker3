//
//  tt3-report/Classes.hpp - Forward declarations & typedefs.
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
    class TT3_REPORT_PUBLIC IReportTemplate;
    class TT3_REPORT_PUBLIC IStyle;
    class TT3_REPORT_PUBLIC ICharacterStyle;
    class TT3_REPORT_PUBLIC IBlockStyle;
    class TT3_REPORT_PUBLIC IParagraphStyle;
    class TT3_REPORT_PUBLIC IListStyle;
    class TT3_REPORT_PUBLIC ITableStyle;
    class TT3_REPORT_PUBLIC ILinkStyle;
    class TT3_REPORT_PUBLIC ISectionStyle;

    //  Collections
    using ReportTemplates = QSet<IReportTemplate*>;
    using Styles = QSet<IStyle*>;
    using CharacterStyles = QSet<ICharacterStyle*>;
    using BlockStyles = QSet<IBlockStyle*>;
    using ParagraphStyles = QSet<IParagraphStyle*>;
    using ListStyles = QSet<IListStyle*>;
    using TableStyles = QSet<ITableStyle*>;
    using LinkStyles = QSet<ILinkStyle*>;
    using SectionStyles = QSet<ISectionStyle*>;
}

//  End of tt3-report/Classes.hpp
