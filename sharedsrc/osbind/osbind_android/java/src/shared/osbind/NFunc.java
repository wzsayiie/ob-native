package src.shared.osbind;

@SuppressWarnings({W.OMIT_0, W.OMIT_1})
public class NFunc {

    public static NFunc find(String name) {
        int funcIndex = findFuncIndex(name);
        if (funcIndex == 0) {
            return null;
        }

        int     retType   = funcRetType  (funcIndex);
        boolean retRetain = funcRetRetain(funcIndex);

        return new NFunc(funcIndex, retType, retRetain);
    }

    public void callVoid(Object... arg) {
        invokeValue(arg);
    }

    public boolean callBoolean(Object... arg) {
        NValue ret = invokeValue(arg);
        return ret != null && ret.asBoolean();
    }

    public char   callChar  (Object... a) { NValue r = invokeValue(a); return r != null ? r.asChar  () : 0; }
    public byte   callByte  (Object... a) { NValue r = invokeValue(a); return r != null ? r.asByte  () : 0; }
    public short  callShort (Object... a) { NValue r = invokeValue(a); return r != null ? r.asShort () : 0; }
    public int    callInt   (Object... a) { NValue r = invokeValue(a); return r != null ? r.asInt   () : 0; }
    public float  callLong  (Object... a) { NValue r = invokeValue(a); return r != null ? r.asLong  () : 0; }
    public float  callFloat (Object... a) { NValue r = invokeValue(a); return r != null ? r.asFloat () : 0; }
    public double callDouble(Object... a) { NValue r = invokeValue(a); return r != null ? r.asDouble() : 0; }

    public String callString(Object... arg) {
        NValue ret = invokeValue(arg);
        return ret != null ? ret.asString() : null;
    }

    public NValue callValue(Object... arg) {
        return invokeValue(arg);
    }

    public NLambda callLambda(Object... arg) {
        return invokeLambda(arg);
    }

    private final int     mFuncIndex;
    private final int     mRetType  ;
    private final boolean mRetRetain;

    private NFunc(int funcIndex, int retType, boolean retRetain) {
        mFuncIndex = funcIndex;
        mRetType   = retType  ;
        mRetRetain = retRetain;
    }

    private long invokeWord(Object[] arg) {

        //convert the arguments.
        int argCount = 0;
        if (arg != null) {
            argCount = arg.length;
        }
        NValue[] argValue = new NValue[argCount];
        for (int i = 0; i < argCount; ++i) {
            argValue[i] = NValue.makeNative(arg[i]);
        }

        //call the native function.
        funcPrepare();
        for (int i = 0; i < argCount; ++i) {
            NValue value = argValue[i];
            if (value != null) {
                funcPushArg(value.nativeType(), value.nativeWord());
            } else {
                funcPushArg(NType.BLUR_PTR, 0);
            }
        }
        return funcCall(mFuncIndex);
    }

    private NValue invokeValue(Object[] arg) {
        long retWord = invokeWord(arg);
        if (mRetRetain) {
            return NValue.autoreleaseWord(mRetType, retWord);
        } else {
            return NValue.holdWord(mRetType, retWord);
        }
    }

    private NLambda invokeLambda(Object[] arg) {
        long retWord = invokeWord(arg);
        if (mRetRetain) {
            return NLambda.autoreleaseWord(mRetType, retWord);
        } else {
            return NLambda.holdWord(mRetType, retWord);
        }
    }

    private static native int     findFuncIndex(String name );
    private static native int     funcRetType  (int    index);
    private static native boolean funcRetRetain(int    index);

    private static native void funcPrepare();
    private static native void funcPushArg(int type, long word);
    private static native long funcCall   (int index);
}
