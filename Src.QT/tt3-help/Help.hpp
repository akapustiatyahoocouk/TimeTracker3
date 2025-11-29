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
    /// \class HelpTopic tt3-help/API.hpp
    /// \brief  node in the help collection.
    class TT3_HELP_PUBLIC HelpTopic
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(HelpTopic)

        //////////
        //  Construction/destruction
    protected:
        /// \brief
        ///     Constructs the help topic.
        /// \param parent
        ///     The immediate parent for the help
        ///     topic, nullptr == none.
        explicit HelpTopic(HelpTopic * parent)
            :   parent(parent), children(this) {}

        /// \brief
        ///     The class destructor.
        virtual ~HelpTopic() = default;

        //////////
        //  Properties
    public:
        /// \class Children tt3-help/API.hpp
        /// \brief The ordered list od help topic's children.
        class TT3_HELP_PUBLIC Children final
        {
            friend class HelpTopic;

            //////////
            //  Types
        public:
            /// \class iterator tt3-help/API.hpp
            /// \brief The iterator over a collection of help topics.
            class TT3_HELP_PUBLIC iterator final
            {
                friend class Children;

                //////////
                //  Construction/destruction/assignment
            private:
                iterator(const HelpTopic * helpTopic, int startIndex)
                    :   _helpTopic(helpTopic),
                    _currentIndex(startIndex)
                {
                    Q_ASSERT(_helpTopic != nullptr);
                    Q_ASSERT(_currentIndex >= 0 &&
                             _currentIndex <= _helpTopic->childCount());
                }
            public:
                /// \brief
                ///     Constructs a finished iterator.
                iterator()
                    :   _helpTopic(nullptr),
                        _currentIndex(0) {}

                /// \brief
                ///     The copy constructor.
                /// \param src
                ///     The source to copu from.
                iterator(const iterator & src)
                    :   _helpTopic(src._helpTopic),
                        _currentIndex(src._currentIndex) {}

                /// \brief
                ///     The class destructor.
                ~iterator() = default;

                /// \brief
                ///     Performs assignment.
                /// \param src
                ///     The source to assign from.
                /// \return
                ///     The assigned-to iterator.
                iterator &      operator = (const iterator & src)
                {
                    _helpTopic = src._helpTopic;
                    _currentIndex = src._currentIndex;
                    return *this;
                }

                //////////
                //  Operators
            public:
                /// \brief
                ///     Compares two iterators for equality.
                /// \details
                ///     Two iterators atr considered "equal" is they
                ///     a) point to the same member of the same gelp
                ///     topics collection oe b) are all "finished".
                /// \param op2
                ///     The 2nd iterator to compare this one to.
                /// \return
                ///     True if the two iterators are "equal@, else false.
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

                /// \brief
                ///     Compares two iterators for equality.
                /// \details
                ///     Two iterators atr considered "equal" is they
                ///     a) point to the same member of the same gelp
                ///     topics collection oe b) are all "finished".
                /// \param op2
                ///     The 2nd iterator to compare this one to.
                /// \return
                ///     False if the two iterators are "equal@, else true.
                bool            operator != (const iterator & op2) const
                {
                    return !(*this == op2);
                }

                /// \brief
                ///     Returns the HelpTopic currently
                ///     pointed-to by this oterator.
                /// \details
                ///     If the iterator is "finished", the
                ///     call is an error.
                /// \return
                ///     The HelpTopic currently pointed-to
                ///     by this oterator.
                HelpTopic *     operator ->() const
                {
                    Q_ASSERT(!_finished());
                    return _helpTopic->child(_currentIndex);
                }

                /// \brief
                ///     Returns the HelpTopic currently
                ///     pointed-to by this oterator.
                /// \details
                ///     If the iterator is "finished", the
                ///     call is an error.
                /// \return
                ///     The HelpTopic currently pointed-to
                ///     by this oterator.
                HelpTopic * operator *() const
                {
                    Q_ASSERT(!_finished());
                    return _helpTopic->child(_currentIndex);
                }

                /// \brief
                ///     Advances this iterator to the next element
                ///     of the HelpTopics collection.
                /// \brief
                ///     If the iterator is "finished", the call has no effect.
                void        operator++()
                {
                    if (!_finished())
                    {
                        _currentIndex++;
                    }
                }

                /// \brief
                ///     Advances this iterator to the next element
                ///     of the HelpTopics collection.
                /// \brief
                ///     If the iterator is "finished", the call has no effect.
                void        operator++(int) { operator++(); }

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

            /// \brief
            ///     The iterator over a help topic collection that
            ///     does not permit modifying that collection.
            using const_iterator = iterator;

            //////////
            //  Construction/destruction
        private:
            explicit Children(HelpTopic * helpTopic)
                :   _helpTopic(helpTopic) { Q_ASSERT(_helpTopic != nullptr); }

            //////////
            //  Operations
        public:
            /// \brief
            ///     Returns the specific element from yhis collection.
            /// \param index
            ///     The 0-based index of the required element.
            /// \return
            ///     The element at "index" in this collectiom.
            HelpTopic *     operator[](int index) const
            {
                return _helpTopic->child(index);
            }

            /// \brief
            ///     Returns the size of this collection.
            /// \return
            ///     The size of this collection.
            int             size() const
            {
                return _helpTopic->childCount();
            }

            //////////
            //  Iteration support
        public:
            /// \brief
            ///     Returns the iterator to the starting
            ///     element of this collection.
            /// \details
            ///     If the collection is empyu, this will be
            ///     the same as the finished "end()" iterator.
            /// \return
            ///     The iterator to the starting
            ///     element of this collection.
            iterator        begin()
            {
                return iterator(_helpTopic, 0);
            }

            /// \brief
            ///     Returns a "finished: iterator to just beyouf
            ///     the last element od this collection.
            /// \return
            ///     A "finished: iterator to just beyouf
            ///     the last element od this collection.
            iterator        end()
            {
                return iterator(_helpTopic, _helpTopic->childCount());
            }

            /// \brief
            ///     Returns the iterator to the starting
            ///     element of this collection.
            /// \details
            ///     If the collection is empyu, this will be
            ///     the same as the finished "end()" iterator.
            /// \return
            ///     The iterator to the starting
            ///     element of this collection.
            const_iterator  begin() const
            {
                return const_iterator(_helpTopic, 0);
            }

            /// \brief
            ///     Returns a "finished: iterator to just beyouf
            ///     the last element od this collection.
            /// \return
            ///     A "finished: iterator to just beyouf
            ///     the last element od this collection.
            const_iterator  end() const
            {
                return const_iterator(_helpTopic, _helpTopic->childCount());
            }

            /// \brief
            ///     Returns the iterator to the starting
            ///     element of this collection.
            /// \details
            ///     If the collection is empyu, this will be
            ///     the same as the finished "cend()" iterator.
            /// \return
            ///     The iterator to the starting
            ///     element of this collection.
            const_iterator  cbegin() const
            {
                return const_iterator(_helpTopic, 0);
            }

            /// \brief
            ///     Returns a "finished: iterator to just beyouf
            ///     the last element od this collection.
            /// \return
            ///     A "finished: iterator to just beyouf
            ///     the last element od this collection.
            const_iterator  cend() const
            {
                return const_iterator(_helpTopic, _helpTopic->childCount());
            }

            //////////
            //  Implementation
        private:
            HelpTopic *     _helpTopic; //  whose children this collection represents
        };

        /// \brief
        ///     The immediate parent of this HelpToopic;
        ///     nullptr == none.
        HelpTopic *const    parent;

        /// \brief
        ///     The ordered list of immediate children
        ///     of this HelpTopic.
        Children            children;

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the internal name of this HelpTopic.
        /// \return
        ///     The internal name of this HelpTopic.
        virtual QString     name() const = 0;

        /// \brief
        ///     Returns the full path of this HelpYopic
        ///     within the help collection.
        /// \details
        ///     This is a string of the form /t1/t2/.../tn, where
        ///     each tN is a name of the Nth help topic along the
        ///     path from the root of the help collection to this
        ///     HelpTopic.
        /// \return
        ///     The full path of this HelpYopic
        ///     within the help collection.
        virtual QString     path() const
        {
            return (parent != nullptr) ? (parent->path() + "/" + name()) : "";
        }

        /// \brief
        ///     Returns the user-readable display name of this
        ///     HelpTopic for the current defult locale.
        /// \return
        ///     The user-readable display name of this
        ///     HelpTopic for the current defult locale.
        virtual QString     displayName() const = 0;

        /// \brief
        ///     Returns the ContentLoader agent that is used
        ///     to load this GelpTopic's content.
        /// \return
        ///     The ContentLoader agent that is used
        ///     to load this GelpTopic's content; nullptr == none.
        virtual auto        contentLoader(
                                ) const -> IContentLoader * = 0;

        /// \brief
        ///     Returns the number of immediate children this HelpTopic has.
        /// \return
        ///     The number of immediate children this HelpTopic has.
        virtual int         childCount() const = 0;

        /// \brief
        ///     Returns the secific child of this HelpTopic.
        /// \param index
        ///     The 0-based index of the child to getch.
        /// \return
        ///     The child at "index" position under this HelpTopic.
        virtual HelpTopic * child(int index) const = 0;
    };

    /// \class HelpCollection tt3-help/API.hpp
    /// \brief A collection of related help topics.
    class TT3_HELP_PUBLIC HelpCollection
        :   public virtual HelpTopic
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(HelpCollection)

        friend class Serializer;

        //////////
        //  Construction/destruction
    protected:
        /// \brief
        ///     Constructs an empty HelpCollection.
        HelpCollection() : HelpTopic(nullptr) {}
    public:
        /// \brief
        ///     The class destructor.
        virtual ~HelpCollection() = default;

        //////////
        //  Serialization
    protected:
        /// \brief
        ///     Returns the XML tag name for the collection element.
        /// \return
        ///     The XML tag name for the collection element.
        virtual QString collectionElementTag() const = 0;

        /// \brief
        ///     Serializes this help collection to an XML element.
        /// \param collectionElement
        ///     The XML element representing the help collection itself.
        virtual void    serialize(QDomElement collectionElement) = 0;
    };
}

//  End of tt3-help/Help.hpp
