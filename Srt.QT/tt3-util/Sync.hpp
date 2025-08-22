//
//  tt3-util/Sync.hpp - Synchronisation helpers
//
//////////

namespace tt3::util
{
    //  A generic object that can be used for inter-thread synchronisation
    class TT3_UTIL_PUBLIC SynchronisationObject
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(SynchronisationObject)

        //////////
        //  Construction/destruction
    public:
        SynchronisationObject() = default;
        virtual ~SynchronisationObject() = default;

        //////////
        //  Operations
    public:
        //  "Grabs" this synchronization object, waiying
        //  until it is available for grabbing.
        virtual void    grab() = 0;

        //  "Grabs" this synchronization object, waiying
        //  until it is available for grabbing OR the specified
        //  timeout expires.
        //  Returns true on grab success, false on timeout.
        virtual bool    tryGrab(int timeoutMs) = 0;

        //  "Releases" a grabbed synchronization objecr.
        virtual void    release() = 0;
    };

    //  A "lock" that "grabs" a synchronisation object in constructor and
    //  "released" it in destructor
    class TT3_UTIL_PUBLIC Lock final
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Lock)

        //////////
        //  Construction/destruction
    public:
        explicit Lock(SynchronisationObject & guard) : _guard(guard) { _guard.grab(); }
        ~Lock() { _guard.release(); }

        //////////
        //  Implementation
    private:
        SynchronisationObject & _guard;
    };

    //  An extended mutex - only one thread can have it "grabbed"/"locked" at a time
    class TT3_UTIL_PUBLIC Mutex : public SynchronisationObject
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Mutex)

        //////////
        //  Construction/destruction
    public:
        Mutex() = default;
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
        //  Same as "grab()".
        void            lock();

        //  Same as "tryGrab(timeoutMs)".
        bool            tryLock(int timeoutMs);

        //  Same as "release()".
        void            unlock();

        //  Checks whether this Mutex is locked by the specified thread.
        bool            isLockedBy(QThread * thread);

        //  Checks whether this Mutex is locked by the current thread.
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
}

//  End of tt3-util/Sync.hpp
