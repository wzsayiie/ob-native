package src.shared.osbind;

public class NPtr {

    NPtr(String string) {
        mPtr = createString(string);
        mRetained = true;
    }

    NPtr(long ptr, boolean retained) {
        mPtr = ptr;
        mRetained = retained;
    }

    String asString() {
        return stringFromPtr(mPtr);
    }

    String asChars() {
        return charsFromPtr(mPtr);
    }

    long asPtr() {
        return mPtr;
    }

    @Override
    protected void finalize() {
        if (mRetained) {
            releasePtr(mPtr);
        }
    }

    private static native long createString(String string);

    //NOTE: these two functions is unsafe.
    //if "ptr" isn't a "NString *" or "const char *", unforeseen results will be occur.
    private static native String stringFromPtr(long ptr);
    private static native String charsFromPtr(long ptr);

    //NOTE: this function is unsafe.
    //if "ptr" doesn't point to a object, unforeseen results will be occur.
    private static native void releasePtr(long ptr);

    private final long mPtr;
    private final boolean mRetained;
}
