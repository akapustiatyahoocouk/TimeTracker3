//
//  tt3-db-sql/Statement.cpp - tt3::db::sql::Statement class implementation
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
#include "tt3-db-sql/API.hpp"
using namespace tt3::db::sql;

//////////
//  Construction/destruction
Statement::Statement(
        Database * database,
        const QString & sqlTemplate
    ) : _database(database),
        _sqlTemplate(sqlTemplate.trimmed()),
        _category(_categorize(_sqlTemplate))
{
    Q_ASSERT(_database != nullptr);

    //  Break _sqlTemplate into literal fragments & parameters
    qsizetype scan = 0;
    QString literalFragment, quotedIdentifier;
    QChar quote = '\x00';   //  ...currently in effect
    while (scan < _sqlTemplate.length())
    {   //  The literal fragment starts at [scan].
        //  Where does it end ?
        if (_sqlTemplate[scan] == '?' && quote.isNull())
        {   //  Literal fragment ends here, followed by a parameter placeholder
            _consumeIdentifier(literalFragment, quotedIdentifier, _database);
            _literalFragments.append(literalFragment);
            scan++; //  to skip the ?
            literalFragment.clear();
            continue;
        }
        //  String ?
        if (_sqlTemplate[scan] == '\'' && quote.isNull())
        {   //  String opening quote
            _consumeIdentifier(literalFragment, quotedIdentifier, _database);
            literalFragment += _sqlTemplate[scan++];
            quote = '\'';
            continue;
        }
        if (_sqlTemplate[scan] == '\'' && quote == '\'')
        {   //  String closing quote
            _consumeIdentifier(literalFragment, quotedIdentifier, _database);
            literalFragment += _sqlTemplate[scan++];
            quote = '\x00';
            continue;
        }
        //  "" - quoted identifier ?
        if (_sqlTemplate[scan] == '"' && quote.isNull())
        {   //  Identifier opening quote
            _consumeIdentifier(literalFragment, quotedIdentifier, _database);
            scan++; //  Skip "
            quote = '"';
            continue;
        }
        if (_sqlTemplate[scan] == '"' && quote == '"')
        {   //  Identifier closing quote
            _consumeIdentifier(literalFragment, quotedIdentifier, _database);
            scan++; //  Skip "
            quote = '\x00';
            continue;
        }
        //  `` - quoted identifier ?
        if (_sqlTemplate[scan] == '`' && quote.isNull())
        {   //  Identifier opening quote
            _consumeIdentifier(literalFragment, quotedIdentifier, _database);
            scan++; //  Skip `
            quote = '`';
            continue;
        }
        if (_sqlTemplate[scan] == '`' && quote == '`')
        {   //  Identifier closing quote
            _consumeIdentifier(literalFragment, quotedIdentifier, _database);
            scan++; //  Skip `
            quote = '\x00';
            continue;
        }
        //  [] - quoted identifier ?
        if (_sqlTemplate[scan] == '[' && quote.isNull())
        {   //  Identifier opening quote
            _consumeIdentifier(literalFragment, quotedIdentifier, _database);
            scan++; //  Skip [
            quote = '[';
            continue;
        }
        if (_sqlTemplate[scan] == ']' && quote == '[')
        {   //  Identifier closing quote
            _consumeIdentifier(literalFragment, quotedIdentifier, _database);
            scan++; //  Skip ]
            quote = '\x00';
            continue;
        }
        //  Comment ?
        if (_sqlTemplate[scan] == '-' &&
            scan + 1 < _sqlTemplate.length() && _sqlTemplate[scan + 1] == '-' &&
            quote.isNull())
        {   //  -- ... \n comment
            scan += 2;   //  Skip --
            while (scan < _sqlTemplate.length() && _sqlTemplate[scan] != '\n')
            {   //  Skip until '\n'
                scan++;
            }
            if (scan < _sqlTemplate.length())
            {   //  Skip '\n'
                scan++;
            }
            literalFragment += '\n';
            continue;
        }
        if (_sqlTemplate[scan] == '/' &&
            scan + 1 < _sqlTemplate.length() && _sqlTemplate[scan + 1] == '*' &&
            quote.isNull())
        {   //  /* ... */ comment
            scan += 2;   //  Skip /*
            bool commentClosed = false;
            while (scan < _sqlTemplate.length())
            {
                if (_sqlTemplate[scan] == '*' &&
                    scan + 1 < _sqlTemplate.length() && _sqlTemplate[scan + 1] == '/')
                {   //  Skip */
                    scan += 2;
                    commentClosed = true;
                    break;
                }
                scan++;  //  skip comment character
            }
            if (!commentClosed)
            {   //  OOPS! TODO throw SQL syntax error
            }
            continue;
        }
        //  Just a character...
        if (quote == '[' || quote == '"' || quote == '`')
        {   //  ...but may be inside a quoted identifier
            quotedIdentifier += _sqlTemplate[scan++];
        }
        else
        {   //  ...else is a part of the literal fragment
            literalFragment += _sqlTemplate[scan++];
        }
    }
    _consumeIdentifier(literalFragment, quotedIdentifier, _database);
    _literalFragments.append(literalFragment);
    Q_ASSERT(!_literalFragments.isEmpty());
    //  All parameters start as unspecified
    _parameters.resize(_literalFragments.size() - 1);
}

Statement::~Statement()
{   //  TODO all live ResultSets become dead
}

//////////
//  Operations
void Statement::resetParameters()
{
    for (int i = 0; i < _parameters.size(); i++)
    {
        _parameters[i].clear();
    }
}

void Statement::resetParameter(int index)
{
    Q_ASSERT(index >= 0 && index < _parameters.size());

    if (index >= 0 && index < _parameters.size())
    {   //  Be defensive in release mode
        _parameters[index].clear();
    }
}

void Statement::setNullParameter(int index)
{
    Q_ASSERT(index >= 0 && index < _parameters.size());

    if (index >= 0 && index < _parameters.size())
    {   //  Be defensive in release mode
        _parameters[index] = "NULL";
    }
}

void Statement::setBoolParameter(int index, bool value)
{
    Q_ASSERT(index >= 0 && index < _parameters.size());

    if (index >= 0 && index < _parameters.size())
    {   //  Be defensive in release mode
        _parameters[index] = (value ? "'Y'" : "'N'");
    }
}

void Statement::setIntParameter(int index, qint64 value)
{
    Q_ASSERT(index >= 0 && index < _parameters.size());

    if (index >= 0 && index < _parameters.size())
    {   //  Be defensive in release mode
        _parameters[index] = tt3::util::toString(value);
    }
}

void Statement::setStringParameter(int index, const QString & value)
{
    Q_ASSERT(index >= 0 && index < _parameters.size());

    if (index >= 0 && index < _parameters.size())
    {   //  Be defensive in release mode
        //  1.  ANSI SQL uses || as string concatenation operator.
        //  2.  We want CHAR(code) for codes 0..31 and 127; any
        //      other character can be part of the string literal
        //  3.  Single quote '' becomes '' within a literal.
        //  4.  Backslash \ does not start an escape sequence
        QString result, literalFragment;
        for (int i = 0; i < value.length(); i++)
        {
            QChar c = value[i];
            if (c.unicode() < 32 || c.unicode() == 127)
            {   //  Need an explicit code point
                if (!literalFragment.isEmpty())
                {   //  Quote & consume
                    if (!result.isEmpty())
                    {
                        result += " || ";
                    }
                    result += '\'' + literalFragment + '\'';
                    literalFragment.clear();
                    result += " || ";
                    result += "CHAR(" + tt3::util::toString(int(c.unicode())) +")";
                }
            }
            else if (c == '\'')
            {   //  ' -> literal ''
                literalFragment += "''";
            }
            else
            {   //  any other literal character
                literalFragment += c;
            }
        }
        //  Must consume the last literal fragment
        if (!literalFragment.isEmpty() || result.isEmpty())
        {   //  Quote & consume
            if (!result.isEmpty())
            {
                result += " || ";
            }
            result += '\'' + literalFragment + '\'';
        }
        _parameters[index] = result;
    }
}

void Statement::setDateTimeParameter(int index, const QDateTime & value)
{
    Q_ASSERT(index >= 0 && index < _parameters.size());

    if (index >= 0 && index < _parameters.size())
    {   //  Be defensive in release mode
        //  TODO se QL syntax
        //  'YYYY-MM-DD HH:MM:SS.mmm'
        throw tt3::util::NotImplementedError();
    }
}

void Statement::setTimeSpanParameter(int index, const tt3::util::TimeSpan & value)
{
    Q_ASSERT(index >= 0 && index < _parameters.size());

    if (index >= 0 && index < _parameters.size())
    {   //  Be defensive in release mode
        _parameters[index] = '\'' + tt3::util::toString(value) + '\'';
    }
}

void Statement::setOidParameter(int index, const tt3::db::api::Oid & value)
{
    Q_ASSERT(index >= 0 && index < _parameters.size());

    if (index >= 0 && index < _parameters.size())
    {   //  Be defensive in release mode
        _parameters[index] = '\'' + tt3::util::toString(value) + '\'';
    }
}

qint64 Statement::execute()
{
    _prepareSql();  //  may throw
    std::unique_ptr<ResultSet> resultSet;
    switch (_category)
    {
        case Category::Select:
            resultSet.reset(_database->executeSelect(_sql));    //  may thro)w
            Q_ASSERT(resultSet != nullptr);
            return resultSet->size();
        case Category::Insert:
            return _database->executeInsert(_sql);  //  may throw
        case Category::Update:
            return _database->executeUpdate(_sql);  //  may throw
        case Category::Delete:
            return _database->executeDelete(_sql);  //  may throw
        case Category::Other:
        default:    //  ...to shut up the compiler;
            _database->execute(_sql);   //  may throw
            return 0;
    }
}

ResultSet * Statement::executeQuery()
{
    _prepareSql();  //  may throw
    qint64 count;
    switch (_category)
    {
        case Category::Select:
            return _database->executeSelect(_sql);  //  may thro)w
        case Category::Insert:
            count = _database->executeInsert(_sql);  //  may throw
            //  TODO return an artificial ResultSet
            //  with a single row and COUNT column
            throw tt3::util::NotImplementedError();
        case Category::Update:
            count = _database->executeUpdate(_sql);  //  may throw
            //  TODO return an artificial ResultSet
            //  with a single row and COUNT column
            throw tt3::util::NotImplementedError();
        case Category::Delete:
            count = _database->executeDelete(_sql);  //  may throw
            //  TODO return an artificial ResultSet
            //  with a single row and COUNT column
            throw tt3::util::NotImplementedError();
        case Category::Other:
        default:    //  ...to shut up the compiler;
            _database->execute(_sql);   //  may throw
            //  TODO return an empty artificial ResultSet
            throw tt3::util::NotImplementedError();
    }
}

//////////
//  Implementation helpers
Statement::Category Statement::_categorize(const QString & sqlTemplate)
{
    if (sqlTemplate.startsWith("SELECT", Qt::CaseInsensitive))
    {
        return Category::Select;
    }
    if (sqlTemplate.startsWith("INSERT", Qt::CaseInsensitive))
    {
        return Category::Insert;
    }
    if (sqlTemplate.startsWith("UPDATE", Qt::CaseInsensitive))
    {
        return Category::Update;
    }
    if (sqlTemplate.startsWith("DELETE", Qt::CaseInsensitive))
    {
        return Category::Delete;
    }
    return Category::Other;
}

void Statement::_consumeIdentifier(
        QString & accumulator,
        QString & identifier,
        Database * database
    )
{
    if (!identifier.isEmpty())
    {
        if (!database->isIdentifier(identifier))
        {   //  OOPS! THDO throw SQL syntax error
        }
        accumulator += database->quoteIdentifier(identifier);
        identifier.clear();
    }
}

void Statement::_prepareSql()
{
    if (_sql.isEmpty())
    {   //  Make sure all parameters are specified
        for (int i = 0; i < _parameters.size(); i++)
        {
            if (_parameters[i].isEmpty())
            {   //  OOPS! TODO throw parameter not specified
            }
        }
        //  Assemble
        for (int i = 0; i < _literalFragments.size(); i++)
        {
            _sql += _literalFragments[i];
            if (i < _parameters.size())
            {
                _sql += _parameters[i];
            }
        }
    }
}

//  End of tt3-db-sql/Statement.cpp
