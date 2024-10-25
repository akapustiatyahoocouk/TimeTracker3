//
//  Tt3.Db.API/Database.hpp
//  
//  Tt3.Db.API "database" ADT
//
//////////
BEGIN_TT3_NAMESPACE

//  A connection to a persistent data store
class TT3_DB_API_PUBLIC CDatabase
{
    CANNOT_ASSIGN_OR_COPY_CONSTRUCT(CDatabase)

    //////////
    //  Construction/destruction
public:
    CDatabase() = default;
    virtual ~CDatabase() = default;

    //////////
    //  Operations (general)
public:
    //  Returns the type of this database
    virtual CDatabaseType *     GetType() const = 0;

    //  Returns the address of this database
    virtual CDatabaseAddress *  GetAddress() const = 0;

    //  Checks if this database [connection] is currently open.
    virtual bool                IsOpen() const = 0;

    //  Closes this database [connection]; has no effect if already closed.
    //  Thrown a "CDatabaseException*" if an error occurs.
    //  On error, the database is marked "closed" before throwing.
    virtual void                Close() throws(CDatabaseException*) = 0;

    //////////
    //  Operations (life cycle)
public:
    //  Creates a new user with the specified properties.
    //  Thrown a "CDatabaseException*" if an error occurs.
    virtual IUser*              CreateUser(bool enabled, const CEmailAddressList& emailAddresses,
                                           const CString& realName, const COptional<CTimeSpan>& inactivityTimeout,
                                           const CLocale& uiLocale) throws(CDatabaseException*) = 0;

    //  Creates a new activity type with the specified properties.
    //  Thrown a "CDatabaseException*" if an error occurs.
    virtual IActivityType*      CreateActivityType(const CString& displayName, const CString& description) throws(CDatabaseException*) = 0;

    //  Creates a new public activity with the specified properties.
    //  If the pActivityType is NULL, no association is initially made.
    //  Thrown a "CDatabaseException*" if an error occurs.
    virtual IPublicActivity*    CreatePublicActivity(const CString &displayName, const CString& descrition,
                                                     const COptional<CTimeSpan>& timeout, bool requireCommentOnStart,
                                                     bool requireCommentOnFinish, bool fullScreenReminder,
                                                     IActivityType* pActivityType) throws(CDatabaseException*) = 0;

    //  Creates a new root (with no parent) public task with the specified properties.
    //  If the pActivityType is NULL, no association is initially made.
    //  Thrown a "CDatabaseException*" if an error occurs.
    virtual IPublicTask*        CreateRootPublicTask(const CString& displayName, const CString& descrition,
                                                     const COptional<CTimeSpan>& timeout, bool requireCommentOnStart,
                                                     bool requireCommentOnFinish, bool fullScreenReminder,
                                                     IActivityType* pActivityType, bool completed,
                                                     bool requireCommentOnCompletion) throws(CDatabaseException*) = 0;

    //  Creates a new beneficiary with the specified properties.
    //  Thrown a "CDatabaseException*" if an error occurs.
    virtual IBeneficiary*       CreateBeneficiary(const CString& displayName, const CString& description) throws(CDatabaseException*) = 0;

    //  Creates a new root (with no parent) project with the specified properties.
    //  Thrown a "CDatabaseException*" if an error occurs.
    virtual IProject*           CreateRootProject(const CString& displayName, const CString& descrition,
                                bool completed) throws(CDatabaseException*) = 0;

    //  Creates a new work stream with the specified properties.
    //  Thrown a "CDatabaseException*" if an error occurs.
    virtual IWorkStream*        CreateWorkStream(const CString& displayName, const CString& descrition) throws(CDatabaseException*) = 0;
};

END_TT3_NAMESPACE
//  End of Tt3.Db.API/Database.hpp
