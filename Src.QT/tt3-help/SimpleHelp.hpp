//
//  tt3-help/SimpleHelp.hpp - The "simple" help system.
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
    /// \class SimpleHelpTopic tt3-help/API.hpp
    /// \brief A help topic that carries its poroperties directly.
    class TT3_HELP_PUBLIC SimpleHelpTopic
        :   public virtual HelpTopic
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(SimpleHelpTopic)

        friend class SimpleHelpCollection;

        //////////
        //  Construction/destruction
    private:
        SimpleHelpTopic(
                SimpleHelpTopic * parent,
                const QString & name,
                const QString & displayName,
                IContentLoader * contentLoader
            );
        virtual ~SimpleHelpTopic();

        //////////
        //  IHelpTopic
    public:
        virtual QString name() const override
        {
            return _name;
        }

        virtual QString displayName() const override
        {
            return _displayName;
        }

        virtual auto    contentLoader(
                            ) const -> IContentLoader * override
        {
            return _contentLoader;
        }

        virtual int     childCount() const override
        {
            return static_cast<int>(_children.size());
        }

        virtual auto    child(int index) const -> SimpleHelpTopic * override
        {
            return _children[index];
        }

        //////////
        //  Properties
    public:
        /// \class Children tt3-help/API.hpp
        /// \brief The ordered list of children of a SimpleHelpTopic.
        class TT3_HELP_PUBLIC Children final
        {
            friend class SimpleHelpTopic;

            //////////
            //  Types
        public:
            /// \class iterator tt3=help/API.hpp
            /// \brief The iterator over a collection of help topics.
            class TT3_HELP_PUBLIC iterator final
            {
                friend class Children;

                //////////
                //  Construction/destruction/assignment
            private:
                iterator(const SimpleHelpTopic * helpTopic, int startIndex)
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
                iterator &  operator = (const iterator & src)
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
                bool        operator == (const iterator & op2) const
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
                bool        operator != (const iterator & op2) const
                {
                    return !(*this == op2);
                }

                /// \brief
                ///     Returns the SimpleHelpTopic currently
                ///     pointed-to by this oterator.
                /// \details
                ///     If the iterator is "finished", the
                ///     call is an error.
                /// \return
                ///     The SimpleHelpTopic currently pointed-to
                ///     by this oterator.
                auto        operator ->(
                                ) const -> SimpleHelpTopic*
                {
                    Q_ASSERT(!_finished());
                    return _helpTopic->child(_currentIndex);
                }

                /// \brief
                ///     Returns the SimpleHelpTopic currently
                ///     pointed-to by this oterator.
                /// \details
                ///     If the iterator is "finished", the
                ///     call is an error.
                /// \return
                ///     The SimpleHelpTopic currently pointed-to
                ///     by this oterator.
                SimpleHelpTopic *   operator *() const
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
                const SimpleHelpTopic * _helpTopic; //  nullptr == invalid iterator, considered finished
                int         _currentIndex;  //  0 for invalid iterators

                //  Helpers
                bool        _finished() const
                {
                    return (_helpTopic == nullptr) ||
                           (_currentIndex >= _helpTopic->childCount());
                }
            };

            /// \class const_iterator tt3-help/API.hpp
            /// \brief
            ///     The iterator over a help topic collection that
            ///     does not permit modifying that collection.
            using const_iterator = iterator;

            //////////
            //  Construction/destruction
        private:
            explicit Children(SimpleHelpTopic * helpTopic)
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
            SimpleHelpTopic*operator[](int index) const
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

            auto            createTopic(
                                    const QString & name,
                                    const QString & displayName,
                                    IContentLoader * contentLoader
                                ) -> SimpleHelpTopic *;

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
            ///     the same as the finished "end()" iterator.
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
            SimpleHelpTopic *   _helpTopic; //  whose children this collection represents
        };

        /// \brief
        ///     The immediate parent of this SimpleHelpToopic;
        ///     nullptr == none.
        SimpleHelpTopic *const  parent;

        /// \brief
        ///     The ordered list of immediate children
        ///     of this SimpleHelpTopic.
        Children            children;

        //////////
        //  Operations
    public:
        void                setName(const QString name);
        void                setDisplayName(const QString & displayName);
        void                setContentUrl(const QUrl & contentUrl);

        //////////
        //  Implementation
    private:
        QString             _name;
        QString             _displayName;
        IContentLoader *    _contentLoader; //  owned by this SimpleHelpTopic, nullptr == none
        QList<SimpleHelpTopic*> _children;  //  all owned by this SimpleHelpTopic

        //  Serialization
        void                _serialize(QDomElement topisElement);
    };

    /// \class SimpleHelpCollection tt3-help/API.hpp
    /// \brief A help collection that carries its poroperties directly.
    class TT3_HELP_PUBLIC SimpleHelpCollection
        :   public HelpCollection,
            public SimpleHelpTopic

    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(SimpleHelpCollection)

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs the simple help collection
        /// \param name
        ///     The internal name of the help collection.
        /// \param displayName
        ///     The user-readable display name of the help collection.
        /// \param contentLoader
        ///     The content loader for the help collection's root
        ///     topic, nullptr == nonr.
        SimpleHelpCollection(
                const QString & name,
                const QString & displayName,
                IContentLoader * contentLoader
            );

        /// \brief
        ///     The class destructor.
        virtual ~SimpleHelpCollection();

        //////////
        //  Serialization
    protected:
        static inline const QString CollectionElementTag = "SimpleHelpCollection";

        virtual QString collectionElementTag() const override;
        virtual void    serialize(QDomElement collectionElement) override;
    };
}

//  End of tt3-help/SimpleHelp.hpp
