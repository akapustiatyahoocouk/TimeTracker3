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
        virtual auto    description() const -> QString override;
        virtual QIcon   smallIcon() const override;
        virtual QIcon   largeIcon() const override;
        virtual QString preferredExtension() const override;
        virtual void    saveReport(
                                const Report * report,
                                const QString & fileName,
                                ProgressListener progressListener = nullptr
                            ) override;

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
            QString     bodyStyle(const IReportTemplate * reportTemplate);
            QString     paragraphStyle(const ReportParagraph * paragraph);
            QString     paragraphStyle(const IParagraphStyle * style);
            QString     tableStyle(const ReportTable * table);
            QString     tableStyle(const ITableStyle * style);
            QString     tableCellStyle(const ReportTableCell * tableCell);
            QString     linkStyle(const ReportLink * link);
            QString     linkStyle(const ILinkStyle * style);
            QString     listStyle(const ReportList * list);
            QString     listStyle(const IListStyle * style);
            QString     css() const;

            //////////
            //  Implementation
        private:
            int         _nextUnusedStyleNumber = 1;

            struct _BodyStyle
            {
                //////////
                //  Construction
                _BodyStyle(
                        int sequenceNumber,
                        const QString fontFamilyString_,
                        const QString fontSizeString_,
                        const QString fontStyleString_,
                        const QString fontWeightString_,
                        const QString colorString_,
                        const QString backgroundColorString_,
                        const QString widthString_
                    );

                QString         css() const;

                //  Properties ("" == omit property from CSS)
                const QString   className;
                const QString   fontFamilyString;
                const QString   fontSizeString;
                const QString   fontStyleString;
                const QString   fontWeightString;
                const QString   textDeorationString;
                const QString   textDecorationString;
                const QString   colorString;
                const QString   backgroundColorString;
                const QString   widthString;
            };

            struct _ParagraphStyle
            {
                _ParagraphStyle(
                        int sequenceNumber,
                        const QString & fontFamilyString_,
                        const QString & fontSizeString_,
                        const QString & fontStyleString_,
                        const QString & fontWeightString_,
                        const QString & textDecorationString_,
                        const QString & colorString_,
                        const QString & backgroundColorString_,
                        const QString & leftMarginString_,
                        const QString & rightMarginString_,
                        const QString & gapAboveString_,
                        const QString & gapBelowString_,
                        const QString & textAlignmentString_,
                        const QString & borderTypeString_
                    );

                QString         css() const;

                //  Properties ("" == omit property from CSS)
                const QString   className;
                const QString   fontFamilyString;
                const QString   fontSizeString;
                const QString   fontStyleString;
                const QString   fontWeightString;
                const QString   textDecorationString;
                const QString   colorString;
                const QString   backgroundColorString;
                const QString   leftMarginString;
                const QString   rightMarginString;
                const QString   gapAboveString;
                const QString   gapBelowString;
                const QString   textAlignmentString;
                const QString   borderTypeString;
            };

            struct _TableStyle
            {
                _TableStyle(
                        int sequenceNumber,
                        const QString & fontFamilyString_,
                        const QString & fontSizeString_,
                        const QString & fontStyleString_,
                        const QString & fontWeightString_,
                        const QString & textDecorationString_,
                        const QString & colorString_,
                        const QString & backgroundColorString_,
                        const QString & leftMarginString_,
                        const QString & rightMarginString_,
                        const QString & gapAboveString_,
                        const QString & gapBelowString_,
                        const QString & tableBorderTypeString_,
                        const QString & cellBorderTypeString_
                    );

                QString         css() const;

                //  Properties ("" == omit property from CSS)
                const QString   className;
                const QString   fontFamilyString;
                const QString   fontSizeString;
                const QString   fontStyleString;
                const QString   fontWeightString;
                const QString   textDecorationString;
                const QString   colorString;
                const QString   backgroundColorString;
                const QString   leftMarginString;
                const QString   rightMarginString;
                const QString   gapAboveString;
                const QString   gapBelowString;
                const QString   tableBorderTypeString;
                const QString   cellBorderTypeString;
            };

            struct _TableCellStyle
            {
                _TableCellStyle(
                        int sequenceNumber,
                        const QString & fontFamilyString_,
                        const QString & fontSizeString_,
                        const QString & fontStyleString_,
                        const QString & fontWeightString_,
                        const QString & textDecorationString_,
                        const QString & colorString_,
                        const QString & backgroundColorString_,
                        const QString & cellBorderTypeString_,
                        const QString & verticalAlignString_,
                        const QString & preferredWidthString_
                    );

                QString         css() const;

                //  Properties ("" == omit property from CSS)
                const QString   className;
                const QString   fontFamilyString;
                const QString   fontSizeString;
                const QString   fontStyleString;
                const QString   fontWeightString;
                const QString   textDecorationString;
                const QString   colorString;
                const QString   backgroundColorString;
                const QString   cellBorderTypeString;
                const QString   verticalAlignString;
                const QString   preferredWidthString;
            };

            struct _LinkStyle
            {
                _LinkStyle(
                        int sequenceNumber,
                        const QString & fontFamilyString_,
                        const QString & fontSizeString_,
                        const QString & fontStyleString_,
                        const QString & fontWeightString_,
                        const QString & textDecorationString_,
                        const QString & colorString_,
                        const QString & backgroundColorString_,
                        const QString & textDecorationStyleString_
                    );

                QString         css() const;

                //  Properties ("" == omit property from CSS)
                const QString   className;
                const QString   fontFamilyString;
                const QString   fontSizeString;
                const QString   fontStyleString;
                const QString   fontWeightString;
                const QString   textDecorationString;
                const QString   colorString;
                const QString   backgroundColorString;
                const QString   textDecorationStyleString; //  can be null (if no underline)!!!
            };

            struct _ListStyle
            {
                _ListStyle(
                        int sequenceNumber,
                        const QString & fontFamilyString_,
                        const QString & fontSizeString_,
                        const QString & fontStyleString_,
                        const QString & fontWeightString_,
                        const QString & textDecorationString_,
                        const QString & colorString_,
                        const QString & backgroundColorString_,
                        const QString & leftMarginString_,
                        const QString & rightMarginString_,
                        const QString & gapAboveString_,
                        const QString & gapBelowString_,
                        const QString & indentString_
                    );

                QString         css() const;

                //  Properties ("" == omit property from CSS)
                const QString   className;
                const QString   fontFamilyString;
                const QString   fontSizeString;
                const QString   fontStyleString;
                const QString   fontWeightString;
                const QString   textDecorationString;
                const QString   colorString;
                const QString   backgroundColorString;
                const QString   leftMarginString;
                const QString   rightMarginString;
                const QString   gapAboveString;
                const QString   gapBelowString;
                const QString   indentString;
            };

            QSet<_BodyStyle*>       _bodyStyles;
            QSet<_ParagraphStyle*>  _paragraphStyles;
            QSet<_TableStyle*>      _tableStyles;
            QSet<_TableCellStyle*>  _tableCellStyles;
            QSet<_LinkStyle*>       _linkStyles;
            QSet<_ListStyle*>       _listStyles;

            //  Helpers
            QString     _formatColor(const ColorSpec & c);
            QString     _formatFontSpecs(const FontSpecs & fontSpecs);
            QString     _formatSize(const TypographicSize & size);
            QString     _formatFontStyle(const FontStyle & fontStyle);
            QString     _formatFontWeight(const FontStyle & fontStyle);
            QString     _formatTextDecoration(const FontStyle & fontStyle);
            QString     _formatHorizontalAlignment(HorizontalAlignment alignment);
            QString     _formatBorderType(BorderType borderType);
            QString     _formatVerticalAlignment(VerticalAlignment alignment);
            QString     _formatUnderlineMode(UnderlineMode underlineMode);
            QString     _formatPreferredWidth(const TypographicSizeOpt & preferredWidth);

            QString     _paragraphStyle(
                                const QString & fontFamilyString,
                                const QString & fontSizeString,
                                const QString & fontStyleString,
                                const QString & fontWeightString,
                                const QString & textDecorationString,
                                const QString & colorString,
                                const QString & backgroundColorString,
                                const QString & leftMarginString,
                                const QString & rightMarginString,
                                const QString & gapAboveString,
                                const QString & gapBelowString,
                                const QString & textAlignmentString,
                                const QString & borderTypeString
                            );
            QString     _tableStyle(
                                const QString & fontFamilyString,
                                const QString & fontSizeString,
                                const QString & fontStyleString,
                                const QString & fontWeightString,
                                const QString & textDecorationString,
                                const QString & colorString,
                                const QString & backgroundColorString,
                                const QString & leftMarginString,
                                const QString & rightMarginString,
                                const QString & gapAboveString,
                                const QString & gapBelowString,
                                const QString & tableBorderTypeString,
                                const QString & cellBorderTypeString
                            );
            QString     _tableCellStyle(
                                const QString & fontFamilyString,
                                const QString & fontSizeString,
                                const QString & fontStyleString,
                                const QString & fontWeightString,
                                const QString & textDecorationString,
                                const QString & colorString,
                                const QString & backgroundColorString,
                                const QString & cellBorderTypeString,
                                const QString & verticalAlignString,
                                const QString & preferredWidthString
                            );
            QString     _listStyle(
                                const QString & fontFamilyString,
                                const QString & fontSizeString,
                                const QString & fontStyleString,
                                const QString & fontWeightString,
                                const QString & textDecorationString,
                                const QString & colorString,
                                const QString & backgroundColorString,
                                const QString & leftMarginString,
                                const QString & rightMarginString,
                                const QString & gapAboveString,
                                const QString & gapBelowString,
                                const QString & indentString
                            );
            QString     _linkStyle(
                                const QString & fontFamilyString,
                                const QString & fontSizeString,
                                const QString & fontStyleString,
                                const QString & fontWeightString,
                                const QString & textDecorationString,
                                const QString & colorString,
                                const QString & backgroundColorString,
                                const QString & textDecorationStyleString
                            );
        };

        class _HtmlGenerator final
        {
            TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(_HtmlGenerator)

            //////////
            //  Construction/destruction
        public:
            explicit _HtmlGenerator(ProgressListener progressListener);
            ~_HtmlGenerator();

            //////////
            //  Operations
        public:
            QString     generateHtml(const Report * report);

            //////////
            //  Implementation
        private:
            ProgressListener _progressListener; //  can be nullptr
            int             _totalSteps;
            int             _completedSteps;
            _HtmlBuilder    _htmlBuilder;
            _CssBuilder     _cssBuilder;

            int             _nextUnusedId = 1;
            QMap<const ReportElement*, QString> _mapElementsToIds;

            //  Helpers
            void            _completeStep();
            static int      _countParagraps(const Report * report);
            static int      _countParagraps(const ReportFlowElement * flowElement);
            static int      _countParagraps(const ReportBlockElement * blockElement);
            static int      _countParagraps(const ReportList * list);
            static int      _countParagraps(const ReportTable * table);
            void            _assignIdsToElements(const Report * report);
            void            _assignIdsToElements(const ReportFlowElement * flowElement);
            void            _assignIdsToElements(const ReportBlockElement * blockElement);
            void            _generateFlowElement(const ReportFlowElement * flowElement);
            void            _generateParagraph(const ReportParagraph * paragraph);
            void            _generateTableOfContent(const ReportTableOfContent * tableOfContent);
            void            _generateTableOfContent(const Report * report);
            void            _generateTableOfContent(const ReportSection * section);;
            void            _generateTableOfContent(const ReportParagraph * paragraph);
            void            _generateText(const ReportText * text);
            void            _generateTable(const ReportTable * table);
            void            _generateList(const ReportList * list);
        };
    };
}

//  End of tt3-report/HtmlReportFormat.hpp
