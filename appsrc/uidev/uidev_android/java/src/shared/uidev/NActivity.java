package src.shared.uidev;

import android.app.Activity;
import android.os.Bundle;

import src.shared.osbind.NFunc;

public class NActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        NFunc launch = new NFunc("NLaunch");
        launch.callVoid();
    }
}
