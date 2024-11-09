using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Xml;
using TimeTracker3.Db.API;
using TimeTracker3.Db.API.Exceptions;
using TimeTracker3.Util;

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
            Debug.Assert(database._Guard.IsLockedByCurrentThread);
            Debug.Assert(!database._Objects.ContainsKey(oid));

            _Database = database;
            _Oid = oid;
            _Live = true;
            database._Objects[oid] = this;
        }

        //////////
        //  IDatabaseObject - Properties

        /// <inheritdoc />  //  TODO use everywhere for overriding/implementing methods and properties
        public abstract DatabaseObjectType ObjectType { get; }

        /// <inheritdoc />
        public bool IsLive
        {
            get
            {
                using (new Lock(_Database._Guard))
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
                using (new Lock(_Database._Guard))
                {
                    return _Oid;
                }
            }
            set => throw new NotImplementedException();
        }

        /// <inheritdoc />
        public IUiTraits UiTraits => DatabaseObjectUiTraits.GetUiTraits(this);

        //////////
        //  IDatabaseObject - Operations - life cycle
        public abstract void Destroy();

        //////////
        //  Implementation
        internal readonly XmlFileDatabase _Database;
        internal readonly XmlFileIDatabaseObjectId _Oid;
        private bool _Live;

        //  Helpers
        internal void _CheckLive()
        {
            Debug.Assert(_Database._Guard.IsLockedByCurrentThread);

            _Database._CheckOpen();
            if (!_Live)
            {   //  OOPS! Dead!
                throw new ObjectDeadDatabaseException(ObjectType, _Oid);
            }
        }

        //////////
        //  Serialization
        internal virtual void _SerializeProperties(XmlElement element)
        {   //  "element" is the XML element representing this database object
            Debug.Assert(_Database._Guard.IsLockedByCurrentThread);
        }

        internal virtual void _SerializeAggregations(XmlElement element)
        {   //  "element" is the XML element representing this database object
            Debug.Assert(_Database._Guard.IsLockedByCurrentThread);
        }

        internal virtual void _SerializeAssociations(XmlElement element)
        {   //  "element" is the XML element representing this database object
            Debug.Assert(_Database._Guard.IsLockedByCurrentThread);
        }

        //////////
        //  Deserialization
        internal virtual void _DeserializeProperties(XmlElement element)
        {   //  "element" is the XML element representing this database object
            Debug.Assert(_Database._Guard.IsLockedByCurrentThread);
        }

        internal virtual void _DeserializeAggregations(XmlElement element)
        {   //  "element" is the XML element representing this database object
            Debug.Assert(_Database._Guard.IsLockedByCurrentThread);
        }

        internal virtual void _DeserializeAssociations(XmlElement element)
        {   //  "element" is the XML element representing this database object
            Debug.Assert(_Database._Guard.IsLockedByCurrentThread);
        }
    }
}
