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

        friend class ReportSection;

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
    };

    /// \class ReportElement tt3-report/API.hpp
    /// \brief A generic element of a Report.
    class TT3_REPORT_PUBLIC ReportElement
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ReportElement)

        friend class ReportFlowElement;

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

        friend class ReportSection;

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
        ReportFlowElement * _parent;    //  can be nullptr
    };

    /// \class ReportSpanElement tt3-report/API.hpp
    /// \brief A report element that does not cause a flow break.
    class TT3_REPORT_PUBLIC ReportSpanElement
        :   public ReportElement
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ReportSpanElement)

        //////////
        //  Construction/destruction - from friends only
    private:
        explicit ReportSpanElement(Report * report);
        virtual ~ReportSpanElement();

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the Link associated with this element.
        /// \return
        ///     The Link associated with this element; nullptr == none.
        ReportLink *    link() const { return _link; }

        //////////
        //  Implementation
    private:
        ReportLink *    _link = nullptr;
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

        //////////
        //  Construction/destruction - from friends only
    private:
        explicit ReportParagraph(Report * report);
        virtual ~ReportParagraph();

        //////////
        //  Operations
    public:

        //////////
        //  Implementation
    private:
    };

    /// \class ReportText tt3-report/API.hpp
    /// \brief A paragraph within a report.
    class TT3_REPORT_PUBLIC ReportText final
        :   public ReportSpanElement
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ReportText)

        //////////
        //  Construction/destruction - from friends only
    private:
        ReportText(
                Report * report,
                const QString & text,
                ICharacterStyle * style
            );
        virtual ~ReportText();

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

        //////////
        //  Construction/destruction - from friends only
    private:
        ReportPicture(
                Report * report,
                const TypographicSize & width,
                const TypographicSize & height,
                const QImage & image
            );
        virtual ~ReportPicture();

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

        //////////
        //  Construction/destruction - from friends only
    private:
        ReportList(Report * report);
        virtual ~ReportList();

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

        //////////
        //  Construction/destruction - from friends only
    private:
        ReportListItem(ReportList * list);
        virtual ~ReportListItem();

        //////////
        //  ReportElement
    public:
        virtual auto    parent() const -> ReportElement * override { return _list; }

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

        //////////
        //  Construction/destruction - from friends only
    private:
        ReportTable(Report * report);
        virtual ~ReportTable();

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

        //////////
        //  Construction/destruction - from friends only
    private:
        ReportTableCell(ReportTable * table);
        virtual ~ReportTableCell();

        //////////
        //  ReportElement
    public:
        virtual auto    parent() const -> ReportElement * override { return _table; }

        //////////
        //  Operations
    public:

        //////////
        //  Implementation
    private:
        ReportTable *   _table;
    };

    //  TODO insert Element classes here

    /// \class ReportAnchor tt3-report/API.hpp
    /// \brief A link anchor within a report.
    class TT3_REPORT_PUBLIC ReportAnchor
        :   public ReportElement
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ReportAnchor)

        friend class ReportElement;

        //////////
        //  Construction/destruction - from friends only
    private:
        explicit ReportAnchor(ReportElement * anchoredElement);
        virtual ~ReportAnchor();

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
        ReportElement * _anchoredElement;
    };


    /// \class ReportLink tt3-report/API.hpp
    /// \brief A link within a report.
    class TT3_REPORT_PUBLIC ReportLink
        :   public ReportElement
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ReportLink)

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

        //////////
        //  Construction/destruction - from friends only
    private:
        explicit ReportTableOfContent(Report * report);
        virtual ~ReportTableOfContent();

        //////////
        //  Operations
    public:

        //////////
        //  Implementation
    private:
    };
}

//  End of tt3-report/Report.hpp
