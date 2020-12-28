package src.shared.osbind;

public class NFunc {

    public NFunc(String name) {
        mFPos = findFunc(name);
        mRetType = funcRetType(mFPos);
        mRetRetained = funcRetRetained(mFPos);
    }

    public void    callVoid   (Object... args) { call(args); }
    public boolean callBoolean(Object... args) { return (boolean) returnBoolean(call(args)); }
    public byte    callByte   (Object... args) { return (byte   ) returnByte   (call(args)); }
    public short   callShort  (Object... args) { return (short  ) returnLong   (call(args)); }
    public int     callInt    (Object... args) { return (int    ) returnLong   (call(args)); }
    public long    callLong   (Object... args) { return (long   ) returnLong   (call(args)); }
    public float   callFloat  (Object... args) { return (float  ) returnDouble (call(args)); }
    public double  callDouble (Object... args) { return (double ) returnDouble (call(args)); }
    public NPtr    callPtr    (Object... args) { return (NPtr   ) returnPtr    (call(args)); }

    //NOTE: these two functions are unsafe.
    //if the return value isn't "NString *" or "const char *", unforeseen result will occur.
    public String callString(Object... args) {
        NPtr ptr = returnPtr(call(args));
        if (ptr != null) {
            return ptr.asString();
        } else {
            return null;
        }
    }
    public String callChars(Object... args) {
        NPtr ptr = returnPtr(call(args));
        if (ptr != null) {
            return ptr.asChars();
        } else {
            return null;
        }
    }

    private static final int MAX_ARG_NUM = 4;

    private long call(Object[] args) {
        if (mFPos == 0) {
            return 0;
        }
        if (args.length > MAX_ARG_NUM) {
            return 0;
        }

        int [] t = new int [MAX_ARG_NUM];
        long[] w = new long[MAX_ARG_NUM];
        for (int idx = 0; idx < args.length; ++idx) {
            boolean okay = convertArgs(args[idx], idx, t, w);
            if (!okay) {
                return 0;
            }
        }

        return callFunc(mFPos, args.length, t[0], t[1], t[2], t[3], w[0], w[1], w[2], w[3]);
    }

    private boolean convertArgs(Object object, int idx, int[] types, long[] words) {
        if (object == null) {
            types[idx] = NPod.TYPE_PTR;
            words[idx] = 0;

        } else if (object instanceof Boolean) {
            types[idx] = NPod.TYPE_BOOLEAN;
            words[idx] = NPod.booleanWord((Boolean) object);

        } else if (object instanceof Byte) {
            types[idx] = NPod.TYPE_BYTE;
            words[idx] = NPod.byteWord((Byte) object);

        } else if (object instanceof Short) {
            types[idx] = NPod.TYPE_SHORT;
            words[idx] = NPod.shortWord((Short) object);

        } else if (object instanceof Integer) {
            types[idx] = NPod.TYPE_INT;
            words[idx] = NPod.intWord((Integer) object);

        } else if (object instanceof Long) {
            types[idx] = NPod.TYPE_LONG;
            words[idx] = NPod.longWord((Long) object);

        } else if (object instanceof Float) {
            types[idx] = NPod.TYPE_FLOAT;
            words[idx] = NPod.floatWord((Float) object);

        } else if (object instanceof Double) {
            types[idx] = NPod.TYPE_DOUBLE;
            words[idx] = NPod.doubleWord((Double) object);

        } else if (object instanceof String) {
            NPtr ptr = new NPtr((String) object);
            types[idx] = NPod.TYPE_PTR;
            words[idx] = NPod.ptrWord(ptr.asPtr());

        } else if (object instanceof NPtr) {
            NPtr ptr = (NPtr) object;
            types[idx] = NPod.TYPE_PTR;
            words[idx] = NPod.ptrWord(ptr.asPtr());

        } else {
            //unsupported types.
            return false;
        }
        return true;
    }

    private boolean returnBoolean(long word) {
        switch (mRetType) {
            case NPod.TYPE_BOOLEAN: return NPod.booleanValue(word);
            case NPod.TYPE_SBYTE  : return NPod.sbyteValue  (word) != 0;
            case NPod.TYPE_SHORT  : return NPod.shortValue  (word) != 0;
            case NPod.TYPE_INT    : return NPod.intValue    (word) != 0;
            case NPod.TYPE_LONG   : return NPod.longValue   (word) != 0;
            case NPod.TYPE_BYTE   : return NPod.byteValue   (word) != 0;
            case NPod.TYPE_USHORT : return NPod.ushortValue (word) != 0;
            case NPod.TYPE_UINT   : return NPod.uintValue   (word) != 0;
            case NPod.TYPE_ULONG  : return NPod.ulongValue  (word) != 0;
            case NPod.TYPE_FLOAT  : return NPod.floatValue  (word) != 0;
            case NPod.TYPE_DOUBLE : return NPod.doubleValue (word) != 0;
            case NPod.TYPE_PTR    : return NPod.ptrValue    (word) != 0;
            default /* unknown */ : return false;
        }
    }

    private byte returnByte(long word) {
        switch (mRetType) {
            case NPod.TYPE_BOOLEAN: return NPod.booleanValue(word) ? (byte) 1 : 0;
            case NPod.TYPE_SBYTE  : return NPod.sbyteValue  (word);

            case NPod.TYPE_SHORT  : return (byte) NPod.shortValue  (word);
            case NPod.TYPE_INT    : return (byte) NPod.intValue    (word);
            case NPod.TYPE_LONG   : return (byte) NPod.longValue   (word);
            case NPod.TYPE_BYTE   : return (byte) NPod.byteValue   (word);
            case NPod.TYPE_USHORT : return (byte) NPod.ushortValue (word);
            case NPod.TYPE_UINT   : return (byte) NPod.uintValue   (word);
            case NPod.TYPE_ULONG  : return (byte) NPod.ulongValue  (word);
            case NPod.TYPE_FLOAT  : return (byte) NPod.floatValue  (word);
            case NPod.TYPE_DOUBLE : return (byte) NPod.doubleValue (word);

            case NPod.TYPE_PTR    : return 0;
            default /* unknown */ : return 0;
        }
    }

    private long returnLong(long word) {
        switch (mRetType) {
            case NPod.TYPE_BOOLEAN: return NPod.booleanValue(word) ? 1 : 0;
            case NPod.TYPE_SBYTE  : return NPod.sbyteValue  (word);
            case NPod.TYPE_SHORT  : return NPod.shortValue  (word);
            case NPod.TYPE_INT    : return NPod.intValue    (word);
            case NPod.TYPE_LONG   : return NPod.longValue   (word);
            case NPod.TYPE_BYTE   : return NPod.byteValue   (word);
            case NPod.TYPE_USHORT : return NPod.ushortValue (word);
            case NPod.TYPE_UINT   : return NPod.uintValue   (word);
            case NPod.TYPE_ULONG  : return NPod.ulongValue  (word);

            case NPod.TYPE_FLOAT  : return (long) NPod.floatValue (word);
            case NPod.TYPE_DOUBLE : return (long) NPod.doubleValue(word);

            case NPod.TYPE_PTR    : return 0;
            default /* unknown */ : return 0;
        }
    }

    private double returnDouble(long word) {
        switch (mRetType) {
            case NPod.TYPE_BOOLEAN: return NPod.booleanValue(word) ? 1 : 0;
            case NPod.TYPE_SBYTE  : return NPod.sbyteValue  (word);
            case NPod.TYPE_SHORT  : return NPod.shortValue  (word);
            case NPod.TYPE_INT    : return NPod.intValue    (word);
            case NPod.TYPE_LONG   : return NPod.longValue   (word);
            case NPod.TYPE_BYTE   : return NPod.byteValue   (word);
            case NPod.TYPE_USHORT : return NPod.ushortValue (word);
            case NPod.TYPE_UINT   : return NPod.uintValue   (word);
            case NPod.TYPE_ULONG  : return NPod.ulongValue  (word);
            case NPod.TYPE_FLOAT  : return NPod.floatValue  (word);
            case NPod.TYPE_DOUBLE : return NPod.doubleValue (word);
            case NPod.TYPE_PTR    : return 0;
            default /* unknown */ : return 0;
        }
    }

    private NPtr returnPtr(long word) {
        if (mRetType == NPod.TYPE_PTR) {
            return new NPtr(NPod.ptrValue(word), mRetRetained);
        } else {
            return null;
        }
    }

    private static native int findFunc(String name);
    private static native int funcRetType(int fPos);
    private static native boolean funcRetRetained(int fPos);

    private static native long callFunc(int fPos, int argc,
        int  type0, int  type1, int  type2, int  type3,
        long word0, long word1, long word2, long word3
    );

    private final int mFPos;
    private final int mRetType;
    private final boolean mRetRetained;
}
