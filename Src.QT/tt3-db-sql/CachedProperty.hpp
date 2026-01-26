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
    /// \class CachedProperty tt3-db-xml/API.hpp
    /// \brief A property that resides in a database and
    ///        is cached for performance.
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
        /// \brief
        ///     Constructs the cached property.
        /// \param loader
        ///     The agent to call to reload property value
        ///     from database.
        explicit CachedProperty(Loader loader);

        /// \brief
        ///     The class destructor.
        ~CachedProperty() = default;

        //////////
        //  Operators
    public:
        /// \brief
        ///     Returns the value of this property.
        /// \details
        ///     If the property value is not cached or the
        ///     cache has expired, reloads the value using
        ///     the associated Loader.
        /// \return
        ///     the value of this property.
        /// \exception DatabaseException
        ///     If the property loading fails.
                    operator T() const { return value(); }

        /// \brief
        ///     Sets the value of this property.
        /// \details
        ///     The valus is not automatically stored to the database.
        ///     The recommended approach is to store the value and
        ///     THEN to update the value of the cached property.
        ///     This way, if the store fails, the cached property
        ///     remains intact.
        /// \param src
        ///     The new value for this property.
        void        operator = (const T & src) { setValue(src); }

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the value of this property.
        /// \details
        ///     If the property value is not cached or the
        ///     cache has expired, reloads the value using
        ///     the associated Loader.
        /// \return
        ///     the value of this property.
        /// \exception DatabaseException
        ///     If the property loading fails.
        T           value() const;

        /// \brief
        ///     Sets the value of this property.
        /// \details
        ///     The valus is not automatically stored to the database.
        ///     The recommended approach is to store the value and
        ///     THEN to update the value of the cached property.
        ///     This way, if the store fails, the cached property
        ///     remains intact.
        /// \param value
        ///     The new value for this property.
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
