//
//  Tt3.Db.API/Task.hpp
//  
//  Tt3.Db.API A generic task in a database
//
//////////
BEGIN_TT3_NAMESPACE

//  A generic task in a database
class TT3_DB_API_PUBLIC ITask : public virtual IActivity
{
    //////////
    //  Operations (properties)
public:
    //  Gets/sets the completion status of this task.
    //  Throws "CDatabaseException *" if an error occurs.
    virtual bool        IsCompleted() const throws(CDatabaseException*) = 0;
    virtual void        SetCompleted(bool completed) throws(CDatabaseException*) = 0;

    //  Gets/sets the flag indicating whether the user must make a comment
    //  when completing this task.
    //  Throws "CDatabaseException *" if an error occurs.
    virtual bool        GetRequireCommentOnCompletion() const throws(CDatabaseException*) = 0;
    virtual void        SetRequireCommentOnCompletion(bool requireCommentOnCompletion) throws(CDatabaseException*) = 0;
};

END_TT3_NAMESPACE
//  End of Tt3.Db.API/Task.hpp
