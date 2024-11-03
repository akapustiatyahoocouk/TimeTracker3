using System;
using System.Diagnostics;
using System.Drawing;
using TimeTracker3.Util;

namespace TimeTracker3.Db.API
{
    /// <summary>
    ///     The factory of "standard" UI traits for
    ///     database objects.
    /// </summary>
    public static class DatabaseObjectUiTraits
    {
        //////////
        //  Operations

        /// <summary>
        ///     Returns the default UI traits for the
        ///     specified database object.
        ///     IMPORTANT: "UI traits" instances will be
        ///     cached and reused as necessary.
        /// </summary>
        /// <param name="databaseObject">
        ///     The database object to return the UI
        ///     traits form
        /// </param>
        /// <returns>
        ///     The default UI traits for the specified
        ///     database object, never null.
        /// </returns>
        public static IUiTraits GetUiTraits(IDatabaseObject databaseObject)
        {
            Debug.Assert(databaseObject != null);

            lock (_UiTraits)   //  for thread safety
            {
                IUiTraits uiTraits = _UiTraits[databaseObject];
                if (uiTraits == null)
                {
                    switch (databaseObject)
                    {
                        //  Must re-create and cache
                        case IUser user:
                            uiTraits = new _UserUiTraits(user);
                            break;
                        case IAccount account:
                            uiTraits = new _AccountUiTraits(account);
                            break;
                        //  TODO for all objects
                        default: //  Fallback
                            uiTraits = new _UnknownUiTraits();
                            break;
                    }
                    _UiTraits[databaseObject] = uiTraits;
                }
                return uiTraits;
            }
        }

        //////////
        //  Implementation
        private static readonly WeakValueDictionary<IDatabaseObject, IUiTraits> _UiTraits =
            new WeakValueDictionary<IDatabaseObject, IUiTraits>();

        private sealed class _UserUiTraits : IUiTraits
        {
            //////////
            //  Construction
            internal _UserUiTraits(IUser user)
            {
                _User = user;
            }

            //////////
            //  Properties
            public string TypeName => "User";
            public string TypeDisplayName => "User";

            public string DisplayName
            {
                get
                {
                    try
                    {
                        return _User.RealName;
                    }
                    catch (Exception ex)
                    {
                        return ex.Message;
                    }
                }
            }

            public Image SmallImage => Properties.Resources.UserSmall;
            public Image LargeImage => Properties.Resources.UserLarge;

            //////////
            //  Implementation
            private readonly IUser _User;
        }

        private sealed class _AccountUiTraits : IUiTraits
        {
            //////////
            //  Construction
            internal _AccountUiTraits(IAccount account)
            {
                _Account = account;
            }

            //////////
            //  Properties
            public string TypeName => "Account";
            public string TypeDisplayName => "Account";

            public string DisplayName
            {
                get
                {
                    try
                    {
                        return _Account.Login;
                    }
                    catch (Exception ex)
                    {
                        return ex.Message;
                    }
                }
            }

            public Image SmallImage => Properties.Resources.AccountSmall;
            public Image LargeImage => Properties.Resources.AccountLarge;

            //////////
            //  Implementation
            private readonly IAccount _Account;
        }

        private sealed class _UnknownUiTraits : IUiTraits
        {
            //////////
            //  Properties
            public string TypeName => "?";
            public string TypeDisplayName => "?";
            public string DisplayName => "?";
            public Image SmallImage => Properties.Resources.ErrorSmall;
            public Image LargeImage => Properties.Resources.ErrorLarge;
        }
    }
}
