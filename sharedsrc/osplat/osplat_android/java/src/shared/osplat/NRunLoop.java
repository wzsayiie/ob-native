package src.shared.osplat;

import android.os.Handler;
import android.os.Looper;

import src.shared.osbind.NLambda;
import src.shared.osbind.NType;

@SuppressWarnings({W.OMIT_0, W.OMIT_1})
public class NRunLoop {

    public static void postOnMain(long lamWord) {
        if (lamWord == 0) {
            return;
        }

        NLambda lambda = NLambda.holdWord(NType.LAMBDA, lamWord);
        if (lambda == null) {
            return;
        }

        Looper looper = Looper.getMainLooper();
        new Handler(looper).post(() -> {
            lambda.callVoid();
            lambda.dispose();
        });
    }
}
