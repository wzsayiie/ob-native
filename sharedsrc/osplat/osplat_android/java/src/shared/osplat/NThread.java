package src.shared.osplat;

import src.shared.osbind.NLambda;
import src.shared.osbind.NType;

@SuppressWarnings({W.OMIT_0, W.OMIT_1})
public class NThread {

    public static void runThread(long lamWord) {
        if (lamWord == 0) {
            return;
        }

        NLambda lambda = NLambda.holdWord(NType.LAMBDA, lamWord);
        if (lambda == null) {
            return;
        }

        Thread thread = new Thread(() -> {
            lambda.callVoid();
            lambda.dispose();
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
