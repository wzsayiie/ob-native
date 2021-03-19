package src.shared.osbind;

@SuppressWarnings({W.OMIT_0, W.OMIT_1})
public class NFunc {

    public NFunc(String name) {
        mFuncIndex  = findFuncByName(name);
        mReturnType = funcReturnType(mFuncIndex);
        mRetRetain  = funcRetRetain (mFuncIndex);
        mArgCount   = funcArgCount  (mFuncIndex);

        mArgTypes = new int[mArgCount];
        for (int idx = 0; idx < mArgCount; ++idx) {
            mArgTypes[idx] = funcArgType(mFuncIndex, idx);
        }
    }

    public void callVoid(Object... arg) {
        call(arg);
    }

    public boolean callBoolean(Object... arg) {
        NValue ret = call(arg);
        if (ret != null) {
            return ret.asBoolean();
        }
        return false;
    }

    public char   callChar  (Object... arg) { NValue ret = call(arg); return ret != null ? ret.asChar   () : 0; }
    public byte   callByte  (Object... arg) { NValue ret = call(arg); return ret != null ? ret.asByte   () : 0; }
    public short  callShort (Object... arg) { NValue ret = call(arg); return ret != null ? ret.asShort  () : 0; }
    public int    callInt   (Object... arg) { NValue ret = call(arg); return ret != null ? ret.asInt    () : 0; }
    public long   callLong  (Object... arg) { NValue ret = call(arg); return ret != null ? ret.asLong   () : 0; }
    public float  callFloat (Object... arg) { NValue ret = call(arg); return ret != null ? ret.asFloat  () : 0; }
    public double callDouble(Object... arg) { NValue ret = call(arg); return ret != null ? ret.asDouble () : 0; }

    public String callString(Object... arg) {
        NValue ret = call(arg);
        if (ret != null) {
            return ret.asString();
        }
        return null;
    }

    public NValue callValue(Object... arg) {
        return call(arg);
    }

    private final int     mFuncIndex ;
    private final int     mReturnType;
    private final boolean mRetRetain ;
    private final int     mArgCount  ;
    private final int[]   mArgTypes  ;

    private static native int     findFuncByName(String name );
    private static native int     funcReturnType(int    index);
    private static native boolean funcRetRetain (int    index);
    private static native int     funcArgCount  (int    index);
    private static native int     funcArgType   (int    index, int argIndex);

    private static native void nativeFuncPrepare();
    private static native void nativeFuncPushArg(int type, long word);
    private static native long nativeFuncCall   (int index);

    private NValue call(Object[] arg) {
        if (mFuncIndex == 0) {
            return null;
        }

        int argCount = (arg != null ? arg.length : 0);
        if (argCount > mArgCount) {
            argCount = mArgCount;
        }

        NValue[] params = new NValue[argCount];
        for (int i = 0; i < argCount; ++i) {
            params[i] = NValue.make(arg[i], mArgTypes[i]);
        }

        nativeFuncPrepare();
        for (int i = 0; i < argCount; ++i) {
            if (params[i] != null) {
                nativeFuncPushArg(params[i].nativeType(), params[i].nativeWord());
            } else {
                nativeFuncPushArg(NType.PTR, 0);
            }
        }

        long ret = nativeFuncCall(mFuncIndex);
        return NValue.hold(mReturnType, ret, mRetRetain);
    }
}
