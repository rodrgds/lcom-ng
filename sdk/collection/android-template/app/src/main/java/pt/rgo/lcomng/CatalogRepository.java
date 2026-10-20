package pt.rgo.lcomng;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import org.json.JSONObject;
import java.io.*;
import java.net.HttpURLConnection;
import java.net.URL;
import java.nio.charset.StandardCharsets;

/** Remote catalog with an offline cache and small artwork cache. */
final class CatalogRepository {
    private static final String CATALOG = "android-catalog.json";

    private CatalogRepository() { }

    static String catalogUrl(Context context) {
        String override = context.getSharedPreferences("catalog", Context.MODE_PRIVATE).getString("url", null);
        return override != null && !override.isEmpty() ? override : context.getString(R.string.catalog_url);
    }

    static File catalogFile(Context context) {
        return new File(context.getFilesDir(), CATALOG);
    }

    static JSONObject load(Context context) throws Exception {
        File cache = catalogFile(context);
        if (cache.isFile()) {
            try {
                JSONObject cached = new JSONObject(read(cache));
                refresh(context);
                return cached;
            } catch (Exception ignored) {
                // A corrupt cache is replaced by the network copy below.
            }
        }
        JSONObject remote = new JSONObject(fetch(catalogUrl(context)));
        write(cache, remote.toString());
        return remote;
    }

    private static void refresh(Context context) {
        final Context app = context.getApplicationContext();
        new Thread(() -> {
            try {
                JSONObject remote = new JSONObject(fetch(catalogUrl(app)));
                write(catalogFile(app), remote.toString());
            } catch (Exception ignored) {
                // Keep the last complete catalog while offline.
            }
        }, "catalog-refresh").start();
    }

    static File iconFile(Context context, String id) {
        File directory = new File(context.getFilesDir(), "icons");
        if (!directory.isDirectory() && !directory.mkdirs()) return null;
        return new File(directory, id + ".png");
    }

    static Bitmap cachedIcon(Context context, String id) {
        File file = iconFile(context, id);
        if (file == null || !file.isFile()) return null;
        BitmapFactory.Options options = new BitmapFactory.Options();
        options.inSampleSize = 2;
        return BitmapFactory.decodeFile(file.getAbsolutePath(), options);
    }

    static void downloadIcon(Context context, CatalogGame game) {
        if (game.icon.isEmpty()) return;
        File target = iconFile(context, game.id);
        if (target == null || target.isFile()) return;
        File temporary = new File(target.getParentFile(), target.getName() + ".part");
        try {
            HttpURLConnection connection = (HttpURLConnection) new URL(game.icon).openConnection();
            connection.setConnectTimeout(15000);
            connection.setReadTimeout(30000);
            connection.setInstanceFollowRedirects(true);
            connection.setRequestProperty("User-Agent", "lcom-ng-android");
            int status = connection.getResponseCode();
            if (status < 200 || status >= 300) return;
            try (InputStream input = connection.getInputStream(); OutputStream output = new FileOutputStream(temporary)) {
                byte[] buffer = new byte[65536];
                int count;
                while ((count = input.read(buffer)) != -1) output.write(buffer, 0, count);
            }
            target.delete();
            temporary.renameTo(target);
        } catch (IOException ignored) {
            temporary.delete();
        }
    }

    private static String fetch(String address) throws IOException {
        HttpURLConnection connection = (HttpURLConnection) new URL(address).openConnection();
        connection.setConnectTimeout(20000);
        connection.setReadTimeout(30000);
        connection.setInstanceFollowRedirects(true);
        connection.setRequestProperty("User-Agent", "lcom-ng-android");
        connection.setRequestProperty("Accept", "application/json");
        try {
            int status = connection.getResponseCode();
            if (status < 200 || status >= 300) throw new IOException("Catalog HTTP " + status);
            try (InputStream input = connection.getInputStream()) {
                ByteArrayOutputStream output = new ByteArrayOutputStream();
                byte[] buffer = new byte[16384];
                int count;
                while ((count = input.read(buffer)) != -1) output.write(buffer, 0, count);
                return new String(output.toByteArray(), StandardCharsets.UTF_8);
            }
        } finally {
            connection.disconnect();
        }
    }

    private static String read(File file) throws IOException {
        try (InputStream input = new FileInputStream(file)) {
            ByteArrayOutputStream output = new ByteArrayOutputStream();
            byte[] buffer = new byte[16384];
            int count;
            while ((count = input.read(buffer)) != -1) output.write(buffer, 0, count);
            return new String(output.toByteArray(), StandardCharsets.UTF_8);
        }
    }

    private static void write(File target, String text) throws IOException {
        File temporary = new File(target.getParentFile(), target.getName() + ".part");
        try (Writer writer = new OutputStreamWriter(new FileOutputStream(temporary), StandardCharsets.UTF_8)) {
            writer.write(text);
        }
        target.delete();
        if (!temporary.renameTo(target)) throw new IOException("Cannot cache catalog");
    }
}
