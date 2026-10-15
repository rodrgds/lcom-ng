package pt.rgo.lcomng;

import android.app.Activity;
import android.content.Intent;
import android.content.res.Configuration;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Color;
import android.graphics.Typeface;
import android.graphics.drawable.GradientDrawable;
import android.os.Build;
import android.os.Bundle;
import android.view.Gravity;
import android.view.View;
import android.view.ViewGroup;
import android.view.WindowInsets;
import android.view.WindowInsetsController;
import android.widget.GridLayout;
import android.widget.EditText;
import android.text.Editable;
import android.text.TextWatcher;
import android.view.inputmethod.InputMethodManager;
import android.view.inputmethod.EditorInfo;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.ScrollView;
import android.widget.TextView;
import org.json.JSONArray;
import org.json.JSONObject;
import java.io.InputStream;
import java.util.ArrayList;

/** Native library UI. SDL and student code are loaded exclusively in :game. */
public final class CollectionActivity extends Activity {
    private final ArrayList<Game> games = new ArrayList<>();
    private String title = "Game library";
    private LinearLayout content;
    private ScrollView scroll;
    private GridLayout grid;
    private int columns;
    private String query = "";
    private TextView summary, empty;
    private String anchorId;
    private int anchorOffset, anchorHeight;
    private boolean restorePending, resumed;
    private int sideInset, topInset, bottomInset;

    private static final class Game {
        String id, name;
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
        TextView loading = text("Loading games…", 18, 0xfff4f7f5);
        loading.setGravity(Gravity.CENTER);
        loading.setBackgroundColor(0xff080a09);
        setContentView(loading);
        immerse();
        // Decoding a whole class's artwork must not block taps or activity startup.
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
                    if (!entry.isNull("icon")) {
                        try (InputStream input = getAssets().open(entry.getString("icon"))) {
                            BitmapFactory.Options options = new BitmapFactory.Options();
                            options.inSampleSize = 2;
                            game.icon = BitmapFactory.decodeStream(input, null, options);
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
                });
            } catch (Exception error) {
                android.util.Log.e("lcom-ng", "Cannot load game library", error);
                runOnUiThread(() -> loading.setText("Could not load this game library."));
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

    private void render() {
        scroll = new ScrollView(this);
        scroll.setBackgroundColor(0xff080a09);
        scroll.setFillViewport(true);
        content = new LinearLayout(this);
        content.setOrientation(LinearLayout.VERTICAL);
        content.setFocusableInTouchMode(true);
        content.setDefaultFocusHighlightEnabled(false);
        content.setPadding(dp(18) + sideInset, dp(28) + topInset, dp(18) + sideInset, dp(24) + bottomInset);
        scroll.addView(content, new ScrollView.LayoutParams(-1, -2));
        TextView heading = text(title, 30, 0xfff4f7f5);
        heading.setTypeface(Typeface.DEFAULT, Typeface.BOLD);
        content.addView(heading);
        summary = text("", 14, 0xffa9b3ad);
        summary.setPadding(0, dp(6), 0, dp(18));
        content.addView(summary);
        EditText search = new EditText(this);
        search.setSingleLine(true);
        search.setTextSize(16);
        search.setTextColor(0xfff4f7f5);
        search.setHintTextColor(0xffa9b3ad);
        search.setHint("Find a game");
        search.setContentDescription("Find a game");
        search.setPadding(dp(14), 0, dp(14), 0);
        search.setMinHeight(dp(48));
        search.setImeOptions(EditorInfo.IME_ACTION_DONE);
        GradientDrawable searchSurface = new GradientDrawable();
        searchSurface.setColor(0xff171d19);
        searchSurface.setCornerRadius(dp(8));
        searchSurface.setStroke(dp(1), 0xff38463d);
        search.setBackground(searchSurface);
        search.setText(query);
        LinearLayout.LayoutParams searchSize = new LinearLayout.LayoutParams(-1, dp(48));
        searchSize.bottomMargin = dp(18);
        content.addView(search, searchSize);
        search.setOnEditorActionListener((view, action, event) -> {
            if (action != EditorInfo.IME_ACTION_DONE) return false;
            hideKeyboard();
            content.requestFocus();
            return true;
        });
        search.addTextChangedListener(new TextWatcher() {
            @Override public void beforeTextChanged(CharSequence value, int start, int count, int after) {}
            @Override public void onTextChanged(CharSequence value, int start, int before, int count) {
                query = value.toString();
                anchorId = null;
                renderGames();
            }
            @Override public void afterTextChanged(Editable value) {}
        });
        grid = new GridLayout(this);
        columns = columnCount();
        grid.setColumnCount(columns);
        grid.setAlignmentMode(GridLayout.ALIGN_BOUNDS);
        grid.setUseDefaultMargins(false);
        content.addView(grid, new LinearLayout.LayoutParams(-1, -2));
        empty = text("No games match. Try another name.", 16, 0xffa9b3ad);
        empty.setPadding(0, dp(18), 0, dp(18));
        content.addView(empty);
        renderGames();
        setContentView(scroll);
        scroll.setOnApplyWindowInsetsListener((view, insets) -> {
            int left = 0, right = 0;
            topInset = bottomInset = 0;
            if (Build.VERSION.SDK_INT >= 28 && insets.getDisplayCutout() != null) {
                left = insets.getDisplayCutout().getSafeInsetLeft();
                right = insets.getDisplayCutout().getSafeInsetRight();
                topInset = insets.getDisplayCutout().getSafeInsetTop();
                bottomInset = insets.getDisplayCutout().getSafeInsetBottom();
            }
            sideInset = Math.max(left, right);
            content.setPadding(dp(18) + left, dp(28) + topInset, dp(18) + right, dp(24) + bottomInset);
            reflowGrid();
            return insets;
        });
        scroll.requestApplyInsets();
        immerse();
    }

    private void hideKeyboard() {
        InputMethodManager keyboard = (InputMethodManager) getSystemService(INPUT_METHOD_SERVICE);
        if (keyboard != null) keyboard.hideSoftInputFromWindow(content.getWindowToken(), 0);
    }

    private void renderGames() {
        grid.removeAllViews();
        int visible = 0;
        String filter = query.trim().toLowerCase(java.util.Locale.ROOT);
        for (Game game : games) {
            game.tile = null;
            if (!game.name.toLowerCase(java.util.Locale.ROOT).contains(filter)) continue;
            visible++;
            LinearLayout tile = new LinearLayout(this);
            game.tile = tile;
            tile.setOrientation(LinearLayout.VERTICAL);
            tile.setGravity(Gravity.TOP | Gravity.CENTER_HORIZONTAL);
            tile.setPadding(dp(6), dp(6), dp(6), dp(8));
            GradientDrawable surface = new GradientDrawable();
            surface.setColor(0xff171d19);
            surface.setCornerRadius(dp(12));
            android.content.res.ColorStateList rippleColor = android.content.res.ColorStateList.valueOf(0x4472d6b0);
            tile.setBackground(new android.graphics.drawable.RippleDrawable(rippleColor, surface, null));
            tile.setClickable(true);
            tile.setFocusable(true);
            tile.setContentDescription("Play " + game.name);
            tile.setOnClickListener(view -> {
                hideKeyboard();
                content.requestFocus();
                startActivity(new Intent(this, GameActivity.class).putExtra("game", game.id));
            });
            GridLayout.LayoutParams cell = new GridLayout.LayoutParams(GridLayout.spec(GridLayout.UNDEFINED), GridLayout.spec(GridLayout.UNDEFINED, 1f));
            cell.width = 0;
            cell.height = ViewGroup.LayoutParams.WRAP_CONTENT;
            cell.setMargins(dp(4), dp(4), dp(4), dp(6));
            grid.addView(tile, cell);
            ImageView artwork = new ImageView(this);
            artwork.setScaleType(ImageView.ScaleType.FIT_CENTER);
            artwork.setImportantForAccessibility(View.IMPORTANT_FOR_ACCESSIBILITY_NO);
            if (game.icon != null) artwork.setImageBitmap(game.icon);
            else artwork.setImageResource(android.R.drawable.ic_media_play);
            GradientDrawable outline = new GradientDrawable();
            outline.setColor(0xff080a09);
            outline.setCornerRadius(dp(12));
            artwork.setBackground(outline);
            artwork.setClipToOutline(true);
            tile.addView(artwork, new LinearLayout.LayoutParams(dp(64), dp(64)));
            TextView label = text(game.name, 12, 0xfff4f7f5);
            label.setGravity(Gravity.TOP | Gravity.CENTER_HORIZONTAL);
            label.setTypeface(Typeface.DEFAULT, Typeface.BOLD);
            label.setMinLines(2);
            label.setPadding(0, dp(6), 0, 0);
            tile.addView(label, new LinearLayout.LayoutParams(-1, -2));
        }
        int blanks = (columns - visible % columns) % columns;
        for (int index = 0; index < blanks; index++) {
            View blank = new View(this);
            GridLayout.LayoutParams cell = new GridLayout.LayoutParams(GridLayout.spec(GridLayout.UNDEFINED), GridLayout.spec(GridLayout.UNDEFINED, 1f));
            cell.width = 0;
            cell.height = 0;
            cell.setMargins(dp(4), 0, dp(4), 0);
            grid.addView(blank, cell);
        }
        empty.setVisibility(visible == 0 ? View.VISIBLE : View.GONE);
        summary.setText(filter.isEmpty()
            ? games.size() + " student games · Pick something to play"
            : visible + " of " + games.size() + " games");
    }

    private int columnCount() {
        int width = getResources().getDisplayMetrics().widthPixels - dp(36) - 2*sideInset;
        float fontScale = Math.max(1f,getResources().getConfiguration().fontScale);
        return Math.max(1, Math.min(6, width / dp(92*fontScale)));
    }

    private void reflowGrid() {
        if (!resumed || grid == null || columns == columnCount()) return;
        rememberAnchor();
        // Rebuild column specs and last-row spacers together. Keep the first
        // visible game's identity, including its clipped fraction, across it.
        render();
        restoreAnchorAfterLayout();
    }

    private void rememberAnchor() {
        if (!resumed || scroll == null || grid == null || restorePending || anchorId != null) return;
        int viewportTop = scroll.getScrollY();
        for (Game game : games) {
            if (game.tile == null || game.tile.getHeight() == 0) continue;
            int top = content.getTop() + grid.getTop() + game.tile.getTop();
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
                    int top = content.getTop() + grid.getTop() + game.tile.getTop();
                    // Keep a partially visible row partially visible when its
                    // artwork becomes wider or narrower after rotation.
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
        reflowGrid();
        if (content != null) content.requestApplyInsets();
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
}
