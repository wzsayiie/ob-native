package src.shared.xbind;

public class NPod {

    public static final int TYPE_VOID    =  1;
    public static final int TYPE_BOOLEAN =  2;
    public static final int TYPE_SBYTE   =  3;
    public static final int TYPE_SHORT   =  4;
    public static final int TYPE_INT     =  5;
    public static final int TYPE_LONG    =  6;
    public static final int TYPE_BYTE    =  7;
    public static final int TYPE_USHORT  =  8;
    public static final int TYPE_UINT    =  9;
    public static final int TYPE_ULONG   = 10;
    public static final int TYPE_FLOAT   = 11;
    public static final int TYPE_DOUBLE  = 12;
    public static final int TYPE_PTR     = 13;

    static native boolean booleanValue(long word);
    static native byte    sbyteValue  (long word);
    static native short   shortValue  (long word);
    static native int     intValue    (long word);
    static native long    longValue   (long word);
    static native byte    byteValue   (long word);
    static native short   ushortValue (long word);
    static native int     uintValue   (long word);
    static native long    ulongValue  (long word);
    static native float   floatValue  (long word);
    static native double  doubleValue (long word);
    static native long    ptrValue    (long word);

    static native long booleanWord(boolean value);
    static native long byteWord   (byte    value);
    static native long shortWord  (short   value);
    static native long intWord    (int     value);
    static native long longWord   (long    value);
    static native long floatWord  (float   value);
    static native long doubleWord (double  value);
    static native long ptrWord    (long    value);
}
