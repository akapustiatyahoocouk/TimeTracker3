//
//  Tt3.Db.API/Project.hpp
//  
//  Tt3.Db.API A project in a database
//
//////////
BEGIN_TT3_NAMESPACE

//  A project in a database - a workload that has a life cycle 
//  (milestones, completion state, etc.)
class TT3_DB_API_PUBLIC IProject : public virtual IWorkload
{
    //////////
    //  Operations (properties)
public:
    //  Gets/sets the completion status of this project.
    //  Throws "CDatabaseException *" if an error occurs.
    virtual bool        IsCompleted() const throws(CDatabaseException*) = 0;
    virtual void        SetCompleted(bool completed) throws(CDatabaseException*) = 0;

    //////////
    //  Operations (associations)
public:
    //  Returns/changes the immediate parent of this project;
    //  NULL if this a root project and has no parent.
    //  Throws "CDatabaseException *" if an error occurs.
    virtual IProject*   GetParent() const throws(CDatabaseException*) = 0;
    virtual void        SetParent(IProject* pParent) throws(CDatabaseException*) = 0;

    //  Returns an unordered list of all immediate children of
    //  this project.
    //  Throws "CDatabaseException *" if an error occurs.
    virtual CProjectList    GetChildren() const throws(CDatabaseException*) = 0;

    //////////
    //  Operations (life cycle)
public:
    //  Creates a new child of this project with the specified properties.
    //  Thrown a "CDatabaseException*" if an error occurs.
    virtual IProject*   CreateChildProject(const CString& displayName, const CString& descrition,
                        bool completed) throws(CDatabaseException*) = 0;
};

END_TT3_NAMESPACE
//  End of Tt3.Db.API/Project.hpp
