//
//  Tt3.Db.API/PrivateTask.hpp
//  
//  Tt3.Db.API A private task in a database
//
//////////
BEGIN_TT3_NAMESPACE

//  A private task in a database
class TT3_DB_API_PUBLIC IPrivateTask : public virtual IPrivateActivity, public virtual ITask
{
    //////////
    //  Operations (properties)
public:

    //////////
    //  Operations (associations)
public:
    //  Returns/changes the immediate parent of this private task;
    //  NULL if this a root private task and has no parent.
    //  Throws "CDatabaseException *" if an error occurs.
    virtual IPrivateTask*   GetParent() const throws(CDatabaseException*) = 0;
    virtual void            SetParent(IPrivateTask* pParent) throws(CDatabaseException*) = 0;

    //  Returns an unordered list of all immediate children of
    //  this private task.
    //  Throws "CDatabaseException *" if an error occurs.
    virtual CPrivateTaskList GetChildren() const throws(CDatabaseException*) = 0;

    //////////
    //  Operations (life cycle)
public:
    //  Creates a new child of this private task with the specified properties.
    //  If the pActivityType is NULL, no association is initially made.
    //  Thrown a "CDatabaseException*" if an error occurs.
    virtual IPrivateTask* CreateChildPrivateTask(const CString& displayName, const CString& descrition,
                                                 const COptional<CTimeSpan>& timeout, bool requireCommentOnStart,
                                                 bool requireCommentOnFinish, bool fullScreenReminder,
                                                 IActivityType* pActivityType, bool completed,
                                                 bool requireCommentOnCompletion) throws(CDatabaseException*) = 0;
};

END_TT3_NAMESPACE
//  End of Tt3.Db.API/PrivateTask.hpp

