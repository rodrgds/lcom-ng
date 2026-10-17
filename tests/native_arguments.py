#!/usr/bin/env python3
"""Execute relocated native export arguments against a resource-owning C entry."""
import argparse
import importlib.util
import os
from pathlib import Path
import shlex
import shutil
import subprocess
import sys
import tempfile


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--sdk', type=Path, required=True)
    args = parser.parse_args()
    sdk = args.sdk.resolve()
    sys.path.insert(0, str(sdk / 'export'))
    spec = importlib.util.spec_from_file_location('native_argument_project', sdk / 'export/project.py')
    project_module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(project_module)
    cc = shlex.split(os.environ.get('CC', 'cc'))
    cxx = shlex.split(os.environ.get('CXX', 'c++'))
    with tempfile.TemporaryDirectory(prefix='lcom-native-arguments-') as temporary:
        root = Path(temporary)
        for kind in ('minix', 'flat'):
            project = root / kind / 'proj'
            (project / 'res').mkdir(parents=True)
            (project / 'res/value.txt').write_text('relocated resource\n')
            expected = './authored-game' if kind == 'minix' else './game'
            source = '''#include <stdio.h>
#include <string.h>
int main(int argc, char **argv) {
  if (argc != 3 || strcmp(argv[1], "alpha") || strcmp(argv[2], "two words")) return 11;
  char *slash = strrchr(argv[0], '/');
  if (!slash) return 10;
  if (strcmp(argv[0], EXPECTED_PROGRAM)) return 12;
  char path[128]; size_t prefix = (size_t)(slash - argv[0]);
  if (prefix + sizeof("/res/value.txt") > sizeof path) return 13;
  memcpy(path, argv[0], prefix); strcpy(path + prefix, "/res/value.txt");
  FILE *file = fopen(path, "r");
  if (!file) return 14;
  char line[64]; char *read = fgets(line, sizeof line, file); fclose(file);
  return read && strcmp(line, "relocated resource\\n") == 0 ? 42 : 15;
}
'''
            (project / 'main.c').write_text(source)
            if kind == 'minix':
                (project / 'Makefile').write_text('PROG=../bin/authored-game\nSRCS=main.c\n')
            output = root / (kind + '-export')
            output.mkdir()
            command = [str(project), '--name', 'Display / title', '--arg', 'alpha', '--arg', 'two words']
            if kind == 'minix':
                command += ['--asset-root', '..', '--asset', 'proj/res']
            else:
                command += ['--asset', 'res']
            options = project_module.argument_parser(__doc__).parse_args(command)
            project_module.project_config.resolve(options)
            project_module.write_native_project(options, output)
            moved = root / (kind + '-relocated')
            output.rename(moved)
            shutil.rmtree(project.parent)
            copied = next((moved / 'native/sources').rglob('main.c'))
            obj = moved / 'student.o'
            subprocess.run([*cc, '-std=c11', '-Dmain=lcom_student_main',
                            '-DEXPECTED_PROGRAM="' + expected + '"', '-c', str(copied), '-o', str(obj)], check=True)
            driver = moved / 'entry.cpp'
            driver.write_text('''#include "native/GameConfig.hpp"
#include <filesystem>
#include <vector>
extern "C" int lcom_student_main(int, char **);
int main(int argc, char **argv) {
  if (argc != 2) return 20;
  std::filesystem::current_path(std::filesystem::path(argv[1]) / game::workingDirectory);
  std::vector<char *> arguments;
  for (const char *value : game::arguments) arguments.push_back(const_cast<char *>(value));
  arguments.push_back(nullptr);
  return lcom_student_main((int)arguments.size() - 1, arguments.data());
}
''')
            binary = moved / 'entry'
            subprocess.run([*cxx, '-std=c++17', str(driver), str(obj), '-o', str(binary)], check=True)
            result = subprocess.run([str(binary), str(moved / 'game-assets')])
            if result.returncode != 42:
                raise AssertionError(f'{kind}: expected student status 42, got {result.returncode}')
            print(kind + ': relocated argv0, arguments, cwd-relative asset and student status 42 passed')


if __name__ == '__main__':
    main()
