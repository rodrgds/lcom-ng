#!/usr/bin/env python3
"""Build declarative Minix LCOM Makefiles without running their recipes.

Source lists, include paths and local archives are inputs. Shell recipes and
Minix system make includes are not host build commands.
"""

import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import shlex
import subprocess
import sys


class BuildError(Exception):
    pass


def words(value):
    return shlex.split(value)


class Makefile:
    def __init__(self, directory):
        self.directory = Path(directory).resolve()
        self.variables = {".CURDIR": shlex.quote(str(self.directory)), "CURDIR": shlex.quote(str(self.directory))}
        self.paths = [self.directory]
        self.object_rules = {}
        self.archive_objects = {}
        self.target_flags = {}
        self.target_variables = {}
        makefile = self.directory / "Makefile"
        if not makefile.is_file():
            raise BuildError(f"No Makefile in {self.directory}")
        text = makefile.read_text().replace("\\\n", " ")
        targets = []
        def declarative_lines(lines):
            iterator = iter(lines)
            for line in iterator:
                match = re.fullmatch(r"\s*\.for\s+(\w+)\s+in\s+(.*)", line)
                if not match:
                    yield line
                    continue
                variable, expression = match.groups()
                body, depth = [], 1
                for nested in iterator:
                    if re.match(r"\s*\.for\b", nested): depth += 1
                    if re.match(r"\s*\.endfor\b", nested): depth -= 1
                    if depth == 0: break
                    body.append(nested)
                if depth:
                    raise BuildError(f"Unterminated .for in {makefile}")
                for value in words(self.expand(expression)):
                    expanded = [part.replace("${" + variable + "}", shlex.quote(value))
                                    .replace("$(" + variable + ")", shlex.quote(value)) for part in body]
                    yield from declarative_lines(expanded)
        for line in declarative_lines(text.splitlines()):
            if line.startswith("\t"):
                # Read compiler options as data. The command itself, automatic
                # variables, shell expressions, and recipes are never executed.
                recipe = line.strip().lstrip("@-")
                match = re.match(r"(?:\$\{CC\}|\$\(CC\))\s+(.*)", recipe)
                if match:
                    for target in targets:
                        self.target_flags.setdefault(target, []).append(match.group(1))
                archive = re.match(r"(?:ar|\$\{AR\}|\$\(AR\))\s+(.*)", recipe)
                if archive:
                    inputs = words(self.expand(archive.group(1)))
                    names = [item for item in inputs if item.endswith(".a")]
                    objects = [item for item in inputs if item.endswith(".o")]
                    if len(names) == 1 and objects:
                        self.archive_objects[Path(names[0]).name] = objects
                continue
            line = line.split("#", 1)[0].strip()
            if re.match(r"\.(?:if|ifdef|ifndef|elif|else|endif|for|endfor)\b", line):
                raise BuildError(f"Unsupported conditional or loop in {makefile}: {line}")
            match = re.match(r"([\w.]+)\s*(\+=|\?=|:=|=)\s*(.*)", line)
            if match:
                key, op, value = match.groups()
                if op == "+=":
                    self.variables[key] = self.variables.get(key, "") + " " + value
                elif op != "?=" or key not in self.variables:
                    self.variables[key] = value
            elif re.match(r"\.PATH(?:\.[\w]+)?:", line):
                self.paths.extend(self.directory / p for p in words(self.expand(line.split(":", 1)[1])))
            elif ":" in line and not line.startswith("."):
                left, right = line.split(":", 1)
                targets = [t for t in words(left) if t.endswith(".o") and "$" not in t]
                local = re.match(r"(CFLAGS|CPPFLAGS)\s*(\+=|=|:=)\s*(.*)", right.strip())
                for target in targets:
                    if local:
                        self.target_variables.setdefault(target, []).append(local.groups())
                    else:
                        inputs = [p for p in words(right) if p.endswith((".c", ".cc", ".cpp", ".s", ".S"))]
                        if inputs:
                            self.object_rules[target] = inputs
            elif ":" in line:
                targets = []
        self.paths.extend(self.directory / p for p in self.get("VPATH").replace(":", " ").split())

    def expand(self, value, seen=(), overrides=None):
        variables = self.variables if overrides is None else {**self.variables, **overrides}
        def replace(match):
            key = match.group(1) or match.group(2)
            if key in seen:
                raise BuildError(f"Recursive Makefile variable: {key}")
            # Only the portable input subset is evaluated, never shell expressions.
            if ":" in key:
                name, modifier = key.split(":", 1)
                original = self.expand(variables.get(name, ""), (*seen, key), overrides)
                if "=" in modifier and not modifier.startswith("sh"):
                    before, after = modifier.split("=", 1)
                    return " ".join(w[:-len(before)] + after if before and w.endswith(before) else w
                                    for w in words(original))
                raise BuildError(f"Unsupported Makefile modifier: {key}")
            return self.expand(variables.get(key, ""), (*seen, key), overrides)
        return re.sub(r"\$\{([^}]+)\}|\$\(([^)]+)\)", replace, value)

    def get(self, key):
        return self.expand(self.variables.get(key, ""))

    def sources(self, value):
        result = []
        for name in words(value):
            if name.endswith((".h", ".hpp")):
                continue
            if not name.endswith(".c"):
                raise BuildError(f"Unsupported source {name!r} in {self.directory}")
            candidates = [p / name for p in self.paths if (p / name).is_file()]
            if not candidates:
                raise BuildError(f"Source {name!r} not found in {self.directory} or .PATH/VPATH")
            result.append(candidates[0].resolve())
        return result

    def libraries(self):
        result = {}
        for key in self.variables:
            if not key.startswith("LIB") or not key.endswith("_SRCS"):
                continue
            prefix = key[:-5]
            name = self.get(prefix) or (self.get("LIB_NAME") if prefix == "LIB" else "")
            if name.endswith(".a"):
                result[Path(name).name] = self.sources(self.get(key))
        name = self.get("LIB_NAME")
        if name.endswith(".a") and name not in result and self.get("LIB_OBJS"):
            objects = words(self.get("LIB_OBJS"))
            result[Path(name).name] = self.object_sources(objects)
        for archive, objects in self.archive_objects.items():
            if archive not in result:
                result[archive] = self.object_sources(objects)
        name = self.get("LIB")
        if name and not name.endswith(".a"):
            result[f"lib{name}.a"] = self.sources(self.get("SRCS"))
        return result

    def object_sources(self, objects):
        names = []
        for obj in objects:
            names.extend(self.object_rules.get(obj, [obj[:-2] + ".c"]))
        return self.sources(" ".join(shlex.quote(name) for name in names))

    def project_sources(self):
        if self.get("SRCS"):
            return self.sources(self.get("SRCS"))
        sources = []
        for names in self.object_rules.values():
            for source in self.sources(" ".join(shlex.quote(name) for name in names)):
                if source not in sources:
                    sources.append(source)
        if not sources:
            raise BuildError("The project Makefile has no SRCS or explicit object source rules")
        return sources

    def flags(self, source=None):
        # Minix's 32-bit ABI and warnings-as-errors are not portable compiler
        # settings. Keep meaningful preprocessing/optimization flags only.
        values = {key: self.get(key) for key in ("CFLAGS", "CPPFLAGS")}
        recipes = []
        if source is not None:
            for target, names in self.object_rules.items():
                if source in self.sources(" ".join(shlex.quote(name) for name in names)):
                    for key, op, setting in self.target_variables.get(target, []):
                        setting = self.expand(setting, overrides=values)
                        values[key] = values.get(key, "") + " " + setting if op == "+=" else setting
                    recipes.extend(self.target_flags.get(target, []))
        value = values["CFLAGS"] + " " + values["CPPFLAGS"]
        value += " " + " ".join(self.expand(f, overrides=values) for f in recipes)
        flags = words(value)
        result = []
        i = 0
        while i < len(flags):
            flag = flags[i]
            if flag in ("-I", "-D", "-U"):
                i += 1
                if i == len(flags) or flags[i].startswith("-"):
                    raise BuildError(f"Missing argument after {flag} in {self.directory}/Makefile")
                value = str((self.directory / flags[i]).resolve()) if flag == "-I" else flags[i]
                result.extend([flag, value])
            elif flag.startswith("-I"):
                result.extend(["-I", str((self.directory / flag[2:]).resolve())])
            elif flag.startswith(("-D", "-U", "-O")) or flag in (
                    "-fcommon", "-fno-common", "-fno-inline", "-fno-unroll-loops",
                    "-fno-builtin", "-funsigned-char", "-fsigned-char", "-fwrapv"):
                result.append(flag)
            i += 1
        return result


class Builder:
    def __init__(self, project, sdk, runtime_build, output, compiler, libraries=(), includes=(), asset_root=None,
                 debug=False, sanitize=None):
        self.project = project.resolve()
        self.sdk = sdk.resolve()
        self.runtime_build = runtime_build.resolve()
        self.output = output.resolve()
        self.compiler = words(compiler)
        self.binary = None
        self.diagnostic_flags = ["-g", "-O0", "-fno-omit-frame-pointer"] if debug or sanitize else []
        if sanitize:
            self.diagnostic_flags.extend(["-fsanitize=" + sanitize, "-fno-sanitize-recover=all"])
        self.sanitize = sanitize
        if asset_root is not None:
            self.asset_root = (self.project / asset_root).resolve()
            if not self.asset_root.is_dir():
                raise BuildError(f"Asset root directory not found: {self.asset_root}")
        else:
            self.asset_root = self.project.parent
            # Student ZIPs often sit inside an unrelated Git checkout. A
            # nearby proj/project boundary takes precedence over that checkout.
            for directory in (self.project, *list(self.project.parents)[:4]):
                if directory.name.lower() in ("proj", "project"):
                    self.asset_root = directory.parent
                    break
                if (directory / ".git").exists():
                    self.asset_root = directory
                    break
        self.manifest = {"project": str(self.project), "sources": {}, "commands": []}
        self.manifest["asset_root"] = str(self.asset_root)
        self.archives = {}
        self.active = set()
        self.includes = []
        for directory in includes:
            directory = (self.project / directory).resolve()
            if not directory.is_dir():
                raise BuildError(f"Include directory not found: {directory}")
            self.includes.extend(["-I", str(directory)])
        self.library_overrides = {}
        for specification in libraries:
            if "=" not in specification:
                raise BuildError("Expected --library name=source1.c,source2.c")
            name, source_list = specification.split("=", 1)
            if not re.fullmatch(r"[A-Za-z0-9_+.-]+", name) or not source_list:
                raise BuildError(f"Invalid library override {specification!r}; use name=source1.c,source2.c")
            name = name if name.endswith(".a") else f"lib{name}.a"
            if name in self.library_overrides:
                raise BuildError(f"Repeated library override: {name}")
            sources = []
            for value in source_list.split(","):
                source = (self.project / value).resolve()
                if not value.endswith(".c"):
                    raise BuildError(f"Unsupported source {value!r} in --library {name}")
                if not source.is_file():
                    raise BuildError(f"Library source not found: {source}")
                sources.append(source)
            self.library_overrides[name] = sources
        self.objects = self.output / "objects"
        self.objects.mkdir(parents=True, exist_ok=True)

    def command(self, args, purpose="running build command"):
        self.manifest["commands"].append(args)
        result = subprocess.run(args, cwd=self.project)
        if result.returncode:
            raise BuildError(f"Failed while {purpose} (status {result.returncode}). "
                             "Review the compiler diagnostics above. Original sources were not changed.")

    def compile(self, source, makefile):
        self.manifest["sources"][str(source)] = hashlib.sha256(source.read_bytes()).hexdigest()
        flags = makefile.flags(source) + self.includes
        flags = [str(self.project_path(value)) if index and flags[index - 1] == "-I" else value
                 for index, value in enumerate(flags)]
        identity = str(source) + "\0" + "\0".join(flags)
        target = self.objects / (hashlib.sha256(identity.encode()).hexdigest()[:20] + ".o")
        args = self.compiler + ["-std=gnu11", "-fcommon", "-Wno-deprecated-non-prototype",
                                "-Wno-incompatible-pointer-types-discards-qualifiers",
                                "-I", str(self.sdk / "compat/include"),
                                "-I", str(self.sdk / "include"), "-I", str(makefile.directory)]
        args += flags + self.diagnostic_flags + ["-c", str(source), "-o", str(target)]
        self.command(args, "compiling " + source.name)
        return target

    def local_archive(self, makefile, archive):
        key = (makefile.directory, archive)
        if key in self.archives:
            return self.archives[key]
        if key in self.active:
            raise BuildError(f"Cyclic archive dependency: {key}")
        declared = self.library_overrides if archive in self.library_overrides else makefile.libraries()
        if archive not in declared:
            raise BuildError(f"No source list for {archive} in {makefile.directory}/Makefile. "
                             "Provide a source Makefile or --library name=source1.c,source2.c.")
        self.active.add(key)
        target = self.output / (hashlib.sha256(str(key).encode()).hexdigest()[:12] + "-" + archive)
        objects = [self.compile(s, makefile) for s in declared[archive]]
        # Recreate the archive so removed objects cannot remain after a rebuild.
        target.unlink(missing_ok=True)
        self.command(["ar", "rcs", str(target)] + [str(o) for o in objects])
        self.active.remove(key)
        self.archives[key] = target
        return target

    def find_archive(self, name, directories):
        if name in self.library_overrides:
            return self.local_archive(Makefile(self.project), name)
        for directory in directories:
            if not (directory / "Makefile").is_file():
                continue
            makefile = Makefile(directory)
            if name in makefile.libraries():
                return self.local_archive(makefile, name)
        raise BuildError(f"Cannot rebuild {name}. Searched: {', '.join(str(d) for d in directories)}. "
                         "Provide --library name=source1.c,source2.c or a source Makefile.")

    def project_path(self, name):
        prefix = "/home/lcom/labs/"
        if name.startswith(prefix):
            return (self.asset_root / name[len(prefix):]).resolve()
        return (self.project / name).resolve()

    def rebuild_object(self, name, project_makefile):
        original = self.project_path(name)
        owner = project_makefile
        for directory in (original.parent, *original.parent.parents):
            if (directory / "Makefile").is_file():
                owner = Makefile(directory)
                break
        declarations = owner.object_rules.get(name) or owner.object_rules.get(original.name)
        if declarations:
            sources = owner.sources(" ".join(shlex.quote(value) for value in declarations))
        elif original.with_suffix(".c").is_file():
            sources = [original.with_suffix(".c")]
        else:
            sources = owner.sources(original.with_suffix(".c").name)
        if len(sources) != 1:
            raise BuildError(f"Object {name!r} must declare exactly one C source")
        return self.compile(sources[0], owner)

    def build(self):
        previous = self.output / "build.json"
        if previous.is_file():
            try:
                report = json.loads(previous.read_text())
                old_binary = Path(report.get("binary", ""))
                if report.get("project") == str(self.project) and old_binary.parent == self.output:
                    old_binary.unlink(missing_ok=True)
            except (ValueError, TypeError):
                pass
        makefile = Makefile(self.project)
        program = makefile.get("PROG") or "proj"
        if Path(program).name != program or program in (".", ".."):
            raise BuildError(f"Invalid PROG: {program!r}")
        self.binary = self.output / program
        self.binary.unlink(missing_ok=True)
        self.manifest["binary"] = str(self.binary)
        if self.sanitize:
            # Compiler support alone does not prove its runtime can initialize
            # on this host. Fail before launching a student program that hangs.
            probe_source = self.output / "sanitizer_probe.c"
            probe_binary = self.output / "sanitizer_probe"
            probe_source.write_text("int main(void) { return 0; }\n")
            self.command(self.compiler + self.diagnostic_flags + [str(probe_source), "-o", str(probe_binary)],
                         "checking compiler sanitizer support")
            try:
                probe = subprocess.run([str(probe_binary)], capture_output=True, text=True, timeout=5)
                if probe.returncode:
                    raise BuildError(f"The {self.sanitize} sanitizer runtime failed to start on this machine. "
                                     "Use --debug without sanitizers or another supported compiler.\n" + probe.stderr)
            except subprocess.TimeoutExpired:
                alternative = "--sanitize undefined or --debug" if "address" in self.sanitize else "--debug"
                raise BuildError(f"The {self.sanitize} sanitizer runtime did not initialize within 5 seconds. "
                                 f"Use {alternative}; this failure occurs before student code runs.")
        sources = makefile.project_sources()
        objects = [self.compile(s, makefile) for s in sources]
        ldflags = words(makefile.get("LDADD") + " " + makefile.get("LDLIBS"))
        directories = [self.project]
        names = []
        system_flags = []
        i = 0
        while i < len(ldflags):
            flag = ldflags[i]
            if flag == "-L":
                i += 1
                if i == len(ldflags):
                    raise BuildError("Missing library directory after -L")
                directories.append(self.project_path(ldflags[i]))
            elif flag.startswith("-L"):
                directories.append(self.project_path(flag[2:]))
            elif flag.startswith("-l") and flag not in ("-llcf", "-llm", "-lxpm", "-ltimers", "-ldriver", "-lsys"):
                if flag in ("-lm", "-lpthread"):
                    system_flags.append(flag)
                else:
                    names.append(f"lib{flag[2:]}.a")
            elif flag.endswith(".a"):
                archive = self.project_path(flag)
                directories.append(archive.parent)
                names.append(archive.name)
            elif flag.endswith(".o"):
                obj = self.rebuild_object(flag, makefile)
                if obj not in objects:
                    objects.append(obj)
            i += 1
        for flag in words(makefile.get("DPADD")):
            if flag.endswith(".a") and Path(flag).name not in ("liblcf.a", "liblm.a", "libxpm.a"):
                archive = self.project_path(flag)
                directories.append(archive.parent)
                if archive.name not in names:
                    names.append(archive.name)
        # An explicit --library also requests linking when the original
        # Makefile forgot that dependency. Matching names still replace its
        # foreign Minix archive instead of linking a second copy.
        names.extend(name for name in self.library_overrides if name not in names)
        archives = [self.find_archive(n, directories) for n in names]
        context = self.output / "project_context.c"
        context.write_text("const char *lcom_compat_project_root = " + json.dumps(str(self.asset_root)) + ";\n")
        objects.append(self.compile(context, makefile))
        binary = self.binary
        compat = self.runtime_build / "liblcom_minix.a"
        native = self.runtime_build / "libmachinelab.a"
        for library in (compat, native):
            if not library.is_file():
                raise BuildError(f"Missing runtime library {library}. Build lcom-ng first.")
        self.command(self.compiler + [str(o) for o in objects] + [str(a) for a in archives]
                     + [str(compat), str(native), "-lm", "-pthread"] + system_flags + self.diagnostic_flags + ["-o", str(binary)],
                     "linking " + program)
        # Sources are never rewritten. The hashes make that property reviewable.
        for name, digest in self.manifest["sources"].items():
            if hashlib.sha256(Path(name).read_bytes()).hexdigest() != digest:
                raise BuildError(f"Source changed during build: {name}")
        self.manifest["binary"] = str(binary)
        self.manifest["status"] = "complete"
        self.write_report()
        return binary

    def write_report(self):
        (self.output / "build.json").write_text(json.dumps(self.manifest, indent=2) + "\n")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("project", type=Path)
    parser.add_argument("--sdk", required=True, type=Path)
    parser.add_argument("--runtime-build", required=True, type=Path)
    parser.add_argument("--output", type=Path)
    parser.add_argument("--cc", default=os.environ.get("CC", "cc"))
    parser.add_argument("--library", action="append", default=[], metavar="NAME=SOURCES",
                        help="rebuild a local archive from comma-separated .c paths relative to PROJECT; repeatable")
    parser.add_argument("--include", action="append", default=[], type=Path, metavar="DIRECTORY",
                        help="add an include directory relative to PROJECT; repeatable")
    parser.add_argument("--asset-root", type=Path, metavar="DIRECTORY",
                        help="workspace root for legacy /home/lcom/labs paths, relative to PROJECT")
    parser.add_argument("--debug", action="store_true", help="build student code with debug symbols and without optimization")
    parser.add_argument("--sanitize", choices=("address", "undefined", "address,undefined"),
                        help="instrument student code for memory or undefined-behavior diagnostics; implies --debug")
    args = parser.parse_args()
    output = args.output or args.project / ".lcom-ng"
    builder = None
    try:
        builder = Builder(args.project, args.sdk, args.runtime_build, output, args.cc,
                          args.library, args.include, args.asset_root, args.debug, args.sanitize)
        print(builder.build())
    except (BuildError, OSError, ValueError) as error:
        print(f"lcom-ng compat: {error}", file=sys.stderr)
        if builder is not None:
            builder.manifest.update(status="failed", error=str(error))
            try:
                if builder.binary is not None:
                    builder.binary.unlink(missing_ok=True)
                builder.write_report()
            except OSError as report_error:
                print(f"lcom-ng compat: Cannot save build report: {report_error}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
