package pt.rgo.lcomng;

import android.content.Context;
import org.json.JSONObject;
import java.io.*;
import java.net.HttpURLConnection;
import java.net.URL;
import java.nio.charset.StandardCharsets;
import java.security.MessageDigest;
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.List;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;

/** Downloads, verifies, installs and removes per-game native bundles. */
final class GameInstaller {
    interface Progress {
        void progress(long read, long total);
    }

    private static final int BUFFER = 65536;

    private GameInstaller() { }

    static File gamesRoot(Context context) {
        File root = new File(context.getFilesDir(), "games");
        if (!root.isDirectory() && !root.mkdirs()) throw new IllegalStateException("Cannot create game storage");
        return root;
    }

    static File gameRoot(Context context, String id) {
        return new File(gamesRoot(context), id);
    }

    static String currentVersion(Context context, String id) {
        File file = new File(gameRoot(context, id), "current");
        if (!file.isFile()) return null;
        try (BufferedReader reader = new BufferedReader(new FileReader(file))) {
            String value = reader.readLine();
            return value == null || value.trim().isEmpty() ? null : value.trim();
        } catch (IOException error) {
            return null;
        }
    }

    static JSONObject installedManifest(Context context, String id) {
        String version = currentVersion(context, id);
        if (version == null) return null;
        File manifest = new File(new File(gameRoot(context, id), version), "manifest.json");
        if (!manifest.isFile()) return null;
        try {
            return new JSONObject(read(manifest));
        } catch (Exception error) {
            return null;
        }
    }

    static boolean isInstalled(Context context, CatalogGame game) {
        JSONObject manifest = installedManifest(context, game.id);
        if (manifest == null) return false;
        String version = manifest.optString("version", "");
        CatalogGame.Version available = game.version();
        return available != null && available.version.equals(version);
    }

    static boolean hasUpdate(Context context, CatalogGame game) {
        JSONObject manifest = installedManifest(context, game.id);
        if (manifest == null) return false;
        CatalogGame.Version available = game.version();
        return available != null && !available.version.equals(manifest.optString("version", ""));
    }

    static File libraryFile(Context context, String id) {
        JSONObject manifest = installedManifest(context, id);
        if (manifest == null) return null;
        String version = manifest.optString("version", "");
        String library = manifest.optString("library", "");
        String abi = manifest.optString("abi", "");
        return new File(new File(new File(gameRoot(context, id), version), "lib"), abi + "/lib" + library + ".so");
    }

    static File assetsDir(Context context, String id) {
        JSONObject manifest = installedManifest(context, id);
        if (manifest == null) return null;
        String version = manifest.optString("version", "");
        return new File(new File(gameRoot(context, id), version), "assets");
    }

    static long installedBytes(Context context) {
        long total = 0;
        File root = gamesRoot(context);
        File[] games = root.listFiles();
        if (games == null) return 0;
        for (File game : games) total += size(game);
        return total;
    }

    private static long size(File file) {
        if (file.isFile()) return file.length();
        File[] children = file.listFiles();
        long total = 0;
        if (children != null) for (File child : children) total += size(child);
        return total;
    }

    static void delete(Context context, String id) {
        deleteTree(gameRoot(context, id));
    }

    private static void deleteTree(File file) {
        File[] children = file.listFiles();
        if (children != null) for (File child : children) deleteTree(child);
        file.delete();
    }

    /** Download and install one bundle. Runs off the UI thread. */
    static void install(Context context, CatalogGame game, Progress progress) throws Exception {
        CatalogGame.Version version = game.version();
        if (version == null) throw new IOException("This device has no supported bundle for " + game.name);
        File root = gameRoot(context, game.id);
        if (!root.isDirectory() && !root.mkdirs()) throw new IOException("Cannot create game storage");
        File downloads = new File(context.getCacheDir(), "downloads");
        if (!downloads.isDirectory() && !downloads.mkdirs()) throw new IOException("Cannot create download cache");
        File archive = new File(downloads, game.id + ".zip.part");
        File complete = new File(downloads, game.id + ".zip");
        try {
            download(version, archive, progress);
            String digest = sha256(archive);
            if (!digest.equalsIgnoreCase(version.sha256)) throw new IOException("Download verification failed");
            if (!archive.renameTo(complete)) {
                copy(archive, complete);
                archive.delete();
            }
            File staging = new File(root, version.version + ".tmp");
            deleteTree(staging);
            if (!staging.mkdirs()) throw new IOException("Cannot create game directory");
            unzip(complete, staging);
            File destination = new File(root, version.version);
            deleteTree(destination);
            if (!staging.renameTo(destination)) {
                copyTree(staging, destination);
                deleteTree(staging);
            }
            File[] old = root.listFiles();
            if (old != null) for (File file : old) {
                if (!file.isDirectory() || file.getName().equals(version.version)) continue;
                if (file.getName().endsWith(".tmp")) deleteTree(file);
                else deleteTree(file);
            }
            File current = new File(root, "current.tmp");
            try (Writer writer = new FileWriter(current)) { writer.write(version.version); }
            File target = new File(root, "current");
            target.delete();
            if (!current.renameTo(target)) throw new IOException("Cannot record installed version");
        } finally {
            archive.delete();
            complete.delete();
        }
    }

    private static void download(CatalogGame.Version version, File target, Progress progress) throws IOException {
        HttpURLConnection connection = (HttpURLConnection) new URL(version.url).openConnection();
        connection.setConnectTimeout(20000);
        connection.setReadTimeout(60000);
        connection.setInstanceFollowRedirects(true);
        connection.setRequestProperty("Accept-Encoding", "identity");
        connection.setRequestProperty("User-Agent", "lcom-ng-android");
        try {
            int status = connection.getResponseCode();
            if (status < 200 || status >= 300) throw new IOException("Download failed: HTTP " + status);
            long total = connection.getContentLengthLong();
            if (total <= 0) total = version.bytes;
            try (InputStream input = connection.getInputStream();
                 OutputStream output = new BufferedOutputStream(new FileOutputStream(target))) {
                byte[] buffer = new byte[BUFFER];
                long read = 0;
                int count;
                while ((count = input.read(buffer)) != -1) {
                    output.write(buffer, 0, count);
                    read += count;
                    if (progress != null) progress.progress(read, total);
                }
            }
        } finally {
            connection.disconnect();
        }
    }

    private static String sha256(File file) throws IOException {
        try (InputStream input = new FileInputStream(file)) {
            MessageDigest digest = MessageDigest.getInstance("SHA-256");
            byte[] buffer = new byte[BUFFER];
            int count;
            while ((count = input.read(buffer)) != -1) digest.update(buffer, 0, count);
            StringBuilder text = new StringBuilder();
            for (byte value : digest.digest()) text.append(String.format("%02x", value));
            return text.toString();
        } catch (java.security.NoSuchAlgorithmException impossible) {
            throw new IOException(impossible);
        }
    }

    private static void unzip(File archive, File destination) throws IOException {
        String root = destination.getCanonicalPath() + File.separator;
        try (ZipFile zip = new ZipFile(archive)) {
            Enumeration<? extends ZipEntry> entries = zip.entries();
            while (entries.hasMoreElements()) {
                ZipEntry entry = entries.nextElement();
                String name = entry.getName().replace('\\', '/');
                if (name.startsWith("/") || name.contains("../") || name.equals("..")) throw new IOException("Unsafe bundle path");
                File target = new File(destination, name);
                if (!target.getCanonicalPath().startsWith(root)) throw new IOException("Unsafe bundle path");
                if (entry.isDirectory()) {
                    if (!target.isDirectory() && !target.mkdirs()) throw new IOException("Cannot create " + name);
                    continue;
                }
                File parent = target.getParentFile();
                if (parent != null && !parent.isDirectory() && !parent.mkdirs()) throw new IOException("Cannot create " + name);
                try (InputStream input = zip.getInputStream(entry); OutputStream output = new FileOutputStream(target)) {
                    byte[] buffer = new byte[BUFFER];
                    int count;
                    while ((count = input.read(buffer)) != -1) output.write(buffer, 0, count);
                }
                if (name.endsWith(".so")) {
                    target.setReadable(true, false);
                    target.setExecutable(true, false);
                }
            }
        }
    }

    private static void copy(File source, File target) throws IOException {
        try (InputStream input = new FileInputStream(source); OutputStream output = new FileOutputStream(target)) {
            byte[] buffer = new byte[BUFFER];
            int count;
            while ((count = input.read(buffer)) != -1) output.write(buffer, 0, count);
        }
    }

    private static void copyTree(File source, File target) throws IOException {
        if (!target.isDirectory() && !target.mkdirs()) throw new IOException("Cannot create " + target);
        File[] children = source.listFiles();
        if (children == null) return;
        for (File child : children) {
            File to = new File(target, child.getName());
            if (child.isDirectory()) copyTree(child, to);
            else {
                copy(child, to);
                if (child.getName().endsWith(".so")) {
                    to.setReadable(true, false);
                    to.setExecutable(true, false);
                }
            }
        }
    }

    private static String read(File file) throws IOException {
        try (InputStream input = new FileInputStream(file)) {
            ByteArrayOutputStream output = new ByteArrayOutputStream();
            byte[] buffer = new byte[4096];
            int count;
            while ((count = input.read(buffer)) != -1) output.write(buffer, 0, count);
            return new String(output.toByteArray(), StandardCharsets.UTF_8);
        }
    }
}
