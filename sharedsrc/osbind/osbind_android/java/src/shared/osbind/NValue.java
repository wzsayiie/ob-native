package src.shared.osbind;

@SuppressWarnings({W.OMIT_0, W.OMIT_1})
public class NValue {

    public static NValue makeNative(Object obj) {
        if (obj == null) {
            return null;
        }
        if (obj instanceof NValue) {
            return (NValue) obj;
        }

        //integer types.
        if (obj instanceof Boolean  ) { return new NValue(NType.BOOLEAN, (boolean) obj ? 1 : 0); }
        if (obj instanceof Character) { return new NValue(NType.CHAR   , (char   ) obj); }
        if (obj instanceof Byte     ) { return new NValue(NType.BYTE   , (byte   ) obj); }
        if (obj instanceof Short    ) { return new NValue(NType.SHORT  , (short  ) obj); }
        if (obj instanceof Integer  ) { return new NValue(NType.INT    , (int    ) obj); }
        if (obj instanceof Long     ) { return new NValue(NType.LONG   , (long   ) obj); }

        //float types.
        if (obj instanceof Float ) { return new NValue(NType.FLOAT , floatWord ((float ) obj)); }
        if (obj instanceof Double) { return new NValue(NType.DOUBLE, doubleWord((double) obj)); }

        //string types.
        if (obj instanceof String) { return new NValue(NType.STRING, createStrWord((String) obj)); }

        return null;
    }

    public static NValue holdWord(int type, long word) {
        if (type >= NType.OBJECT && word != 0) {
            retainWord(word);
        }
        return new NValue(type, word);
    }

    public static NValue autoreleaseWord(int type, long word) {
        return new NValue(type, word);
    }

    public int  nativeType() { return mType; }
    public long nativeWord() { return mWord; }

    public boolean asBoolean() {
        return mWord != 0;
    }

    public long asLong() {
        if (NType.BOOLEAN <= mType && mType <= NType.ULONG) {
            return mWord;
        }
        if (mType == NType.FLOAT) {
            return (long) floatValue(mWord);
        }
        if (mType == NType.DOUBLE) {
            return (long) doubleValue(mWord);
        }
        return 0;
    }

    public double asDouble() {
        if (NType.BOOLEAN <= mType && mType <= NType.ULONG) {
            return (double) mWord;
        }
        if (mType == NType.FLOAT) {
            return floatValue(mWord);
        }
        if (mType == NType.DOUBLE) {
            return doubleValue(mWord);
        }
        return 0;
    }

    public char  asChar () { return (char ) asLong  (); }
    public byte  asByte () { return (byte ) asLong  (); }
    public short asShort() { return (short) asLong  (); }
    public int   asInt  () { return (int  ) asLong  (); }
    public float asFloat() { return (float) asDouble(); }

    public String asString() {
        if (mType == NType.STRING) {
            return stringValue(mWord);
        }
        return null;
    }

    //NOTE:
    //the method is used to manually release native objects.
    //the java vm doesn't know the type and size of the resources occupied by native objects,
    //and can't release them in time.
    public void dispose() {
        synchronized (this) {
            disposeWord();
        }
    }

    private final int  mType;
    private /* */ long mWord;

    private NValue(int type, long word) {
        mType = type;
        mWord = word;
    }

    @Override
    protected void finalize() throws Throwable {
        super.finalize();
        disposeWord();
    }

    private void disposeWord() {
        if (mType >= NType.OBJECT && mWord != 0) {
            releaseWord(mWord);
            mWord = 0;
        }
    }

    private static native long   floatWord    (float  value);
    private static native long   doubleWord   (double value);
    private static native float  floatValue   (long   word );
    private static native double doubleValue  (long   word );
    private static native void   retainWord   (long   word );
    private static native void   releaseWord  (long   word );
    private static native long   createStrWord(String value);
    private static native String stringValue  (long   word );
}
