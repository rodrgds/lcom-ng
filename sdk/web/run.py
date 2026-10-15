#!/usr/bin/env python3
"""Build a game, serve its browser export, and open it for play."""

from functools import partial
from http.server import SimpleHTTPRequestHandler, ThreadingHTTPServer
import subprocess
import sys
import webbrowser

from export import argument_parser, build, compat


class GameHandler(SimpleHTTPRequestHandler):
    extensions_map = {**SimpleHTTPRequestHandler.extensions_map, ".wasm": "application/wasm"}

    def end_headers(self):
        self.send_header("Cache-Control", "no-store")
        super().end_headers()

    def log_message(self, *_):
        pass


def main():
    parser = argument_parser()
    parser.description = __doc__
    parser.add_argument("--port", type=int, default=0, help="HTTP port (default: choose a free port)")
    parser.add_argument("--host", default="127.0.0.1", help="listen address; use 0.0.0.0 for other devices")
    parser.add_argument("--no-open", action="store_true", help="serve without opening a browser")
    args = parser.parse_args()
    if not 0 <= args.port <= 65535:
        parser.error("--port must be between 0 and 65535")
    try:
        output = build(args)
        with ThreadingHTTPServer((args.host, args.port), partial(GameHandler, directory=str(output))) as server:
            host = "127.0.0.1" if args.host == "0.0.0.0" else args.host
            url = f"http://{host}:{server.server_port}/"
            print(f"\nPlay: {url}\nClick the game to capture the mouse. Right Ctrl releases it.\nPress Ctrl+C here to stop.", flush=True)
            if not args.no_open:
                webbrowser.open(url)
            try:
                server.serve_forever()
            except KeyboardInterrupt:
                pass
        return 0
    except (compat.BuildError, OSError, ValueError, subprocess.CalledProcessError) as error:
        print(f"lcom-ng web: {error}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    sys.exit(main())
