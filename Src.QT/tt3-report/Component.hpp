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
    struct TT3_REPORT_PUBLIC KnownCustomReportTemplate
    {
        //////////
        //  Operators (needed by QList)
        bool                operator == (const KnownCustomReportTemplate & op2) const;
        bool                operator != (const KnownCustomReportTemplate & op2) const;

        //////////
        //  Properties
        tt3::util::Mnemonic mnemonic;
        QString             displayName;
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
        };

        //////////
        //  IComponent
    public:
        virtual Mnemonic        mnemonic() const override;
        virtual QString         displayName() const override;
        virtual QString         description() const override;
        virtual QString         copyright() const override;
        virtual QVersionNumber  version() const override;
        virtual QString         buildNumber() const override;
        virtual ISubsystem *    subsystem() const override;
        virtual Resources *     resources() const override;
        virtual Settings *      settings() override;
        virtual const Settings *settings() const override;
        virtual void            initialize() override;
        virtual void            deinitialize() override;
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
