using System.Runtime.CompilerServices;

namespace TheEngine
{
    class Transform : Component
    {
        public Vector3 position
        {
            get
            {
                Vector3 result;
                this.Get_position(out result);
                return result;
            }
            set
            {
                Set_position(ref value);
            }
        }

        public Quaternion rotation
        {
            get
            {
                Quaternion result;
                this.Get_rotation(out result);
                return result;
            }
            set
            {
                Set_rotation(ref value);
            }
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern void Get_position(out Vector3 value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern void Set_position(ref Vector3 value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern void Get_rotation(out Quaternion value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern void Set_rotation(ref Quaternion value);
    }
}
