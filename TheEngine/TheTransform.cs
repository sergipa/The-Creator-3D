using System.Runtime.CompilerServices;

namespace TheEngine
{
    public class TheTransform : TheComponent
    {
        public TheVector3 Position
        {
            get
            {
                return GetPosition();
            }
            set
            {
                SetPosition(value);
            }
        }

        public TheQuaternion Rotation
        {
            get
            {
                GetRotation(out TheQuaternion ret);
                return ret;
            }
            set
            {
                SetRotation(ref value);
            }
        }

        public TheVector3 Scale
        {
            get
            {
                GetScale(out TheVector3 ret);
                return ret;
            }
            set
            {
                SetScale(ref value);
            }
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern TheVector3 GetPosition();

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern void SetPosition(TheVector3 value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern void GetRotation(out TheQuaternion value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern void SetRotation(ref TheQuaternion value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern void GetScale(out TheVector3 value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern void SetScale(ref TheVector3 value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern void LookAt(ref TheVector3 value);

    }
}
