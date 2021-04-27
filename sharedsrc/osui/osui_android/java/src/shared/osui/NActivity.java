package src.shared.osui;

import android.app.Activity;
import android.os.Bundle;

import src.shared.osbind.NFunc;
import src.shared.osplat.NAppContext;

public class NActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        NAppContext.notifyMainActivityCreated(this);

        NFunc launch = NFunc.find("NLaunch");
        if (launch != null) {
            launch.callVoid();
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        NAppContext.notifyMainActivityDestroy();
    }
}
