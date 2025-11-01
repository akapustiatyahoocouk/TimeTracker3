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
        class TT3_HELP_PUBLIC Children final
        {
            friend class SimpleHelpTopic;

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
                iterator(const SimpleHelpTopic * helpTopic, int startIndex)
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

                iterator &  operator = (const iterator & src)
                {
                    _helpTopic = src._helpTopic;
                    _currentIndex = src._currentIndex;
                    return *this;
                }

                //////////
                //  Operators
            public:
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

                bool        operator != (const iterator & op2) const
                {
                    return !(*this == op2);
                }

                auto        operator ->(
                                ) const -> SimpleHelpTopic*
                {
                    Q_ASSERT(!_finished());
                    return _helpTopic->child(_currentIndex);
                }

                SimpleHelpTopic *   operator *() const
                {
                    Q_ASSERT(!_finished());
                    return _helpTopic->child(_currentIndex);
                }

                void        operator++()
                {
                    if (!_finished())
                    {
                        _currentIndex++;
                    }
                }
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
            using const_iterator = iterator;

            //////////
            //  Construction/destruction
        private:
            explicit Children(SimpleHelpTopic * helpTopic)
                :   _helpTopic(helpTopic) { Q_ASSERT(_helpTopic != nullptr); }

            //////////
            //  Operations
        public:
            SimpleHelpTopic*operator[](int index) const
            {
                return _helpTopic->child(index);
            }

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
            SimpleHelpTopic *   _helpTopic; //  whose children this collection represents
        };
        SimpleHelpTopic *const  parent;
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
        SimpleHelpCollection(
                const QString & name,
                const QString & displayName,
                IContentLoader * contentLoader
            );
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
