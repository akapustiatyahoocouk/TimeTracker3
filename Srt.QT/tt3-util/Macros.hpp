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
        static Clazz theInstance;                       \
        return &theInstance;                            \
    }

//  A helper macro to document exceptions thrown by a method or function
#define throws(E,...) noexcept(false)

//  Helper macros for executing code at
//  pre-main() time once
class TT3_UTIL_PUBLIC OnceExecutor final
{
public:
    OnceExecutor(const char * file, int line, void (*f)())
        :   _file(file), _line(line)
    {
        for (OnceExecutor * oi = _onceExecutors; oi != nullptr; oi = oi->_nextInChain)
        {
            if (line == oi->_line && strcmp(file, oi->_file) == 0)
            {   //  Already done!
                return;
            }
        }
        _nextInChain = _onceExecutors;
        _onceExecutors = this;
        qDebug() << _file;
        qDebug() << _line;
        f();
    }
    ~OnceExecutor()
    {
        for (OnceExecutor ** oi = &_onceExecutors; (*oi) != nullptr; oi = &((*oi)->_nextInChain))
        {
            if (_line == (*oi)->_line && strcmp(_file, (*oi)->_file) == 0)
            {   //  This one!
                *oi = _nextInChain;
                _nextInChain = nullptr;
                break;
            }
        }
    }
private:
    const char *const   _file;
    const int           _line;
    OnceExecutor *      _nextInChain = nullptr;
    static inline OnceExecutor *    _onceExecutors = nullptr;
};

#define CONCAT_IMPL(x,y)    x##y
#define CONCAT(x,y)         CONCAT_IMPL(x, y)

#define EXECUTE_ONCE_IMPL(statement,suffix) \
    static OnceExecutor CONCAT(onceExecutor,suffix){__FILE__,__LINE__,[] { statement; }};
#define EXECUTE_ONCE(statement) EXECUTE_ONCE_IMPL(statement,__COUNTER__)

//  End of tt3-util/Macros.cpp
