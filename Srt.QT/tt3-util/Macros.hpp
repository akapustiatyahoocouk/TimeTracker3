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

#define CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Clazz)          \
    private:                                            \
        Clazz(const Clazz &) = delete;                  \
        Clazz(Clazz &&) = delete;                       \
        Clazz & operator = (const Clazz &) = delete;    \
        Clazz & operator = (Clazz &&) = delete;

#define UTILITY_CLASS(Clazz)                            \
    CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Clazz)              \
    private:                                            \
        Clazz() = delete;                               \
        ~Clazz() = delete;

#define DECLARE_SINGLETON(Clazz)                        \
    CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Clazz)              \
    private:                                            \
        Clazz();                                        \
        ~Clazz();                                       \
    public:                                             \
        static Clazz *  instance();

#define IMPLEMENT_SINGLETON(Clazz)                      \
    Clazz * Clazz::instance()                           \
    {                                                   \
        static Clazz * theInstance = new Clazz();       \
        return theInstance;                            \
    }

//  A helper macro to document exceptions thrown by a method or function
#if defined(_MSC_VER)
    //  MSVC at least allows the syntax (with a warning!)
    #define throws(...) throw(__VA_ARGS__)
    #pragma warning(disable:5040)   //  dynamic exception specifications are valid only in C++14 and earlier; treating as noexcept(false)
#else
    #define throws(E,...) noexcept(false)
#endif

//  End of tt3-util/Macros.cpp
