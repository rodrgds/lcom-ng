package pt.rgo.lcomng;

import android.app.Activity;
import android.app.Dialog;
import android.content.Intent;
import android.content.res.Configuration;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Typeface;
import android.graphics.drawable.Drawable;
import android.graphics.drawable.GradientDrawable;
import android.graphics.drawable.RippleDrawable;
import android.graphics.drawable.StateListDrawable;
import android.os.Build;
import android.os.Bundle;
import android.text.Editable;
import android.text.InputType;
import android.text.TextWatcher;
import android.view.Gravity;
import android.view.View;
import android.view.ViewGroup;
import android.view.WindowInsets;
import android.view.WindowInsetsController;
import android.view.inputmethod.EditorInfo;
import android.view.inputmethod.InputMethodManager;
import android.widget.Button;
import android.widget.EditText;
import android.widget.GridLayout;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.ProgressBar;
import android.widget.ScrollView;
import android.widget.TextView;
import android.widget.Toast;
import org.json.JSONArray;
import org.json.JSONObject;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.LinkedHashMap;
import android.graphics.Rect;
import android.os.Handler;
import android.os.Looper;
import android.text.TextUtils;
import android.view.Window;
import android.view.WindowManager;
import android.widget.FrameLayout;
import java.util.Locale;

import java.util.Random;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

/** Native library UI. SDL and student code are loaded exclusively in :game. */
public final class CollectionActivity extends Activity {
    private static final int BACKGROUND = 0xff080a09, SURFACE = 0xff171d19;
    private static final int INK = 0xfff4f7f5, MUTED = 0xffa9b3ad, ACCENT = 0xff72d6b0;
    private final ArrayList<Game> games = new ArrayList<>();
    private final Random random = new Random();
    private String title = "Game library", query = "";
    private LinearLayout root, content;
    private ScrollView scroll;
    private GridLayout grid;
    private EditText search;
    private Button clear, surprise, allChip, downloadedChip;
    private TextView summary, empty;
    private String anchorId;
    private int columns, anchorOffset, anchorHeight, sideInset, topInset, bottomInset;
    private boolean restorePending, resumed, installedOnly;
    private long cachedBytes;
    private final Handler mainHandler = new Handler(Looper.getMainLooper());
    private Runnable pendingFilter;
    private final ExecutorService io = Executors.newFixedThreadPool(2);
    private final ExecutorService artwork = Executors.newFixedThreadPool(4);

    private static final class Game {
        CatalogGame data;
        String id, name, archetype;
        Bitmap icon;
        ImageView iconView;
        TextView placeholder;
        TextView status;
        ProgressBar progress;
        View tile;
        boolean busy, installed, updateAvailable;
        int downloadPercent;
    }

    private int dp(float value) { return Math.round(value * getResources().getDisplayMetrics().density); }

    private static String formatBytes(long bytes) {
        if (bytes < 1024) return bytes + " B";
        if (bytes < 1024L * 1024L) return String.format(Locale.ROOT, "%.0f KB", bytes / 1024.0);
        if (bytes < 1024L * 1024L * 1024L) return String.format(Locale.ROOT, "%.1f MB", bytes / (1024.0 * 1024.0));
        return String.format(Locale.ROOT, "%.1f GB", bytes / (1024.0 * 1024.0 * 1024.0));
    }

    @Override public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        getWindow().setStatusBarColor(Color.TRANSPARENT);
        getWindow().setNavigationBarColor(Color.TRANSPARENT);
        if (Build.VERSION.SDK_INT >= 28) {
            android.view.WindowManager.LayoutParams attributes = getWindow().getAttributes();
            attributes.layoutInDisplayCutoutMode = android.view.WindowManager.LayoutParams.LAYOUT_IN_DISPLAY_CUTOUT_MODE_SHORT_EDGES;
            getWindow().setAttributes(attributes);
        }
        if (Build.VERSION.SDK_INT >= 30) getWindow().setDecorFitsSystemWindows(false);
        if (savedInstanceState != null) {
            query = savedInstanceState.getString("query", "");
            installedOnly = savedInstanceState.getBoolean("installedOnly", false);
            anchorId = savedInstanceState.getString("anchor");
            anchorOffset = savedInstanceState.getInt("anchorOffset");
            anchorHeight = savedInstanceState.getInt("anchorHeight");
        }
        loadCatalog();
    }

    private void loadCatalog() {
        LinearLayout loading = new LinearLayout(this);
        loading.setOrientation(LinearLayout.VERTICAL);
        loading.setGravity(Gravity.CENTER);
        loading.setPadding(dp(24), dp(24), dp(24), dp(24));
        loading.setBackgroundColor(BACKGROUND);
        ProgressBar progress = new ProgressBar(this);
        progress.setIndeterminateTintList(android.content.res.ColorStateList.valueOf(ACCENT));
        loading.addView(progress, new LinearLayout.LayoutParams(dp(36), dp(36)));
        TextView message = text("Loading games…", 16, INK);
        message.setGravity(Gravity.CENTER);
        message.setPadding(0, dp(20), 0, 0);
        loading.addView(message);
        setContentView(loading);
        immerse();
        io.execute(() -> {
            try {
                JSONObject catalog = CatalogRepository.load(this);
                String loadedTitle = catalog.optString("name", "Game library");
                String baseUrl = catalog.optString("baseUrl", "");
                if (baseUrl.isEmpty()) {
                    String address = CatalogRepository.catalogUrl(this);
                    baseUrl = address.endsWith("/catalog.json")
                        ? address.substring(0, address.length() - "/catalog.json".length()) : address;
                }
                ArrayList<Game> loaded = new ArrayList<>();
                JSONArray entries = catalog.optJSONArray("games");
                if (entries != null) for (int index = 0; index < entries.length(); index++) {
                    CatalogGame data = CatalogGame.from(entries.getJSONObject(index), baseUrl);
                    if (data.id.isEmpty()) continue;
                    Game game = new Game();
                    game.data = data;
                    game.id = data.id;
                    game.name = data.name;
                    game.archetype = data.archetype;
                    game.icon = CatalogRepository.cachedIcon(this, data.id);
                    loaded.add(game);
                }
                refreshStates(loaded);
                runOnUiThread(() -> {
                    if (isFinishing() || isDestroyed()) return;
                    title = loadedTitle;
                    games.clear();
                    games.addAll(loaded);
                    render();
                    restoreAnchorAfterLayout();
                    for (Game game : games) if (game.icon == null && !game.data.icon.isEmpty()) {
                        artwork.execute(() -> {
                            CatalogRepository.downloadIcon(this, game.data);
                            Bitmap bitmap = CatalogRepository.cachedIcon(this, game.id);
                            if (bitmap == null) return;
                            runOnUiThread(() -> {
                                if (isFinishing() || isDestroyed() || game.iconView == null) return;
                                game.icon = bitmap;
                                game.iconView.setImageBitmap(bitmap);
                                if (game.placeholder != null) game.placeholder.setVisibility(View.GONE);
                            });
                        });
                    }
                });
            } catch (Exception error) {
                android.util.Log.w("lcom-ng", "Cannot load remote game library", error);
                runOnUiThread(() -> {
                    if (isFinishing() || isDestroyed()) return;
                    loading.removeView(progress);
                    message.setText("Games could not be loaded.\nCheck your connection and try again.");
                    Button retry = action("Try again", false);
                    LinearLayout.LayoutParams size = new LinearLayout.LayoutParams(-2, dp(48));
                    size.topMargin = dp(20);
                    loading.addView(retry, size);
                    retry.setOnClickListener(view -> loadCatalog());
                });
            }
        });
    }

    private TextView text(String value, int size, int color) {
        TextView view = new TextView(this);
        view.setText(value);
        view.setTextSize(size);
        view.setTextColor(color);
        view.setFontFeatureSettings("kern");
        return view;
    }

    private GradientDrawable surface(int color, int stroke) {
        GradientDrawable drawable = new GradientDrawable();
        drawable.setColor(color);
        drawable.setCornerRadius(dp(20));
        if (stroke != 0) drawable.setStroke(dp(2), stroke);
        return drawable;
    }

    private GradientDrawable pillSurface(int color, int stroke) {
        GradientDrawable drawable = new GradientDrawable();
        drawable.setColor(color);
        drawable.setCornerRadius(dp(999));
        if (stroke != 0) drawable.setStroke(dp(2), stroke);
        return drawable;
    }

    private Drawable interactive(int color) {
        StateListDrawable states = new StateListDrawable();
        states.addState(new int[]{android.R.attr.state_focused}, surface(color, ACCENT));
        states.addState(new int[]{}, surface(color, 0));
        return new RippleDrawable(android.content.res.ColorStateList.valueOf(0x4472d6b0), states, surface(Color.WHITE, 0));
    }

    private Button action(String label, boolean icon) {
        Button button = new Button(this);
        button.setText(label);
        button.setTextColor(INK);
        button.setTextSize(14);
        button.setAllCaps(false);
        button.setMinWidth(dp(48));
        button.setMinHeight(dp(48));
        button.setPadding(icon ? 0 : dp(18), 0, icon ? 0 : dp(18), 0);
        button.setBackground(interactive(SURFACE));
        return button;
    }

    private void render() {
        restorePending = false;
        root = new LinearLayout(this);
        root.setOrientation(LinearLayout.VERTICAL);
        root.setBackgroundColor(BACKGROUND);
        root.setFocusableInTouchMode(true);
        root.setDefaultFocusHighlightEnabled(false);
        Configuration configuration = getResources().getConfiguration();
        boolean compactHeader = configuration.screenWidthDp >= 600 && configuration.screenHeightDp < 600;
        LinearLayout header = new LinearLayout(this);
        header.setOrientation(compactHeader ? LinearLayout.HORIZONTAL : LinearLayout.VERTICAL);
        header.setGravity(Gravity.CENTER_VERTICAL);
        header.setPadding(dp(4), dp(8), dp(4), dp(16));
        root.addView(header, new LinearLayout.LayoutParams(-1, -2));
        LinearLayout titleRow = new LinearLayout(this);
        titleRow.setGravity(Gravity.CENTER_VERTICAL);
        LinearLayout titleBlock = new LinearLayout(this);
        titleBlock.setOrientation(LinearLayout.VERTICAL);
        TextView heading = text(title, compactHeader ? 24 : 28, INK);
        heading.setTypeface(Typeface.DEFAULT, Typeface.BOLD);
        if (Build.VERSION.SDK_INT >= 28) heading.setAccessibilityHeading(true);
        titleBlock.addView(heading);
        summary = text("", 14, MUTED);
        summary.setPadding(0, dp(4), 0, 0);
        titleBlock.addView(summary);
        titleRow.addView(titleBlock, new LinearLayout.LayoutParams(0, -2, 1));
        surprise = action("", true);
        surprise.setContentDescription("Choose a random game");
        surprise.setTooltipText("Choose a random game");
        surprise.setForeground(new ControlIcon(false));
        surprise.setOnClickListener(view -> {
            ArrayList<Game> matches = matchingGames();
            ArrayList<Game> installed = new ArrayList<>();
            for (Game game : matches) if (game.installed) installed.add(game);
            if (!installed.isEmpty()) matches = installed;
            if (!matches.isEmpty()) launch(matches.get(random.nextInt(matches.size())));
        });
        LinearLayout.LayoutParams randomSize = new LinearLayout.LayoutParams(dp(48), dp(48));
        randomSize.leftMargin = dp(12);
        titleRow.addView(surprise, randomSize);
        header.addView(titleRow, compactHeader
            ? new LinearLayout.LayoutParams(0, -2, 1) : new LinearLayout.LayoutParams(-1, -2));
        LinearLayout searchRow = new LinearLayout(this);
        searchRow.setGravity(Gravity.CENTER_VERTICAL);
        LinearLayout.LayoutParams rowSize = compactHeader
            ? new LinearLayout.LayoutParams(0, -2, 1) : new LinearLayout.LayoutParams(-1, -2);
        rowSize.topMargin = compactHeader ? 0 : dp(16);
        rowSize.leftMargin = compactHeader ? dp(24) : 0;
        header.addView(searchRow, rowSize);
        search = new EditText(this);
        search.setInputType(InputType.TYPE_CLASS_TEXT | InputType.TYPE_TEXT_VARIATION_FILTER
            | InputType.TYPE_TEXT_FLAG_NO_SUGGESTIONS);
        search.setSingleLine(true);
        search.setTextSize(16);
        search.setTextColor(INK);
        search.setHintTextColor(MUTED);
        search.setHint("Find a game");
        search.setContentDescription("Find a game");
        search.setPadding(dp(16), 0, dp(16), 0);
        search.setMinHeight(dp(52));
        search.setImeOptions(EditorInfo.IME_ACTION_SEARCH | EditorInfo.IME_FLAG_NO_EXTRACT_UI);
        search.setBackground(interactive(0xff121815));
        search.setText(query);
        searchRow.addView(search, new LinearLayout.LayoutParams(0, dp(52), 1));
        clear = action("", true);
        clear.setContentDescription("Clear search");
        clear.setTooltipText("Clear search");
        clear.setForeground(new ControlIcon(true));
        clear.setOnClickListener(view -> search.setText(""));
        LinearLayout.LayoutParams clearSize = new LinearLayout.LayoutParams(dp(48), dp(48));
        clearSize.leftMargin = dp(8);
        searchRow.addView(clear, clearSize);
        LinearLayout chips = new LinearLayout(this);
        chips.setGravity(Gravity.CENTER_VERTICAL);
        LinearLayout.LayoutParams chipsSize = new LinearLayout.LayoutParams(-1, -2);
        chipsSize.topMargin = dp(12);
        header.addView(chips, chipsSize);
        allChip = filterChip("All");
        allChip.setOnClickListener(view -> setFilter(false));
        LinearLayout.LayoutParams allSize = new LinearLayout.LayoutParams(-2, dp(40));
        chips.addView(allChip, allSize);
        downloadedChip = filterChip("Downloaded");
        downloadedChip.setOnClickListener(view -> setFilter(true));
        LinearLayout.LayoutParams downloadedSize = new LinearLayout.LayoutParams(-2, dp(40));
        downloadedSize.leftMargin = dp(8);
        chips.addView(downloadedChip, downloadedSize);
        search.setOnEditorActionListener((view, action, event) -> {
            if (action != EditorInfo.IME_ACTION_SEARCH &&
                    (event == null || event.getKeyCode() != android.view.KeyEvent.KEYCODE_ENTER)) return false;
            hideKeyboard();
            root.requestFocus();
            return true;
        });
        search.addTextChangedListener(new TextWatcher() {
            @Override public void beforeTextChanged(CharSequence value, int start, int count, int after) {}
            @Override public void onTextChanged(CharSequence value, int start, int before, int count) {
                query = value.toString();
                anchorId = null;
                // Rebuilding the whole grid on every keystroke is janky. Debounce
                // until typing pauses, and ignore stale filters after rotation.
                if (pendingFilter != null) mainHandler.removeCallbacks(pendingFilter);
                pendingFilter = () -> {
                    pendingFilter = null;
                    if (isFinishing() || isDestroyed() || grid == null) return;
                    renderGames();
                    scroll.scrollTo(0, 0);
                };
                mainHandler.postDelayed(pendingFilter, 280);
            }
            @Override public void afterTextChanged(Editable value) {}
        });
        scroll = new ScrollView(this);
        scroll.setFillViewport(true);
        scroll.setClipToPadding(false);
        root.addView(scroll, new LinearLayout.LayoutParams(-1, 0, 1));
        content = new LinearLayout(this);
        content.setOrientation(LinearLayout.VERTICAL);
        content.setPadding(0, 0, 0, dp(12));
        scroll.addView(content, new ScrollView.LayoutParams(-1, -2));
        grid = new GridLayout(this);
        columns = columnCount();
        grid.setColumnCount(columns);
        grid.setAlignmentMode(GridLayout.ALIGN_BOUNDS);
        grid.setUseDefaultMargins(false);
        content.addView(grid, new LinearLayout.LayoutParams(-1, -2));
        empty = text("No games match. Try another name.", 16, MUTED);
        empty.setGravity(Gravity.CENTER);
        empty.setPadding(dp(16), dp(40), dp(16), dp(40));
        content.addView(empty, new LinearLayout.LayoutParams(-1, -2));
        renderGames();
        setContentView(root);
        applyPadding();
        root.setOnApplyWindowInsetsListener((view, insets) -> {
            int left = 0, right = 0;
            topInset = bottomInset = 0;
            if (Build.VERSION.SDK_INT >= 28 && insets.getDisplayCutout() != null) {
                left = insets.getDisplayCutout().getSafeInsetLeft();
                right = insets.getDisplayCutout().getSafeInsetRight();
                topInset = insets.getDisplayCutout().getSafeInsetTop();
                bottomInset = insets.getDisplayCutout().getSafeInsetBottom();
            }
            if (Build.VERSION.SDK_INT >= 30) {
                bottomInset = Math.max(bottomInset, insets.getInsets(WindowInsets.Type.ime() | WindowInsets.Type.mandatorySystemGestures()).bottom);
            } else {
                bottomInset = Math.max(bottomInset, insets.getSystemWindowInsetBottom());
            }
            sideInset = Math.max(left, right);
            applyPadding();
            reflowGrid();
            return insets;
        });
        root.requestApplyInsets();
        immerse();
    }

    private void applyPadding() {
        root.setPadding(dp(12) + sideInset, dp(16) + topInset, dp(12) + sideInset, bottomInset);
    }

    private ArrayList<Game> matchingGames() {
        ArrayList<Game> matches = new ArrayList<>();
        String filter = query.trim().toLowerCase(Locale.ROOT);
        for (Game game : games) {
            if (installedOnly && !game.installed) continue;
            if (game.name.toLowerCase(Locale.ROOT).contains(filter)) matches.add(game);
        }
        return matches;
    }

    private Button filterChip(String label) {
        Button chip = action(label, false);
        chip.setMinWidth(0);
        chip.setMinHeight(dp(40));
        chip.setPadding(dp(16), 0, dp(16), 0);
        chip.setTextSize(13);
        chip.setTypeface(Typeface.DEFAULT, Typeface.BOLD);
        return chip;
    }

    private void setFilter(boolean downloaded) {
        if (installedOnly == downloaded) return;
        installedOnly = downloaded;
        anchorId = null;
        updateFilterChips();
        renderGames();
        scroll.scrollTo(0, 0);
    }

    private void updateFilterChips() {
        if (allChip == null) return;
        int installed = 0;
        for (Game game : games) if (game.installed) installed++;
        allChip.setText("All");
        downloadedChip.setText(installed > 0 ? "Downloaded · " + installed : "Downloaded");
        allChip.setTextColor(installedOnly ? MUTED : ACCENT);
        downloadedChip.setTextColor(installedOnly ? ACCENT : MUTED);
        allChip.setBackground(installedOnly ? pillSurface(0xff1d2521, 0) : pillSurface(0x2272d6b0, 0));
        downloadedChip.setBackground(installedOnly ? pillSurface(0x2272d6b0, 0) : pillSurface(0xff1d2521, 0));
    }

    /** Recompute every game's install state away from the UI thread, then re-render. */
    private void refreshAndRender() {
        io.execute(() -> {
            refreshStates(games);
            runOnUiThread(() -> {
                if (isFinishing() || isDestroyed() || grid == null) return;
                renderGames();
            });
        });
    }

    private void refreshStates(ArrayList<Game> list) {
        java.util.HashSet<String> known = new java.util.HashSet<>();
        for (Game game : list) known.add(game.id);
        GameInstaller.prune(this, known);
        long bytes = 0;
        for (Game game : list) {
            JSONObject manifest = GameInstaller.installedManifest(this, game.id);
            CatalogGame.Version version = game.data.version();
            game.updateAvailable = manifest != null && version != null
                && !version.version.equals(manifest.optString("version", ""));
            game.installed = manifest != null && !game.updateAvailable;
            if (game.installed) bytes += GameInstaller.gameBytes(this, game.id);
        }
        cachedBytes = bytes;
    }

    private void hideKeyboard() {
        InputMethodManager keyboard = (InputMethodManager) getSystemService(INPUT_METHOD_SERVICE);
        if (keyboard != null) keyboard.hideSoftInputFromWindow(root.getWindowToken(), 0);
    }

    private void launch(Game game) {
        if (!GameInstaller.isInstalled(this, game.data)) {
            download(game);
            return;
        }
        hideKeyboard();
        root.requestFocus();
        startActivity(new Intent(this, GameActivity.class).putExtra("game", game.id));
    }

    private void download(Game game) {
        if (game.busy) return;
        CatalogGame.Version version = game.data.version();
        if (version == null) {
            Toast.makeText(this, "No download is available for this device.", Toast.LENGTH_LONG).show();
            return;
        }
        game.busy = true;
        updateStatus(game, 0, 0);
        io.execute(() -> {
            try {
                GameInstaller.install(this, game.data, (read, total) -> runOnUiThread(() -> {
                    if (isFinishing() || isDestroyed() || game.busy != true) return;
                    updateStatus(game, read, total);
                }));
                runOnUiThread(() -> {
                    game.busy = false;
                    game.downloadPercent = 0;
                    if (isFinishing() || isDestroyed()) return;
                    refreshAndRender();
                    launch(game);
                });
            } catch (Exception error) {
                android.util.Log.e("lcom-ng", "Cannot install " + game.id, error);
                runOnUiThread(() -> {
                    game.busy = false;
                    game.downloadPercent = 0;
                    if (isFinishing() || isDestroyed()) return;
                    if (game.status != null) {
                        game.status.setText("Failed · tap to retry");
                        game.status.setTextColor(0xffff8a80);
                    }
                    Toast.makeText(this, "Download failed. Check your connection and try again.", Toast.LENGTH_LONG).show();
                });
            }
        });
    }

    private void updateStatus(Game game, long read, long total) {
        if (game.status == null) return;
        int percent = total > 0 ? (int) Math.min(100, read * 100 / total) : 0;
        game.downloadPercent = percent;
        game.status.setTextColor(ACCENT);
        game.status.setText(percent > 0 ? percent + "%" : "…");
        GradientDrawable pill = new GradientDrawable();
        pill.setCornerRadius(dp(999));
        pill.setColor(0x2272d6b0);
        game.status.setBackground(pill);
        if (game.progress != null) {
            game.progress.setVisibility(View.VISIBLE);
            game.progress.setProgress(percent);
        }
    }

    private String instructionText(JSONObject instructions) {
        if (instructions == null) return "";
        JSONArray sections = instructions.optJSONArray("sections");
        if (sections == null) return "";
        StringBuilder text = new StringBuilder();
        for (int index = 0; index < sections.length(); index++) {
            JSONObject section = sections.optJSONObject(index);
            if (section == null) continue;
            String title = section.optString("title", "");
            String body = section.optString("text", "");
            StringBuilder rows = new StringBuilder();
            JSONArray controls = section.optJSONArray("controls");
            if (controls != null) for (int row = 0; row < controls.length(); row++) {
                JSONObject control = controls.optJSONObject(row);
                if (control == null) continue;
                StringBuilder keys = new StringBuilder();
                JSONArray bindings = control.optJSONArray("bindings");
                if (bindings != null) for (int binding = 0; binding < bindings.length(); binding++) {
                    JSONObject value = bindings.optJSONObject(binding);
                    if (value == null) continue;
                    if (keys.length() > 0) keys.append(" or ");
                    String label = value.optString("label", "");
                    if (!label.isEmpty()) { keys.append(label); continue; }
                    JSONArray values = value.optJSONArray("keys");
                    if (values == null) continue;
                    for (int key = 0; key < values.length(); key++) {
                        if (key > 0) keys.append("+");
                        keys.append(values.optString(key));
                    }
                }
                if (keys.length() > 0) rows.append("\n").append(control.optString("action", "")).append(": ").append(keys);
            }
            if (title.isEmpty() && body.isEmpty() && rows.length() == 0) continue;
            text.append("\n\n");
            if (!title.isEmpty()) text.append(title).append("\n");
            if (!body.isEmpty()) text.append(body).append("\n");
            text.append(rows.toString().trim());
        }
        String result = text.toString().trim();
        return result.isEmpty() ? "" : "\n\n" + result;
    }

    private void showGameDialog(Game game) {
        hideKeyboard();
        LinearLayout panel = new LinearLayout(this);
        panel.setOrientation(LinearLayout.VERTICAL);
        TextView heading = text(game.name, 22, INK);
        heading.setTypeface(Typeface.DEFAULT, Typeface.BOLD);
        panel.addView(heading);
        if (!game.data.description.isEmpty()) {
            TextView description = text(game.data.description, 15, MUTED);
            description.setPadding(0, dp(10), 0, dp(4));
            panel.addView(description);
        }
        Dialog[] holder = new Dialog[1];
        if (game.installed) {
            panel.addView(dialogRow("Play", ACCENT, 0x2272d6b0, () -> { holder[0].dismiss(); launch(game); }));
        }
        if (!game.installed || game.updateAvailable) {
            panel.addView(dialogRow(game.installed ? "Update" : "Download and play", ACCENT, 0x2272d6b0,
                () -> { holder[0].dismiss(); download(game); }));
        }
        if (game.data.instructions != null) {
            panel.addView(dialogRow("Controls", INK, 0xff1d2521, () -> { holder[0].dismiss(); showControls(game); }));
        }
        if (game.installed) {
            panel.addView(dialogRow("Delete download", 0xffff8a80, 0xff241b1b, () -> {
                holder[0].dismiss();
                confirmDelete(game);
            }));
        }
        panel.addView(dialogRow("Close", INK, 0xff1d2521, () -> holder[0].dismiss()));
        holder[0] = showCard(panel);
    }

    private void showControls(Game game) {
        String body = instructionText(game.data.instructions).trim();
        if (body.isEmpty()) return;
        LinearLayout panel = new LinearLayout(this);
        panel.setOrientation(LinearLayout.VERTICAL);
        TextView heading = text("Controls", 22, INK);
        heading.setTypeface(Typeface.DEFAULT, Typeface.BOLD);
        panel.addView(heading);
        TextView message = text(body, 15, INK);
        message.setPadding(0, dp(14), 0, dp(10));
        panel.addView(message);
        Dialog dialog = showCard(panel);
        panel.addView(dialogRow("Close", INK, 0xff1d2521, dialog::dismiss));
    }

    private void confirmDelete(Game game) {
        LinearLayout panel = new LinearLayout(this);
        panel.setOrientation(LinearLayout.VERTICAL);
        TextView heading = text("Delete " + game.name + "?", 20, INK);
        heading.setTypeface(Typeface.DEFAULT, Typeface.BOLD);
        panel.addView(heading);
        TextView message = text("The installed game files will be removed. You can download it again later.", 15, MUTED);
        message.setPadding(0, dp(10), 0, dp(4));
        panel.addView(message);
        Dialog dialog = showCard(panel);
        panel.addView(dialogRow("Delete", 0xffff8a80, 0xff241b1b, () -> {
            dialog.dismiss();
            GameInstaller.delete(this, game.id);
            refreshAndRender();
        }));
        panel.addView(dialogRow("Cancel", INK, 0xff1d2521, dialog::dismiss));
    }

    private Button dialogRow(String label, int textColor, int fillColor, Runnable action) {
        Button row = action(label, false);
        row.setTextColor(textColor);
        row.setBackground(interactive(fillColor));
        row.setGravity(Gravity.CENTER);
        LinearLayout.LayoutParams size = new LinearLayout.LayoutParams(-1, dp(50));
        size.topMargin = dp(8);
        row.setLayoutParams(size);
        row.setOnClickListener(view -> action.run());
        return row;
    }

    /** Show a panel inside a rounded dark card over a dimmed scrim. */
    private Dialog showCard(LinearLayout panel) {
        LinearLayout card = new LinearLayout(this);
        card.setOrientation(LinearLayout.VERTICAL);
        card.setPadding(dp(22), dp(22), dp(22), dp(14));
        card.setBackground(surface(0xff121a16, 0));
        card.addView(panel);
        ScrollView wrapper = new ScrollView(this);
        wrapper.addView(card, new FrameLayout.LayoutParams(-1, -2));
        FrameLayout outer = new FrameLayout(this);
        outer.setPadding(dp(16), dp(16), dp(16), dp(16));
        outer.addView(wrapper, new FrameLayout.LayoutParams(-1, -2, Gravity.CENTER));
        Dialog dialog = new Dialog(this);
        dialog.requestWindowFeature(Window.FEATURE_NO_TITLE);
        dialog.setContentView(outer);
        dialog.setCanceledOnTouchOutside(true);
        Window window = dialog.getWindow();
        if (window != null) {
            window.setBackgroundDrawable(new android.graphics.drawable.ColorDrawable(Color.TRANSPARENT));
            window.setLayout(-1, -1);
            window.setDimAmount(0.5f);
        }
        dialog.show();
        return dialog;
    }

    @Override protected void onDestroy() {
        mainHandler.removeCallbacksAndMessages(null);
        io.shutdownNow();
        artwork.shutdownNow();
        super.onDestroy();
    }

    private void renderGames() {
        grid.removeAllViews();
        for (Game game : games) game.tile = null;
        ArrayList<Game> matches = matchingGames();
        float fontScale = Math.max(1f, getResources().getConfiguration().fontScale);
        int availableWidth = getResources().getDisplayMetrics().widthPixels - dp(24) - 2 * sideInset;
        int gap = dp(10);
        int blockWidth = Math.max(dp(120), availableWidth / columns - gap);
        int iconSize = Math.min(dp(300), Math.max(dp(96), blockWidth));
        int tileHeight = iconSize + Math.round(dp(46 * fontScale)) + dp(48);
        LinkedHashMap<String, ArrayList<Game>> families = new LinkedHashMap<>();
        if (columns >= 2) for (Game game : matches) if (!game.archetype.isEmpty()) {
            if (!families.containsKey(game.archetype)) families.put(game.archetype, new ArrayList<>());
            families.get(game.archetype).add(game);
        }
        LinkedHashMap<Game, ArrayList<Game>> clusters = new LinkedHashMap<>();
        HashSet<Game> grouped = new HashSet<>();
        for (ArrayList<Game> family : families.values()) for (int start = 0; start + 9 <= family.size(); start += 9) {
            ArrayList<Game> members = new ArrayList<>(family.subList(start, start + 9));
            clusters.put(members.get(0), members);
            grouped.addAll(members);
        }
        ArrayList<boolean[]> occupied = new ArrayList<>();
        for (Game game : matches) {
            ArrayList<Game> members = clusters.get(game);
            if (grouped.contains(game) && members == null) continue;
            int span = members == null ? 1 : 2;
            int row = 0, column = 0;
            boolean found = false;
            while (!found) {
                while (occupied.size() < row + span) occupied.add(new boolean[columns]);
                for (column = 0; column + span <= columns; column++) {
                    boolean free = true;
                    for (int y = row; y < row + span; y++) for (int x = column; x < column + span; x++)
                        if (occupied.get(y)[x]) free = false;
                    if (free) { found = true; break; }
                }
                if (!found) row++;
            }
            for (int y = row; y < row + span; y++) for (int x = column; x < column + span; x++) occupied.get(y)[x] = true;
            int pitch = blockWidth + gap;
            int width = pitch * span - gap;
            int height = (tileHeight + gap) * span - gap;
            View block;
            if (members == null) block = gameTile(game, iconSize, false);
            else {
                GridLayout cluster = new GridLayout(this);
                cluster.setColumnCount(3);
                cluster.setRowCount(3);
                int smallWidth = (width - dp(16)) / 3;
                int smallHeight = (height - dp(16)) / 3;
                int smallIcon = Math.max(dp(40), smallHeight - Math.round(dp(38 * fontScale)) - dp(34));
                for (int index = 0; index < members.size(); index++) {
                    GridLayout.LayoutParams small = new GridLayout.LayoutParams(
                        GridLayout.spec(index / 3), GridLayout.spec(index % 3));
                    small.width = smallWidth;
                    small.height = smallHeight;
                    small.setMargins(0, 0, index % 3 < 2 ? dp(8) : 0, index / 3 < 2 ? dp(8) : 0);
                    cluster.addView(gameTile(members.get(index), smallIcon, true), small);
                }
                block = cluster;
            }
            GridLayout.LayoutParams cell = new GridLayout.LayoutParams(
                GridLayout.spec(row, span, GridLayout.FILL), GridLayout.spec(column, span, GridLayout.FILL));
            cell.width = width;
            cell.height = height;
            cell.setMargins(dp(5), dp(5), dp(5), dp(5));
            grid.addView(block, cell);
        }
        empty.setVisibility(matches.isEmpty() ? View.VISIBLE : View.GONE);
        empty.setText(installedOnly && query.trim().isEmpty()
            ? "Nothing downloaded yet.\nTap a game to get it."
            : "No games match. Try another name.");
        int installed = 0;
        for (Game game : games) if (game.installed) installed++;
        String count = query.trim().isEmpty() ? games.size() + " games"
            : matches.size() + " of " + games.size() + " games";
        if (installed > 0) count += " · " + installed + " installed";
        if (cachedBytes > 0) count += " · " + formatBytes(cachedBytes) + " used";
        summary.setText(count);
        clear.setVisibility(query.isEmpty() ? View.GONE : View.VISIBLE);
        surprise.setEnabled(!matches.isEmpty());
        surprise.setAlpha(matches.isEmpty() ? .4f : 1f);
        updateFilterChips();
    }

    private View gameTile(Game game, int iconSize, boolean compact) {
        LinearLayout tile = new LinearLayout(this);
        game.tile = tile;
        tile.setOrientation(LinearLayout.VERTICAL);
        tile.setGravity(Gravity.CENTER_HORIZONTAL);
        tile.setBackground(interactive(SURFACE));
        tile.setClipToOutline(true);
        tile.setClickable(true);
        tile.setFocusable(true);
        tile.setContentDescription((game.installed ? "Play " : "Download ") + game.name);
        tile.setTooltipText(game.name);
        tile.setOnClickListener(view -> launch(game));
        tile.setOnLongClickListener(view -> { showGameDialog(game); return true; });

        FrameLayout art = new FrameLayout(this);
        ImageView artwork = new ImageView(this);
        artwork.setScaleType(ImageView.ScaleType.FIT_CENTER);
        artwork.setImportantForAccessibility(View.IMPORTANT_FOR_ACCESSIBILITY_NO);
        art.addView(artwork, new FrameLayout.LayoutParams(-1, -1));
        game.iconView = artwork;
        TextView initial = text(game.name.isEmpty() ? "?" : game.name.substring(0, 1).toUpperCase(Locale.ROOT), compact ? 24 : 34, ACCENT);
        initial.setGravity(Gravity.CENTER);
        initial.setImportantForAccessibility(View.IMPORTANT_FOR_ACCESSIBILITY_NO);
        art.addView(initial, new FrameLayout.LayoutParams(-1, -1));
        game.placeholder = initial;
        if (game.icon != null) {
            artwork.setImageBitmap(game.icon);
            initial.setVisibility(View.GONE);
        }
        tile.addView(art, new LinearLayout.LayoutParams(-1, iconSize));

        TextView label = text(game.name, compact ? 12 : 14, INK);
        label.setTypeface(Typeface.DEFAULT, Typeface.BOLD);
        label.setMaxLines(2);
        label.setEllipsize(TextUtils.TruncateAt.END);
        label.setGravity(Gravity.CENTER);
        label.setPadding(dp(4), dp(compact ? 6 : 10), dp(4), dp(compact ? 2 : 6));
        label.setImportantForAccessibility(View.IMPORTANT_FOR_ACCESSIBILITY_NO);
        tile.addView(label, new LinearLayout.LayoutParams(-1, -2));

        game.status = text("", compact ? 10 : 12, MUTED);
        game.status.setTypeface(Typeface.DEFAULT, Typeface.BOLD);
        game.status.setMaxLines(1);
        game.status.setPadding(dp(compact ? 8 : 12), dp(compact ? 2 : 4), dp(compact ? 8 : 12), dp(compact ? 2 : 4));
        game.status.setImportantForAccessibility(View.IMPORTANT_FOR_ACCESSIBILITY_NO);
        FrameLayout chipRow = new FrameLayout(this);
        chipRow.addView(game.status, new FrameLayout.LayoutParams(-2, -2, Gravity.CENTER));
        tile.addView(chipRow, new LinearLayout.LayoutParams(-1, -2));

        game.progress = new ProgressBar(this, null, android.R.attr.progressBarStyleHorizontal);
        game.progress.setMax(100);
        game.progress.getProgressDrawable().setColorFilter(ACCENT, android.graphics.PorterDuff.Mode.SRC_IN);
        game.progress.setVisibility(View.GONE);
        game.progress.setImportantForAccessibility(View.IMPORTANT_FOR_ACCESSIBILITY_NO);
        LinearLayout.LayoutParams barSize = new LinearLayout.LayoutParams(-1, dp(compact ? 3 : 4));
        barSize.topMargin = dp(2);
        barSize.leftMargin = barSize.rightMargin = dp(compact ? 8 : 10);
        tile.addView(game.progress, barSize);

        refreshStatus(game);
        return tile;
    }

    private void refreshStatus(Game game) {
        if (game.status == null) return;
        CatalogGame.Version version = game.data.version();
        GradientDrawable pill = new GradientDrawable();
        pill.setCornerRadius(dp(999));
        if (game.busy) {
            game.status.setText(game.downloadPercent > 0 ? game.downloadPercent + "%" : "…");
            game.status.setTextColor(ACCENT);
            pill.setColor(0x2272d6b0);
            if (game.progress != null) game.progress.setVisibility(View.VISIBLE);
        } else if (game.installed) {
            game.status.setText("Installed");
            game.status.setTextColor(ACCENT);
            pill.setColor(0x2272d6b0);
            if (game.progress != null) game.progress.setVisibility(View.GONE);
        } else if (game.updateAvailable) {
            game.status.setText("Update · " + (version == null ? "" : formatBytes(version.bytes)));
            game.status.setTextColor(ACCENT);
            pill.setColor(0x2272d6b0);
            if (game.progress != null) game.progress.setVisibility(View.GONE);
        } else if (version == null) {
            game.status.setText("Not available");
            game.status.setTextColor(MUTED);
            pill.setColor(0xff1d2521);
            if (game.progress != null) game.progress.setVisibility(View.GONE);
        } else {
            game.status.setText("Get · " + formatBytes(version.bytes));
            game.status.setTextColor(MUTED);
            pill.setColor(0xff1d2521);
            if (game.progress != null) game.progress.setVisibility(View.GONE);
        }
        game.status.setBackground(pill);
        game.tile.setContentDescription((game.installed ? "Play " : "Download ") + game.name);
    }

    private int tileTop(Game game) {
        Rect bounds = new Rect(0, 0, game.tile.getWidth(), game.tile.getHeight());
        grid.offsetDescendantRectToMyCoords(game.tile, bounds);
        return grid.getTop() + bounds.top;
    }

    private int columnCount() {
        int width = getResources().getConfiguration().screenWidthDp;
        if (width >= 900) return 4;
        if (width >= 600) return 3;
        return 2;
    }

    private void reflowGrid() {
        if (!resumed || grid == null || columns == columnCount()) return;
        rememberAnchor();
        render();
        restoreAnchorAfterLayout();
    }

    private void rememberAnchor() {
        if (!resumed || scroll == null || grid == null || restorePending || anchorId != null) return;
        int viewportTop = scroll.getScrollY();
        for (Game game : games) {
            if (game.tile == null || game.tile.getHeight() == 0) continue;
            int top = tileTop(game);
            if (top + game.tile.getHeight() <= viewportTop) continue;
            anchorId = game.id;
            anchorOffset = top - viewportTop;
            anchorHeight = game.tile.getHeight();
            return;
        }
    }

    private void restoreAnchorAfterLayout() {
        if (!resumed || scroll == null || anchorId == null || restorePending) return;
        restorePending = true;
        scroll.getViewTreeObserver().addOnPreDrawListener(new android.view.ViewTreeObserver.OnPreDrawListener() {
            @Override public boolean onPreDraw() {
                scroll.getViewTreeObserver().removeOnPreDrawListener(this);
                restorePending = false;
                for (Game game : games) if (game.id.equals(anchorId) && game.tile != null) {
                    int top = tileTop(game);
                    int offset = anchorOffset < 0 && anchorHeight > 0
                        ? Math.round((float) anchorOffset * game.tile.getHeight() / anchorHeight) : anchorOffset;
                    scroll.scrollTo(0, top - offset);
                    anchorId = null;
                    break;
                }
                return true;
            }
        });
        scroll.invalidate();
    }

    @Override public void onSaveInstanceState(Bundle state) {
        rememberAnchor();
        state.putString("query", query);
        state.putBoolean("installedOnly", installedOnly);
        state.putString("anchor", anchorId);
        state.putInt("anchorOffset", anchorOffset);
        state.putInt("anchorHeight", anchorHeight);
        super.onSaveInstanceState(state);
    }

    @Override public void onPause() {
        rememberAnchor();
        resumed = false;
        super.onPause();
    }

    @Override public void onResume() {
        super.onResume();
        resumed = true;
        if (grid != null) refreshAndRender();
        reflowGrid();
        restoreAnchorAfterLayout();
    }

    @Override public void onConfigurationChanged(Configuration config) {
        rememberAnchor();
        super.onConfigurationChanged(config);
        // Font-size changes need a rebuild even when the column count is stable.
        if (grid != null) render();
        restoreAnchorAfterLayout();
        immerse();
    }

    @Override public void onWindowFocusChanged(boolean focus) {
        super.onWindowFocusChanged(focus);
        if (focus) {
            immerse();
            restoreAnchorAfterLayout();
        }
    }

    private void immerse() {
        if (Build.VERSION.SDK_INT >= 30) {
            WindowInsetsController controller = getWindow().getInsetsController();
            if (controller != null) {
                controller.hide(WindowInsets.Type.systemBars());
                controller.setSystemBarsBehavior(WindowInsetsController.BEHAVIOR_SHOW_TRANSIENT_BARS_BY_SWIPE);
            }
        } else getWindow().getDecorView().setSystemUiVisibility(
            View.SYSTEM_UI_FLAG_FULLSCREEN | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION |
            View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN |
            View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION | View.SYSTEM_UI_FLAG_LAYOUT_STABLE);
    }

    /** The two utility actions share a single authored, 2 dp stroke family. */
    private final class ControlIcon extends Drawable {
        private final boolean close;
        private final Paint paint = new Paint(Paint.ANTI_ALIAS_FLAG);
        ControlIcon(boolean close) { this.close = close; paint.setStyle(Paint.Style.STROKE); paint.setStrokeCap(Paint.Cap.ROUND); paint.setStrokeJoin(Paint.Join.ROUND); }
        @Override public void draw(Canvas canvas) {
            canvas.save();
            canvas.translate(getBounds().centerX() - dp(12), getBounds().centerY() - dp(12));
            canvas.scale(dp(24) / 24f, dp(24) / 24f);
            paint.setColor(INK); paint.setStrokeWidth(2);
            if (close) {
                canvas.drawLine(6, 6, 18, 18, paint); canvas.drawLine(6, 18, 18, 6, paint);
            } else {
                android.graphics.Path path = new android.graphics.Path();
                path.moveTo(3, 6); path.lineTo(6, 6); path.cubicTo(10, 6, 14, 18, 18, 18); path.lineTo(21, 18);
                path.moveTo(18, 15); path.lineTo(21, 18); path.lineTo(18, 21);
                path.moveTo(3, 18); path.lineTo(6, 18); path.cubicTo(8, 18, 9, 15, 10, 13);
                path.moveTo(14, 9); path.cubicTo(15, 7, 16, 6, 18, 6); path.lineTo(21, 6);
                path.moveTo(18, 3); path.lineTo(21, 6); path.lineTo(18, 9);
                canvas.drawPath(path, paint);
            }
            canvas.restore();
        }
        @Override public void setAlpha(int alpha) { paint.setAlpha(alpha); }
        @Override public void setColorFilter(android.graphics.ColorFilter filter) { paint.setColorFilter(filter); }
        @Override public int getOpacity() { return android.graphics.PixelFormat.TRANSLUCENT; }
    }
}
