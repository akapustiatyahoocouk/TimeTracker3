using System;
using System.Diagnostics;
using TimeTracker3.Db.API;

namespace TimeTracker3.Db.XmlFile
{
    /// <summary>
    ///     A generic object in an "XML file" database.
    /// </summary>
    public abstract class XmlFileDatabaseObject : IDatabaseObject
    {
        //////////
        //  Construction
        internal XmlFileDatabaseObject(XmlFileDatabase database, XmlFileIDatabaseObjectId oid)
        {
            Debug.Assert(database != null);

            _Database = database;
            _Oid = oid;
            _Live = true;
        }

        //////////
        //  Properties

        /// <inheritdoc />  //  TODO use everywhere for overriding/implementing methods and properties
        public abstract DatabaseObjectType ObjectType { get; }

        /// <inheritdoc />
        public bool IsLive
        {
            get
            {
                lock (_Database._Guard)
                {
                    return _Live;
                }
            }
        }

        /// <inheritdoc />
        public IDatabase Database => _Database;

        /// <inheritdoc />
        public IDatabaseObjectId Oid
        {
            get
            {
                lock (_Database._Guard)
                {
                    return _Oid;
                }
            }
            set => throw new NotImplementedException();
        }

        /// <inheritdoc />
        public IUiTraits UiTraits => DatabaseObjectUiTraits.GetUiTraits(this);

        //////////
        //  Operations - life cycle
        public abstract void Destroy();

        //////////
        //  Implementation
        private readonly XmlFileDatabase _Database;
        private readonly XmlFileIDatabaseObjectId _Oid;
        private bool _Live;
    }
}
