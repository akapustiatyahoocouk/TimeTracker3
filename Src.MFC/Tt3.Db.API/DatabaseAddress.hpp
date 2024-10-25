//
//  Tt3.Db.API/DatabaseAddress.hpp
//  
//  Tt3.Db.API "database address" ADT
//
//////////
BEGIN_TT3_NAMESPACE

//  A "database address" is a specification of where a given database
//  is located. Database addresses are database type - specific.
//  Instances of the concrete CDatabaseAddress-derived classes are
//  managed by their corresponding database type. The database type
//  implementation will ensure that two CDatabaseAddress instances 
//  never refer to the same database.
class TT3_DB_API_PUBLIC CDatabaseAddress
{
    CANNOT_ASSIGN_OR_COPY_CONSTRUCT(CDatabaseAddress)

    //////////
    //  Construction/destruction
public:
    CDatabaseAddress() = default;
    virtual ~CDatabaseAddress() = default;

    //////////
    //  Operations (general)
public:
    //  Returns the database type to which this database address belongs.
    virtual CDatabaseType * GetDatabaseType() const = 0;

    //  Returns a user-readable form of this database address.
    virtual CString         GetDisplayForm() const = 0;

    //  Returns an external (re-parsable) form of this database address.
    virtual CString         GetExternalForm() const = 0;
};

END_TT3_NAMESPACE
//  End of Tt3.Db.API/DatabaseAddress.hpp
