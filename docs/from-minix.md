# Run existing Minix labs and projects

[Install a release archive](../docs-site/students/install.md), then enter the folder
containing the original Makefile:

```sh
lcom-ng compat build .
lcom-ng run -- .lcom-ng/proj
```

Use the binary path printed by the builder. Its name comes from `PROG` in the
Makefile. Keep the complete project tree and run from the original project folder
so assets and sibling lab libraries remain available. Build again after edits.

Original labs use their usual LCF function arguments:

```sh
lcom-ng run -- .lcom-ng/lab2 timer_test_int 2
```

The importer reads the original Makefile and writes host binaries and source hashes
under `.lcom-ng/`. C files, includes and Makefiles stay unchanged. Minix executables
and compiled archives must be rebuilt for the host.

The [compatibility quick start](../docs-site/students/compatibility.md) and
[full reference](../docs-site/developers/from-minix.md) cover labs, games, libraries,
asset roots and device limits. The [trial workflow](compatibility-workflow.md)
records acceptance checks and the [decision trail](compatibility-decisions.tsv)
records changes and evidence.

Use the native `lcom_init` and `lcom_event_wait` API for new projects. Use the
compatibility SDK for existing Minix course code.
