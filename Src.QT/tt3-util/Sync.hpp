//
//  tt3-util/Sync.hpp - Synchronisation helpers
//
//////////

namespace tt3::util
{
    /// \class SynchronisationObject tt3-util/API.hpp
    /// \brief A generic object that can be used for thread synchronisation.
    class TT3_UTIL_PUBLIC SynchronisationObject
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(SynchronisationObject)

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     The class constructor.
        SynchronisationObject() = default;

        /// \brief
        ///     The class destructor.
        virtual ~SynchronisationObject() = default;

        //////////
        //  Operations
    public:
        /// \brief
        ///     "Grabs" this synchronization object, idle-waiting
        ///     until it is available for grabbing.
        virtual void    grab() = 0;

        /// \brief
        ///     "Grabs" this synchronization object, waiying
        ///     until it is available for grabbing OR the specified
        ///     timeout expires.
        /// \param timeoutMs
        ///     The timeout, in milliseconds, to want for.
        /// \return
        ///     True on grab success, false on timeout.
        virtual bool    tryGrab(int timeoutMs) = 0;

        /// \brief
        ///     "Releases" a grabbed synchronization object.
        virtual void    release() = 0;
    };

    /// \class Lock tt3-util/API.hpp
    /// \brief
    ///     A helper object that "grabs" a synchronisation object
    ///     in constructor and "released" it in destructor.
    class TT3_UTIL_PUBLIC Lock final
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Lock)

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     The class constructor; "grabs" the synchronization object.
        /// \param guard
        ///     The synchronization object to "grab".
        explicit Lock(SynchronisationObject & guard)
            :   _guard(guard) { _guard.grab(); }

        /// \brief
        ///     The class destructor; "releases" the synchronization
        ///     object specified to the lock constructor.
        ~Lock() { _guard.release(); }

        //////////
        //  Implementation
    private:
        SynchronisationObject & _guard;
    };

    /// \class Mutex tt3-util/API.hpp
    /// \brief An extended mutex - only one thread can
    ///        have it "grabbed"/"locked" at a time.
    class TT3_UTIL_PUBLIC Mutex final
        :   public SynchronisationObject
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Mutex)

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs an initially un-"grabbed" mutex.
        Mutex() = default;

        /// \brief
        ///     The class destructor.
        virtual ~Mutex() = default;

        //////////
        //  SynchronisationObject
    public:
        virtual void    grab() override { lock(); }
        virtual bool    tryGrab(int timeoutMs) override { return tryLock(timeoutMs); }
        virtual void    release() override { unlock(); }

        //////////
        //  Operations
    public:
        /// \brief
        ///     "Locks" this Mutex, idle-waiting until it is
        ///     available for locking.
        /// \details
        ///     Same as "grab()".
        ///     A thread is allowed to repeatedly "lock" the same
        ///     Mutex; later it must "unlock" that Mutex an equal
        ///     number of times before another thread can "lock" it.
        void            lock();

        /// \brief
        ///     "Locks" this Mutex, waiying until it is available for
        ///     locking OR the specified timeout expires.
        /// \details
        ///     Same as "tryGrab(timeoutMs)".
        ///     A thread is allowed to repeatedly "lock" the same
        ///     Mutex; later it must "unlock" that Mutex an equal
        ///     number of times before another thread can "lock" it.
        /// \param timeoutMs
        ///     The timeout, in milliseconds, to want for.
        /// \return
        ///     True on lock success, false on timeout.
        bool            tryLock(int timeoutMs);

        /// \brief
        ///     "Unlocks" a locked Mutex.
        /// \details
        ///     Same as "release()".
        void            unlock();

        /// \brief
        ///     Checks whether this Mutex is locked by
        ///     the specified thread.
        /// \param thread
        ///     The thread to check toe Mutex state for.
        /// \return
        ///     True if this Mutex is locked by the
        ///     specified thread, else false.
        bool            isLockedBy(QThread * thread);

        /// \brief
        ///     Checks whether this Mutex is locked by
        ///     the current thread.
        /// \return
        ///     True if this Mutex is locked by the
        ///     current thread, else false.
        bool            isLockedByCurrentThread();

        //////////
        //  Implementation
    private:
        QRecursiveMutex _impl = {};

        //  Extended mutex state
        QMutex          _extendedStateGuard = {};
        int             _lockCount = 0;
        QThread *       _lockingThread = nullptr;
    };

    /// \class BlockingQueue tt3-util/API.hpp
    /// \brief A "blocking inter-thread queue" ADT.
    template <class T>
    class BlockingQueue
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(BlockingQueue)

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs an empty queue.
        BlockingQueue()
            :   _data(),
                _dataSize(),
                _dataGuard() {}

        /// \brief
        ///     The class destructor.
        ~BlockingQueue() {}

        //////////
        //  Operations
    public:
        /// \brief
        ///     Adds value to the end of the queue.
        /// \param value
        ///     The value to add.
        void        enqueue(const T & value);

        /// \brief
        ///     Removes the value at the start of the queue and returns
        ///     it, idle-waiting until one is actually available.
        /// \return
        ///     The value removed from the head of the queue.
        T           dequeue();

        /// \brief
        ///     Attempts to remove and store the item at the start
        ///     of the queue.
        /// \param value
        ///     The placeholder to store the value to if available.
        ///     Remains unchanged if the call times out.
        /// \param timeoutMs
        ///     The timeout, in milliseconds, to wait for the queue
        ///     to provide a value before giving up.
        /// \return
        ///     True on success (storing the value), false on timeout.
        bool        tryDequeue(T & value, int timeoutMs);

        //////////
        //  Implementaion
    private:
        QQueue<T>   _data;
        QSemaphore  _dataSize;  //  value == _data.size()
        QMutex      _dataGuard;
    };

    template <class T>
    void BlockingQueue<T>::enqueue(const T & value)
    {
        QMutexLocker lock(&_dataGuard);
        _data.enqueue(value);
        _dataSize.release();
    }

    template <class T>
    T BlockingQueue<T>::dequeue()
    {
        _dataSize.acquire();
        QMutexLocker lock(&_dataGuard);
        return _data.dequeue();
    }

    template <class T>
    bool BlockingQueue<T>::tryDequeue(T & value, int timeoutMs)
    {
        if (_dataSize.tryAcquire(1, timeoutMs))
        {
            QMutexLocker lock(&_dataGuard);
            value = _data.dequeue();
            return true;
        }
        return false;
    }
}

//  End of tt3-util/Sync.hpp
