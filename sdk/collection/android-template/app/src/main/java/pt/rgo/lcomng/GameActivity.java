package pt.rgo.lcomng;

import org.libsdl.app.SDLActivity;
import org.json.JSONArray;
import org.json.JSONObject;
import android.os.Bundle;
import android.os.Build;
import android.graphics.Color;
import android.view.Gravity;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.FrameLayout;
import java.io.*;
import java.nio.charset.StandardCharsets;
import java.util.concurrent.atomic.AtomicBoolean;

/** One SDL/student runtime per child process, preserving the native library UI. */
public final class GameActivity extends SDLActivity {
    private String game, library;
    private Button returnButton;
    private int[] gameLayout;
    private final AtomicBoolean stopping = new AtomicBoolean();

    @Override protected void onCreate(Bundle state) {
        try {
            game = getIntent().getStringExtra("game");
            JSONObject catalog = CollectionCatalog.read(this);
            JSONArray games = catalog.getJSONArray("games");
            for (int index = 0; index < games.length(); index++) {
                JSONObject candidate = games.getJSONObject(index);
                if (candidate.getString("id").equals(game)) {
                    library = candidate.getString("library");
                    String orientation = candidate.getString("orientation");
                    if (orientation.equals("landscape")) setRequestedOrientation(android.content.pm.ActivityInfo.SCREEN_ORIENTATION_SENSOR_LANDSCAPE);
                    else if (orientation.equals("portrait")) setRequestedOrientation(android.content.pm.ActivityInfo.SCREEN_ORIENTATION_SENSOR_PORTRAIT);
                    break;
                }
            }
            if (library == null) throw new IOException("Unknown collection game");
        } catch (Exception error) {
            android.util.Log.e("lcom-ng", "Cannot launch game", error);
            // SDLActivity requires its onCreate before normal lifecycle cleanup.
            android.os.Process.killProcess(android.os.Process.myPid());
            return;
        }
        super.onCreate(state);
        if (Build.VERSION.SDK_INT >= 33) {
            getOnBackInvokedDispatcher().registerOnBackInvokedCallback(
                android.window.OnBackInvokedDispatcher.PRIORITY_DEFAULT, this::finish);
        }
        returnButton = new Button(this);
        returnButton.setText("");
        returnButton.setTextColor(Color.WHITE);
        returnButton.setAllCaps(false);
        returnButton.setMinWidth(0);
        returnButton.setMinHeight(0);
        returnButton.setPadding(0,0,0,0);
        returnButton.setContentDescription("Return to game library");
        returnButton.setBackground(new HomeDrawable(0xd2222222,0xffeeeeee));
        returnButton.setOnClickListener(view -> finish());
        returnButton.setVisibility(android.view.View.INVISIBLE);
        ((ViewGroup)getWindow().getDecorView()).addView(returnButton,
            new FrameLayout.LayoutParams(dp(48),dp(48),Gravity.TOP | Gravity.LEFT));
        returnButton.setOnApplyWindowInsetsListener((view,insets) -> {
            positionReturnButton();
            return insets;
        });
        getWindow().getDecorView().addOnLayoutChangeListener((view,left,top,right,bottom,
                oldLeft,oldTop,oldRight,oldBottom) -> positionReturnButton());
        returnButton.requestApplyInsets();
    }

    /** Optional SDL host callback, emitted only when rendered geometry changes. */
    public void onGameLayout(int[] geometry) {
        if (geometry.length < 9 || (geometry.length-9)%4 != 0) return;
        int[] copy = geometry.clone();
        runOnUiThread(() -> {
            gameLayout = copy;
            positionReturnButton();
        });
    }

    private void positionReturnButton() {
        if (returnButton == null || gameLayout == null || mSurface == null) return;
        android.view.View decor = getWindow().getDecorView();
        if (decor.getWidth() == 0 || decor.getHeight() == 0 || gameLayout[0] <= 0 || gameLayout[1] <= 0) return;
        int safeLeft=0, safeTop=0, safeRight=0, safeBottom=0;
        android.view.WindowInsets insets = decor.getRootWindowInsets();
        if (insets != null && Build.VERSION.SDK_INT >= 30) {
            android.graphics.Insets safe = insets.getInsets(android.view.WindowInsets.Type.systemBars() |
                android.view.WindowInsets.Type.systemGestures() | android.view.WindowInsets.Type.mandatorySystemGestures() |
                android.view.WindowInsets.Type.tappableElement() | android.view.WindowInsets.Type.displayCutout());
            safeLeft=safe.left; safeTop=safe.top; safeRight=safe.right; safeBottom=safe.bottom;
        } else if (insets != null && Build.VERSION.SDK_INT >= 28 && insets.getDisplayCutout() != null) {
            android.view.DisplayCutout cutout = insets.getDisplayCutout();
            safeLeft=cutout.getSafeInsetLeft(); safeTop=cutout.getSafeInsetTop();
            safeRight=cutout.getSafeInsetRight(); safeBottom=cutout.getSafeInsetBottom();
        }
        int[] surfaceOrigin = new int[2], decorOrigin = new int[2];
        mSurface.getLocationInWindow(surfaceOrigin); decor.getLocationInWindow(decorOrigin);
        float sx = (float)mSurface.getWidth()/gameLayout[0], sy = (float)mSurface.getHeight()/gameLayout[1];
        int count = (gameLayout.length-9)/4;
        int[][] occupied = new int[count][];
        int[] frame = mapRect(3,sx,sy,surfaceOrigin,decorOrigin);
        for (int i=0;i<count;i++) occupied[i]=mapRect(7+4*i,sx,sy,surfaceOrigin,decorOrigin);
        int size = Math.max(dp(44),Math.round(gameLayout[2]*Math.min(sx,sy)));
        int[] position = ReturnButtonLayout.place(
            new int[]{safeLeft,safeTop,decor.getWidth()-safeRight,decor.getHeight()-safeBottom},
            size,dp(4),frame,occupied);
        if (position == null) { returnButton.setVisibility(android.view.View.INVISIBLE); return; }
        int fill=gameLayout[gameLayout.length-2];
        int ink=0xff000000 | (gameLayout[gameLayout.length-1]&0xffffff);
        ((HomeDrawable)returnButton.getBackground()).colors(fill,ink);
        FrameLayout.LayoutParams params = (FrameLayout.LayoutParams)returnButton.getLayoutParams();
        if (params.leftMargin != position[0] || params.topMargin != position[1] || params.width != size || params.height != size) {
            params.leftMargin=position[0]; params.topMargin=position[1]; params.width=size; params.height=size;
            returnButton.setLayoutParams(params);
        }
        returnButton.setVisibility(android.view.View.VISIBLE);
    }

    private int[] mapRect(int index,float sx,float sy,int[] origin,int[] decor) {
        return new int[]{Math.round(gameLayout[index]*sx)+origin[0]-decor[0],
            Math.round(gameLayout[index+1]*sy)+origin[1]-decor[1],
            Math.round(gameLayout[index+2]*sx)+origin[0]-decor[0],
            Math.round(gameLayout[index+3]*sy)+origin[1]-decor[1]};
    }

    private int dp(int value) { return Math.round(value * getResources().getDisplayMetrics().density); }
    @Override protected String[] getLibraries() { return new String[] {"c++_shared", "SDL3", "lcom_runtime", library}; }
    @Override protected String getMainSharedObject() { return getApplicationInfo().nativeLibraryDir + "/lib" + library + ".so"; }
    @Override protected String[] getArguments() { return new String[] {new File(getFilesDir(), "games/" + game).getAbsolutePath()}; }

    @Override protected void main() {
        // SDL invokes this on its native thread. Keep bulk asset I/O off the UI thread.
        try {
            File root = new File(getFilesDir(), "games/" + game);
            if (!root.isDirectory() && !root.mkdirs()) throw new IOException("Cannot create game directory");
            String prefix = "games/" + game + "/";
            String version;
            try (BufferedReader reader = new BufferedReader(new InputStreamReader(getAssets().open(prefix + "lcom-assets-version.txt"), StandardCharsets.UTF_8))) {
                version = reader.readLine();
            }
            File stamps = new File(getFilesDir(), "asset-versions");
            if (!stamps.isDirectory() && !stamps.mkdirs()) throw new IOException("Cannot create asset version directory");
            File stamp = new File(stamps, game + ".txt");
            String previous = null;
            if (stamp.isFile()) try (BufferedReader reader = new BufferedReader(new FileReader(stamp))) { previous = reader.readLine(); }
            if (!version.equals(previous)) {
                try (BufferedReader files = new BufferedReader(new InputStreamReader(getAssets().open(prefix + "lcom-assets.txt"), StandardCharsets.UTF_8))) {
                    String name;
                    while ((name = files.readLine()) != null) {
                        File target = new File(root, name);
                        if (!target.getCanonicalPath().startsWith(root.getCanonicalPath() + File.separator)) throw new IOException("Invalid bundled asset path");
                        target.getParentFile().mkdirs();
                        // Keep the previous complete asset if copying is interrupted.
                        File temporary = new File(target.getParentFile(), target.getName() + ".lcom-copy");
                        try (InputStream input = getAssets().open(prefix + name); OutputStream output = new FileOutputStream(temporary)) {
                            byte[] buffer = new byte[65536];
                            int count;
                            while ((count = input.read(buffer)) != -1) output.write(buffer, 0, count);
                        }
                        if (!temporary.renameTo(target)) throw new IOException("Cannot install game asset");
                    }
                }
                try (Writer writer = new FileWriter(stamp)) { writer.write(version); }
            }
            NativeGameThread.run(() -> super.main());
        } catch (IOException error) {
            android.util.Log.e("lcom-ng", "Cannot prepare game assets", error);
            runOnUiThread(this::finish);
        }
    }

    @Override public boolean dispatchKeyEvent(android.view.KeyEvent event) {
        if (event.getKeyCode() == android.view.KeyEvent.KEYCODE_BACK) {
            if (event.getAction() == android.view.KeyEvent.ACTION_UP) finish();
            return true;
        }
        return super.dispatchKeyEvent(event);
    }

    @Override public void onBackPressed() { finish(); }

    @Override public void finish() {
        super.finish();
        // Original projects can call exit(), retain static state or stop pumping
        // events. Reaping only :game leaves the launcher and saves intact, and
        // avoids waiting indefinitely for an uncooperative native thread.
        if (stopping.compareAndSet(false, true)) new Thread(() -> {
            try { Thread.sleep(300); } catch (InterruptedException ignored) { }
            android.os.Process.killProcess(android.os.Process.myPid());
        }, "collection-game-exit").start();
    }
}
