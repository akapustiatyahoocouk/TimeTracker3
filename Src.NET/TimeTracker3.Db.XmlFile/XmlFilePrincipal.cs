using System.Diagnostics;
using System.Linq;
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
    }
}
