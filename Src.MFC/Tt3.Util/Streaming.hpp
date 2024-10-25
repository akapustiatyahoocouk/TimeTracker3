//
//  Tt3.Util/Streaming.hpp
//  
//  Tt3 Streaming framework base
//
//////////
BEGIN_TT3_NAMESPACE

//  A "data source" is an agent that can provide data item(s) when asked to.
template <class T>
class TT3_UTIL_PUBLIC IDataSource
{
    //////////
    //  This is an interface
public:
    virtual ~IDataSource() = default;
};

//  A "data destination" is an agent to which data items can be sent.
template <class T>
class TT3_UTIL_PUBLIC IDataDestination
{
    //////////
    //  This is an interface
public:
    virtual ~IDataDestination() = default;

    //////////
    //  Operations
public:
    //  Sends an item to this destination; returns true on
    //  success, false on failure
    virtual bool        Send(const T& item) = 0;
};

END_TT3_NAMESPACE
//  End of Tt3.Util/Streaming.hpp
