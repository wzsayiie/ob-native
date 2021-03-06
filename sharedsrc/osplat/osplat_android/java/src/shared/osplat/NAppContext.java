package src.shared.osplat;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.Context;

@SuppressWarnings({W.OMIT_0, W.OMIT_1})
public class NAppContext {

    @SuppressLint("StaticFieldLeak")
    private static Context sAppContext;

    @SuppressLint("StaticFieldLeak")
    private static Activity sMainActivity;

    //ui host need call these methods:

    public static void notifyMainActivityCreated(Activity activity) {
        sMainActivity = activity;

        if (sAppContext == null) {
            sAppContext = activity.getApplicationContext();
        }
    }

    public static void notifyMainActivityDestroy() {
        sMainActivity = null;
    }

    //user methods:

    public static Context getAppContext() {
        return sAppContext;
    }

    public static Activity getMainActivity() {
        return sMainActivity;
    }
}
