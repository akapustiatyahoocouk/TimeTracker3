//
//  Tt3.Db.API/DatabaseObject.hpp
//  
//  Tt3.Db.API Generic database object
//
//////////
BEGIN_TT3_NAMESPACE

//  An abstract interface to a generic "object" residing in a database.
//  Database objects are reference counted. Each time a client stores a
//  pointer to a database object it MUST increment its reference count
//  and each time it releases that pointer the reference count must be
//  decremnented. When it reaches zero, the IDatabaseObject instance
//  becomes "dead" and can be recycled by the managing CDatabase.
//
//  The life cycle of an IDatabaseObject instance is:
//  *   NEW -     every IDatabaseObject is created in this state with 
//                reference count of 0
//  *   MANAGED - once the reference count of a IDatabaseObject is 
//                incremented from 0 to 1 for the FIRST time, the instance
//                becomes MANAGED.
//  *   UNUSED  - once a reference count of a MANAGED IDatabaseObject becomes
//                0 after a decrement, that instance becomes UNUSED; the
//                containing CDatabase has an option to mark that instance
//                as "not live".
class TT3_DB_API_PUBLIC IDatabaseObject
{
    //////////
    //  Types
public:
    //  An OID of a database object
    enum class OID : DWORD
    {
        Invalid = 0 //  no object will ever have this OID
    };

    //////////
    //  This is an interface
public:
    virtual ~IDatabaseObject() = default;

    //////////
    //  Operations (reference counting)
public:
    //  Increments the reference count of this database object by 1.
    virtual void            Acquire() = 0;

    //  Decrements the reference count of this database object by 1.
    //  If the reference count becomes 0 as a result, the instance can
    //  be recycled at the discretion of the managing CDatabase.
    virtual void            Release() = 0;

    //////////
    //  Operations (general)
public:
    //  Returns the database where this object resides (if live) or 
    //  used to reside if dead).
    virtual CDatabase *     GetDatabase() const = 0;

    //  Checks if this object is "live" (i.e. can be used).
    virtual bool            IsLive() const = 0;

    //  Returns/sets the OID of this database object.
    //  All objects in a given database must have different OIDs.
    //  Throws "CDatabaseException *" if an error occurs.
    virtual OID             GetOID() const throws(CDatabaseException *) = 0;
    virtual void            SetOID(OID oid) throws(CDatabaseException *) = 0;

    //////////
    //  Operations (life cycle)
public:
    //  Destroys this object, delete-cascading as necessary.
    //  Throws "CDatabaseException *" if an error occurs.
    virtual void            Destroy() throws(CDatabaseException*) = 0;
};

END_TT3_NAMESPACE
//  End of Tt3.Db.API/DatabaseObject.hpp
