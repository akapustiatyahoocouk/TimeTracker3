//
//  tt3-db-sql/CachedProperty.hpp - caching
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

namespace tt3::db::sql
{
    template <class T>
    class CachedProperty final
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(CachedProperty)

        //////////
        //  Types
    public:
        /// \brief
        ///     The function that reloads the value of the cached
        ///     property; may throw DatabaseException.
        using Loader = std::function<void()>;

        //////////
        //  Construction/destruction
    public:
        explicit CachedProperty(Loader loader);
        ~CachedProperty() = default;

        //////////
        //  Operators
    public:
                    operator T() const { return value(); }
        void        operator = (const T & src) { setValue(src); }

        //////////
        //  Operations
    public:
        T           value() const;
        void        setValue(const T & value);

        /// \brief
        ///     Invalidates this cached property.
        /// \details
        ///     Any subsequent value access is guaranteed
        ///     to invoke the associated Loader.
        void        invalidate();

        //////////
        //  Implementation
    private:
        T           _value;         //  may be invalid
        mutable QDateTime   _validUntil;    //  UTC; invalid == on next access
        Loader      _loader;
    };

    template <class T>
    CachedProperty<T>::CachedProperty(Loader loader)
        :   _loader(loader)
    {
        Q_ASSERT(_loader != nullptr);
    }

    template <class T>
    T CachedProperty<T>::value() const
    {
        if (!_validUntil.isValid() ||
            QDateTime::currentDateTimeUtc() > _validUntil)
        {   //  Must reload
            _loader();  //  may throw
            //  The _loader() must setValue() of this CachedProperty
            Q_ASSERT(_validUntil.isValid() &&
                     _validUntil > QDateTime::currentDateTimeUtc());
        }
        return _value;
    }

    template <class T>
    void CachedProperty<T>::setValue(const T & value)
    {
        _value = value;
        _validUntil = QDateTime::currentDateTimeUtc().addSecs(60);  //  TODO adjustable/random
        //  The caller will save the change before calling setValue()
    }

    template <class T>
    void CachedProperty<T>::invalidate()
    {
        _validUntil = QDateTime();
    }
}

//  End of tt3-db-sql/CachedProperty.hpp
