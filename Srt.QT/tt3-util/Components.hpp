//
//  tt3-util/Components.hpp - TT3-util Components
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

namespace util
{
    //  The "TT3 Util" component settings
    class TT3_UTIL_PUBLIC UtilSettings final : public Settings
    {
        DECLARE_SINGLETON(UtilSettings)

        //////////
        //  Properties
    public:
    };

    //  The "TT3 Util" component
    class TT3_UTIL_PUBLIC UtilComponent final : public Component
    {
        DECLARE_SINGLETON(UtilComponent)

        //////////
        //  Component
    public:
        virtual UtilSettings &   settings() override { return *UtilSettings::instance(); }

        //////////
        //  Implementation
    private:
    };
}

//  TODO use similar technique for component
//  registration in form of e.g.
//  EXECUTE_ONCE(STATEMENT) macro
#define CONCAT_IMPL(x,y)    x##y
#define CONCAT(x,y)         CONCAT_IMPL(x, y)

#define EXECUTE_ONCE_IMPL(statement,suffix) \
class CONCAT(X,suffix) final                \
{                                           \
public:                                     \
    CONCAT(X,suffix)()                      \
    {                                       \
        if (_counter == 0) { statement; }   \
        _counter++;                         \
    }                                       \
private:                                    \
    static inline int _counter = 0;         \
};                                          \
static CONCAT(X,suffix) CONCAT(x,suffix);
#define EXECUTE_ONCE(statement) EXECUTE_ONCE_IMPL(statement,__COUNTER__)
//  EXECUTE_ONCE(util::ComponentRegistry::registerComponent(util::UtilComponent::instance()))
//  End of tt3-util/Components.hpp
