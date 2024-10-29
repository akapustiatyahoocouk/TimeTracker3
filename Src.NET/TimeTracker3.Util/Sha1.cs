using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Security.Cryptography;
using System.Text;
using System.Threading.Tasks;

namespace TimeTracker3.Util
{
    /// <summary>
    ///     SHA-1 helpers
    /// </summary>
    public static class Sha1
    {
        //////////
        //  Operations

        /// <summary>
        ///     Calculates a SHA-1 hash of the input data as a hex string.
        /// </summary>
        /// <param name="data">
        ///     The data to calculate the SHA-1 hash for.
        /// </param>
        /// <returns>
        ///     The uppercase hex string containing the SHA-1 hash 
        ///     of the input data.
        /// </returns>
        public static string HashString(string data)
        {
            Debug.Assert(data != null);

            using (SHA1Managed sha1 = new SHA1Managed())
            {
                var hash = sha1.ComputeHash(Encoding.UTF8.GetBytes(data));
                var sb = new StringBuilder(hash.Length * 2);
                foreach (byte b in hash)
                {
                    // can be "x2" if you want lowercase
                    sb.Append(b.ToString("X2"));
                }
                return sb.ToString();
            }
        }
    }
}
