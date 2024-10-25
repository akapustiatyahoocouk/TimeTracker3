//
//  Tt3.Db.API/User.hpp
//  
//  Tt3.Db.API A user in a database
//
//////////
BEGIN_TT3_NAMESPACE

//  A user in a database
class TT3_DB_API_PUBLIC IUser : public virtual IPrincipal
{
    //////////
    //  Operations (properties)
public:
    //  Gets/sets this user's "real name" (needs not be unique per database)
    //  Throws "CDatabaseException *" if an error occurs.
    virtual CString     GetRealName() const throws(CDatabaseException *) = 0;
    virtual void        SetRealName(const CString & realName) throws(CDatabaseException *) = 0;

    //  Gets/sets the "inactivity timeout" of this user
    //  Throws "CDatabaseException *" if an error occurs.
    virtual COptional<CTimeSpan>    GetInactivityTimeout() const throws(CDatabaseException *) = 0;
    virtual void        SetInactivityTimeout(const COptional<CTimeSpan> & inactivityTimeout) throws(CDatabaseException *) = 0;

    //  Gets/sets the "UI locale" of this user (none == use system locale)
    //  Throws "CDatabaseException *" if an error occurs.
    virtual COptional<CLocale>  GetUiLocale() const throws(CDatabaseException *) = 0;
    virtual void        SetUiLocale(const COptional<CLocale> & uiLocale) throws(CDatabaseException *) = 0;

    //////////
    //  Operations (associations)
public:
    //  Returns the list of all accounts of this user
    //  Throws "CDatabaseException *" if an error occurs.
    virtual CAccountList    GetAccounts() const throws(CDatabaseException *) = 0;

    //  Returns the list of all private activities and tasks of this user.
    //  Throws "CDatabaseException *" if an error occurs.
    virtual CPrivateActivityList    GetPrivateActivitiesAndTasks() const throws(CDatabaseException*) = 0;

    //  Returns the list of all private activities (but not tasks) of this user.
    //  Throws "CDatabaseException *" if an error occurs.
    virtual CPrivateActivityList    GetPrivateActivities() const throws(CDatabaseException*) = 0;

    //  Returns the list of all private tasks of this user.
    //  Throws "CDatabaseException *" if an error occurs.
    virtual CPrivateTaskList    GetPrivateTasks() const throws(CDatabaseException*) = 0;

    //  Returns an unordered list of all works logged by this user.
    //  Throws "CDatabaseException *" if an error occurs.
    virtual CWorkList   GetWorks() const throws(CDatabaseException*) = 0;

    //  Returns an unordered list of all events logged by this user.
    //  Throws "CDatabaseException *" if an error occurs.
    virtual CEventList  GetEvents() const throws(CDatabaseException*) = 0;

    //  Returns an unordered list of all workloads permitted to this user.
    //  Throws "CDatabaseException *" if an error occurs.
    virtual CWorkloadList   GetPermittedWorkloads() const throws(CDatabaseException*) = 0;

    //////////
    //  Operations (life cycle)
public:
    //  Creates a new account for this user with the specified properties.
    //  Thrown a "CDatabaseException*" if an error occurs.
    virtual IAccount*       CreateAccount(bool enabled, const CEmailAddressList& emailAddresses,
                                          const CString& login, CString& password,
                                          IAccount::Capabilities capabilities) throws(CDatabaseException*) = 0;

    //  Creates a new private activity for this user with the specified properties.
    //  If the pActivityType is NULL, no association is initially made.
    //  Thrown a "CDatabaseException*" if an error occurs.
    virtual IPrivateActivity* CreatePrivateActivity(const CString& displayName, const CString& descrition,
                                                    const COptional<CTimeSpan>& timeout, bool requireCommentOnStart,
                                                    bool requireCommentOnFinish, bool fullScreenReminder,
                                                    IActivityType* pActivityType) throws(CDatabaseException*) = 0;

    //  Creates a new root (with no parent) private task for this user with the specified properties.
    //  If the pActivityType is NULL, no association is initially made.
    //  Thrown a "CDatabaseException*" if an error occurs.
    virtual IPrivateTask* CreateRootPrivateTask(const CString& displayName, const CString& descrition,
                                                const COptional<CTimeSpan>& timeout, bool requireCommentOnStart,
                                                bool requireCommentOnFinish, bool fullScreenReminder,
                                                IActivityType* pActivityType, bool completed,
                                                bool requireCommentOnCompletion) throws(CDatabaseException*) = 0;

    //  Creates a new event with the specified properties for this user.
    //  If the pActivity is NULL, the newly created event is not associated
    //  with any activity.
    //  Thrown a "CDatabaseException*" if an error occurs.
    virtual IEvent*     CreateEvent(const CTime& occurredAt, const CString& summary,
                                    IActivity* pActivity) throws(CDatabaseException*) = 0;

    //  Creates a new work with the specified properties for this user.
    //  The pActivity must not be NULL.
    //  Thrown a "CDatabaseException*" if an error occurs.
    virtual IWork*      CreateWork(const CTime& startedAt, const CTime& finishedAt,
                                   IActivity* pActivity) throws(CDatabaseException*) = 0;
};

END_TT3_NAMESPACE
//  End of Tt3.Db.API/User.hpp
