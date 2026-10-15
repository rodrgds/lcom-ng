#!/usr/bin/env python3
"""Compile and exercise collection chrome placement without an Android device."""
from pathlib import Path
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[1]


def main():
    with tempfile.TemporaryDirectory(prefix="lcom Android layout ") as temporary:
        root = Path(temporary)
        fixture = root / "ReturnLayoutContract.java"
        fixture.write_text('''package pt.rgo.lcomng;
public final class ReturnLayoutContract {
    static boolean intersects(int[] a,int[] b) {
        return a[0]<b[2] && a[2]>b[0] && a[1]<b[3] && a[3]>b[1];
    }
    static void check(int[] safe,int[] game,int[][] controls,boolean gutter) {
        int[] rect=ReturnButtonLayout.place(safe,121,10,game,controls);
        if(rect==null || rect[0]<safe[0] || rect[1]<safe[1] || rect[2]>safe[2] || rect[3]>safe[3])
            throw new AssertionError("Return action is outside the safe display");
        if(rect[2]-rect[0]!=121 || rect[3]-rect[1]!=121)
            throw new AssertionError("Return action lost its SDK-sized hit target");
        for(int[] control:controls) if(intersects(rect,control))
            throw new AssertionError("Return action covers an SDK control");
        if(gutter && intersects(rect,game)) throw new AssertionError("Return action covers framebuffer despite a usable gutter");
    }
    public static void main(String[] args) {
        int[][] toggle={{100,85,221,206}};
        // 800x600 PvZ in a wide phone, with a left controls toggle and cutout.
        check(new int[]{78,63,2322,1037},new int[]{467,0,1933,1100},toggle,true);
        // No letterbox: keep controls independent while overlaying guest pixels.
        check(new int[]{78,63,2322,1037},new int[]{0,0,2400,1100},toggle,false);
        // Portrait top/bottom letterbox and a bottom control panel.
        check(new int[]{0,96,1100,2337},new int[]{0,787,1100,1612},
            new int[][]{{22,118,143,239},{22,1850,1078,2300}},true);
        System.out.println("Native return chrome geometry passed");
    }
}
''')
        source=ROOT / "sdk/collection/android-template/app/src/main/java/pt/rgo/lcomng/ReturnButtonLayout.java"
        subprocess.run(["javac","-d",str(root),str(source),str(fixture)],check=True)
        subprocess.run(["java","-cp",str(root),"pt.rgo.lcomng.ReturnLayoutContract"],check=True)


if __name__ == "__main__": main()
