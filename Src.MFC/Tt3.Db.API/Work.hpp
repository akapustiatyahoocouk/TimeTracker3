//
//  Tt3.Db.API/Work.hpp
//  
//  Tt3.Db.API A recorded work in a database
//
//////////
BEGIN_TT3_NAMESPACE

//  A recorded work in a database
class TT3_DB_API_PUBLIC IWork : public virtual IDatabaseObject
{
    //////////
    //  Operations (properties)
public:
    //  Gets the work started/finished time, both expressed in UTC
    //  Throws "CDatabaseException *" if an error occurs.
    virtual CTime       GetStartedAt() const throws(CDatabaseException*) = 0;
    virtual CTime       GetFinishedAt() const throws(CDatabaseException*) = 0;

    //////////
    //  Operations (associations)
public:
    //  Returns the user who has logged this work; never NULL.
    //  Throws "CDatabaseException *" if an error occurs.
    virtual IUser*      GetUser() const throws(CDatabaseException*) = 0;

    //  Returns the activity against which this work was logged; never NULL.
    //  Throws "CDatabaseException *" if an error occurs.
    virtual IActivity*  GetActivity() const throws(CDatabaseException*) = 0;
};

END_TT3_NAMESPACE
//  End of Tt3.Db.API/Work.hpp
