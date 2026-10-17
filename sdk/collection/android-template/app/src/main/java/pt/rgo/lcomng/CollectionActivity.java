package pt.rgo.lcomng;

import android.app.Activity;
import android.content.Intent;
import android.content.res.Configuration;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
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
import org.json.JSONArray;
import org.json.JSONObject;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.LinkedHashMap;
import android.graphics.Rect;
import android.text.TextUtils;
import java.util.Locale;
import java.util.Random;

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
    private Button clear, surprise;
    private TextView summary, empty;
    private String anchorId;
    private int columns, anchorOffset, anchorHeight, sideInset, topInset, bottomInset;
    private boolean restorePending, resumed;

    private static final class Game {
        String id, name, archetype;
        Bitmap icon;
        View tile;
    }

    private int dp(float value) { return Math.round(value * getResources().getDisplayMetrics().density); }

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
        message.setPadding(0, dp(20), 0, 0);
        loading.addView(message);
        setContentView(loading);
        immerse();
        // Decode bundled artwork away from the UI thread. A bad icon must not
        // make the other games inaccessible.
        new Thread(() -> {
            try {
                JSONObject catalog = CollectionCatalog.read(this);
                String loadedTitle = catalog.getString("name");
                ArrayList<Game> loaded = new ArrayList<>();
                JSONArray entries = catalog.getJSONArray("games");
                for (int index = 0; index < entries.length(); index++) {
                    JSONObject entry = entries.getJSONObject(index);
                    Game game = new Game();
                    game.id = entry.getString("id");
                    game.name = entry.getString("name");
                    game.archetype = entry.isNull("archetype") ? "" : entry.optString("archetype", "");
                    if (!entry.isNull("icon")) {
                        try (InputStream input = getAssets().open(entry.getString("icon"))) {
                            BitmapFactory.Options options = new BitmapFactory.Options();
                            options.inSampleSize = 2;
                            game.icon = BitmapFactory.decodeStream(input, null, options);
                        } catch (Exception error) {
                            android.util.Log.w("lcom-ng", "Cannot load game artwork: " + game.id, error);
                        }
                    }
                    loaded.add(game);
                }
                runOnUiThread(() -> {
                    if (isFinishing() || isDestroyed()) return;
                    title = loadedTitle;
                    games.clear();
                    games.addAll(loaded);
                    render();
                    restoreAnchorAfterLayout();
                });
            } catch (Exception error) {
                android.util.Log.e("lcom-ng", "Cannot load game library", error);
                runOnUiThread(() -> {
                    if (isFinishing() || isDestroyed()) return;
                    loading.removeView(progress);
                    message.setText("Could not load this game library.");
                    message.setGravity(Gravity.CENTER);
                    Button retry = action("Try again", false);
                    LinearLayout.LayoutParams size = new LinearLayout.LayoutParams(-2, dp(48));
                    size.topMargin = dp(20);
                    loading.addView(retry, size);
                    retry.setOnClickListener(view -> loadCatalog());
                });
            }
        }, "collection-artwork").start();
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
        drawable.setCornerRadius(dp(16));
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
        rowSize.topMargin = compactHeader ? 0 : dp(20);
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
        search.setBackground(interactive(SURFACE));
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
                renderGames();
                scroll.scrollTo(0, 0);
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
        for (Game game : games) if (game.name.toLowerCase(Locale.ROOT).contains(filter)) matches.add(game);
        return matches;
    }

    private void hideKeyboard() {
        InputMethodManager keyboard = (InputMethodManager) getSystemService(INPUT_METHOD_SERVICE);
        if (keyboard != null) keyboard.hideSoftInputFromWindow(root.getWindowToken(), 0);
    }

    private void launch(Game game) {
        hideKeyboard();
        root.requestFocus();
        startActivity(new Intent(this, GameActivity.class).putExtra("game", game.id));
    }

    private void renderGames() {
        grid.removeAllViews();
        for (Game game : games) game.tile = null;
        ArrayList<Game> matches = matchingGames();
        int iconSize = Math.max(dp(64), (getResources().getDisplayMetrics().widthPixels - dp(24) - 2 * sideInset) / columns - dp(12));
        int tileHeight = iconSize + dp(64 * Math.max(1f, getResources().getConfiguration().fontScale));
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
        int availableWidth = getResources().getDisplayMetrics().widthPixels - dp(24) - 2 * sideInset;
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
            int blockWidth = availableWidth * span / columns - dp(8);
            int blockHeight = (tileHeight + dp(8)) * span - dp(8);
            View block;
            if (members == null) block = gameTile(game, iconSize, false);
            else {
                GridLayout cluster = new GridLayout(this);
                cluster.setColumnCount(3);
                cluster.setRowCount(3);
                int smallHeight = (blockHeight - dp(16)) / 3;
                int smallIcon = Math.max(dp(24), smallHeight - dp(48 * Math.max(1f, getResources().getConfiguration().fontScale)));
                for (int index = 0; index < members.size(); index++) {
                    GridLayout.LayoutParams cell = new GridLayout.LayoutParams(GridLayout.spec(index / 3), GridLayout.spec(index % 3));
                    cell.width = (blockWidth - dp(16)) / 3;
                    cell.height = smallHeight;
                    cell.setMargins(0, 0, index % 3 < 2 ? dp(8) : 0, index / 3 < 2 ? dp(8) : 0);
                    cluster.addView(gameTile(members.get(index), smallIcon, true), cell);
                }
                block = cluster;
            }
            GridLayout.LayoutParams cell = new GridLayout.LayoutParams(GridLayout.spec(row, span, GridLayout.FILL), GridLayout.spec(column, span, GridLayout.FILL));
            cell.width = blockWidth;
            cell.height = blockHeight;
            cell.setMargins(dp(4), dp(4), dp(4), dp(4));
            grid.addView(block, cell);
        }
        empty.setVisibility(matches.isEmpty() ? View.VISIBLE : View.GONE);
        summary.setText(query.trim().isEmpty() ? games.size() + " games" : matches.size() + " of " + games.size() + " games");
        clear.setVisibility(query.isEmpty() ? View.GONE : View.VISIBLE);
        surprise.setEnabled(!matches.isEmpty());
        surprise.setAlpha(matches.isEmpty() ? .4f : 1f);
    }

    private View gameTile(Game game, int iconSize, boolean compact) {
        LinearLayout tile = new LinearLayout(this);
        game.tile = tile;
        tile.setOrientation(LinearLayout.VERTICAL);
        tile.setBackground(interactive(SURFACE));
        tile.setClipToOutline(true);
        tile.setClickable(true);
        tile.setFocusable(true);
        tile.setContentDescription("Play " + game.name);
        tile.setTooltipText(game.name);
        tile.setOnClickListener(view -> launch(game));
        if (game.icon != null) {
            ImageView artwork = new ImageView(this);
            artwork.setScaleType(ImageView.ScaleType.FIT_CENTER);
            artwork.setImageBitmap(game.icon);
            artwork.setImportantForAccessibility(View.IMPORTANT_FOR_ACCESSIBILITY_NO);
            tile.addView(artwork, new LinearLayout.LayoutParams(-1, iconSize));
        } else {
            TextView initial = text(game.name.isEmpty() ? "?" : game.name.substring(0, 1).toUpperCase(Locale.ROOT), compact ? 28 : 40, ACCENT);
            initial.setGravity(Gravity.CENTER);
            initial.setImportantForAccessibility(View.IMPORTANT_FOR_ACCESSIBILITY_NO);
            tile.addView(initial, new LinearLayout.LayoutParams(-1, iconSize));
        }
        TextView label = text(game.name, compact ? 12 : 14, INK);
        label.setTypeface(Typeface.DEFAULT, Typeface.BOLD);
        label.setMinLines(2);
        label.setMaxLines(2);
        label.setEllipsize(TextUtils.TruncateAt.END);
        label.setPadding(dp(compact ? 6 : 12), dp(compact ? 4 : 10), dp(compact ? 6 : 12), dp(8));
        label.setImportantForAccessibility(View.IMPORTANT_FOR_ACCESSIBILITY_NO);
        tile.addView(label, new LinearLayout.LayoutParams(-1, -2));
        return tile;
    }

    private int tileTop(Game game) {
        Rect bounds = new Rect(0, 0, game.tile.getWidth(), game.tile.getHeight());
        grid.offsetDescendantRectToMyCoords(game.tile, bounds);
        return grid.getTop() + bounds.top;
    }

    private int columnCount() {
        int width = getResources().getDisplayMetrics().widthPixels - dp(24) - 2 * sideInset;
        float fontScale = Math.max(1f, getResources().getConfiguration().fontScale);
        return Math.max(1, Math.min(6, width / dp(144 * Math.min(fontScale, 1.5f))));
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
