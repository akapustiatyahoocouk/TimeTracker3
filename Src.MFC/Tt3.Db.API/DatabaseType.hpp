//
//  Tt3.Db.API/DatabaseType.hpp
//  
//  Tt3.Db.API "database type" ADT
//
//////////
BEGIN_TT3_NAMESPACE

//  A "database type" represents a storage technology that can 
//  be used to keep the data persistent.
//  Concrete derived subclasses will normally be singletons
//  registered by plugins during their intiialization.
class TT3_DB_API_PUBLIC CDatabaseType
{
    CANNOT_ASSIGN_OR_COPY_CONSTRUCT(CDatabaseType)

    //////////
    //  Construction/destruction
public:
    CDatabaseType() = default;
    virtual ~CDatabaseType() = default;

    //////////
    //  Operations (general)
public:
    //  Returns a mnemonic identifier of this database type.
    virtual CString     GetMnemonic() const = 0;

    //  Returns a user-readable display name of this database type.
    virtual CString     GetDisplayName() const = 0;

    //  Returns true if this database type is operational (i.e. can
    //  be used), false if not.
    virtual bool        IsOperational() const = 0;

    //  Returns a status report for this database type (i.e. 
    //  "operational", "not operational and why", etc.)
    //  The result can contain multiple '\n' - separated lines.
    virtual CString     GetStatusReport() const = 0;

    //////////
    //  Operations (address handling)
public:
    //  Prompts the user to interactively specify an address for a new
    //  database of this type, using "pParentWnd" as a parent for any
    //  modal dialogs that may be presented during address entry.
    //  Returns the entered address or NULL if the user has chosen
    //  to cancel the database address entry.
    virtual CDatabaseAddress * EnterNewDatabaseAddress(CWnd * pParentWnd) = 0;

    //  Prompts the user to interactively specify an address of an existing
    //  database of this type, using "pParentWnd" as a parent for any
    //  modal dialogs that may be presented during address entry.
    //  Returns the entered address or NULL if the user has chosen
    //  to cancel the database address entry.
    virtual CDatabaseAddress * EnterExistingDatabaseAddress(CWnd * pParentWnd) = 0;

    //  If this database type has a concept of a "default" database, returns
    //  its address; else returns NULL.
    virtual CDatabaseAddress * GetDefaultDatabaseAddress() = 0;

    //  Parses an external form of a database address of this type and returns it.
    //  Throws "CDatabaseException *" if the address parsing fails (e.g. invalid 
    //  external form syntax).
    virtual CDatabaseAddress * ParseDatabaseAddress(const CString & externalForm) throws(CDatabaseException *) = 0;

    //////////
    //  Operations (database)
public:
    //  Creates (and opens) a new, initially empty, database at
    //  the specified address.
    //  Throws "CDatabaseException *" if an error occurs.
    virtual CDatabase * CreateDatabase(CDatabaseAddress * pAddress) throws(CDatabaseException *) = 0;

    //  Opens an existing database at the specified address.
    //  Throws "CDatabaseException *" if an error occurs.
    virtual CDatabase * OpenDatabase(CDatabaseAddress * pAddress) throws(CDatabaseException *) = 0;

    //  Destroys an existing database at the specified address.
    //  Throws "CDatabaseException *" if an error occurs.
    //  Apart from other possible causes of an error, database destruction
    //  will typically fail if the database is currently open by
    //  some other process or even by the same process.
    virtual void        DestroyDatabase(CDatabaseAddress * pAddress) throws(CDatabaseException *) = 0;
};

END_TT3_NAMESPACE
//  End of Tt3.Db.API/DatabaseType.hpp
