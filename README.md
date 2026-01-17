# TimeTracker3
A portable tool for tracking the working time dedicated to various activities.

## Versioning and release cycle
- Versions are of the form MAJOR.MINOR.PATCH, all of which are decimal integers.
- MAJOR increments by 1 each time new concepts are introduced.
- MINOR increments by 1 each time new APIs (services) are introduced and restart from 0 when MAJOR increments.
- PATCH increments by 1 with each bug fix, feature optimization, etc. and restarts from 0 every time MAJOR ir MINOR changes.
- A separate STAGE (alpha, beta, RTM, GA) tags every MAJOR.MINOR.PATCH tuple.
- After a release is published, SOMETHING in the MAJOR.MINOR.PATCH tuple has to change.
- Each release is git-tagged as "vMAJOR.MINOR.PATCH" from where it was made.
- Each release is also a merge from "dev" to "main"; this is where a git tag is created.
- Build number is auto-generated based on build date/time.
- The MAJOR version 0 is reserved for "before the first RTM" builds.

## Projects and dependencies
- When adding new components, add corresponding sub-projects to tt3-all.
- Use the same project file structure as existing projrect; specifically rely on a common tt3.pri definitions.
- Prefer the use of API.hpp for a master header for the new project and set it as a precompiled header.
- All Qt header files go to the tt3-util, not to the new sub-project that uses them. Qt headers are full of build warnings and tt3-util uses some trickery with diagnostic pragmas to suppress those.
- The preferred policy for TT3 code base is "zero warnings in TT3 code". Don't relax the diagnostic settings unless nothing else works.
- New images, icons, etc. go to the common Artwork folder and THEN are copied to the particular components that need them. Don't create new images/icons if existing ones will do.

## Style and conventions
- Stick to existing coding and indentation style and naming convensions.
- Header files are .hpp and are one-per-feature.
- Source files are .cpp and are one-per-class.
- Stick to US English for code (identifiers, etc.) and comments (including goxygen comments).
- User-visible strings are all in resource files and shall stick to the resource file's locale (which is part of the resource file name).
- Doxygen comments must be written for anything that is piblic or protected (i.e. visible from outside the class). For privates, this is not necessary.
- ALWAYS document thrown exceptions, as C++ currently has no other exception specification mechanism.
- Catch Exceptions, don't bother with Errors - Errors are typically unrecoverable. Don't bother with std::exceptions - they are unrecoverable (such as std::bad_alloc) or indicate programming errors (std::runtime_error or std::bad_cast).