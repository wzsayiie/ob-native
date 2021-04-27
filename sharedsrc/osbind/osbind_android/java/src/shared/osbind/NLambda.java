package src.shared.osbind;

@SuppressWarnings({W.OMIT_0, W.OMIT_1})
public class NLambda {

    public static NLambda holdValue(NValue value) {
        if (value != null && value.nativeType() == NType.LAMBDA) {
            long word = value.nativeWord();
            if (word != 0) {
                retainWord(word);
                return new NLambda(word);
            }
        }
        return null;
    }

    public static NLambda holdWord(int type, long word) {
        if (type == NType.LAMBDA && word != 0) {
            retainWord(word);
            return new NLambda(word);
        }
        return null;
    }

    public static NLambda autoreleaseWord(int type, long word) {
        if (type == NType.LAMBDA && word != 0) {
            return new NLambda(word);
        }
        return null;
    }

    public int  nativeType() { return NType.LAMBDA; }
    public long nativeWord() { return mWord; }

    public void    callVoid   (Object... arg) { push(mWord, arg); /*  */ /*vid*/ invokeVoid  (mWord); }
    public boolean callBoolean(Object... arg) { push(mWord, arg); return /*bol*/ invokeBool  (mWord); }
    public char    callChar   (Object... arg) { push(mWord, arg); return (char ) invokeLong  (mWord); }
    public byte    callByte   (Object... arg) { push(mWord, arg); return (byte ) invokeLong  (mWord); }
    public short   callShort  (Object... arg) { push(mWord, arg); return (short) invokeLong  (mWord); }
    public int     callInt    (Object... arg) { push(mWord, arg); return (int  ) invokeLong  (mWord); }
    public long    callLong   (Object... arg) { push(mWord, arg); return /*lng*/ invokeLong  (mWord); }
    public float   callFloat  (Object... arg) { push(mWord, arg); return (float) invokeDouble(mWord); }
    public double  callDouble (Object... arg) { push(mWord, arg); return /*dbl*/ invokeDouble(mWord); }
    public String  callString (Object... arg) { push(mWord, arg); return /*str*/ invokeString(mWord); }
    public NValue  callValue  (Object... arg) { push(mWord, arg); return /*val*/ invokeValue (mWord); }
    public NLambda callLambda (Object... arg) { push(mWord, arg); return /*lam*/ invokeLambda(mWord); }

    //NOTE:
    //the method is used to manually release native objects.
    //the java vm doesn't know the type and size of the resources occupied by native objects,
    //and can't release them in time.
    public void dispose() {
        synchronized (this) {
            disposeWord();
        }
    }

    private long mWord;

    private NLambda(long word) {
        mWord = word;
    }

    @Override
    protected void finalize() throws Throwable {
        super.finalize();
        disposeWord();
    }

    private void disposeWord() {
        if (mWord != 0) {
            releaseWord(mWord);
            mWord = 0;
        }
    }

    private static native void retainWord (long word);
    private static native void releaseWord(long word);

    private static void push(long lamWord, Object[] arg) {
        prepare(lamWord);
        if (arg != null) {
            for (Object val : arg) {
                pushArg(lamWord, val);
            }
        }
    }

    private static void pushArg(long lamWord, Object value) {

        if /**/ (value instanceof Boolean  ) { pushBool  (lamWord, (boolean) value); }
        else if (value instanceof Character) { pushLong  (lamWord, (char   ) value); }
        else if (value instanceof Byte     ) { pushLong  (lamWord, (byte   ) value); }
        else if (value instanceof Short    ) { pushLong  (lamWord, (short  ) value); }
        else if (value instanceof Integer  ) { pushLong  (lamWord, (int    ) value); }
        else if (value instanceof Long     ) { pushLong  (lamWord, (long   ) value); }
        else if (value instanceof Float    ) { pushDouble(lamWord, (Float  ) value); }
        else if (value instanceof Double   ) { pushDouble(lamWord, (Double ) value); }
        else if (value instanceof String   ) { pushString(lamWord, (String ) value); }

        else if (value instanceof NValue) {
            NValue object = (NValue) value;
            int type = object.nativeType();

            if (NType.BOOLEAN <= type && type <= NType.ULONG ) {
                pushLong(lamWord, object.asLong());

            } else if (NType.FLOAT <= type && type <= NType.DOUBLE) {
                pushDouble(lamWord, object.asDouble());

            } else if (type >= NType.OBJECT) {
                pushObjWord(lamWord, object.nativeWord());

            } else {
                pushObjWord(lamWord, 0);
            }
        }

        else if (value instanceof NLambda) {
            NLambda lambda = (NLambda) value;
            pushLamWord(lamWord, lambda.nativeWord());
        }

        else {
            pushObjWord(lamWord, 0);
        }
    }

    private static native void prepare    (long lamWord);
    private static native void pushBool   (long lamWord, boolean value);
    private static native void pushLong   (long lamWord, long    value);
    private static native void pushDouble (long lamWord, double  value);
    private static native void pushString (long lamWord, String  value);
    private static native void pushObjWord(long lamWord, long    word );
    private static native void pushLamWord(long lamWord, long    word );

    private static native void    invokeVoid   (long lamWord);
    private static native boolean invokeBool   (long lamWord);
    private static native long    invokeLong   (long lamWord);
    private static native double  invokeDouble (long lamWord);
    private static native String  invokeString (long lamWord);
    private static native long    invokeObjWord(long lamWord);
    private static native long    invokeLamWord(long lamWord);

    private static NValue invokeValue(long lamWord) {
        long retWord = invokeObjWord(lamWord);
        return NValue.holdWord(NType.OBJECT, retWord);
    }

    private static NLambda invokeLambda(long lamWord) {
        long retWord = invokeLamWord(lamWord);
        return NLambda.holdWord(NType.LAMBDA, retWord);
    }
}
