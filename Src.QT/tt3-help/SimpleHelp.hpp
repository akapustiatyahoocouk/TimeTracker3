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
        :   public virtual IHelpTopic
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(SimpleHelpTopic)

        friend class SimpleHelpTopicCollection;
        friend class SimpleHelpCollection;

        //////////
        //  Construction/destruction
    protected:
        SimpleHelpTopic(
                const QString & name,
                const QString & path,
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

        virtual QString path() const override
        {
            return _path;
        }

        virtual QString displayName() const override
        {
            return _displayName;
        }

        virtual auto        contentLoader(
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
        //  Implementation
    private:
        QString             _name;
        QString             _path;
        QString             _displayName;
        IContentLoader *    _contentLoader; //  owned by this SimpleHelpTopic
        QList<SimpleHelpTopic*> _children;  //  all owned by this SimpleHelpTopic
    };

    /// \class SimpleHelpTopicCollection tt3-help/API.hpp
    /// \brief A collection of SimpleHelpTopics.
    class TT3_HELP_PUBLIC SimpleHelpTopicCollection
        :   public virtual IHelpTopicCollection
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(SimpleHelpTopicCollection)

        //////////
        //  Types
    public:
        class TT3_HELP_PUBLIC iterator
                :   public IHelpTopicCollection::iterator
        {
            using Base = IHelpTopicCollection::iterator;

            //////////
            //  Construction/destruction/assignment
        public:
            iterator() : Base() {}
            iterator(const SimpleHelpTopicCollection * collection, int startIndex)
                :   Base(collection, startIndex) {}
            iterator(const iterator & src) : Base(src) {}
            virtual ~iterator() {}

            iterator &      operator = (const iterator & src)
            {
                Base::operator = (src);
                return *this;
            }

            //////////
            //  Operators
        public:
            SimpleHelpTopic*operator ->() const;
                            operator SimpleHelpTopic *() const;
        };
        using const_iterator = iterator;

        //////////
        //  Construction/destruction
    protected:
        SimpleHelpTopicCollection();
        virtual ~SimpleHelpTopicCollection();

        //////////
        //  Operators
    public:
        virtual auto    operator[](int index) const -> SimpleHelpTopic * override
        {
            return _members[index];
        }

        //////////
        //  IHelpTopicCollection
    public:
        virtual int   size() const override
        {
            return static_cast<int>(_members.size());
        }

        //////////
        //  Iteration support
    public:
        iterator        begin()
        {
            return iterator(this, 0);
        }

        const_iterator  end() const
        {
            return const_iterator(this, static_cast<int>(_members.size()));
        }

        const_iterator  begin() const
        {
            return const_iterator(this, 0);
        }

        iterator        end()
        {
            return iterator(this, static_cast<int>(_members.size()));
        }

        const_iterator  cbegin() const
        {
            return const_iterator(this, 0);
        }

        const_iterator  cend() const
        {
            return const_iterator(this, static_cast<int>(_members.size()));
        }

        //////////
        //  Implementation
    private:
        QList<SimpleHelpTopic*> _members;   //  owned by this SimpleHelpTopicCollection
    };

    /// \class SimpleHelpCollection tt3-help/API.hpp
    /// \brief A help collection that carries its poroperties directly.
    class TT3_HELP_PUBLIC SimpleHelpCollection
        :   public SimpleHelpTopicCollection,
            public IHelpCollection
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(SimpleHelpCollection)

        //////////
        //  Construction/destruction
    protected:
        SimpleHelpCollection();
        virtual ~SimpleHelpCollection();

        //////////
        //  IHelpCollection
    public:
        virtual int     rootCount() const override
        {
            return static_cast<int>(_roots.size());
        }

        virtual auto    root(int index) -> SimpleHelpTopic * const override
        {
            return _roots[index];
        }

        //////////
        //  Implementation
    private:
        QList<SimpleHelpTopic*> _roots; //  owned by this SimpleHelpTopicCollection
    };
}

//  End of tt3-help/SimpleHelp.hpp
