using System;
using System.Diagnostics;

namespace TimeTracker3.Util
{
    /// <summary>
    ///     An object that grabs a monitor when constructed
    ///     and releases that monitor when disposed.
    /// </summary>
    public sealed class Lock : IDisposable
    {
        //////////
        //  Construction
        public Lock(Guard guard)
        {
            Debug.Assert(guard != null);

            _Guard = guard;
            _Guard.Acquire();
        }

        //////////
        //  IDisposable
        public void Dispose()
        {
            if (_Guard != null)
            {
                _Guard.Release();
                _Guard = null;
            }
        }

        //////////
        //  Implementation
        private volatile Guard _Guard;  //  null == lock already disposed
    }
}
