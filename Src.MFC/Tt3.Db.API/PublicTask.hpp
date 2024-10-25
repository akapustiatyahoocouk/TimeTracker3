//
//  Tt3.Db.API/PublicTask.hpp
//  
//  Tt3.Db.API A public task in a database
//
//////////
BEGIN_TT3_NAMESPACE

//  A public task in a database
class TT3_DB_API_PUBLIC IPublicTask : public virtual IPublicActivity, public virtual ITask
{
    //////////
    //  Operations (properties)
public:

    //////////
    //  Operations (associations)
public:
    //  Returns/changes the immediate parent of this public task;
    //  NULL if this a root public task and has no parent.
    //  Throws "CDatabaseException *" if an error occurs.
    virtual IPublicTask*    GetParent() const throws(CDatabaseException*) = 0;
    virtual void            SetParent(IPublicTask* pParent) throws(CDatabaseException*) = 0;

    //  Returns an unordered list of all immediate children of
    //  this public task.
    //  Throws "CDatabaseException *" if an error occurs.
    virtual CPublicTaskList GetChildren() const throws(CDatabaseException*) = 0;

    //////////
    //  Operations (life cycle)
public:
    //  Creates a new child of this public task with the specified properties.
    //  If the pActivityType is NULL, no association is initially made.
    //  Thrown a "CDatabaseException*" if an error occurs.
    virtual IPublicTask*    CreateChildPublicTask(const CString& displayName, const CString& descrition,
                                                  const COptional<CTimeSpan>& timeout, bool requireCommentOnStart,
                                                  bool requireCommentOnFinish, bool fullScreenReminder,
                                                  IActivityType* pActivityType, bool completed,
                                                  bool requireCommentOnCompletion) throws(CDatabaseException*) = 0;
};

END_TT3_NAMESPACE
//  End of Tt3.Db.API/PublicTask.hpp
