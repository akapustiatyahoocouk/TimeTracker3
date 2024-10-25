//
//  Tt3.Db.API/ActivityType.hpp
//  
//  Tt3.Db.API An activity type in a database
//
//////////
BEGIN_TT3_NAMESPACE

//  An activity type in a database - the activity category (e.g. "R&R",
//  "engineering", "umbrella activities", etc.)
class TT3_DB_API_PUBLIC IActivityType : public virtual IDatabaseObject
{
    //////////
    //  Operations (properties)
public:
    //  Gets/sets this activity type's "display name" (needs to be unique per database)
    //  Throws "CDatabaseException *" if an error occurs.
    virtual CString     GetDisplayName() const throws(CDatabaseException*) = 0;
    virtual void        SetDisplayName(const CString& displayName) throws(CDatabaseException*) = 0;

    //  Gets/sets this activity type's "description". Can be a multi-line
    //  string with '\n' - separated lines.
    //  Throws "CDatabaseException *" if an error occurs.
    virtual CString     GetDescription() const throws(CDatabaseException*) = 0;
    virtual void        SetDescription(const CString& description) throws(CDatabaseException*) = 0;

    //////////
    //  Operations (associations)
public:
    //  Returns the list of all activities and tasks (public and private) of this type.
    //  Throws "CDatabaseException *" if an error occurs.
    virtual CActivityList   GetAllActivitiesAndTasks() const throws(CDatabaseException*) = 0;

    //  Returns the list of all public activities and tasks of this type.
    //  Throws "CDatabaseException *" if an error occurs.
    virtual CPublicActivityList     GetPublicActivitiesAndTasks() const throws(CDatabaseException*) = 0;

    //  Returns the list of all private activities and tasks of this type.
    //  Throws "CDatabaseException *" if an error occurs.
    virtual CPrivateActivityList    GetPrivateActivitiesAndTasks() const throws(CDatabaseException*) = 0;

    //  Returns the list of all activities (but not tasks) (public and private) of this type.
    //  Throws "CDatabaseException *" if an error occurs.
    virtual CActivityList   GetAllActivities() const throws(CDatabaseException*) = 0;

    //  Returns the list of all public activities (but not tasks) of this type.
    //  Throws "CDatabaseException *" if an error occurs.
    virtual CPublicActivityList     GetPublicActivities() const throws(CDatabaseException*) = 0;

    //  Returns the list of all private activities (but not tasks) of this type.
    //  Throws "CDatabaseException *" if an error occurs.
    virtual CPrivateActivityList    GetPrivateActivities() const throws(CDatabaseException*) = 0;

    //  Returns the list of all tasks (public and private) of this type.
    //  Throws "CDatabaseException *" if an error occurs.
    virtual CTaskList       GetAllTasks() const throws(CDatabaseException*) = 0;

    //  Returns the list of all public tasks of this type.
    //  Throws "CDatabaseException *" if an error occurs.
    virtual CPublicTaskList     GetPublicTasks() const throws(CDatabaseException*) = 0;

    //  Returns the list of all private tasks of this type.
    //  Throws "CDatabaseException *" if an error occurs.
    virtual CPrivateTaskList    GetPrivateTasks() const throws(CDatabaseException*) = 0;
};

END_TT3_NAMESPACE
//  End of Tt3.Db.API/ActivityType.hpp
