//
//  Tt3.Db.API/Activity.hpp
//  
//  Tt3.Db.API A generic activity in a database
//
//////////
BEGIN_TT3_NAMESPACE

//  A generic activity in a database - a particular activity that 
//  a user can spend time doing
class TT3_DB_API_PUBLIC IActivity : public virtual IDatabaseObject
{
    //////////
    //  Operations (properties)
public:
    //  Gets/sets this activity's "display name" (needs to be unique per database)
    //  Throws "CDatabaseException *" if an error occurs.
    virtual CString     GetDisplayName() const throws(CDatabaseException*) = 0;
    virtual void        SetDisplayName(const CString& displayName) throws(CDatabaseException*) = 0;

    //  Gets/sets this activity's "description". Can be a multi-line
    //  string with '\n' - separated lines.
    //  Throws "CDatabaseException *" if an error occurs.
    virtual CString     GetDescription() const throws(CDatabaseException*) = 0;
    virtual void        SetDescription(const CString& description) throws(CDatabaseException*) = 0;

    //  Gets/sets this activity's "timeout" - an interval after which,
    //  if no user activity occurs, the activity is "stopped automatically.
    //  An absent timeout meant activity has no own timeout.
    //  Throws "CDatabaseException *" if an error occurs.
    virtual COptional<CTimeSpan>    GetTimeout() const throws(CDatabaseException*) = 0;
    virtual void        SetTimeout(const COptional<CTimeSpan>& timeout) throws(CDatabaseException*) = 0;

    //  Gets/sets the flag indicating whether the user must make a comment
    //  when starting this activity.
    //  Throws "CDatabaseException *" if an error occurs.
    virtual bool        GetRequireCommentOnStart() const throws(CDatabaseException*) = 0;
    virtual void        SetRequireCommentOnStart(bool requireCommentOnStart) throws(CDatabaseException*) = 0;

    //  Gets/sets the flag indicating whether a full-screen reminder is
    //  displayed when this activity is underway. This is to ensure that
    //  when an AFK activity is underway (lunch break, meeting, etc.) the
    //  user doesn't forget to "finish it" when back at the keyboard.
    //  Throws "CDatabaseException *" if an error occurs.
    virtual bool        GetFullScreenReminder() const throws(CDatabaseException*) = 0;
    virtual void        SetFullScreenReminder(bool fullScreenReminder) throws(CDatabaseException*) = 0;

    //////////
    //  Operations (associations)
public:
    //  Returns/sets the type of this activity; NULL if none.
    //  Throws "CDatabaseException *" if an error occurs.
    virtual IActivityType*  GetActivityTypeType() const throws(CDatabaseException*) = 0;
    virtual void            SetActivityTypeType(IActivityType* activityType) throws(CDatabaseException*) = 0;

    //  Returns an unordered list of all works logged against this activity.
    //  Throws "CDatabaseException *" if an error occurs.
    virtual CWorkList   GetWorks() const throws(CDatabaseException*) = 0;

    //  Returns an unordered list of all events logged against this activity.
    //  Throws "CDatabaseException *" if an error occurs.
    virtual CEventList  GetEvents() const throws(CDatabaseException*) = 0;

    //  Gets/sets the workload to which this activity contributes; NULL if none.
    //  Throws "CDatabaseException *" if an error occurs.
    virtual IWorkload*  GetWorkload() const throws(CDatabaseException*) = 0;
    virtual void        SetWorkload(IWorkload* pWorkload) throws(CDatabaseException*) = 0;
};

END_TT3_NAMESPACE
//  End of Tt3.Db.API/Activity.hpp
