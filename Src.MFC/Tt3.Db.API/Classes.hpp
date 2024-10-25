//
//  Tt3.Db.API/Classes.hpp
//  
//  Tt3.Db.API forward declarations and typedefs
//
//////////
BEGIN_TT3_NAMESPACE

class TT3_DB_API_PUBLIC CDatabaseType;
class TT3_DB_API_PUBLIC CDatabaseAddress;
class TT3_DB_API_PUBLIC CDatabase;

class TT3_DB_API_PUBLIC IDatabaseObject;
class TT3_DB_API_PUBLIC IPrincipal;
class TT3_DB_API_PUBLIC IUser;
class TT3_DB_API_PUBLIC IAccount;
class TT3_DB_API_PUBLIC IActivityType;
class TT3_DB_API_PUBLIC IActivity;
class TT3_DB_API_PUBLIC IPublicActivity;
class TT3_DB_API_PUBLIC IPrivateActivity;
class TT3_DB_API_PUBLIC ITask;
class TT3_DB_API_PUBLIC IPublicTask;
class TT3_DB_API_PUBLIC IPrivateTask;
class TT3_DB_API_PUBLIC IWork;
class TT3_DB_API_PUBLIC IEvent;
class TT3_DB_API_PUBLIC IBeneficiary;
class TT3_DB_API_PUBLIC IWorkload;
class TT3_DB_API_PUBLIC IProject;
class TT3_DB_API_PUBLIC IWorkStream;

using CEmailAddressList = CVector<CString>;
using CUserList = CVector<IUser*>;
using CAccountList = CVector<IAccount*>;
using CActivityList = CVector<IActivity*>;
using CPublicActivityList = CVector<IPublicActivity*>;
using CPrivateActivityList = CVector<IPrivateActivity*>;
using CTaskList = CVector<ITask*>;
using CPublicTaskList = CVector<IPublicTask*>;
using CPrivateTaskList = CVector<IPrivateTask*>;
using CWorkList = CVector<IWork*>;
using CEventList = CVector<IEvent*>;
using CBeneficiaryList = CVector<IBeneficiary*>;
using CWorkloadList = CVector<IWorkload*>;
using CProjectList = CVector<IProject*>;
using CWorkStreamList = CVector<IWorkStream*>;

END_TT3_NAMESPACE
//  End of Tt3.Db.API/Classes.hpp
