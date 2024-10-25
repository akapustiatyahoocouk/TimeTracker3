//
//  Tt3.Db.API/Account.hpp
//  
//  Tt3.Db.API An account in a database
//
//////////
BEGIN_TT3_NAMESPACE

//  An account is a role that a person can play in TimeTracker.
class TT3_DB_API_PUBLIC IAccount : public virtual IPrincipal
{
    //////////
    //  Types
public:
    enum class Capabilities
    {
        //  Individual capabilities
        Administrator = 0x0001,
        ManageUsers = 0x0002,
        ManageBeneficiaries = 0x0004,
        ManageWorkloads = 0x0008,
        ManagePublicActivities = 0x0010,
        ManagePublicTasks = 0x0020,
        ManagePrivateActivities = 0x0040,
        ManagePrivateTasks = 0x0080,
        LogWork = 0x0100,
        LogEvents = 0x0200,
        GenerateReports = 0x0400,
        BackupAndRestore = 0x0800,

        //  Capability sets
        None = 0x0000,
        All = 0x0FFF
    };

    //////////
    //  Operations (properties)
public:
    //  Gets/sets this account's login identifier (needs to be unique per database)
    //  Throws "CDatabaseException *" if an error occurs.
    virtual CString     GetLogin() const throws(CDatabaseException *) = 0;
    virtual void        SetLogin(const CString & login) throws(CDatabaseException *) = 0;

    //  Gets/sets the password [hash] of this account.
    //  The "password hash" if an uppercase hex string representing the
    //  SHA-1 hash of the password.
    //  Throws "CDatabaseException *" if an error occurs.
    virtual CString     GetPasswordHash() const throws(CDatabaseException *) = 0;
    virtual void        SetPassword(const CString & password) throws(CDatabaseException *) = 0;

    //  Gets/sets the capabilities of this account
    //  Throws "CDatabaseException *" if an error occurs.
    virtual Capabilities    GetCapabilities() const throws(CDatabaseException *) = 0;
    virtual void        SetCapabilities(Capabilities capabilities) throws(CDatabaseException *) = 0;

    //////////
    //  Operations (associations)
public:
    //  Returns the user to which this account belongs.
    //  Throws "CDatabaseException *" if an error occurs.
    virtual IUser *     GetUser() const throws(CDatabaseException *) = 0;
};

END_TT3_NAMESPACE
//  End of Tt3.Db.API/Account.hpp
