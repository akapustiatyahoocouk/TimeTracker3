//
//  Tt3.Db.API/Beneficiary.hpp
//  
//  Tt3.Db.API A beneficiary in a database
//
//////////
BEGIN_TT3_NAMESPACE

//  A beneficiary in a database - an agent (person or organization, 
//  whether internal or external) who benefits from a given workload; 
//  this will typically be the one billed for it.
class TT3_DB_API_PUBLIC IBeneficiary : public virtual IDatabaseObject
{
    //////////
    //  Operations (properties)
public:
    //  Gets/sets this beneficiary's "display name" (needs to be unique per database)
    //  Throws "CDatabaseException *" if an error occurs.
    virtual CString     GetDisplayName() const throws(CDatabaseException*) = 0;
    virtual void        SetDisplayName(const CString& displayName) throws(CDatabaseException*) = 0;

    //  Gets/sets this beneficiary's "description". Can be a multi-line
    //  string with '\n' - separated lines.
    //  Throws "CDatabaseException *" if an error occurs.
    virtual CString     GetDescription() const throws(CDatabaseException*) = 0;
    virtual void        SetDescription(const CString& description) throws(CDatabaseException*) = 0;

    //////////
    //  Operations (associations)
public:
    //  Returns the list of all workloads associated with this beneficiary.
    //  Throws "CDatabaseException *" if an error occurs.
    virtual CWorkloadList   GetWorkloads() const throws(CDatabaseException*) = 0;
};

END_TT3_NAMESPACE
//  End of Tt3.Db.API/Beneficiary.hpp
