using System;
using System.Diagnostics;
using System.IO;
using System.Threading;
using TimeTracker3.Db.API.Exceptions;

namespace TimeTracker3.Db.XmlFile
{
    /// <summary>
    ///     The "lock" used to restrict access to an "XML
    ///     file" database to a single user.
    /// </summary>
    internal class _XmlFileDatabaseLock
    {
        //////////
        //  Construction

        /// <summary>
        ///     Creates/opens an "XML file" database.
        /// </summary>
        /// <param name="address">
        ///     The database address.
        /// </param>
        /// <exception cref="DatabaseException">
        ///     If an error occurs.
        /// </exception>
        internal _XmlFileDatabaseLock(XmlFileDatabaseAddress address)
        {
            Debug.Assert(address != null);

            _LockDirectoryName = address._FileName + ".lock";
            //  Does the lock directory already exist ?
            if (Directory.Exists(_LockDirectoryName))
            {   //  Yes - check its timestamp...
                DateTime lockUtc = Directory.GetLastWriteTimeUtc(_LockDirectoryName);
                DateTime nowUtc = DateTime.UtcNow;
                if ((nowUtc - lockUtc).TotalMilliseconds > _LockTimeoutMs)
                {   //  ...and the lock is stale - take it over
                }
                else
                {   //  ...and the lock is live - give up
                    throw new DatabaseInUseDatabaseException(address);
                }
            }
            else if (File.Exists(_LockDirectoryName))
            {   //  OOPS! A file with the same name is already there
                throw new AlreadyExistsDatabaseException("File", "path", _LockDirectoryName);
            }
            else
            {   //  No - create one now atomically
                try
                {
                    Directory.CreateDirectory(_LockDirectoryName);
                }
                catch (Exception ex)
                {   //  OOPS! Translate & re-throw
                    throw new DatabaseException(ex);
                }
            }
            //  Start lock refresh thread
            _RefreshThread = new Thread(_RefreshThreadProc);
        }

        //////////
        //  Properties
        internal bool _IsOpen => _RefreshThread != null;

        //////////
        //  Operations
        internal void _Close()
        {
            Thread refreshThread = _RefreshThread;
            _RefreshThreadStopRequested = true;
            if (refreshThread != null)
            {
                refreshThread.Join();
            }
        }

        //////////
        //  Implementation
        private readonly string _LockDirectoryName;
        private const int _LockRefreshIntervalMs = 5000;
        private const int _LockTimeoutMs = 20000;

        //  Threads
        private void _RefreshThreadProc()
        {
            DateTime lastRefreshedAtUtc = DateTime.UtcNow;
            while (!_RefreshThreadStopRequested)
            {
                if ((DateTime.UtcNow - lastRefreshedAtUtc).TotalMilliseconds < _LockRefreshIntervalMs)
                {   //  It's not yet time to refresh the lock timestamp
                    Thread.Sleep(_RefreshThreadSleepChunlMs);
                    continue;
                }
                //  Must refresh the lock
                try
                {
                    Directory.SetLastWriteTimeUtc(_LockDirectoryName, DateTime.UtcNow);
                }
                catch (Exception)
                {   //  OOPS! Could not refresh the lock directory
                    //  timestamp - abort the lock
                    break;
                }
            }
            try
            {
                Directory.Delete(_LockDirectoryName);
            }
            catch (Exception)
            {   //  OOPS! Suppress, though
            }
            _RefreshThread = null;
        }
        private volatile Thread _RefreshThread;  //  null == the lock is "closed"
        private volatile bool _RefreshThreadStopRequested /* = false*/ ;
        private const int _RefreshThreadSleepChunlMs = 250;
    }
}
