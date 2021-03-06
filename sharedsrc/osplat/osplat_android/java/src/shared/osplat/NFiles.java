package src.shared.osplat;

import android.content.Context;

import java.io.File;

@SuppressWarnings({W.OMIT_0, W.OMIT_1})
public class NFiles {

    public static String documentPath() {
        Context context = NAppContext.getAppContext();
        if (context != null) {
            return context.getFilesDir().getAbsolutePath();
        }
        return null;
    }

    public static String cachesPath() {
        Context context = NAppContext.getAppContext();
        if (context != null) {
            return context.getCacheDir().getAbsolutePath();
        }
        return null;
    }

    public static boolean makeDirectory(String path, boolean intermediate) {
        if (path == null || path.isEmpty()) {
            return false;
        }

        File file = new File(path);
        if (intermediate) {
            return file.mkdirs();
        } else {
            return file.mkdir();
        }
    }

    private static void setOutBoolean(boolean[] out, boolean value) {
        if (out != null && out.length > 0) {
            out[0] = value;
        }
    }

    public static String[] copySubItems(String path, boolean[] outSuccess) {
        if (path == null || path.isEmpty()) {
            setOutBoolean(outSuccess, false);
            return null;
        }

        File file = new File(path);
        if (!file.exists() || !file.isDirectory()) {
            setOutBoolean(outSuccess, false);
            return null;
        }

        File[] subFiles = file.listFiles();
        if (subFiles == null || subFiles.length == 0) {
            //the processing was successful, but empty data was obtained.
            setOutBoolean(outSuccess, true);
            return null;
        }

        String[] subItems = new String[subFiles.length];
        for (int it = 0; it < subFiles.length; ++it) {
            subItems[it] = subFiles[it].getName();
        }
        setOutBoolean(outSuccess, true);
        return subItems;
    }

    public static boolean pathExists(String path, boolean[] outIsDirectory) {
        if (path == null || path.isEmpty()) {
            setOutBoolean(outIsDirectory, false);
            return false;
        }

        File file = new File(path);
        if (!file.exists()) {
            setOutBoolean(outIsDirectory, false);
            return false;
        }

        setOutBoolean(outIsDirectory, file.isDirectory());
        return true;
    }

    private static void removeRecursively(File file) {
        if (file.isDirectory()) {
            File[] subFiles = file.listFiles();
            if (subFiles != null) {
                for (File it : subFiles) {
                    removeRecursively(it);
                }
            }
        } else if (file.isFile()) {
            boolean ignored = file.delete();
        }
    }

    public static void removePath(String path) {
        if (path == null || path.isEmpty()) {
            return;
        }

        File file = new File(path);
        if (file.exists()) {
            removeRecursively(file);
        }
    }
}
