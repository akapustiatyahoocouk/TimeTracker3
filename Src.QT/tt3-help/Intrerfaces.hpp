//
//  tt3-help/Intrerfaces.hpp - Help system interfaces
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
    class TT3_HELP_PUBLIC IHelpTopicCollection
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(IHelpTopicCollection)

        //////////
        //  Types
    public:
        class TT3_HELP_PUBLIC iterator
        {
            //////////
            //  Construction/destruction/assignment
        public:
            iterator()
                :   _collection(nullptr),
                    _currentIndex(0) {}
            iterator(const IHelpTopicCollection * collection, int startIndex)
                :   _collection(collection),
                    _currentIndex(startIndex)
            {
                Q_ASSERT(_collection != nullptr);
                Q_ASSERT(_currentIndex >= 0 && _currentIndex <= _collection->size());
            }
            iterator(const iterator & src)
                :   _collection(src._collection),
                    _currentIndex(src._currentIndex) {}
            virtual ~iterator() {}

            iterator &      operator = (const iterator & src)
            {
                _collection = src._collection;
                _currentIndex = src._currentIndex;
                return *this;
            }

            //////////
            //  Operators
        public:
            bool            operator == (const iterator & op2) const
            {
                if (_finished())
                {
                    return op2._finished();
                }
                else if (op2._finished())
                {
                    return false;
                }
                else
                {
                    return (_collection == op2._collection) &&
                           (_currentIndex == op2._currentIndex);
                }
            }
            bool            operator != (const iterator & op2) const
            {
                return !(*this == op2);
            }

            IHelpTopic *    operator ->() const
            {
                Q_ASSERT(!_finished());
                return _collection->operator[](_currentIndex);
            }
                            operator IHelpTopic *() const
            {
                Q_ASSERT(!_finished());
                return _collection->operator[](_currentIndex);
            }

            void            operator++();
            void            operator++(int);
            void            operator--();
            void            operator--(int);

            //////////
            //  Implementation
        private:
            const IHelpTopicCollection *    _collection;    //  nullptr == invalid iterator, considered finished
            int             _currentIndex;  //  0 for invalid iterators
            //  Helpers
            bool            _finished() const
            {
                return (_collection == nullptr) ||
                       (_currentIndex >= _collection->size());
            }
        };
        using const_iterator = iterator;

        //////////
        //  This is an interface
    protected:
        IHelpTopicCollection() = default;
        virtual ~IHelpTopicCollection() = default;

        //////////
        //  Operators
    public:
        virtual IHelpTopic *operator[](int index) const = 0;

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the number of topics in this collection.
        /// \return
        ///     The number of topics in this collection.
        virtual int     size() const = 0;

        //////////
        //  Iteration support
    public:
        iterator        begin()
        {
            return iterator(this, 0);
        }

        const_iterator  end() const
        {
            return const_iterator(this, this->size());
        }

        const_iterator  begin() const
        {
            return const_iterator(this, 0);
        }

        iterator        end()
        {
            return iterator(this, this->size());
        }

        const_iterator  cbegin() const
        {
            return const_iterator(this, 0);
        }

        const_iterator  cend() const
        {
            return const_iterator(this, this->size());
        }
    };

    class TT3_HELP_PUBLIC IHelpTopic
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(IHelpTopic)

        //////////
        //  This is an interface
    protected:
        IHelpTopic() = default;
        virtual ~IHelpTopic() = default;

        //////////
        //  Properties
    public:
        class TT3_HELP_PUBLIC Childern final
            :   public virtual IHelpTopicCollection
        {
            friend class IHelpTopic;

            //////////
            //  Construction/destruction
        private:
            explicit Childern(IHelpTopic * helpTopic)
                :   _helpTopic(helpTopic) {}

            //////////
            //  IHelpTopicCollection
        public:
            virtual IHelpTopic *operator[](int index) const override
            {
                return _helpTopic->child(index);
            }

            virtual int     size() const override
            {
                return _helpTopic->childCount();
            }

            //////////
            //  Implementation
        private:
            IHelpTopic *_helpTopic; //  whose children this collection represents
        };
        Childern        children { this };

        //////////
        //  Operations
    public:
        virtual QString name() const = 0;
        virtual QString path() const = 0;
        virtual QString displayName() const = 0;
        virtual auto    contentLoader(
                            ) const -> IContentLoader * = 0;
        virtual int     childCount() const = 0;
        virtual IHelpTopic *    child(int index) const = 0;
    };

    class TT3_HELP_PUBLIC IHelpCollection
        :   public virtual IHelpTopicCollection
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(IHelpCollection)

        //////////
        //  This is an interface
    protected:
        IHelpCollection() = default;
        virtual ~IHelpCollection() = default;

        //////////
        //  Properties
    public:
        class TT3_HELP_PUBLIC Roots final
            :   public virtual IHelpTopicCollection
        {
            friend class IHelpCollection;

            //////////
            //  Construction/destruction
        private:
            explicit Roots(IHelpCollection * helpCollection)
                :   _helpCollection(helpCollection) {}

            //////////
            //  IHelpTopicCollection
        public:
            virtual IHelpTopic *operator[](int index) const override
            {
                return _helpCollection->root(index);
            }

            virtual int     size() const override
            {
                return _helpCollection->rootCount();
            }

            //////////
            //  Implementation
        private:
            IHelpCollection *   _helpCollection;    //  whose roots this collection represents
        };
        Roots           roots { this };

        //////////
        //  Operations
    public:
        virtual int     rootCount() const = 0;
        virtual auto    root(int index) -> IHelpTopic * const  = 0;
    };
}

//  End of tt3-help/Intrerfaces.hpp
