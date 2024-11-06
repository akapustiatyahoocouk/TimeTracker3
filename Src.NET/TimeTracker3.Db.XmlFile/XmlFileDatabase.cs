using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Threading;
using System.Xml;
using TimeTracker3.Db.API;
using TimeTracker3.Db.API.Exceptions;
using TimeTracker3.Util;

namespace TimeTracker3.Db.XmlFile
{
    /// <summary>
    ///     The single-user database stored as an XML file.
    /// </summary>
    public sealed class XmlFileDatabase : IDatabase
    {
        //////////
        //  Types
        public enum Mode { CreateNew, OpenExisting }

        //////////
        //  Construction

        /// <summary>
        ///     Creates or opens an "XML file" database.
        /// </summary>
        /// <param name="address">
        ///     The address of the "XML file" database.
        /// </param>
        /// <param name="mode">
        ///     The "XML file" database construction mode.
        /// </param>
        public XmlFileDatabase(XmlFileDatabaseAddress address, Mode mode)
        {
            Debug.Assert(address != null);

            _Address = address;
            //  We need to grab a lock first
            _DatabaseFileLock = new _XmlFileDatabaseLock(address);
            //  and the rest depends on the object construction mode
            try
            {
                using (new Lock(_Guard))
                {
                    switch (mode)
                    {
                        case Mode.CreateNew:
                            _Save(); //  can throw DatabaseException
                            break;
                        case Mode.OpenExisting:
                            _Load(); //  can throw DatabaseException
                            break;
                        default:
                            throw new ArgumentOutOfRangeException(nameof(mode), mode, null);
                    }
                    //  Begin handling database notifications
                    _NotificationThread = new Thread(_NotificationThreadProc);
                    _NotificationThread.Start();
                }
            }
            catch (Exception ex)
            {   //  OOPS! Cleanup & re-throw
                _DatabaseFileLock._Close();
                throw DatabaseException.Translate(ex);
            }
        }

        //////////
        //  Properties
        public IDatabaseType Type => XmlFileDatabaseType.Instance;
        public IDatabaseAddress Address => _Address;

        public bool IsOpen
        {
            get
            {
                using (new Lock(_Guard))
                {
                    return _DatabaseFileLock != null && _DatabaseFileLock._IsOpen;
                }
            }
        }

        public IValidator Validator => _Validator;

        //////////
        //  Operations - general
        public void Close()
        {
            using (new Lock(_Guard))
            {
                if (_DatabaseFileLock != null)
                {   //  Still "open" - save...
                    Exception ex = null;
                    if (_NeedsSaving)
                    {
                        try
                        {
                            _Save();
                        }
                        catch (Exception exx)
                        {
                            ex = exx;
                        }
                    }
                    //  ...release the lock...
                    _DatabaseFileLock._Close();
                    _DatabaseFileLock = null;
                    //  ...stop handling notifications...
                    _NotificationThreadStopRequested = true;
                    _NotificationThread.Join();
                    _NotificationThread = null;
                    //  ...then mark all objects as "dead" and
                    //  release references to them...
                    //  TODO
                    //  ...and we're closed
                    if (ex != null)
                    {
                        throw DatabaseException.Translate(ex);
                    }
                }
            }
        }

        //////////
        //  Associations
        public IUser[] Users => throw new NotImplementedException();
        public IActivityType[] ActivityTypes => throw new NotImplementedException();
        public IPublicActivity[] PublicActivitiesAndTasks => throw new NotImplementedException();
        public IPublicActivity[] PublicActivities => throw new NotImplementedException();
        public IPublicTask[] PublicTasks => throw new NotImplementedException();
        public IPublicTask[] RootPublicTasks => throw new NotImplementedException();
        public IWork[] Works => throw new NotImplementedException();
        public IEvent[] Events => throw new NotImplementedException();
        public IBeneficiary[] Beneficiaries => throw new NotImplementedException();
        public IWorkload[] Workloads => throw new NotImplementedException();
        public IProject[] Projects => throw new NotImplementedException();
        public IProject[] RootProjects => throw new NotImplementedException();
        public IWorkStream[] WorkStreams => throw new NotImplementedException();

        //////////
        //  Access control
        public IAccount TryLogin(string login, string password)
        {
            Debug.Assert(login != null);
            Debug.Assert(password != null);

            using (new Lock(_Guard))
            {
                _CheckOpen();

                string passwordHash = Sha1.HashString(password);
                return _Objects.Values
                               .Where(o => o is XmlFileAccount)
                               .Cast<XmlFileAccount>()
                               .FirstOrDefault(a => a._Enabled && a._User._Enabled &&
                                               a._Login == login && a._PasswordHash == passwordHash);
            }
        }

        //////////
        //  Events
        public event EventHandler<DatabaseObjectCreatedEventArgs> ObjectCreated;
        public event EventHandler<DatabaseObjectModifiedEventArgs> ObjectModified;
        public event EventHandler<DatabaseObjectDestroyedEventArgs> ObjectDestroyed;

        //////////
        //  Implementation
        private readonly XmlFileDatabaseAddress _Address;
        private _XmlFileDatabaseLock _DatabaseFileLock; //  null == database is closed
        internal readonly Guard _Guard = new Guard();   //  for all access synchronization
        private bool _NeedsSaving /* = false */;
        private readonly IValidator _Validator = new DefaultValidator();

        //  Object tables
        internal readonly IDictionary<XmlFileIDatabaseObjectId, XmlFileDatabaseObject> _Objects =
            new Dictionary<XmlFileIDatabaseObjectId, XmlFileDatabaseObject>();
        internal static long _NextUnusedOid = 1;

        //  Helpers
        internal void _CheckOpen()
        {
            Debug.Assert(_Guard.IsLockedByCurrentThread);

            if (_DatabaseFileLock == null)
            {   //  OOPS! Closed!
                throw new DatabaseClosedDatabaseException(_Address);
            }
        }

        private void _Load()
        {
            Debug.Assert(_Guard.IsLockedByCurrentThread);
            _CheckOpen();

            try
            {
                //  Load XML DOM...
                XmlDocument document = new XmlDocument();
                document.Load(_Address._FileName);
                //  ...and analyze it
                if (document.DocumentElement == null ||
                    document.DocumentElement.Name != "TimeTrackerDatabase" ||
                    document.DocumentElement.GetAttribute("FormatVersion") != "3")
                {   //  OOPS! Not a valid TT3 database file!
                    throw new DatabaseCorruptDatabaseException(_Address);
                }

                Dictionary<XmlFileDatabaseObject, XmlElement> xmlElementsForDatabaseObjects =
                    new Dictionary<XmlFileDatabaseObject, XmlElement>();
                foreach (XmlElement userElement in
                         document.DocumentElement.ChildNodes
                             .OfType<XmlElement>()
                             .Where(e => e.Name == DatabaseObjectTypes.User.Name))
                {
                    long oidValue = long.Parse(userElement.GetAttribute("OID"));
                    XmlFileIDatabaseObjectId oid = new XmlFileIDatabaseObjectId(oidValue);
                    if (_Objects.ContainsKey(oid))
                    {   //  OOPS! Duplicate OID!
                        throw new DatabaseCorruptDatabaseException(_Address);
                    }
                    //  Create & de-serialize user
                    XmlFileUser user = new XmlFileUser(this, oid, new string[0], true, null, "unknown", null);
                    user._DeserializeProperties(userElement);
                    user._DeserializeAggregations(userElement);
                    xmlElementsForDatabaseObjects[user] = userElement;
                }
                //  TODO finish implementation

                //  Now that all objects have been created, it's time
                //  to de-serialize associations between them
                foreach (XmlFileDatabaseObject databaseObject in _Objects.Values)
                {
                    databaseObject._DeserializeAssociations(xmlElementsForDatabaseObjects[databaseObject]);
                }
            }
            catch (Exception ex)
            {   //  TODO log ?
                throw new DatabaseCorruptDatabaseException(_Address, ex);
            }
        }

        private void _Save()
        {
            Debug.Assert(_Guard.IsLockedByCurrentThread);
            _CheckOpen();

            //  Create XML DOM...
            XmlDocument document = new XmlDocument();
            XmlElement documentElement = document.CreateElement("TimeTrackerDatabase");
            documentElement.SetAttribute("FormatVersion", "3");
            document.AppendChild(documentElement);
            Debug.Assert(document.DocumentElement != null);

            //  Serialize Users and Accounts
            foreach (XmlFileUser user in
                     _Objects.Values
                             .Where(o => o is XmlFileUser)
                             .Cast<XmlFileUser>())
            {
                _SerializeObject(user, documentElement);
            }
            //  TODO finish implementation

            //  XML DOM created - now save it
            try
            {
                document.Save(_Address._FileName);          //  2. may throw, oldFileName contains the original database
            }
            catch (Exception ex)
            {   //  OOPS! Cleanup and re-throw
                throw DatabaseException.Translate(ex);
            }
        }

        private void _SerializeObject(XmlFileDatabaseObject databaseObject, XmlElement parentElement)
        {
            Debug.Assert(databaseObject != null);
            Debug.Assert(parentElement != null);
            Debug.Assert(parentElement.OwnerDocument != null);

            XmlElement objectElement = parentElement.OwnerDocument.CreateElement(databaseObject.ObjectType.Name);
            objectElement.SetAttribute("OID", databaseObject._Oid.ToString());
            parentElement.AppendChild(objectElement);

            databaseObject._SerializeProperties(objectElement);
            databaseObject._SerializeAggregations(objectElement);
            databaseObject._SerializeAssociations(objectElement);
        }

        //  Notification handling
        private struct _Notification
        {
            public _Notification(object sender, DatabaseEventArgs e)
            {
                Sender = sender;
                E = e;
            }
            public readonly object Sender;
            public readonly DatabaseEventArgs E;
        }
        private readonly InterthreadQueue<_Notification> _PendingNotifications = new InterthreadQueue<_Notification>();
        private Thread _NotificationThread;
        private volatile bool _NotificationThreadStopRequested /* = false */;

        internal void _ScheduleNotification(DatabaseEventArgs e)
        {
            Debug.Assert(_Guard.IsLockedByCurrentThread);

            _PendingNotifications.Enqueue(new _Notification(this, e));
            _NeedsSaving = true;
        }

        private void _NotificationThreadProc()
        {
            while (!_NotificationThreadStopRequested)
            {
                //  Are there any pending notifications ?
                if (_PendingNotifications.TryDequeue(250, out _Notification notification))
                {   //  Yes - dispatch
                    switch (notification.E)
                    {
                        case DatabaseObjectCreatedEventArgs e:
                            ObjectCreated?.Invoke(notification.Sender, e);
                            break;
                        case DatabaseObjectModifiedEventArgs e:
                            ObjectModified?.Invoke(notification.Sender, e);
                            break;
                        case DatabaseObjectDestroyedEventArgs e:
                            ObjectDestroyed?.Invoke(notification.Sender, e);
                            break;
                        default:
                            throw new NotImplementedException();
                    }
                }
            }
        }
    }
}
