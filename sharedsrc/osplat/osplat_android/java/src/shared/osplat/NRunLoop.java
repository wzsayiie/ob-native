package src.shared.osplat;

import android.os.Handler;
import android.os.Looper;

import src.shared.osbind.NFunc;
import src.shared.osbind.NValue;

@SuppressWarnings({W.OMIT_0, W.OMIT_1})
public class NRunLoop {

    public static void postOnMain(NValue action) {
        if (action == null) {
            return;
        }

        Looper looper = Looper.getMainLooper();
        new Handler(looper).post(() -> {
            NFunc func = new NFunc("NActionRun");
            func.callVoid(action, null);
            action.dispose();
        });
    }
}
