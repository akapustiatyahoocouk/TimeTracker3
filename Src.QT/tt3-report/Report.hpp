//
//  tt3-report/Report.hpp - tt3-report Report IR
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
    /// \class Report tt3-report/API.hpp
    /// \brief The generated report IR.
    class TT3_REPORT_PUBLIC Report final
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Report)

        friend class ReportElement;
        friend class ReportFlowElement;
        friend class ReportSpanElement;
        friend class ReportParagraph;
        friend class ReportSection;
        friend class ReportList;
        friend class ReportTable;
        friend class ReportAnchor;
        friend class ReportLink;
        friend class ReportText;

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs the report.
        /// \param name
        ///     The report name.
        /// \param reportTemplate
        ///     The report template; cannot be nullptr.
        ///     IMPORTANT: must NOT be "delete"d whihc the Report
        ///     instance(s) that use that ReportTemplate exist.
        Report(const QString & name,
               IReportTemplate * reportTemplate
            );

        /// \brief
        ///     The class destructor.
        ~Report();

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the report name.
        /// \return
        ///     The report name.
        QString         name() const { return _name; }

        /// \brief
        ///     Returns the report template.
        /// \return
        ///     The report template.
        auto            reportTemplate() const -> IReportTemplate * { return _reportTemplate; }

        /// \brief
        ///     Returns the UTC date+time when this Report was created.
        /// \return
        ///     The UTC date+time when this Report was created.
        QDateTime       createdAt() const { return _createdAt; }

        /// \brief
        ///     Returns the list of all Sections in this Report.
        /// \return
        ///     The list of all Sections in this Report, in order of creation.
        ReportSections  sections() const { return _sections; }

        /// \brief
        ///     Returns the list of all Anchors in this Report.
        /// \return
        ///     The list of all Anchors in this Report, in order of creation.
        ReportAnchors   anchors() const { return _anchors; }

        /// \brief
        ///     Returns the list of all Links in this Report.
        /// \return
        ///     The list of all Links in this Report, in order of creation.
        ReportLinks     links() const { return _links; }

        /// \brief
        ///     Creates a new Section at the end of this Report.
        /// \param name
        ///     The name for the new Section.
        /// \param style
        ///     The style for the new Section; nullptr == none.
        /// \return
        ///     The newly created Section.
        ReportSection * createSection(
                                const QString & name,
                                ISectionStyle * style = nullptr
                            );

        //////////
        //  Implementation
    private:
        QString         _name;
        IReportTemplate*_reportTemplate;
        QDateTime       _createdAt;

        ReportSections  _sections;  //  in order of creation
        ReportAnchors   _anchors;   //  in order of creation
        ReportLinks     _links;     //  in order of creation

#ifdef QT_DEBUG
        //////////
        //  Validation
    private:
        void            _validate();
#endif
    };

    /// \class ReportElement tt3-report/API.hpp
    /// \brief A generic element of a Report.
    class TT3_REPORT_PUBLIC ReportElement
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ReportElement)

        friend class Report;
        friend class ReportFlowElement;
        friend class ReportBlockElement;
        friend class ReportSection;
        friend class ReportSpanElement;
        friend class ReportParagraph;
        friend class ReportText;
        friend class ReportList;
        friend class ReportListItem;
        friend class ReportTable;
        friend class ReportTableCell;
        friend class ReportAnchor;
        friend class ReportLink;
        friend class ReportInternalLink;

        //////////
        //  Construction/destruction - from friends only
    private:
        explicit ReportElement(Report * report);
        virtual ~ReportElement();

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the Report to which this Element belongs.
        /// \return
        ///     The Report to which this Element belongs.
        Report *        report() const { return _report; }

        /// \brief
        ///     Returns the immediate parent of this Element.
        /// \return
        ///     The immediate parent of this Element; nullptr == none.
        virtual auto    parent() const -> ReportElement * = 0;

        /// \brief
        ///     Returns the list of anchors assigned to this Element.
        /// \return
        ///     The list of anchors assigned to this Element,
        ///     in order of creation.
        ReportAnchors   anchors() const { return _anchors; }

        /// \brief
        ///     Creates a new Anchor associated with this Element.
        /// \return
        ///     The newly created Anchor.
        ReportAnchor *  createAnchor();

        /// \brief
        ///     Returns the list of font specs of the font used
        ///     by this element, in order of decreasing priority.
        /// \details
        ///     If this element does not have an associated
        ///     style, or if that style does not specify the font
        ///     specs, goes to the parent element, eventuually
        ///     returns the default value from the report template.
        /// \return
        ///     The list of face names of the font used by this element.
        virtual auto    resolveFontSpecs() const -> FontSpecs = 0;

        /// \brief
        ///     Returns the resolved font size for this element.
        /// \details
        ///     This is taken from the element's associated style
        ///     or, if that style does not specify the font size,
        ///     goes to the parent element, eventuually returns
        ///     the default value from the report template.
        /// \return
        ///     The resolved font size for this element.
        virtual auto    resolveFontSize() const -> TypographicSize = 0;

        /// \brief
        ///     Returns the resolved font style for this element.
        /// \details
        ///     This is taken from the element's associated style
        ///     or, if that style does not specify the font style,
        ///     goes to the parent element, eventuually returns
        ///     the default value from the report template.
        /// \return
        ///     The resolved font style for this element.
        virtual auto    resolveFontStyle() const -> FontStyle = 0;

        /// \brief
        ///     Returns the resolved text color for this element.
        /// \details
        ///     This is taken from the element's associated style
        ///     or, if that style does not specify the text color,
        ///     goes to the parent element, eventuually returns
        ///     the default value from the report template.
        /// \return
        ///     The resolved text color for this element.
        virtual auto    resolveTextColor() const -> ColorSpec = 0;

        /// \brief
        ///     Returns the resolved background color for this element.
        /// \details
        ///     This is taken from the element's associated style
        ///     or, if that style does not specify the background color,
        ///     goes to the parent element, eventuually returns
        ///     the default value from the report template.
        /// \return
        ///     The resolved background color for this element.
        virtual auto    resolveBackgroundColor() const -> ColorSpec = 0;

        //////////
        //  Implementation
    private:
        Report *        _report;
        ReportAnchors   _anchors;   //  in order of creation
    };

    /// \class ReportFlowElement tt3-report/API.hpp
    /// \brief A report element that represents a top-down sequential "flow" of "blocks".
    class TT3_REPORT_PUBLIC ReportFlowElement
        :   public ReportElement
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ReportFlowElement)

        friend class ReportBlockElement;
        friend class ReportSection;
        friend class ReportListItem;
        friend class ReportTableCell;

        //////////
        //  Construction/destruction - from friends only
    private:
        explicit ReportFlowElement(Report * report);
        virtual ~ReportFlowElement();

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the list of immediate children of this FlowElement.
        /// \return
        ///     The list of immediate children of this FlowElement,
        ///     in order of creation.
        auto            children() const -> ReportBlockElements { return _children; }

        /// \brief
        ///     Creates a new empty Paragraph at the end of this Flow.
        /// \param style
        ///     The style for the new Paragraph; nullptr == none.
        /// \return
        ///     The newly created Paragraph.
        ReportParagraph*createParagraph(IParagraphStyle * style = nullptr);

        /// \brief
        ///     Creates a new empty Table at the end of this Flow.
        /// \param style
        ///     The style for the new Table; nullptr == none.
        /// \return
        ///     The newly created Table.
        ReportTable *   createTable(ITableStyle * style = nullptr);

        /// \brief
        ///     Creates a new empty List at the end of this Flow.
        /// \param style
        ///     The style for the new List; nullptr == none.
        /// \return
        ///     The newly created List.
        ReportList *    createList(IListStyle * style = nullptr);

        /// \brief
        ///     Creates a new TOC at the end of this Flow.
        /// \return
        ///     The newly created TOC.
        auto            createTableOfContent() -> ReportTableOfContent *;

        //////////
        //  Implementation
    private:
        ReportBlockElements _children;  //  in order of creations
    };

    /// \class ReportBlockElement tt3-report/API.hpp
    /// \brief A report element that causes text flow break before and after itself.
    class TT3_REPORT_PUBLIC ReportBlockElement
        :   public ReportElement
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ReportBlockElement)

        friend class ReportFlowElement;
        friend class ReportParagraph;
        friend class ReportList;
        friend class ReportTable;
        friend class ReportTableOfContent;

        //////////
        //  Construction/destruction - from friends only
    private:
        explicit ReportBlockElement(ReportFlowElement * parent);
        virtual ~ReportBlockElement();

        //////////
        //  ReportElement
    public:
        virtual auto    parent() const -> ReportFlowElement * override { return _parent; }

        //////////
        //  Operations
    public:

        //////////
        //  Implementation
    private:
        ReportFlowElement * _parent;    //  never nullptr
    };

    /// \class ReportSection tt3-report/API.hpp
    /// \brief A section within a report.
    class TT3_REPORT_PUBLIC ReportSection final
        :   public ReportFlowElement
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ReportSection)

        friend class Report;

        //////////
        //  Construction/destruction - from friends only
    private:
        ReportSection(
                Report * report,
                const QString & name,
                ISectionStyle * style
            );  //  registers Section with Report
        virtual ~ReportSection();

        //////////
        //  ReportElement
    public:
        virtual auto    parent() const -> ReportElement * override { return nullptr; }
        virtual auto    resolveFontSpecs() const -> FontSpecs override;
        virtual auto    resolveFontSize() const -> TypographicSize override;
        virtual auto    resolveFontStyle() const -> FontStyle override;
        virtual auto    resolveTextColor() const -> ColorSpec override;
        virtual auto    resolveBackgroundColor() const -> ColorSpec override;

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the name of this section.
        /// \return
        ///     The name of this section.
        QString         name() const { return _name; }

        /// \brief
        ///     Sets the name of this section.
        /// \param name
        ///     The new name for this section.
        void            setName(const QString & name) { _name = name; }

        /// \brief
        ///     Returns the style of this section.
        /// \return
        ///     The style of this section; nullptr == none.
        ISectionStyle * style() const { return _style; }

        /// \brief
        ///     Sets the style of this Section.
        /// \details
        ///     The new Style (unless nullptr) must belong to
        ///     the report template associated with the Report.
        /// \param style
        ///     The new style for this Section; nullptr == none.
        void            setStyle(ISectionStyle * style);

        //////////
        //  Implementation
    private:
        QString         _name;
        ISectionStyle * _style; //  may be null
    };

    /// \class ReportParagraph tt3-report/API.hpp
    /// \brief A paragraph within a report.
    class TT3_REPORT_PUBLIC ReportParagraph final
        :   public ReportBlockElement
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ReportParagraph)

        friend class ReportFlowElement;
        friend class ReportSpanElement;

        //////////
        //  Construction/destruction - from friends only
    private:
        ReportParagraph(
                ReportFlowElement * parent,
                IParagraphStyle * style
            );
        virtual ~ReportParagraph();

        //////////
        //  ReportElement
    public:
        virtual auto    resolveFontSpecs() const -> FontSpecs override;
        virtual auto    resolveFontSize() const -> TypographicSize override;
        virtual auto    resolveFontStyle() const -> FontStyle override;
        virtual auto    resolveTextColor() const -> ColorSpec override;
        virtual auto    resolveBackgroundColor() const -> ColorSpec override;

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the style of this paragraph.
        /// \return
        ///     The style of this paragraph; nullptr == none.
        auto            style() const -> IParagraphStyle * { return _style; }

        /// \brief
        ///     Sets the style of this Paragraph.
        /// \details
        ///     The new Style (unless nullptr) must belong to
        ///     the report template associated with the Report.
        /// \param style
        ///     The new style for this Paragrapph; nullptr == none.
        void            setStyle(IParagraphStyle * style);

        /// \brief
        ///     Returns the list of children of this Paragraph.
        /// \return
        ///     The list of children of this Paragraph, in order of creation.
        auto            children() const -> ReportSpanElements { return _children; }

        /// \brief
        ///     Creates a new Text at the end of this Paragraph.
        /// \param text
        ///     The actual text to appear in the report.
        /// \param style
        ///     The style for the new Text, nullptr == none.
        /// \return
        ///     The newly created Text.
        ReportText *    createText(
                                const QString & text,
                                ICharacterStyle * style = nullptr
                            );

        /// \brief
        ///     Creates a new Picture at the end of this Paragraph.
        /// \param width
        ///     The width of the Picture.
        /// \param height
        ///     The height of the Picture.
        /// \param image
        ///     The image to be drawi as a Picture.
        /// \return
        ///     The newly created Picture.
        ReportPicture * createPicture(
                                const TypographicSize & width,
                                const TypographicSize & height,
                                const QImage & image
                            );

        //////////
        //  Implementation
    private:
        IParagraphStyle *   _style; //  may be null
        ReportSpanElements  _children;
    };

    /// \class ReportSpanElement tt3-report/API.hpp
    /// \brief A report element that does not cause a flow break.
    class TT3_REPORT_PUBLIC ReportSpanElement
        :   public ReportElement
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ReportSpanElement)

        friend class ReportParagraph;
        friend class ReportText;
        friend class ReportPicture;
        friend class ReportLink;

        //////////
        //  Construction/destruction - from friends only
    private:
        explicit ReportSpanElement(ReportParagraph * paragraph);
        virtual ~ReportSpanElement();

        //////////
        //  ReportElement
    public:
        virtual auto    parent() const -> ReportParagraph * override { return _paragraph; }

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the Paragraph to which this SpanElement belongs.
        /// \return
        ///     The Paragraph to which this SpanElement belongs, never nullptr.
        ReportParagraph*paragraph() const { return _paragraph; }

        /// \brief
        ///     Returns the Link associated with this element.
        /// \return
        ///     The Link associated with this element; nullptr == none.
        ReportLink *    link() const { return _link; }

        /// \brief
        ///     Creates a new InternalLink for this Span.
        /// \param anchor
        ///     The link target Anchow (must belong to the same Report).
        /// \param style
        ///     The style for the new Link, nullptr == none.
        /// \return
        ///     The newly created InternalLink.
        auto            createInternalLink(
                                ReportAnchor * anchor,
                                ILinkStyle * style = nullptr
                            ) -> ReportInternalLink *;

        /// \brief
        ///     Creates a new ExternalLink for this Span.
        /// \param url
        ///     The link target URL.
        /// \param style
        ///     The style for the new Link, nullptr == none.
        /// \return
        ///     The newly created ExternalLink.
        auto            createExternalLink(
                                const QString & url,
                                ILinkStyle * style = nullptr
                            ) -> ReportExternalLink *;

        //////////
        //  Implementation
    private:
        ReportParagraph*_paragraph; //  ...to which this span element belongs
        ReportLink *    _link = nullptr;
    };

    /// \class ReportText tt3-report/API.hpp
    /// \brief A paragraph within a report.
    class TT3_REPORT_PUBLIC ReportText final
        :   public ReportSpanElement
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ReportText)

        friend class ReportParagraph;

        //////////
        //  Construction/destruction - from friends only
    private:
        ReportText(
                ReportParagraph * paragraph,
                const QString & text,
                ICharacterStyle * style
            );
        virtual ~ReportText();

        //////////
        //  ReportElement
    public:
        virtual auto    resolveFontSpecs() const -> FontSpecs override;
        virtual auto    resolveFontSize() const -> TypographicSize override;
        virtual auto    resolveFontStyle() const -> FontStyle override;
        virtual auto    resolveTextColor() const -> ColorSpec override;
        virtual auto    resolveBackgroundColor() const -> ColorSpec override;

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the text of this element.
        /// \return
        ///     The text of this element.
        QString         text() const { return _text; }

        /// \brief
        ///     Sets the text of this element.
        /// \param text
        ///     The new text for this element.
        void            setText(const QString & text) { _text = text; }

        /// \brief
        ///     Returns the Style assigned to this element.
        /// \return
        ///     The Style assigned to this element; nullptr == none.
        ICharacterStyle*style() const { return _style; }

        /// \brief
        ///     Sets the style of this element.
        /// \details
        ///     The new Style (unless nullptr) must belong to
        ///     the report template associated with the Report.
        /// \param style
        ///     The new style for this element; nullptr == none.
        void            setStyle(ICharacterStyle * style);

        //////////
        //  Implementation
    private:
        QString             _text;
        ICharacterStyle *   _style; //  can be null
    };

    /// \class ReportPicture tt3-report/API.hpp
    /// \brief A picture within a report.
    class TT3_REPORT_PUBLIC ReportPicture final
        :   public ReportSpanElement
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ReportPicture)

        friend class ReportParagraph;

        //////////
        //  Construction/destruction - from friends only
    private:
        ReportPicture(
                ReportParagraph * paragraph,
                const TypographicSize & width,
                const TypographicSize & height,
                const QImage & image
            );
        virtual ~ReportPicture();

        //////////
        //  ReportElement
    public:
        virtual auto    resolveFontSpecs() const -> FontSpecs override;
        virtual auto    resolveFontSize() const -> TypographicSize override;
        virtual auto    resolveFontStyle() const -> FontStyle override;
        virtual auto    resolveTextColor() const -> ColorSpec override;
        virtual auto    resolveBackgroundColor() const -> ColorSpec override;

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the width of the picture.
        /// \return
        ///     The width of the picture.
        TypographicSize width() const { return _width; }

        /// \brief
        ///     Returns the height of the picture.
        /// \return
        ///     The height of the picture.
        TypographicSize height() const { return _height; }

        /// \brief
        ///     Returns the image to display as a picture.
        /// \return
        ///     The image to display as a picture.
        QImage          image() const { return _image; }

        //////////
        //  Implementation
    private:
        TypographicSize _width;     //  always > 0
        TypographicSize _height;    //  always > 0
        QImage          _image;
    };

    /// \class ReportList tt3-report/API.hpp
    /// \brief A list within a report.
    class TT3_REPORT_PUBLIC ReportList final
        :   public ReportBlockElement
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ReportList)

        friend class ReportFlowElement;
        friend class ReportListItem;

        //////////
        //  Construction/destruction - from friends only
    private:
        ReportList(
                ReportFlowElement * parent,
                IListStyle * style
            );
        virtual ~ReportList();

        //////////
        //  ReportElement
    public:
        virtual auto    resolveFontSpecs() const -> FontSpecs override;
        virtual auto    resolveFontSize() const -> TypographicSize override;
        virtual auto    resolveFontStyle() const -> FontStyle override;
        virtual auto    resolveTextColor() const -> ColorSpec override;
        virtual auto    resolveBackgroundColor() const -> ColorSpec override;

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the Style assigned to this List.
        /// \return
        ///     The Style assigned to this List; nullptr == none.
        IListStyle *        style() const { return _style; }

        /// \brief
        ///     Sets the style of this List.
        /// \details
        ///     The new Style (unless nullptr) must belong to
        ///     the report template associated with the Report.
        /// \param style
        ///     The new style for this List; nullptr == none.
        void                setStyle(IListStyle * style);

        /// \brief
        ///     Returns the list of all Items in this List.
        /// \return
        ///     The list of all Items in this List, in order of creation.
        ReportListItems     items() const { return _items; }

        /// \brief
        ///     Creates a new ListItem at the end of this List.
        /// \param label
        ///     The label to appear as a list bullet, "" == default.
        /// \return
        ///     The newly created ListItem.
        ReportListItem *    createItem(const QString & label = "");

        //////////
        //  Implementation
    private:
        IListStyle *        _style; //  may be null
        ReportListItems     _items;
    };

    /// \class ReportListItem tt3-report/API.hpp
    /// \brief An item within a list.
    class TT3_REPORT_PUBLIC ReportListItem final
        :   public ReportFlowElement
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ReportListItem)

        friend class ReportList;

        //////////
        //  Construction/destruction - from friends only
    private:
         ReportListItem(
                ReportList * list,
                const QString & label);
        virtual ~ReportListItem();

        //////////
        //  ReportElement
    public:
        virtual auto    parent() const -> ReportElement * override { return _list; }
        virtual auto    resolveFontSpecs() const -> FontSpecs override;
        virtual auto    resolveFontSize() const -> TypographicSize override;
        virtual auto    resolveFontStyle() const -> FontStyle override;
        virtual auto    resolveTextColor() const -> ColorSpec override;
        virtual auto    resolveBackgroundColor() const -> ColorSpec override;

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the List to which this ListItem belongs.
        /// \return
        ///     The List to which this ListItem belongs; never nullptr.
        ReportList *    list() const { return _list; }

        /// \brief
        ///     Returns the label of this ListItem.
        /// \return
        ///     The label of this ListItem.
        QString         label() const { return _label; }

        /// \brief
        ///     Sets the label of this ListItem.
        /// \param label
        ///     The new label for this ListItem.
        void            setLabel(const QString & label) { _label = label; }

        //////////
        //  Implementation
    private:
        ReportList *    _list;
        QString         _label; //  "1.", "2.1", etc.
    };

    /// \class ReportTable tt3-report/API.hpp
    /// \brief A table within a report.
    class TT3_REPORT_PUBLIC ReportTable final
        :   public ReportBlockElement
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ReportTable)

        friend class ReportFlowElement;
        friend class ReportTableCell;

        //////////
        //  Construction/destruction - from friends only
    private:
        ReportTable(
                ReportFlowElement * parent,
                ITableStyle * style
            );
        virtual ~ReportTable();

        //////////
        //  ReportElement
    public:
        virtual auto    resolveFontSpecs() const -> FontSpecs override;
        virtual auto    resolveFontSize() const -> TypographicSize override;
        virtual auto    resolveFontStyle() const -> FontStyle override;
        virtual auto    resolveTextColor() const -> ColorSpec override;
        virtual auto    resolveBackgroundColor() const -> ColorSpec override;

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns The style assigned to this Table.
        /// \return
        ///     The style assigned to this Table; nullptr == none.
        ITableStyle *   style() const { return _style; }

        /// \brief
        ///     Sets the style of this Table.
        /// \details
        ///     The new Style (unless nullptr) must belong to
        ///     the report template associated with the Report.
        /// \param style
        ///     The new style for this Table; nullptr == none.
        void            setStyle(ITableStyle * style);

        /// \brief
        ///     Returns the list of all Cells of this Table.
        /// \return
        ///     The list of all Cells of this Table, in order of creation.
        ReportTableCells cells() const { return _cells; }

        /// \brief
        ///     Returns the number of columns in this Table.
        /// \return
        ///     The number of columns in this Table.
        int             columnCount() const { return _columnCount; }

        /// \brief
        ///     Returns the number of rows in this Table.
        /// \return
        ///     The number of rows in this Table.
        int             rowCount() const { return _rowCount; }

        /// \brief
        ///     Creates a new TableCell in this Table.
        /// \param startColumn
        ///     The start colunm for the TableCell (0-based).
        /// \param startRow
        ///     The start row for the TableCell (0-based).
        /// \param columnSpan
        ///     The column span for the TableCell (1 or more).
        /// \param rowSpan
        ///     The row span for the TableCell (1 or more).
        /// \param contentAlignment
        ///     The alignment of content within thew TableCell.
        /// \param preferredWidth
        ///     The preferred width of the column(s) occupied by
        ///     the TableCell; no value == choose automatically.
        /// \return
        ///     The newly created TableCell.
        ReportTableCell*createCell(
                                int startColumn,
                                int startRow,
                                int columnSpan,
                                int rowSpan,
                                VerticalAlignment contentAlignment = VerticalAlignment::Top,
                                TypographicSizeOpt preferredWidth = TypographicSizeOpt()
                            );

        /// \brief
        ///     Returns the TableCell that appears at the specified row/column.
        /// \param column
        ///    The 0-based column number.
        /// \param row
        ///    The 0-based row number.
        /// \return
        ///     The TableCell that appears at the specified row/column;
        ///     nullptr if there is no such TableCell.
        ReportTableCell*findTableCellAt(int column, int row);

        /// \brief
        ///     Returns the resolved table border style for this table.
        /// \details
        ///     This is taken from the element's associated style
        ///     or, if that style does not specify the table border style
        ///     goes to the parent element, eventuually returns
        ///     the default value from the report template.
        /// \return
        ///     The resolved table border style for this table.
        auto            resolveTableBorderType() const -> BorderType;

        /// \brief
        ///     Returns the resolved cell border style for this table.
        /// \details
        ///     This is taken from the element's associated style
        ///     or, if that style does not specify the cell border style
        ///     goes to the parent element, eventuually returns
        ///     the default value from the report template.
        /// \return
        ///     The resolved cell border style for this table.
        auto            resolveCellBorderType() const -> BorderType;

        //////////
        //  Implementation
    private:
        ITableStyle *   _style; //  may be null
        ReportTableCells    _cells;
        int             _columnCount = 0;
        int             _rowCount = 0;
    };

    /// \class ReportTableCell tt3-report/API.hpp
    /// \brief A cll within a table.
    class TT3_REPORT_PUBLIC ReportTableCell final
        :   public ReportFlowElement
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ReportTableCell)

        friend class ReportTable;

        //////////
        //  Construction/destruction - from friends only
    private:
        ReportTableCell(
                ReportTable * table,
                int startColumn,
                int startRow,
                int columnSpan,
                int rowSpan,
                VerticalAlignment contentAlignment, //  TODO replace with ITableCellStyle
                const TypographicSizeOpt & preferredWidth
            );
        virtual ~ReportTableCell();

        //////////
        //  ReportElement
    public:
        virtual auto    parent() const -> ReportElement * override { return _table; }
        virtual auto    resolveFontSpecs() const -> FontSpecs override;
        virtual auto    resolveFontSize() const -> TypographicSize override;
        virtual auto    resolveFontStyle() const -> FontStyle override;
        virtual auto    resolveTextColor() const -> ColorSpec override;
        virtual auto    resolveBackgroundColor() const -> ColorSpec override;

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the Table to which this TableCell belongs.
        /// \return
        ///     The Table to which this TableCell belongs.
        ReportTable *   table() const { return _table; }

        /// \brief
        ///     Returns the start column of this TableCell.
        /// \return
        ///     The start column of this TableCell (0-based).
        int             startColumn() const { return _startColumn; }

        /// \brief
        ///     Returns the start row of this TableCell.
        /// \return
        ///     The start row of this TableCell (0-based).
        int             startRow() const { return _startRow; }

        /// \brief
        ///     Returns the number of columns occupied by this TableCell.
        /// \return
        ///     The number of columns occupied by this TableCell (1 or more).
        int             columnSpan() const { return _columnSpan; }

        /// \brief
        ///     Returns the number of ows occupied by this TableCell.
        /// \return
        ///     The number of rows occupied by this TableCell (1 or more).
        int             rowSpan() const { return _rowSpan; }

        /// \brief
        ///     Returns the alignment of content within this TableCell.
        /// \return
        ///     The alignment of content within this TableCell.
        auto            contentAlignment() const -> VerticalAlignment { return _contentAlignment; }

        /// \brief
        ///     Returns the preferred width of the column(s)
        ///     occupied by this TableCell.
        /// \return
        ///     The preferred width of the column(s) occupied
        ///     by this TableCell; no va;ue == choose automatically.
        auto            preferredWidth() const -> TypographicSizeOpt { return _preferredWidth; }

        /// \brief
        ///     Returns the resolved border type for this table cell.
        /// \return
        ///     The resolved vertical border type for this table cell.
        auto            resolveCellBorderType() const -> BorderType;

        /// \brief
        ///     Returns the resolved vertical content alignment for this table cell.
        /// \return
        ///     The resolved vertical text alignment for this table cell.
        auto            resolveContentAlignment() const -> VerticalAlignment;

        //////////
        //  Implementation
    private:
        ReportTable *   _table;
        int             _startColumn;           //  always >= 0
        int             _startRow;              //  always >= 0
        int             _columnSpan;            //  always >= 1
        int             _rowSpan;               //  always >= 1
        VerticalAlignment   _contentAlignment;
        TypographicSizeOpt  _preferredWidth;    //  no value == choose automatically
    };

    /// \class ReportAnchor tt3-report/API.hpp
    /// \brief A link anchor within a report.
    class TT3_REPORT_PUBLIC ReportAnchor final
        :   public ReportElement
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ReportAnchor)

        friend class Report;
        friend class ReportElement;

        //////////
        //  Construction/destruction - from friends only
    private:
        explicit ReportAnchor(ReportElement * anchoredElement);
        virtual ~ReportAnchor();

        //////////
        //  ReportElement
    public:
        virtual auto    parent() const -> ReportElement * override { return _anchoredElement; }
        virtual auto    resolveFontSpecs() const -> FontSpecs override;
        virtual auto    resolveFontSize() const -> TypographicSize override;
        virtual auto    resolveFontStyle() const -> FontStyle override;
        virtual auto    resolveTextColor() const -> ColorSpec override;
        virtual auto    resolveBackgroundColor() const -> ColorSpec override;

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the element to which this Anchor is assigned.
        /// \return
        ///     The element to which this Anchor is assigned; never nullptr.
        ReportElement * anchoredElement() const { return _anchoredElement; }

        //////////
        //  Implementation
    private:
        ReportElement * _anchoredElement;   //  never nullptr
    };


    /// \class ReportLink tt3-report/API.hpp
    /// \brief A link within a report.
    class TT3_REPORT_PUBLIC ReportLink
        :   public ReportElement
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ReportLink)

        friend class Report;
        friend class ReportSpanElement;
        friend class ReportInternalLink;
        friend class ReportExternalLink;

        //////////
        //  Construction/destruction - from friends only
    private:
        ReportLink(
                ReportSpanElement * spanElement,
                ILinkStyle * style);
        virtual ~ReportLink();

        //////////
        //  ReportElement
    public:
        virtual auto    parent() const -> ReportElement * override { return _spanElement; }
        virtual auto    resolveFontSpecs() const -> FontSpecs override;
        virtual auto    resolveFontSize() const -> TypographicSize override;
        virtual auto    resolveFontStyle() const -> FontStyle override;
        virtual auto    resolveTextColor() const -> ColorSpec override;
        virtual auto    resolveBackgroundColor() const -> ColorSpec override;

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the SpanElement that acts as a link.
        /// \return
        ///     The SpanElement that acts as a link; never nullptr.
        ReportSpanElement * spanElement() const { return _spanElement; }

        /// \brief
        ///     Returns The style assigned to this Link.
        /// \return
        ///     The style assigned to this Link; nullptr == none.
        ILinkStyle *    style() const { return _style; }

        /// \brief
        ///     Sets the style of this link.
        /// \details
        ///     The new Style (unless nullptr) must belong to
        ///     the report template associated with the Report.
        /// \param style
        ///     The new style for this link; nullptr == none.
        void        setStyle(ILinkStyle * style);

        //////////
        //  Implementation
    private:
        ReportSpanElement * _spanElement;
        ILinkStyle *    _style;
    };

    /// \class ReportInternalLink tt3-report/API.hpp
    /// \brief A link within a report to anchor within the same report.
    class TT3_REPORT_PUBLIC ReportInternalLink final
        :   public ReportLink
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ReportInternalLink)

        friend class ReportSpanElement;
        friend class ReportAnchor;

        //////////
        //  Construction/destruction - from friends only
    private:
        ReportInternalLink(
                ReportSpanElement * spanElement,
                ILinkStyle * style,
                ReportAnchor * anchor
            );
        virtual ~ReportInternalLink();

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the link target Anchor.
        /// \return
        ///     The link target Anchor; never nullptr.
        ReportAnchor *  anchor() const { return _anchor; }

        //////////
        //  Implementation
    private:
        ReportAnchor *  _anchor;
    };

    /// \class ReportExternalLink tt3-report/API.hpp
    /// \brief A link within a report to an external URL.
    class TT3_REPORT_PUBLIC ReportExternalLink final
        :   public ReportLink
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ReportExternalLink)

        friend class ReportSpanElement;

        //////////
        //  Construction/destruction - from friends only
    private:
        ReportExternalLink(
                ReportSpanElement * spanElement,
                ILinkStyle * style,
                const QString & url
            );
        virtual ~ReportExternalLink();

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the link target URL.
        /// \return
        ///     The link target URL.
        QString         url() const { return _url; }

        //////////
        //  Implementation
    private:
        QString         _url;
    };

    /// \class ReportTableOfContent tt3-report/API.hpp
    /// \brief A TOC within a report.
    class TT3_REPORT_PUBLIC ReportTableOfContent final
        :   public ReportBlockElement
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ReportTableOfContent)

        friend class ReportFlowElement;

        //////////
        //  Construction/destruction - from friends only
    private:
        explicit ReportTableOfContent(
                ReportFlowElement * parent
            );
        virtual ~ReportTableOfContent();

        //////////
        //  ReportElement
    public:
        virtual auto    resolveFontSpecs() const -> FontSpecs override;
        virtual auto    resolveFontSize() const -> TypographicSize override;
        virtual auto    resolveFontStyle() const -> FontStyle override;
        virtual auto    resolveTextColor() const -> ColorSpec override;
        virtual auto    resolveBackgroundColor() const -> ColorSpec override;

        //////////
        //  Operations
    public:

        //////////
        //  Implementation
    private:
    };
}

//  End of tt3-report/Report.hpp
