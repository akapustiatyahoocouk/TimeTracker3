//
//  tt3-report/ReportTemplate.hpp - tt3-report template API
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
    /// \class IReportTemplate tt3-report/API.hpp
    /// \brief An abstract template that can be chosen to format a printed report.
    class TT3_REPORT_PUBLIC IReportTemplate
    {
        //////////
        //  This is an interface
    protected:
        /// \brief
        ///     The default [interface] constructor.
        IReportTemplate() = default;

        /// \brief
        ///     The default [interface] destructor.
        virtual ~IReportTemplate() = default;

        //////////
        //  Operations
    public:
        /// \brief A type alias to improve code readability.
        using Mnemonic = tt3::util::Mnemonic;

        virtual Mnemonic    mnemonic() const = 0;
        virtual QString     displayName() const = 0;
        virtual PageSetup   pageSetup() const = 0;
        virtual FontSpecs   defaultFontSpecs() const = 0;
        virtual auto        defaultFontSize() const -> TypographicSize = 0;
        virtual FontStyle   defaultFontStyle() const = 0;
        virtual ColorSpec   defaultTextColor() const = 0;
        virtual ColorSpec   defaultBackgroundColor() const = 0;
        virtual auto        defaultListIndent() const -> TypographicSize = 0;
        virtual BorderType  defaultTableBorderType() const = 0;
        virtual auto        defaultLinkUnderlineMode() const -> UnderlineMode = 0;

        virtual auto        styles() const -> Styles = 0;
        virtual auto        characterStyles() const -> CharacterStyles;
        virtual auto        blockStyles() const -> BlockStyles;
        virtual auto        paragraphStyles() const -> ParagraphStyles;
        virtual auto        listStyles() const -> ListStyles;
        virtual auto        tableStyles() const -> TableStyles;
        virtual auto        linkStyles() const -> LinkStyles;
        virtual auto        sectionStyles() const -> SectionStyles;

        virtual auto        findStyleByName(
                                    const QString & name
                                ) const -> IStyle * = 0;
        virtual auto        findCharacterStyleByName(
                                    const QString & name
                                ) const -> ICharacterStyle *;
        virtual auto        findBlockStyleByName(
                                    const QString & name
                                ) const -> IBlockStyle *;
        virtual auto        findParagraphStyleByName(
                                    const QString & name
                                ) const -> IParagraphStyle *;
        virtual auto        findListStyleByName(
                                    const QString & name
                                ) const -> IListStyle *;
        virtual auto        findTableStyleByName(
                                    const QString & name
                                ) const -> ITableStyle *;
        virtual auto        findLinkStyleByName(
                                    const QString & name
                                ) const -> ILinkStyle *;
        virtual auto        findSectionStyleByName(
                                    const QString & name
                                ) const -> ISectionStyle *;

        /// \brief
        ///     Returns the XML DOM document representing this report template.
        /// \return
        ///     The XML DOM document representing this report template.
        QDomDocument        toXmlDocument() const;

        /// \brief
        ///     Returns the formatted XML text representing this report template.
        /// \return
        ///     The formatted XML text representing this report template.
        QString             toXmlString() const;
    };
}

//  End of tt3-report/ReportTemplate.hpp
