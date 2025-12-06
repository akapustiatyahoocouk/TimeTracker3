//
//  tt3-report/HtmlReportFormat.hpp - tt3-report HTML report format
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
    /// \class HtmlReportFormat tt3-report/API.hpp
    /// \brief A report format that writes a single HTML file.
    class TT3_REPORT_PUBLIC HtmlReportFormat
        :   public virtual IReportFormat
    {
        TT3_DECLARE_SINGLETON(HtmlReportFormat)

        //////////
        //  IReportFormat
    public:
        virtual auto    mnemonic() const -> Mnemonic override;
        virtual auto    displayName() const -> QString override;
        virtual QIcon   smallIcon() const override;
        virtual QIcon   largeIcon() const override;
        virtual QString preferredExtension() const override;
        virtual void    saveReport(Report * report, const QString & fileName) override;

        //////////
        //  Implementation
    private:
        //  Helpers
        class _HtmlBuilder final
        {
            TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(_HtmlBuilder)

            //////////
            //  Construction/destruction
        public:
            _HtmlBuilder();
            ~_HtmlBuilder();

            //////////
            //  Operations
        public:
            void        reset();
            void        openTag(const QString & tagName);
            void        openTag(const QString & tagName,
                                const QString & attributeName1,
                                const QString & attributeValue1);
            void        openTag(const QString & tagName,
                                const QString & attributeName1,
                                const QString & attributeValue1,
                                const QString & attributeName2,
                                const QString & attributeValue2);
            void        openTag(const QString & tagName,
                                const QString & attributeName1,
                                const QString & attributeValue1,
                                const QString & attributeName2,
                                const QString & attributeValue2,
                                const QString & attributeName3,
                                const QString & attributeValue3);
            void        openTag(const QString & tagName,
                                const QString & attributeName1,
                                const QString & attributeValue1,
                                const QString & attributeName2,
                                const QString & attributeValue2,
                                const QString & attributeName3,
                                const QString & attributeValue3,
                                const QString & attributeName4,
                                const QString & attributeValue4);
            void        closeTag(const QString & tagName);
            void        writeTag(const QString & tagName);
            void        writeTag(const QString & tagName,
                                const QString & attributeName1,
                                const QString & attributeValue1);
            void        writeText(const QString & text);
            QString     html();

            //////////
            //  Implementation
        private:
            QString     _htmlAccumulator;
            QString     _spanAccumulator;

            QStack<QString> _openTags;

            //  Helpers
            bool        _isSpanTag(const QString & tagName);
            void        _commitSpanData();
            QString     _indent(qsizetype level);
            bool        _isTagNameStart(QChar c);
            bool        _isTagNameChar(QChar c);
            bool        _isValidTagName(const QString & tagName);
            bool        _isAttributeNameStart(QChar c);
            bool        _isAttributeNameChar(QChar c);
            bool        _isValidAttributeName(const QString & attributeName);
            bool        _isValidAttributeValue(const QString & attributeValue);
            QString     _escapeAttributeValue(const QString & value);
            QString     _escapeText(const QString & text);
        };

        class _CssBuilder final
        {
            TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(_CssBuilder)

            //////////
            //  Construction/destruction
        public:
            _CssBuilder();
            ~_CssBuilder();

            //////////
            //  Operations
        public:
            void        reset();
            QString     bodyStyle(IReportTemplate * reportTemplate);
            QString     paragraphStyle(ReportParagraph * paragraph);
            QString     paragraphStyle(IParagraphStyle * style);
            QString     tableStyle(ReportTable * table);
            QString     tableCellStyle(ReportTableCell * tableCell);
            QString     linkStyle(ReportLink * link);
            QString     linkStyle(ILinkStyle * linkStyle);
            QString     listStyle(ReportList * list);
            QString     css();

            //////////
            //  Implementation
        private:
            int         _nextUnusedStyleNumber = 1;

            class _BodyStyle;
            class _ParagraphStyle;
            class _TableStyle;
            class _TableCellStyle;
            class _LinkStyle;
            class _ListStyle;

            QSet<_BodyStyle*>       _bodyStyles;
            QSet<_ParagraphStyle*>  _paragraphStyles;
            QSet<_TableStyle*>      _tableStyles;
            QSet<_TableCellStyle*>  _tableCellStyles;
            QSet<_LinkStyle*>       _linkStyles;
            QSet<_ListStyle*>       _listStyles;

            //  Helpers
            QString     _formatColor(const QColor & c);
            QString     _formatFontSpecs(const FontSpecs & fontSpecs);
            QString     _formatSize(const TypographicSize & size);
            QString     _formatFontStyle(int fontStyle);
            QString     _formatFontWeight(int fontStyle);
            QString     _formatHorisontalAlignment(HorizontalAlignment alignment);
            QString     _formatBorderType(BorderType borderType);
            QString     _formatVerticalAlignment(VerticalAlignment alignment);
            QString     _formatUnderlineMode(UnderlineMode underlineMode);
            QString     _formatPreferredWidth(const TypographicSize & preferredWidth);
        };

        class _HtmlGenerator final
        {
            TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(_HtmlGenerator)

            //////////
            //  Construction/destruction
        public:
            _HtmlGenerator();
            ~_HtmlGenerator();

            //////////
            //  Operations
        public:
            QString     generateHtml(Report * report);

            //////////
            //  Implementation
        private:
            int             _totalSteps;
            int             _completedSteps;
            _HtmlBuilder    _htmlBuilder;
            _CssBuilder     _cssBuilder;

            int             _nextUnusedId = 1;
            QMap<ReportElement*, QString>   _mapElementsToIds;

            //  Helpers
            void            _completeStep();
            static int      _countParagraps(Report * report);
            static int      _countParagraps(ReportFlowElement * flowElement);
            static int      _countParagraps(ReportBlockElement * blockElement);
            static int      _countParagraps(ReportList * list);
            static int      _countParagraps(ReportTable * table);
            void            _assignIdsToElements(Report * report);
            void            _assignIdsToElements(ReportFlowElement * flowElement);
            void            _assignIdsToElements(ReportBlockElement * blockElement);
            void            _generateFlowElement(ReportFlowElement * flowElement);
            void            _generateParagraph(ReportParagraph * paragraph);
            void            _generateTableOfContent(ReportTableOfContent * tableOfContent);
            void            _generateTableOfContentForDocument(Report * report);
            void            _generateTableOfContentForSection(ReportSection * section);;
            void            _generateTableOfContentForParagraph(ReportParagraph * paragraph);
            void            _generateText(ReportText * text);
            void            _generateTable(ReportTable * table);
            void            _generateList(ReportList * list);
        };
    };
}

//  End of tt3-report/HtmlReportFormat.hpp
