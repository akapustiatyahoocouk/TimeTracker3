//
//  Tt3.Db.API/PrivateActivity.hpp
//  
//  Tt3.Db.API A private activity in a database
//
//////////
BEGIN_TT3_NAMESPACE

//  A private activity in a database
class TT3_DB_API_PUBLIC IPrivateActivity : public virtual IActivity
{
    //////////
    //  Operations (properties)
public:

    //////////
    //  Operations (associations)
public:
    //  Returns the user to which this private activity belongs.
    //  Throws "CDatabaseException *" if an error occurs.
    virtual IUser* GetOwner() const throws(CDatabaseException*) = 0;
};

END_TT3_NAMESPACE
//  End of Tt3.Db.API/PrivateActivity.hpp
