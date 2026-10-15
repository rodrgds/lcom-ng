package pt.rgo.lcomng;

import org.libsdl.app.SDLActivity;
import java.util.concurrent.atomic.AtomicReference;

/** Give unchanged desktop/Minix projects room for stack-resident framebuffers. */
final class NativeGameThread {
    private NativeGameThread() { }

    static void run(Runnable main) {
        AtomicReference<Throwable> failure = new AtomicReference<>();
        Thread worker = new Thread(null, () -> {
            SDLActivity.nativeInitMainThread();
            try { main.run(); }
            catch (Throwable error) { failure.set(error); }
            finally { SDLActivity.nativeCleanupMainThread(); }
        }, "LCOMGame", 16 * 1024 * 1024);
        worker.start();
        boolean interrupted = false;
        while (true) {
            try { worker.join(); break; }
            catch (InterruptedException error) { interrupted = true; }
        }
        if (interrupted) Thread.currentThread().interrupt();
        Throwable error = failure.get();
        if (error instanceof Error) throw (Error) error;
        if (error instanceof RuntimeException) throw (RuntimeException) error;
        if (error != null) throw new RuntimeException("Native game failed", error);
    }
}
