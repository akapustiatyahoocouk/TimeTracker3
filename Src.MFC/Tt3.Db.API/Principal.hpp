//
//  Tt3.Db.API/Principal.hpp
//  
//  Tt3.Db.API A generic access control principal in a database
//
//////////
BEGIN_TT3_NAMESPACE

//  A generic access control principal
class TT3_DB_API_PUBLIC IPrincipal : public virtual IDatabaseObject
{
    //////////
    //  Operations (properties)
public:
    //  Gets/sets the "enabled" status of this principal. "Disabled" 
    //  principals are considered as nonexistent when access control is performed.
    //  Throws "CDatabaseException *" if an error occurs.
    virtual bool            IsEnabled() const throws(CDatabaseException *) = 0;
    virtual void            SetEnabled(bool enabled) throws(CDatabaseException *)  = 0;

    //  Gets/sets the list of e-mail addresses of this principal.
    //  Throws "CDatabaseException *" if an error occurs.
    virtual CEmailAddressList   GetEmailAddresses() const throws(CDatabaseException *) = 0;
    virtual void            SetEmailAddresses(const CEmailAddressList & emailAddresses) throws(CDatabaseException *) = 0;
};

END_TT3_NAMESPACE
//  End of Tt3.Db.API/Principal.hpp
