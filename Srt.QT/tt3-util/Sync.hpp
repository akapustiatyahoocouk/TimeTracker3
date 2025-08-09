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
        //  TODO document
        virtual void        grab() = 0;
        virtual bool        tryGrab(int timeoutMs) = 0;
        virtual void        release() = 0;
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
        virtual void        grab() override { lock(); }
        virtual bool        tryGrab(int timeoutMs) override { return tryLock(timeoutMs); }
        virtual void        release() override { unlock(); }

        //////////
        //  Operations
    public:
        //  TODO document
        void                lock();
        bool                tryLock(int timeoutMs);
        void                unlock();

        bool                isLockedBy(QThread * thread);
        bool                isLockedByCurrentThread();

        //////////
        //  Implementation
    private:
        QRecursiveMutex     _impl = {};

        //  Extended mutex state
        QMutex              _extendedStateGuard = {};
        int                 _lockCount = 0;
        QThread *           _lockingThread = nullptr;
    };
}

//  End of tt3-util/Sync.hpp
