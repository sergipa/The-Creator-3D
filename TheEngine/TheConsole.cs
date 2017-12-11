using System.Runtime.CompilerServices;

namespace TheEngine.TheConsole
{
    public class TheConsole
    {
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void Log(object message);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void Warning(object message);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void Error(object message);
    }
}
