using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Xml;
using TimeTracker3.Db.API;
using TimeTracker3.Util;

namespace TimeTracker3.Db.XmlFile
{
    /// <summary>
    ///     A "XML file" database Principal implementation.
    /// </summary>
    public abstract class XmlFilePrincipal : XmlFileDatabaseObject, IPrincipal
    {
        //////////
        //  Construction
        internal XmlFilePrincipal(XmlFileDatabase database, XmlFileIDatabaseObjectId oid,
            string[] emailAddresses, bool enabled)
        : base(database, oid)
        {
            Debug.Assert(emailAddresses != null && !emailAddresses.Contains(null));

            _EmailAddresses = emailAddresses;
            _Enabled = enabled;
        }

        //////////
        //  IPrincipal - Properties
        public string[] EmailAddresses
        {
            get
            {
                using (new Lock(_Database._Guard))
                {
                    _CheckLive();
                    return (string[])_EmailAddresses.Clone();
                }
            }
            set
            {
                Debug.Assert(value != null && !value.Contains(null));

                using (new Lock(_Database._Guard))
                {
                    _CheckLive();
                    if (!value.SequenceEqual(_EmailAddresses))
                    {   //  There IS actually a change - make it...
                        _EmailAddresses = (string[])value.Clone();
                        //  ...and issue change notification
                        _Database._ScheduleNotification(
                            new DatabaseObjectModifiedEventArgs(_Database, ObjectType, _Oid));
                    }
                }
            }
        }

        public bool Enabled
        {
            get
            {
                using (new Lock(_Database._Guard))
                {
                    _CheckLive();
                    return _Enabled;
                }
            }
            set
            {
                using (new Lock(_Database._Guard))
                {
                    _CheckLive();
                    if (value != _Enabled)
                    {   //  There IS actually a change - make it...
                        _Enabled = value;
                        //  ...and issue change notification
                        _Database._ScheduleNotification(
                            new DatabaseObjectModifiedEventArgs(_Database, ObjectType, _Oid));
                    }
                }
            }
        }

        //////////
        //  Implementation
        internal string[] _EmailAddresses;
        internal bool _Enabled;

        //////////
        //  Serialization
        internal override void _SerializeProperties(XmlElement element)
        {   //  "element" is the XML element representing this database object
            base._SerializeProperties(element);

            element.SetAttribute("EmailAddresses", string.Join("|", _EmailAddresses));
            element.SetAttribute("Enabled", Util.Formatting.FormatBool(_Enabled));
        }

        internal override void _SerializeAggregations(XmlElement element)
        {   //  "element" is the XML element representing this database object
            base._SerializeAggregations(element);
            //  Principal has no aggregations of its own
        }

        internal override void _SerializeAssociations(XmlElement element)
        {   //  "element" is the XML element representing this database object
            base._SerializeAssociations(element);
            //  Principal has no associations of its own
        }

        //////////
        //  Deserialization
        internal XmlFilePrincipal(XmlFileDatabase database, XmlFileIDatabaseObjectId oid)
            : base(database, oid)
        {
            _EmailAddresses = Array.Empty<string>();
            _Enabled = false;
        }

        internal override void _DeserializeProperties(XmlElement element)
        {   //  "element" is the XML element representing this database object
            base._DeserializeProperties(element);

            _EmailAddresses =
                element.GetAttribute("EmailAddresses")
                       .Split('|')
                       .Where(a => a.Trim().Length > 0)
                       .ToArray();
            _Enabled = Util.Parsing.Parse<bool>(element.GetAttribute("Enabled"), false);
        }

        internal override void _DeserializeAggregations(XmlElement element)
        {   //  "element" is the XML element representing this database object
            base._DeserializeAggregations(element);
            //  Principal has no aggregations of its own
        }

        internal override void _DeserializeAssociations(XmlElement element)
        {   //  "element" is the XML element representing this database object
            base._DeserializeAssociations(element);
            //  Principal has no associations of its own
        }
    }
}
