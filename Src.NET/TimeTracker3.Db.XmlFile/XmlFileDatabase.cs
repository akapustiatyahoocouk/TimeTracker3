using System;
using System.Diagnostics;
using TimeTracker3.Db.API;
using TimeTracker3.Db.API.Exceptions;

namespace TimeTracker3.Db.XmlFile
{
    /// <summary>
    ///     The single-user database stored as an XML file.
    /// </summary>
    public sealed class XmlFileDatabase : IDatabase
    {
        //////////
        //  Construction
        public XmlFileDatabase(XmlFileDatabaseAddress address)
        {
            Debug.Assert(address != null);

            _Address = address;
            //  We need to grab a lock first
            _Lock = new _XmlFileDatabaseLock(address);
        }

        //////////
        //  Properties
        public IDatabaseType Type => XmlFileDatabaseType.Instance;
        public IDatabaseAddress Address => _Address;

        public bool IsOpen
        {
            get
            {
                lock (_Guard)
                {
                    return _Lock != null && _Lock._IsOpen;
                }
            }
        }

        //////////
        //  Operations - general
        public void Close()
        {
            lock (_Guard)
            {
                if (_Lock != null)
                {   //  Still "open" - save...
                    Exception ex = null;
                    try
                    {
                        //  TODO
                    }
                    catch (Exception exx)
                    {
                        ex = exx;
                    }
                    //  ...release the lock...
                    _Lock._Close();
                    //  ...then mark all objects as "dead" and
                    //  release references to them
                    //  TODO
                    //  ...and we're closed
                    _Lock = null;
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
        //  Events
        public event EventHandler<DatabaseObjectCreatedEventArgs> ObjectCreated;
        public event EventHandler<DatabaseObjectModifiedEventArgs> ObjectModified;
        public event EventHandler<DatabaseObjectDestroyedEventArgs> ObjectDestroyed;

        //////////
        //  Implementation
        private readonly XmlFileDatabaseAddress _Address;
        private _XmlFileDatabaseLock _Lock; //  null == database is closed
        private readonly object _Guard = new object();  //  for all access synchronization
    }
}
