//
//  Tt3.Db.API/Event.hpp
//  
//  Tt3.Db.API A recorded event in a database
//
//////////
BEGIN_TT3_NAMESPACE

//  A recorded event in a database - a one-off occasion that, for 
//  the purposes of timekeeping, occurs at a given date+time but 
//  does not have a "duration".
class TT3_DB_API_PUBLIC IEvent  : public virtual IDatabaseObject
{
    //////////
    //  Operations (properties)
public:
    //  Gets the UTC date+time when this event had occurred.
    //  Throws "CDatabaseException *" if an error occurs.
    virtual CTime       GetOccurredAt() const throws(CDatabaseException*) = 0;

    //  Gets short summary of this event.
    //  Throws "CDatabaseException *" if an error occurs.
    virtual CString     GetSummary() const throws(CDatabaseException*) = 0;

    //////////
    //  Operations (associations)
public:
    //  Returns the user who has logged this event; never NULL.
    //  Throws "CDatabaseException *" if an error occurs.
    virtual IUser* GetUser() const throws(CDatabaseException*) = 0;

    //  Returns the activity against which this event was logged; NULL if none.
    //  Throws "CDatabaseException *" if an error occurs.
    virtual IActivity* GetActivity() const throws(CDatabaseException*) = 0;
};

END_TT3_NAMESPACE
//  End of Tt3.Db.API/Event.hpp

