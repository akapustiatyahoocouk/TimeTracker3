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
            QString     paragraphStyle(IParagraphStyle * style);    //  TODO kill off
            QString     tableStyle(ReportTable * table);
            QString     tableStyle(ITableStyle * style);//  TODO kill off
            QString     tableCellStyle(ReportTableCell * tableCell);
            QString     linkStyle(ReportLink * link);
            QString     linkStyle(ILinkStyle * style);  //  TODO kill off
            QString     listStyle(ReportList * list);
            QString     listStyle(IListStyle * style);  //  TODO kill off
            QString     css();

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
                    ) : className("class" + tt3::util::toString(sequenceNumber)),
                        fontFamilyString(fontFamilyString_),
                        fontSizeString(fontSizeString_),
                        fontStyleString(fontStyleString_),
                        fontWeightString(fontWeightString_),
                        colorString(colorString_),
                        backgroundColorString(backgroundColorString_),
                        widthString(widthString_)
                {
                }

                QString         css() const
                {
                    return "." + className + "\n" +
                           "{\n" +
                           "    font-family: " + fontFamilyString + ";\n" +
                           "    font-size: " + fontSizeString + ";\n" +
                           "    font-style: " + fontStyleString + ";\n" +
                           "    font-weight: " + fontWeightString + ";\n" +
                           "    color: " + colorString + ";\n" +
                           "    background-color: " + backgroundColorString + ";\n" +
                           "    width: " + widthString + ";\n" +
                           "    margin: 0 auto;\n" +
                           "}\n";
                }

                //  Properties ("" == omit property from CSS)
                const QString   className;
                const QString   fontFamilyString;
                const QString   fontSizeString;
                const QString   fontStyleString;
                const QString   fontWeightString;
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
                        const QString & colorString_,
                        const QString & backgroundColorString_,
                        const QString & leftMarginString_,
                        const QString & rightMarginString_,
                        const QString & gapAboveString_,
                        const QString & gapBelowString_,
                        const QString & textAlignmentString_,
                        const QString & borderTypeString_
                    ) : className("class" + tt3::util::toString(sequenceNumber)),
                        fontFamilyString(fontFamilyString_),
                        fontSizeString(fontSizeString_),
                        fontStyleString(fontStyleString_),
                        fontWeightString(fontWeightString_),
                        colorString(colorString_),
                        backgroundColorString(backgroundColorString_),
                        leftMarginString(leftMarginString_),
                        rightMarginString(rightMarginString_),
                        gapAboveString(gapAboveString_),
                        gapBelowString(gapBelowString_),
                        textAlignmentString(textAlignmentString_),
                        borderTypeString(borderTypeString_)
                {
                }

                QString css() const
                {
                    return "." + className + "\n" +
                           "{\n" +
                           "    font-family: " + fontFamilyString + ";\n" +
                           "    font-size: " + fontSizeString + ";\n" +
                           "    font-style: " + fontStyleString + ";\n" +
                           "    font-weight: " + fontWeightString + ";\n" +
                           "    color: " + colorString + ";\n" +
                           "    background-color: " + backgroundColorString + ";\n" +
                           "    margin-left: " + leftMarginString + ";\n" +
                           "    margin-right: " + rightMarginString + ";\n" +
                           "    margin-top: " + gapAboveString + ";\n" +
                           "    margin-bottom: " + gapBelowString + ";\n" +
                           "    text-align: " + textAlignmentString + ";\n" +
                           "    border-style: " + borderTypeString + ";\n" +
                           "    border-color: " + colorString + ";\n" +
                           "    line-height: 100%;\n" +
                           "}\n";
                }

                //  Properties ("" == omit property from CSS)
                const QString   className;
                const QString   fontFamilyString;
                const QString   fontSizeString;
                const QString   fontStyleString;
                const QString   fontWeightString;
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
                        const QString & colorString_,
                        const QString & backgroundColorString_,
                        const QString & leftMarginString_,
                        const QString & rightMarginString_,
                        const QString & gapAboveString_,
                        const QString & gapBelowString_,
                        const QString & tableBorderTypeString_,
                        const QString & cellBorderTypeString_
                    ) : className("class" + tt3::util::toString(sequenceNumber)),
                        fontFamilyString(fontFamilyString_),
                        fontSizeString(fontSizeString_),
                        fontStyleString(fontStyleString_),
                        fontWeightString(fontWeightString_),
                        colorString(colorString_),
                        backgroundColorString(backgroundColorString_),
                        leftMarginString(leftMarginString_),
                        rightMarginString(rightMarginString_),
                        gapAboveString(gapAboveString_),
                        gapBelowString(gapBelowString_),
                        tableBorderTypeString(tableBorderTypeString_),
                        cellBorderTypeString(cellBorderTypeString_)
                {
                }

                QString css() const
                {
                    return "." + className + "\n" +
                           "{\n" +
                           "    font-family: " + fontFamilyString + ";\n" +
                           "    font-size: " + fontSizeString + ";\n" +
                           "    font-style: " + fontStyleString + ";\n" +
                           "    font-weight: " + fontWeightString + ";\n" +
                           "    color: " + colorString + ";\n" +
                           "    background-color: " + backgroundColorString + ";\n" +
                           "    margin-left: " + leftMarginString + ";\n" +
                           "    margin-right: " + rightMarginString + ";\n" +
                           "    margin-top: " + gapAboveString + ";\n" +
                           "    margin-bottom: " + gapBelowString + ";\n" +
                           "    border-style: " + tableBorderTypeString + ";\n" +
                           /* TODO kill off "    border-width: " + ((tableBorderTypeString == "") ? "auto" : "1px") + ";\n" + */
                           "    border-color: " + colorString + ";\n" +
                           "    border-spacing: 0pt;\n" +
                           "    border-collapse: collapse;\n" +
                           "}\n";
                }

                //  Properties ("" == omit property from CSS)
                const QString   className;
                const QString   fontFamilyString;
                const QString   fontSizeString;
                const QString   fontStyleString;
                const QString   fontWeightString;
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
                        const QString & colorString_,
                        const QString & backgroundColorString_,
                        const QString & cellBorderTypeString_,
                        const QString & verticalAlignString_,
                        const QString & preferredWidthString_
                    ) : className("class" + tt3::util::toString(sequenceNumber)),
                        fontFamilyString(fontFamilyString_),
                        fontSizeString(fontSizeString_),
                        fontStyleString(fontStyleString_),
                        fontWeightString(fontWeightString_),
                        colorString(colorString_),
                        backgroundColorString(backgroundColorString_),
                        cellBorderTypeString(cellBorderTypeString_),
                        verticalAlignString(verticalAlignString_),
                        preferredWidthString(preferredWidthString_)
                {
                }

                QString css() const
                {
                    return "." + className + "\n" +
                           "{\n" +
                           "    font-family: " + fontFamilyString + ";\n" +
                           "    font-size: " + fontSizeString + ";\n" +
                           "    font-style: " + fontStyleString + ";\n" +
                           "    font-weight: " + fontWeightString + ";\n" +
                           "    color: " + colorString + ";\n" +
                           "    background-color: " + backgroundColorString + ";\n" +
                           "    border-style: " + cellBorderTypeString + ";\n" +
                           "    border-width: " + ((cellBorderTypeString == "solid") ? "1px" : "auto") + ";\n" +
                           "    border-color: " + colorString + ";\n" +
                           "    vertical-align: " + verticalAlignString + ";\n" +
                           "    width: " + preferredWidthString + ";\n" +
                           "}\n";
                }

                //  Properties ("" == omit property from CSS)
                const QString   className;
                const QString   fontFamilyString;
                const QString   fontSizeString;
                const QString   fontStyleString;
                const QString   fontWeightString;
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
                        const QString & colorString_,
                        const QString & backgroundColorString_,
                        const QString & textDecorationStyleString_
                    ) : className("class" + tt3::util::toString(sequenceNumber)),
                        fontFamilyString(fontFamilyString_),
                        fontSizeString(fontSizeString_),
                        fontStyleString(fontStyleString_),
                        fontWeightString(fontWeightString_),
                        colorString(colorString_),
                        backgroundColorString(backgroundColorString_),
                        textDecorationStyleString(textDecorationStyleString_)
                {
                }

                QString css() const
                {
                    return "." + className + "\n" +
                           "{\n" +
                           "    font-family: " + fontFamilyString + ";\n" +
                           "    font-size: " + fontSizeString + ";\n" +
                           "    font-style: " + fontStyleString + ";\n" +
                           "    font-weight: " + fontWeightString + ";\n" +
                           "    color: " + colorString + ";\n" +
                           "    background-color: " + backgroundColorString + ";\n" +
                           (!textDecorationStyleString.isEmpty() ?
                                ("    text-decoration-line: underline;\n" + QString() +
                                 "    text-decoration-style:" + textDecorationStyleString + ";\n" +
                                 "    text-decoration-color: " + colorString + ";\n") :
                                "") +
                           "}\n";
                }

                //  Properties ("" == omit property from CSS)
                const QString   className;
                const QString   fontFamilyString;
                const QString   fontSizeString;
                const QString   fontStyleString;
                const QString   fontWeightString;
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
                        const QString & colorString_,
                        const QString & backgroundColorString_,
                        const QString & leftMarginString_,
                        const QString & rightMarginString_,
                        const QString & gapAboveString_,
                        const QString & gapBelowString_,
                        const QString & indentString_
                    ) : className("class" + tt3::util::toString(sequenceNumber)),
                        fontFamilyString(fontFamilyString_),
                        fontSizeString(fontSizeString_),
                        fontStyleString(fontStyleString_),
                        fontWeightString(fontWeightString_),
                        colorString(colorString_),
                        backgroundColorString(backgroundColorString_),
                        leftMarginString(leftMarginString_),
                        rightMarginString(rightMarginString_),
                        gapAboveString(gapAboveString_),
                        gapBelowString(gapBelowString_),
                        indentString(indentString_)
                {
                }

                QString css() const
                {
                    return "." + className + "-table\n" +
                           "{\n" +
                           "    font-family: " + fontFamilyString + ";\n" +
                           "    font-size: " + fontSizeString + ";\n" +
                           "    font-style: " + fontStyleString + ";\n" +
                           "    font-weight: " + fontWeightString + ";\n" +
                           "    color: " + colorString + ";\n" +
                           "    background-color: " + backgroundColorString + ";\n" +
                           "    margin-left: " + leftMarginString + ";\n" +
                           "    margin-right: " + rightMarginString + ";\n" +
                           "    margin-top: " + gapAboveString + ";\n" +
                           "    margin-bottom: " + gapBelowString + ";\n" +
                           "    border-style: none;\n" +
                           "    border-collapse: collapse;\n" +
                           "    border-spacing:0pt;\n" +
                           "    padding: 0pt;\n" +
                           "}\n" +
                           "\n" +
                           "." + className + "-td1\n" +
                           "{\n" +
                           "    font-family: " + fontFamilyString + ";\n" +
                           "    font-size: " + fontSizeString + ";\n" +
                           "    font-style: " + fontStyleString + ";\n" +
                           "    font-weight: " + fontWeightString + ";\n" +
                           "    color: " + colorString + ";\n" +
                           "    background-color: " + backgroundColorString + ";\n" +
                           "    margin-left: 0pt;\n" +
                           "    margin-right: 0pt;\n" +
                           "    margin-top: 0pt;\n" +
                           "    margin-bottom: 0pt;\n" +
                           "    border-style: none;\n" +
                           "    width: " + indentString + ";\n" +
                           "    border-spacing:0pt;\n" +
                           "    padding: 0pt;\n" +
                           "    vertical-align: top;\n" +
                           "}\n" +
                           "\n" +
                           "." + className + "-td2\n" +                            "{\n" +
                           "    font-family: " + fontFamilyString + ";\n" +
                           "    font-size: " + fontSizeString + ";\n" +
                           "    font-style: " + fontStyleString + ";\n" +
                           "    font-weight: " + fontWeightString + ";\n" +
                           "    color: " + colorString + ";\n" +
                           "    background-color: " + backgroundColorString + ";\n" +
                           "    margin-left: 0pt;\n" +
                           "    margin-right: 0pt;\n" +
                           "    margin-top: 0pt;\n" +
                           "    margin-bottom: 0pt;\n" +
                           "    border-style: none;\n" +
                           "    border-spacing:0pt;\n" +
                           "    padding: 0pt;\n" +
                           "    vertical-align: top;\n" +
                           "}\n";
                }

                //  Properties ("" == omit property from CSS)
                const QString   className;
                const QString   fontFamilyString;
                const QString   fontSizeString;
                const QString   fontStyleString;
                const QString   fontWeightString;
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
            void            _generateTableOfContentForReport(Report * report);
            void            _generateTableOfContentForSection(ReportSection * section);;
            void            _generateTableOfContentForParagraph(ReportParagraph * paragraph);
            void            _generateText(ReportText * text);
            void            _generateTable(ReportTable * table);
            void            _generateList(ReportList * list);
        };
    };
}

//  End of tt3-report/HtmlReportFormat.hpp
