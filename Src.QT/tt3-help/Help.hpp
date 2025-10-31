//
//  tt3-help/Help.hpp - Help system interfaces
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
    class TT3_HELP_PUBLIC HelpTopic
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(HelpTopic)

        //////////
        //  Construction/destruction
    protected:
        explicit HelpTopic(HelpTopic * parent)
            :   parent(parent) {}
        virtual ~HelpTopic() = default;

        //////////
        //  Properties
    public:
        class TT3_HELP_PUBLIC Children final
        {
            friend class HelpTopic;

            //////////
            //  Types
        public:
            class TT3_HELP_PUBLIC iterator final
            {
                //////////
                //  Construction/destruction/assignment
            public:
                iterator()
                    :   _helpTopic(nullptr),
                        _currentIndex(0) {}
                iterator(const HelpTopic * helpTopic, int startIndex)
                    :   _helpTopic(helpTopic),
                        _currentIndex(startIndex)
                {
                    Q_ASSERT(_helpTopic != nullptr);
                    Q_ASSERT(_currentIndex >= 0 &&
                             _currentIndex <= _helpTopic->childCount());
                }
                iterator(const iterator & src)
                    :   _helpTopic(src._helpTopic),
                        _currentIndex(src._currentIndex) {}
                virtual ~iterator() {}

                iterator &      operator = (const iterator & src)
                {
                    _helpTopic = src._helpTopic;
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
                        return (_helpTopic == op2._helpTopic) &&
                               (_currentIndex == op2._currentIndex);
                    }
                }
                bool            operator != (const iterator & op2) const
                {
                    return !(*this == op2);
                }

                HelpTopic *     operator ->() const
                {
                    Q_ASSERT(!_finished());
                    return _helpTopic->child(_currentIndex);
                }

                operator HelpTopic *() const
                {
                    Q_ASSERT(!_finished());
                    return _helpTopic->child(_currentIndex);
                }

                void            operator++();
                void            operator++(int);
                void            operator--();
                void            operator--(int);

                //////////
                //  Implementation
            private:
                const HelpTopic*_helpTopic;     //  nullptr == invalid iterator, considered finished
                int             _currentIndex;  //  0 for invalid iterators
                //  Helpers
                bool            _finished() const
                {
                    return (_helpTopic == nullptr) ||
                           (_currentIndex >= _helpTopic->childCount());
                }
            };
            using const_iterator = iterator;

            //////////
            //  Construction/destruction
        private:
            explicit Children(HelpTopic * helpTopic)
                :   _helpTopic(helpTopic) { Q_ASSERT(_helpTopic != nullptr); }

            //////////
            //  Operations
        public:
            HelpTopic *     operator[](int index) const
            {
                return _helpTopic->child(index);
            }

            int             size() const
            {
                return _helpTopic->childCount();
            }

            //////////
            //  Iteration support
        public:
            iterator        begin()
            {
                return iterator(_helpTopic, 0);
            }

            const_iterator  end() const
            {
                return const_iterator(_helpTopic, _helpTopic->childCount());
            }

            const_iterator  begin() const
            {
                return const_iterator(_helpTopic, 0);
            }

            iterator        end()
            {
                return iterator(_helpTopic, _helpTopic->childCount());
            }

            const_iterator  cbegin() const
            {
                return const_iterator(_helpTopic, 0);
            }

            const_iterator  cend() const
            {
                return const_iterator(_helpTopic, _helpTopic->childCount());
            }

            //////////
            //  Implementation
        private:
            HelpTopic *     _helpTopic; //  whose children this collection represents
        };
        HelpTopic *const    parent;
        Children            children { this };

        //////////
        //  Operations
    public:
        virtual QString     name() const = 0;
        virtual QString     path() const
        {
            return (parent != nullptr) ? (parent->path() + "/" + name()) : "";
        }
        virtual QString     displayName() const = 0;
        virtual auto        contentLoader(
                                ) const -> IContentLoader * = 0;
        virtual int         childCount() const = 0;
        virtual HelpTopic * child(int index) const = 0;
    };

    class TT3_HELP_PUBLIC HelpCollection
        :   public virtual HelpTopic
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(HelpCollection)

        //////////
        //  Construction/destruction
    protected:
        HelpCollection() : HelpTopic(nullptr) {}
        virtual ~HelpCollection() = default;
    };
}

//  End of tt3-help/Help.hpp
