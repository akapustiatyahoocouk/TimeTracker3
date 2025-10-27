//
//  tt3-gui/WidgetDecorations.hpp - Widget decoration/styling helpers
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

namespace tt3::gui
{
    /// \class TreeWidgetDecorations tt3-gui/API.hpp
    /// \brief Decorations for a QTreeWidget.
    class TT3_GUI_PUBLIC TreeWidgetDecorations
    {
        //////////
        //  Construction/destruction/assignment
    public:
        /// \brief
        ///     The class constructor.
        /// \details
        ///     Picks up initial values from app's palette.
        TreeWidgetDecorations();

        /// \brief
        ///     The class constructor.
        /// \param treeWidget
        ///     The tree widget to pick up decoration values from.
        explicit TreeWidgetDecorations(
                QTreeWidget * treeWidget
            );

        //  The default copy constructor, assignment and
        //  destructors are all OK

        //////////
        //  Properties
    public:
        /// \brief
        ///     The brush to use for background of tree widget.
        QBrush      background;

        /// \brief
        ///     The brush to use for text of "normal" tree items.
        QBrush      itemForeground;

        /// \brief
        ///     The brush to use for text of "disabled" tree items.
        QBrush      disabledItemForeground;

        /// \brief
        ///     The brush to use for text of "error" tree items.
        QBrush      errorItemForeground;

        /// \brief
        ///     The brush to use for text of "filter match" tree items.
        QBrush      filterMatchItemForeground;

        /// \brief
        ///     The font to use for text of "normal" tree items.
        QFont       itemFont;

        /// \brief
        ///     The font to use for text of "emphasized" tree items (normally bold).
        QFont       itemEmphasisFont;

        //////////
        //  Implementation
    private:
        //  Helpers
        void        _adjustTextColor(QColor & textColor);
        void        _adjustBackColor(QColor & backColor);
        void        _initialize(
                            const QColor & textColor,
                            const QColor & backColor,
                            const QFont & baseFont
                        );
    };

    /// \class LabelDecorations tt3-gui/API.hpp
    /// \brief Decorations for a QLabel.
    class TT3_GUI_PUBLIC LabelDecorations
    {
        //////////
        //  Construction/destruction/assignment
    public:
        /// \brief
        ///     The class constructor.
        /// \details
        ///     Picks up initial values from app's palette.
        LabelDecorations();

        /// \brief
        ///     The class constructor.
        /// \param label
        ///     The label to pick up decoration values from.
        explicit LabelDecorations(
                QLabel * label
            );

        //  The default copy constructor, assignment and
        //  destructors are all OK

        //////////
        //  Properties
    public:
        /// \brief
        ///     The color to use for text of "normal" labels.
        QColor      foreground;

        /// \brief
        ///     The color to use for text of "disabled" labels.
        QColor      disabledForeground;

        /// \brief
        ///     The color to use for text of "error" labels.
        QColor      errorForeground;

        /// \brief
        ///     The color to use for background of "normal" labels.
        QColor      background;

        /// \brief
        ///     The color to use for background of "error" labels.
        QColor      errorBackground;

        /// \brief
        ///     The color to use for background of "live status" labels.
        QColor      liveStatusBackground;

        /// \brief
        ///     The font to use for text of "normal" labelss.
        QFont       font;

        /// \brief
        ///     The font to use for text of "emphasized" labels (normally bold).
        QFont       emphasisFont;

        //////////
        //  Implementation
    private:
        //  Helpers
        void        _initialize(
                            const QColor & textColor,
                            const QColor & backColor,
                            const QFont & baseFont
                        );
    };

    /// \class PushButtonDecorations tt3-gui/API.hpp
    /// \brief Decorations for a QPushButton.
    class TT3_GUI_PUBLIC PushButtonDecorations
    {
        //////////
        //  Construction/destruction/assignment
    public:
        /// \brief
        ///     The class constructor.
        /// \details
        ///     Picks up initial values from app's palette.
        PushButtonDecorations();

        /// \brief
        ///     The class constructor.
        /// \param pushButton
        ///     The push button to pick up decoration values from.
        explicit PushButtonDecorations(
                QPushButton * pushButton
            );

        //  The default copy constructor, assignment and
        //  destructors are all OK

        //////////
        //  Properties
    public:
        /// \brief
        ///     The color to use for text of "normal" labels.
        QColor      foreground;

        /// \brief
        ///     The color to use for text of "disabled" labels.
        QColor      disabledForeground;

        /// \brief
        ///     The color to use for text of "error" labels.
        QColor      errorForeground;

        /// \brief
        ///     The color to use for background of "normal" labels.
        QColor      background;

        /// \brief
        ///     The color to use for background of "error" labels.
        QColor      errorBackground;

        /// \brief
        ///     The color to use for background of "live status" labels.
        QColor      liveStatusBackground;

        /// \brief
        ///     The font to use for text of "normal" labelss.
        QFont       font;

        /// \brief
        ///     The font to use for text of "emphasized" labels (normally bold).
        QFont       emphasisFont;

        //////////
        //  Operations
    public:
        /// \brief
        ///     The role a button can play in a UI.
        enum class ButtonRole
        {
            NormalButton,       ///< A "normal" button.
            DisabledButton,     ///< A "disabled" button.
            LiveStatusButton,   ///< A "live status" button, typically changing its text on timer.
            ErrorButton         ///< A button displaying an error message.
        };

        /// \brief
        ///     Applies this decorations to a specific push button.
        /// \param pushButton
        ///     The push button to apply this decorations to.
        /// \param buttonRole
        ///     The role the push button plays in the UI.
        void        applyTo(QPushButton * pushButton,
                            ButtonRole buttonRole);

        //////////
        //  Implementation
    private:
        //  Helpers
        void        _initialize(
                            const QColor & textColor,
                            const QColor & backColor,
                            const QFont & baseFont
                        );
    };

    /// \class ListWidgetDecorations tt3-gui/API.hpp
    /// \brief Decorations for a QListWidget.
    class TT3_GUI_PUBLIC ListWidgetDecorations
    {
        //////////
        //  Construction/destruction/assignment
    public:
        /// \brief
        ///     The class constructor.
        /// \details
        ///     Picks up initial values from app's palette.
        ListWidgetDecorations();

        /// \brief
        ///     The class constructor.
        /// \param listWidget
        ///     The list widget to pick up decoration values from.
        explicit ListWidgetDecorations(
                QListWidget * listWidget
            );

        //  The default copy constructor, assignment and
        //  destructors are all OK

        //////////
        //  Properties
    public:
        /// \brief
        ///     The brush to use for text of "normal" list items.
        QBrush      itemForeground;

        /// \brief
        ///     The brush to use for text of "disabled" list items.
        QBrush      disabledItemForeground;

        /// \brief
        ///     The brush to use for text of "error" list items.
        QBrush      errorItemForeground;

        /// \brief
        ///     The brush to use for text of "filter match" list items.
        QBrush      filterMatchItemForeground;

        /// \brief
        ///     The font to use for text of "normal" list items.
        QFont       itemFont;

        /// \brief
        ///     The font to use for text of "emphasized" list items (normally bold).
        QFont       itemEmphasisFont;

        //////////
        //  Implementation
    private:
        //  Helpers
        void        _initialize(
                            const QColor & textColor,
                            const QColor & backColor,
                            const QFont & baseFont
                        );
    };
}

//  End of tt3-gui/WidgetDecorations.hpp
