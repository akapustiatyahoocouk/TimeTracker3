//
//  tt3-util/BlockingQueue.hpp - A "blocking inter-thread queue" ADT
//
//////////

namespace tt3::util
{
    //////////
    //  A "blocking inter-thread queue" ADT
    template <class T>
    class BlockingQueue
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(BlockingQueue)

        //////////
        //  Construction/destruction
    public:
        BlockingQueue() : _data(), _dataSize(), _dataGuard() {}
        ~BlockingQueue() {}

        //////////
        //  Operations
    public:
        void                enqueue(const T & value);
        T                   dequeue();
        bool                tryDequeue(T & value, int timeoutMs);

        //////////
        //  Implementaion
    private:
        QQueue<T>           _data;
        QSemaphore          _dataSize;  //  value == _data.size()
        QMutex              _dataGuard;
    };

    //////////
    //  Operations
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

//  End of tt3-util/BlockingQueue.hpp
