package src.shared.osplat;

import src.shared.osbind.NFunc;
import src.shared.osbind.NValue;

@SuppressWarnings({W.OMIT_0, W.OMIT_1})
public class NThread {

    public static void runThread(NValue action) {
        if (action == null) {
            return;
        }

        Thread thread = new Thread(() -> {
            NFunc func = new NFunc("NActionRun");
            func.callVoid(action, null);
        });
        thread.start();
    }

    public static void threadSleep(float seconds) {
        if (seconds <= 0) {
            return;
        }

        try {
            Thread.sleep((long) (seconds * 1000));
        } catch (InterruptedException ignored) {
            //blank.
        }
    }
}
