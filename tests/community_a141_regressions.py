#!/usr/bin/env python3
"""Actual student source owner regressions for the A141 arcade additions."""
import os
from pathlib import Path
import shlex
import subprocess
import sys
import tempfile
import unittest

ROOT = Path(os.environ.get('LCOM_SOURCE_TEST_ROOT', Path(__file__).resolve().parents[1]))
COMMUNITY = ROOT / 'examples/community'
FIXTURES = Path(__file__).resolve().parent / 'fixtures/community/a141'
sys.path.insert(0, str(ROOT / 'sdk/export'))
import project as project_export


class ArcadeA141SourceRegressions(unittest.TestCase):
    def compile_owner(self, relative, unit, fixture=None, defines=(), flags=(), args=(), link_runtime=False, compiler_override=None):
        project = (COMMUNITY / relative).resolve()
        compiler = compiler_override or shlex.split(os.environ.get('CC', 'cc'))
        with tempfile.TemporaryDirectory(prefix='lcom-a141-source-') as temporary:
            folder = Path(temporary)
            plan = project_export.CMakeProject(project=project, sdk=ROOT/'sdk',
                runtime_build=ROOT/'build', output=folder/'plan', compiler=compiler[0])
            plan.build()
            source = project / unit
            source_flags = next(f for _, p, f in plan.entries if p == source)
            command = [*compiler, *source_flags, '-O1', *flags]
            if fixture:
                for name, value in defines:
                    command += ['-D' + name + '="' + str(project/value) + '"']
                command += [str(FIXTURES/fixture)]
                if link_runtime:
                    command += [str(ROOT/'build/liblcom_minix.a'), str(ROOT/'build/libmachinelab.a')]
                command += ['-Wl,-dead_strip' if sys.platform == 'darwin' else '-Wl,--gc-sections', '-o', str(folder/'owner')]
            else:
                command += ['-c', str(source), '-o', str(folder/'owner.o')]
            result = subprocess.run(command, capture_output=True, text=True)
            self.assertEqual(result.returncode, 0, result.stdout+result.stderr)
            if fixture:
                result = subprocess.run([str(folder/'owner'), *args], capture_output=True, text=True, timeout=15)
                self.assertEqual(result.returncode, 0, result.stdout+result.stderr)

    def test_lc_typer_speed_hud_keeps_full_wpm_text_and_terminator(self):
        self.compile_owner('jlcrodrigues--lc-typer/proj/src','states/game.c','typer-renderer.c',
            [('LC_SOURCE','states/game.c')], ['-U_FORTIFY_SOURCE','-D_FORTIFY_SOURCE=2'], ['wpm'])

    def test_lc_typer_render_copy_keeps_terminator_inside_owned_scratch(self):
        self.compile_owner('jlcrodrigues--lc-typer/proj/src','states/game.c','typer-renderer.c',
            [('LC_SOURCE','states/game.c')], ['-U_FORTIFY_SOURCE','-D_FORTIFY_SOURCE=2'], ['text'])

    def test_lc_typer_mouse_checks_packet_sync_in_data_not_status(self):
        self.compile_owner('jlcrodrigues--lc-typer/proj/src','drivers/kbc/mouse.c',
            'typer-mouse.c',[('MOUSE_SOURCE','drivers/kbc/mouse.c')])

    def test_lc_typer_phrase_retains_final_character_after_newline_removal(self):
        self.compile_owner('jlcrodrigues--lc-typer/proj/src','states/game.c',
            'typer-phrase.c',[('GAME_SOURCE','states/game.c')])

    def test_asteroid_startup_initializes_only_owned_shot_slots(self):
        self.compile_owner('gcandal--minixasteroid','proj.c','asteroid-owner.c',
            [('ASTEROID_SOURCE','proj.c')], ['-fsanitize=bounds','-fno-sanitize-recover=all'], ['startup'], True)

    def test_asteroid_timer_has_one_destination_slot_per_iteration(self):
        self.compile_owner('gcandal--minixasteroid','proj.c','asteroid-owner.c',
            [('ASTEROID_SOURCE','proj.c')], ['-fsanitize=bounds','-fno-sanitize-recover=all'], ['timer'], True)

    def test_asteroid_sprite_renderer_uses_declared_pixel_accessor_types(self):
        self.compile_owner('gcandal--minixasteroid','sprite.c','asteroid-sprite.c',
            [('SPRITE_SOURCE','sprite.c')], ['-Werror=implicit-function-declaration'])

    def android_compiler(self):
        sdk = Path(os.environ.get('ANDROID_HOME', Path.home()/'Library/Android/sdk'))
        candidates = sorted(sdk.glob('ndk/*/toolchains/llvm/prebuilt/*/bin/aarch64-linux-android26-clang'))
        if not candidates:
            self.skipTest('Android NDK is required to prove that host ncurses headers cannot mask a stale menu.h dependency')
        return [str(candidates[-1])]

    def test_meteor_shooter_main_includes_only_available_game_headers(self):
        self.compile_owner('Litone11--LEIC-FEUP/2nd_year/LCOM/proj/src','main.c', compiler_override=self.android_compiler())

    def test_meteor_shooter_game_uses_available_menu_state_header(self):
        self.compile_owner('Litone11--LEIC-FEUP/2nd_year/LCOM/proj/src','game/game.c', compiler_override=self.android_compiler())

    def test_mario_pipe_run_original_entrypoint_has_a_complete_function_body(self):
        self.compile_owner('rpedro10--MinixGame/LCOM/projeto/src','main.c')


if __name__ == '__main__':
    unittest.main()
