//
//  tt3-help/Classes.hpp - forward declarations and typedefs
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

namespace tt3::help
{
    class TT3_HELP_PUBLIC IContentType;
    class TT3_HELP_PUBLIC IContentLoader;

    class TT3_HELP_PUBLIC HelpTopic;
    class TT3_HELP_PUBLIC HelpCollection;

    class TT3_HELP_PUBLIC SimpleHelpTopic;
    class TT3_HELP_PUBLIC SimpleHelpCollection;

    //  Collections
    using ContentTypes = QSet<IContentType*>;

    //  Helpers
    class TT3_HELP_PUBLIC LocalSiteHelpLoader;
}

//  End of tt3-help/Classes.hpp

