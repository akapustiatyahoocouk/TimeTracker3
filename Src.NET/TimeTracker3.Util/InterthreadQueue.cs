using System;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Threading;

namespace TimeTracker3.Util
{
    /// <summary>
    ///     A multiple producer/multiple consumer queue
    ///     with idle wait on the consumer side.
    /// </summary>
    /// <typeparam name="T">
    ///     tHE TYPE OF THE QUEUE ELEMENTS.
    /// </typeparam>
    public class InterthreadQueue<T> : ICollection, IReadOnlyCollection<T>
    {
        //////////
        //  IEnumerable<T>
        public IEnumerator<T> GetEnumerator()
        {
            lock (_Guard)
            {
                return _Queue.ToArray().Cast<T>().GetEnumerator();
            }
        }

        //////////
        //  IEnumerable
        IEnumerator IEnumerable.GetEnumerator()
        {
            lock (_Guard)
            {
                return _Queue.ToArray().GetEnumerator();
            }
        }

        //////////
        //  ICollection
        public void CopyTo(Array array, int index)
        {
            lock (_Guard)
            {
                _Queue.CopyTo((T[])array, index);
            }
        }

        public int Count
        {
            get
            {
                lock (_Guard)
                {
                    return _Queue.Count;
                }
            }
        }


        public object SyncRoot => _Guard;

        public bool IsSynchronized => true;

        //////////
        //  IReadOnlyCollection<T>

        //////////
        //  Properties

        /// <summary>
        ///     Enqueues the specified element at the end of
        ///     this queue.
        /// </summary>
        /// <param name="element">
        ///     The element to enqueue.
        /// </param>
        public void Enqueue(T element)
        {
            lock (_Guard)
            {
                _Queue.Enqueue(element);
                _QueueSize.Release();
            }
        }

        /// <summary>
        ///     Attempts to dequeue a single element at the
        ///     front of this queue.
        /// </summary>
        /// <param name="timeoutMs">
        ///     The timeout, in milliseconds, to wait for an
        ///     element at the head of the queue to become available.
        /// </param>
        /// <param name="element">
        ///     The dequeued element (on success), undefined
        ///     on failure to dequeue.
        /// </param>
        /// <returns>
        ///     True if an element was dequeued and stored
        ///     successfully, else false.
        /// </returns>
        public bool TryDequeue(int timeoutMs, out T element)
        {
            Debug.Assert(timeoutMs >= 0);

            if (_QueueSize.Wait(timeoutMs))
            {   //  There was at least 1 element in the _Queue
                lock (_Guard)
                {
                    element = _Queue.Dequeue();
                    return true;
                }
            }
            element = default(T);
            return false;
        }

        //////////
        //  Implementation
        private readonly object _Guard = new object();
        private readonly Queue<T> _Queue = new Queue<T>();
        private readonly SemaphoreSlim _QueueSize = new SemaphoreSlim(0);
    }
}
