package pt.rgo.lcomng;

import android.os.Build;
import org.json.JSONObject;
import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;

/** One game in the remote Android catalog. */
final class CatalogGame {
    static final class Version {
        String url, version, sha256;
        long bytes;
    }

    String id, name, description, archetype, orientation, icon;
    JSONObject instructions;
    final Map<String, Version> versions = new LinkedHashMap<>();

    static CatalogGame from(JSONObject item, String baseUrl) {
        CatalogGame game = new CatalogGame();
        game.id = item.optString("id", "");
        game.name = item.optString("name", game.id);
        game.description = item.optString("description", "");
        game.archetype = item.optString("archetype", "");
        game.orientation = item.optString("orientation", "auto");
        game.icon = absolute(baseUrl, item.optString("icon", ""));
        game.instructions = item.optJSONObject("instructions");
        JSONObject abis = item.optJSONObject("abis");
        if (abis != null) {
            for (java.util.Iterator<String> it = abis.keys(); it.hasNext(); ) {
                String abi = it.next();
                JSONObject value = abis.optJSONObject(abi);
                if (value == null) continue;
                Version version = new Version();
                version.url = absolute(baseUrl, value.optString("url", ""));
                version.bytes = value.optLong("bytes", 0);
                version.version = value.optString("version", "");
                version.sha256 = value.optString("sha256", "");
                game.versions.put(abi, version);
            }
        }
        return game;
    }

    private static String absolute(String base, String path) {
        if (path.isEmpty()) return "";
        if (path.startsWith("http://") || path.startsWith("https://")) return path;
        return base.replaceAll("/+$", "") + "/" + path.replaceAll("^/+", "");
    }

    /** Best available bundle for this device, or null when the device ABI is unsupported. */
    Version version() {
        List<String> order = new ArrayList<>();
        for (String abi : Build.SUPPORTED_ABIS) order.add(abi);
        order.add("arm64-v8a");
        order.add("x86_64");
        order.add("armeabi-v7a");
        for (String abi : order) {
            Version version = versions.get(abi);
            if (version != null) return version;
        }
        return null;
    }

    String abi() {
        for (String abi : Build.SUPPORTED_ABIS) if (versions.containsKey(abi)) return abi;
        return versions.containsKey("arm64-v8a") ? "arm64-v8a" : versions.keySet().iterator().next();
    }
}
