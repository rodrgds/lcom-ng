package pt.rgo.lcomng;

import org.libsdl.app.SDLActivity;
import android.util.Log;
import java.io.*;

public final class GameActivity extends SDLActivity {
    @Override protected String[] getLibraries() { return new String[] {"c++_shared", "SDL3", "main"}; }
    @Override protected String[] getArguments() {
        return new String[] {new File(getFilesDir(), "game").getAbsolutePath()};
    }
    @Override protected void main() {
        // SDL calls this on its native thread, keeping asset I/O off Android's UI thread.
        try {
            File root = new File(getFilesDir(), "game");
            root.mkdirs();
            String version;
            try (BufferedReader input = new BufferedReader(new InputStreamReader(getAssets().open("lcom-assets-version.txt"), "UTF-8"))) {
                version = input.readLine();
            }
            File stamp = new File(getFilesDir(), "lcom-assets-version.txt");
            String previous = null;
            if (stamp.isFile()) try (BufferedReader input = new BufferedReader(new FileReader(stamp))) { previous = input.readLine(); }
            if (version.equals(previous)) { NativeGameThread.run(() -> super.main()); return; }
            // Explicit file list distinguishes empty files from asset directories.
            try (BufferedReader files = new BufferedReader(new InputStreamReader(getAssets().open("lcom-assets.txt"), "UTF-8"))) {
                String name;
                while ((name = files.readLine()) != null) {
                    File target = new File(root, name);
                    if (!target.getCanonicalPath().startsWith(root.getCanonicalPath() + File.separator))
                        throw new IOException("Invalid bundled asset path");
                    target.getParentFile().mkdirs();
                    try (InputStream input = getAssets().open(name); OutputStream output = new FileOutputStream(target)) {
                        byte[] buffer = new byte[65536];
                        int count;
                        while ((count = input.read(buffer)) != -1) output.write(buffer, 0, count);
                    }
                }
            }
            try (Writer output = new FileWriter(stamp)) { output.write(version); }
            NativeGameThread.run(() -> super.main());
        } catch (IOException error) {
            Log.e("lcom-ng", "Cannot prepare game assets", error);
            runOnUiThread(this::finish);
        }
    }
}
