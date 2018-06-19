using System;
using System.IO;
using System.Runtime.InteropServices;

namespace DesktopChanger
{
    /// <summary>
    /// Reference
    /// https://social.msdn.microsoft.com/Forums/vstudio/en-US/dfd41d37-3486-4b9f-b7e7-fa1faea3fbbd/change-the-desktop-folder?forum=csharpgeneral
    /// </summary>
    class Program
    {
        public static class KnownFolder
        {
            public static Guid Desktop = new Guid("B4BFCC3A-DB2C-424C-B029-7FE99A87C641");
        }

        [DllImport("shell32.dll")]
        private extern static int SHSetKnownFolderPath(ref Guid folderId, uint flags, IntPtr token, [MarshalAs(UnmanagedType.LPWStr)] string path);

        [DllImport("shell32.dll")]
        public static extern int SHChangeNotify(int eventId, int flags, IntPtr item1, IntPtr item2);

        static void Main(string[] args)
        {
            string newPath = $@"{Environment.GetFolderPath(Environment.SpecialFolder.UserProfile)}\Desktop";
            if(args.Length > 0)
            {
                if (System.IO.Directory.Exists(args[0]))
                {
                    newPath = args[0];
                }
            }
            else
            {
                if (!Directory.Exists(newPath))
                {
                    return;
                }
            }
            SHSetKnownFolderPath(ref KnownFolder.Desktop, 0, IntPtr.Zero, newPath);
            SHChangeNotify(0x8000000, 0x1000, IntPtr.Zero, IntPtr.Zero);
        }
    }
}
