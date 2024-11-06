using System.Diagnostics;
using System.Threading;

namespace TimeTracker3.Util
{
    /// <summary>
    ///     A synchronization object that can be
    ///     used with Lock for automated release.
    /// </summary>
    public sealed class Guard
    {
        //////////
        //  Properties

        /// <summary>
        ///     True if this Guard is currently "acquired"
        ///     by the current thread, else false.
        /// </summary>
        public bool IsLockedByCurrentThread
        {
            get
            {
                lock (_Sync)
                {
                    return _AcquireCount > 0 && _OwnerThread == Thread.CurrentThread;
                }
            }
        }

        //////////
        //  Operations

        /// <summary>
        ///     "Acquires" this Guard once on behalf
        ///     of the current thread.
        /// </summary>
        public void Acquire()
        {
            lock (_Sync)
            {
                Monitor.Enter(_Impl);
                _OwnerThread = Thread.CurrentThread;
                Interlocked.Increment(ref _AcquireCount);
            }
        }

        /// <summary>
        ///     "Releases" this Guard once on behalf
        ///     of the current thread.
        /// </summary>
        public void Release()
        {
            lock (_Sync)
            {
                Debug.Assert(_AcquireCount > 0);
                Debug.Assert(_OwnerThread == Thread.CurrentThread);

                Monitor.Exit(_Impl);
                if (Interlocked.Decrement(ref _AcquireCount) == 0)
                {
                    _OwnerThread = null;
                }
            }
        }

        //////////
        //  Implementation
        private readonly object _Sync = new object();
        private readonly object _Impl = new object();
        private volatile Thread _OwnerThread /* = null */;
        private volatile int _AcquireCount /*= 0*/;
    }
}
