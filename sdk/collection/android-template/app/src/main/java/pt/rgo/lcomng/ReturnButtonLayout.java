package pt.rgo.lcomng;

import java.util.ArrayList;

/** Finds free native chrome space using physical framebuffer/control rectangles. */
final class ReturnButtonLayout {
    // Rectangles are [left, top, right, bottom] in decor coordinates.
    static int[] place(int[] safe, int size, int gap, int[] game, int[][] controls) {
        size = Math.min(size, Math.min(safe[2]-safe[0], safe[3]-safe[1]));
        ArrayList<Integer> xs = new ArrayList<>(), ys = new ArrayList<>();
        xs.add(safe[0]); xs.add(safe[2]-size);
        ys.add(safe[1]); ys.add(safe[3]-size);
        ArrayList<int[]> obstacles = new ArrayList<>();
        obstacles.add(game);
        for (int[] rect : controls) obstacles.add(rect);
        for (int[] rect : obstacles) {
            xs.add(rect[0]-size-gap); xs.add(rect[2]+gap);
            ys.add(rect[1]-size-gap); ys.add(rect[3]+gap);
        }
        int[] best = null;
        long bestScore = Long.MAX_VALUE;
        for (int x : xs) for (int y : ys) {
            if (x < safe[0] || y < safe[1] || x+size > safe[2] || y+size > safe[3]) continue;
            int[] candidate = {x,y,x+size,y+size};
            boolean collision = false;
            for (int[] rect : controls) if (overlap(candidate,rect,gap) > 0) { collision = true; break; }
            if (collision) continue;
            // A real gutter wins over covering any guest pixels. Within it,
            // prefer the top/left corner near the SDK's controls toggle.
            long score = overlap(candidate,game,0) * 100000000L
                + (long)(x-safe[0]+y-safe[1])*10000 + x-safe[0];
            if (score < bestScore) { best = candidate; bestScore = score; }
        }
        return best;
    }

    private static long overlap(int[] a, int[] b, int gap) {
        return (long)Math.max(0, Math.min(a[2],b[2]+gap)-Math.max(a[0],b[0]-gap))
            * Math.max(0, Math.min(a[3],b[3]+gap)-Math.max(a[1],b[1]-gap));
    }
}
