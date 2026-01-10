//
//  tt3-report/Component.hpp - tt3-report Component
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
    /// \class KnownCustomReportTemplate tt3-report/API.hpp
    /// \brief The descriptor of a custom report template known to exist.
    struct TT3_REPORT_PUBLIC KnownCustomReportTemplate
    {
        //////////
        //  Operators (needed by QList)

        /// \brief
        ///     Compares two specs for equality.
        /// \param op2
        ///     The 2nd spec to compare this spec with.
        /// \return
        ///     True if the two specs are equal in all properties, else false.
        bool                operator == (const KnownCustomReportTemplate & op2) const;

        /// \brief
        ///     Compares two specs for equality.
        /// \param op2
        ///     The 2nd spec to compare this spec with.
        /// \return
        ///     False if the two specs are equal in all properties, else true.
        bool                operator != (const KnownCustomReportTemplate & op2) const;

        //////////
        //  Properties

        /// \brief
        ///     The mnemonic identifier of the custom report template.
        tt3::util::Mnemonic mnemonic;

        /// \brief
        ///     The user-readable display name of the custom report template.
        QString             displayName;

        /// \brief
        ///     The full path of the custom report template XML file.
        QString             location;
    };
    using KnownCustomReportTemplates = QList<KnownCustomReportTemplate>;

    /// \class Component tt3-report/API.hpp
    /// \brief The "TT3 Reporting" component.
    class TT3_REPORT_PUBLIC Component final
        :   public virtual tt3::util::IComponent
    {
        TT3_DECLARE_COMPONENT(Component)

        //////////
        //  Types
    public:
        /// \class Resources tt3-report/API.hpp
        /// \brief The component's resources.
        class TT3_REPORT_PUBLIC Resources final
            :   public tt3::util::FileResourceFactory
        {
            TT3_DECLARE_SINGLETON(Resources)
        };

        /// \class Settings tt3-report/API.hpp
        /// \brief The component's settings.
        class TT3_REPORT_PUBLIC Settings final
            :   public tt3::util::Settings
        {
            TT3_DECLARE_SINGLETON(Settings)

            //////////
            //  Properties
        public:
            /// \brief
            ///     The set of "known" custom reports.
            tt3::util::Setting<KnownCustomReportTemplates> knownCustomReportTemplates;

            /// \brief
            ///     The mnemonic of the last used report type.
            tt3::util::Setting<tt3::util::Mnemonic> lastUsedReportType;

            /// \brief
            ///     The mnemonic of the last used report format.
            tt3::util::Setting<tt3::util::Mnemonic> lastUsedReportFormat;

            /// \brief
            ///     The mnemonic of the last used report template.
            tt3::util::Setting<tt3::util::Mnemonic> lastUsedReportTemplate;
        };

        //////////
        //  IComponent
    public:
        virtual auto    mnemonic() const -> Mnemonic override;
        virtual QString displayName() const override;
        virtual QString description() const override;
        virtual QString copyright() const override;
        virtual auto    version() const -> QVersionNumber override;
        virtual QString stage() const override;
        virtual QString buildNumber() const override;
        virtual auto    license() const -> tt3::util::ILicense * override;
        virtual auto    subsystem() const -> ISubsystem * override;
        virtual auto    resources() const -> Resources * override;
        virtual auto    settings() -> Settings * override;
        virtual auto    settings() const -> const Settings * override;
        virtual void    initialize() override;
        virtual void    deinitialize() override;
    };
}

//  Formatting/parsing
namespace tt3::util
{
    template <> TT3_REPORT_PUBLIC
    QString toString<tt3::report::KnownCustomReportTemplate>(
            const tt3::report::KnownCustomReportTemplate & value
        );
    template <> TT3_REPORT_PUBLIC
    QString toString<tt3::report::KnownCustomReportTemplates>(
            const tt3::report::KnownCustomReportTemplates & value
        );

    template <> TT3_REPORT_PUBLIC
    auto fromString<tt3::report::KnownCustomReportTemplate>(
            const QString & s,
            qsizetype & scan
        ) -> tt3::report::KnownCustomReportTemplate;
    template <> TT3_REPORT_PUBLIC
    auto fromString<tt3::report::KnownCustomReportTemplates>(
            const QString & s,
            qsizetype & scan
        ) -> tt3::report::KnownCustomReportTemplates;
    }

//  End of tt3-report/Component.hpp
