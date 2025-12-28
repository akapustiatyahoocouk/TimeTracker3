//
//  tt3-util/Macros.cpp - Helper macros
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

#define TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Clazz)      \
    private:                                            \
        Clazz(const Clazz &) = delete;                  \
        Clazz(Clazz &&) = delete;                       \
        Clazz & operator = (const Clazz &) = delete;    \
        Clazz & operator = (Clazz &&) = delete;

#define TT3_UTILITY_CLASS(Clazz)                        \
    TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Clazz)          \
    private:                                            \
        Clazz() = delete;                               \
        ~Clazz() = delete;

#define TT3_DECLARE_SINGLETON(Clazz)                    \
    TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Clazz)          \
    private:                                            \
        Clazz();                                        \
        ~Clazz();                                       \
    public:                                             \
        static Clazz *  instance();

#define TT3_IMPLEMENT_SINGLETON(Clazz)                  \
    Clazz * Clazz::instance()                           \
    {                                                   \
        static Clazz * theInstance = new Clazz();       \
        return theInstance;                             \
    }

//  We need some dark magic with C++ keywords
#define register register_  /*  it's a verb, not a description! */

//  End of tt3-util/Macros.cpp
