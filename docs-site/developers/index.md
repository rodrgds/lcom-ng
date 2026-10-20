# Developer guide

For people building lcom-ng itself: runtime code, SDKs, lab scaffolding, docs,
tests, examples, and releases.

## Audience boundaries

| Audience | Stable surface | Keep out |
| --- | --- | --- |
| Students | generated workspace, SDK headers, lab handouts, `lcom-ng test` | CMake, runtime C++, CI, reference solutions |
| Instructors | labs, examples, scripts, adoption docs, selected tests | accidental protocol internals |
| Developers | whole repository | unclear or internal-only copy in public pages |

## Source map

| Path | Ownership |
| --- | --- |
| `runtime/` | C++ host runtime and devices |
| `runtime/embedded/` | in-process runtime shared by web and Android exports |
| `runtime/cli/` | CLI file operations and lab catalog helpers |
| `sdk/include/lcom/` | public C headers copied into workspaces |
| `sdk/client/` | C client linked into student programs |
| `sdk/export/` | export configuration and shared source discovery |
| `sdk/android/` | Android packaging and launcher |
| `sdk/compat/` | Minix/LCF compatibility SDK and project builder |
| `course/labs/templates/` | starter contracts |
| `course/labs/docs/` | source lab handouts |
| `collections/arcade/` | Arcade manifest, metadata, galleries and publisher |
| `examples/community/` | pinned student sources and `UPSTREAM.json` provenance |
| `docs-site/` | public website |
| `dev/lab-solutions/` | reference implementations |
| `dev/lab-tests/` | predefined lab checks |

[Architecture](/developers/architecture) explains how these pieces fit together.
[Runtime and CLI](/developers/runtime-cli) covers day-to-day commands.

## Test gate

```sh
devenv shell -- lcom-ng-verify
```

`lcom-ng-test` runs the complete runtime and lab suite. During C/C++ iteration,
`lcom-ng-build` builds the tree, then run a focused target inside the Devenv
shell with `ctest --test-dir build -R <name> --output-on-failure`.

## Docs site

The public site is VitePress, deployed from `docs-site/` to Cloudflare Pages.
Build, asset sync, checks, and deployment details live in
[Docs site](/developers/docs-site).
