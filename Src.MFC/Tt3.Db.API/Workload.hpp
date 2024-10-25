//
//  Tt3.Db.API/Workload.hpp
//  
//  Tt3.Db.API A generic workload in a database
//
//////////
BEGIN_TT3_NAMESPACE

//  A generic workload in a database
class TT3_DB_API_PUBLIC IWorkload : public virtual IDatabaseObject
{
    //////////
    //  Operations (properties)
public:
    //  Gets/sets this workload's "display name" (needs to be unique per database)
    //  Throws "CDatabaseException *" if an error occurs.
    virtual CString     GetDisplayName() const throws(CDatabaseException*) = 0;
    virtual void        SetDisplayName(const CString& displayName) throws(CDatabaseException*) = 0;

    //  Gets/sets this workload's "description". Can be a multi-line
    //  string with '\n' - separated lines.
    //  Throws "CDatabaseException *" if an error occurs.
    virtual CString     GetDescription() const throws(CDatabaseException*) = 0;
    virtual void        SetDescription(const CString& description) throws(CDatabaseException*) = 0;

    //////////
    //  Operations (associations)
public:
    //  Returns the list of all beneficiaries associated with this workload.
    //  Throws "CDatabaseException *" if an error occurs.
    virtual CBeneficiaryList    GetBeneficiaries() const throws(CDatabaseException*) = 0;
};

END_TT3_NAMESPACE
//  End of Tt3.Db.API/Beneficiary.hpp

