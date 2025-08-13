//
//  tt3-util/API.hpp - tt3-util master header
//
//  TimeTracker3
//  Copyright (C) `2026, Andrey Kapustin
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
#pragma once

//////////
//  CRT+QT
//  These headers are dirty, so disable some warnings for their processing
#if defined(__GNUC__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Weffc++"
    #pragma GCC diagnostic ignored "-Wsign-promo"
    #pragma GCC diagnostic ignored "-Wctor-dtor-privacy"
    #pragma GCC diagnostic ignored "-Wswitch-default"
    #pragma GCC diagnostic ignored "-Wconversion"
#elif defined(_MSC_VER)
    #pragma warning(push)
    #pragma warning(disable:4355)   //  'this': used in base member initializer list
    #pragma warning(disable:4365)   //  '=': conversion from '<TYPE>' to '<TYPE>', signed/unsigned mismatch
    #pragma warning(disable:4371)   //  '<CLASS>': layout of class may have changed from a previous version of the compiler due to better packing of member '<MEMBER>'
    #pragma warning(disable:4710)   //  '<FUNCTION>': function not inlined
    #pragma warning(disable:4711)   //  '<FUNCTION>': function selected for automatic inline expansion
    #pragma warning(disable:5045)   //  Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
    #pragma warning(disable:5219)   //  implicit conversion from 'const int' to 'float', possible loss of data
    #pragma warning(disable:5249)   //  '<CLASS>::<MEMBER>' of type '<TYPE' has named enumerators with values that cannot be represented in the given bit field width of '<WIDTH>'.
#else
    #error Unsupported C++ toolchain
#endif

#include <regex>

#include <QtCore/qglobal.h>

#include <QApplication>
#include <QAction>
#include <QBoxLayout>
#include <QDateTime>
#include <QDialog>
#include <QDir>
#include <QDomDocument>
#include <QDomElement>
#include <QException>
#include <QFileDialog>
#include <QGridLayout>
#include <QIcon>
#include <QLibrary>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QMoveEvent>
#include <QMutex>
#include <QMutexLocker>
#include <QStatusBar>
#include <QTextEdit>
#include <QThread>
#include <QTimer>
#include <QVariant>
#include <QVersionNumber>
#include <QWidget>

#if defined(__GNUC__)
    #pragma GCC diagnostic pop
#elif defined(_MSC_VER)
    #pragma warning(pop)
    //  Some warnings shall be disabled in user code as well
    #pragma warning(disable:4250)   //  '<DERIVED CLASS>': inherits '<BASE MEMBER>' via dominance
    #pragma warning(disable:4251)   //  '<VARIABLE>': '<CLASS>' needs to have dll-interface to be used by clients of '<CLASS>'
    #pragma warning(disable:4355)   //  'this': used in base member initializer list
    #pragma warning(disable:4710)   //  '<FUNCTION>': function not inlined
    #pragma warning(disable:4711)   //  '<FUNCTION>': function selected for automatic inline expansion
    #pragma warning(disable:4866)   //  compiler may not enforce left-to-right evaluation order for call to 'C++17 operator'
    #pragma warning(disable:5045)   //  Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
    #pragma warning(disable:5249)   //  '<CLASS>::<MEMBER>' of type '<TYPE' has named enumerators with values that cannot be represented in the given bit field width of '<WIDTH>'.
#else
    #error Unsupported C++ toolchain
#endif

//////////
//  tt3-util components
#include "tt3-util/Linkage.hpp"
#include "tt3-util/Macros.hpp"
#include "tt3-util/Exceptions.hpp"

#include "tt3-util/Sync.hpp"
#include "tt3-util/DateTime.hpp"

#include "tt3-util/Collections.hpp"
#include "tt3-util/ToString.hpp"
#include "tt3-util/FromString.hpp"

#include "tt3-util/MessageDigest.hpp"

#include "tt3-util/Plugin.hpp"
#include "tt3-util/Settings.hpp"
#include "tt3-util/Component.hpp"

#include "tt3-util/Components.hpp"

//  End of tt3-util/API.hpp
