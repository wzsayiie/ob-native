package src.shared.uidev;

import android.app.Activity;
import android.os.Bundle;

public class NActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        handleCreate();
    }

    private static native void handleCreate();
}
