package src.shared.osbind;

@SuppressWarnings({W.OMIT_0, W.OMIT_1})
public class NFunc {

    public NFunc(String name) {
        mFuncIndex   = findFuncByName (name);
        mReturnType  = funcReturnType (mFuncIndex);
        mRetRetained = funcRetRetained(mFuncIndex);
        mArgCount    = funcArgCount   (mFuncIndex);

        mArgTypes = new int[mArgCount];
        for (int n = 0; n < mArgCount; ++n) {
            mArgTypes[n] = funcArgType(mFuncIndex, n);
        }
    }

    public void callVoid(Object... a) {
        call(a);
    }

    public boolean callBoolean(Object... a) {
        NValue value = call(a);
        return value != null && value.asBoolean();
    }

    public char   callChar  (Object... a) { NValue v = call(a); return v != null ? v.asChar   () : 0; }
    public byte   callByte  (Object... a) { NValue v = call(a); return v != null ? v.asByte   () : 0; }
    public short  callShort (Object... a) { NValue v = call(a); return v != null ? v.asShort  () : 0; }
    public int    callInt   (Object... a) { NValue v = call(a); return v != null ? v.asInt    () : 0; }
    public long   callLong  (Object... a) { NValue v = call(a); return v != null ? v.asLong   () : 0; }
    public float  callFloat (Object... a) { NValue v = call(a); return v != null ? v.asFloat  () : 0; }
    public double callDouble(Object... a) { NValue v = call(a); return v != null ? v.asDouble () : 0; }
    public String callString(Object... a) { NValue v = call(a); return v != null ? v.asString () : null; }

    public NValue callValue(Object... a) {
        return call(a);
    }

    private final int     mFuncIndex  ;
    private final int     mReturnType ;
    private final boolean mRetRetained;
    private final int     mArgCount   ;
    private final int[]   mArgTypes   ;

    private static native int     findFuncByName (String fName );
    private static native int     funcReturnType (int    fIndex);
    private static native boolean funcRetRetained(int    fIndex);
    private static native int     funcArgCount   (int    fIndex);
    private static native int     funcArgType    (int    fIndex, int aIndex);

    private static native long invokeFunc(int fIndex, int argc,
        int  type0, int  type1, int  type2, int  type3,
        long word0, long word1, long word2, long word3
    );

    private NValue call(Object[] args) {
        if (mFuncIndex == 0) {
            return null;
        }

        final int I_0 = 0;
        final int I_1 = 1;
        final int I_2 = 2;
        final int I_3 = 3;
        final int I_M = 4;

        //NOTE: items hold the c objects.
        @SuppressWarnings({"MismatchedReadAndWriteOfArray"})
        NValue[] items = new NValue[I_M];

        int [] types = new int [I_M];
        long[] words = new long[I_M];

        int argc = (args != null ? args.length : 0);
        if (argc > I_M) {
            argc = I_M;
        }
        if (argc > mArgCount) {
            argc = mArgCount;
        }
        for (int n = 0; n < argc; ++n) {
            NValue value = NValue.make(args[n], mArgTypes[n]);
            if (value == null) {
                continue;
            }

            items[n] = value;
            types[n] = value.nativeType();
            words[n] = value.nativeWord();
        }

        long retWord = invokeFunc(mFuncIndex, argc,
            types[I_0], types[I_1], types[I_2], types[I_3],
            words[I_0], words[I_1], words[I_2], words[I_3]
        );
        return NValue.hold(mReturnType, retWord, mRetRetained);
    }
}
