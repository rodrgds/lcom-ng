#!/usr/bin/env python3
"""Additional actual card and result-text owners from real A141 gameplay."""
import unittest
from pathlib import Path
HERE=Path(__file__).resolve().parent/'fixtures/community/a141'
import community_a141_regressions as base

class A141FollowupSourceRegressions(unittest.TestCase):
    compile_owner = base.ArcadeA141SourceRegressions.compile_owner

    def test_blackjack_loads_all_fifty_two_cards_and_the_card_back_inside_owned_slots(self):
        self.compile_owner('Coutinho-David--Drivers/proj/src','controllers/graphics.c',HERE/'blackjack-cards.c',[('GRAPHICS_SOURCE','controllers/graphics.c')],args=['load'])

    def test_blackjack_card_renderer_preserves_every_source_row_and_framebuffer_neighbor(self):
        self.compile_owner('Coutinho-David--Drivers/proj/src','controllers/graphics.c',HERE/'blackjack-cards.c',[('GRAPHICS_SOURCE','controllers/graphics.c')],args=['render'])

    def test_lc_typer_result_formats_perfect_accuracy_with_a_terminated_percent(self):
        self.compile_owner('jlcrodrigues--lc-typer/proj/src','states/game_over.c',HERE/'typer-result.c',[('GAME_OVER_SOURCE','states/game_over.c')],flags=['-U_FORTIFY_SOURCE','-D_FORTIFY_SOURCE=2'],args=['accuracy'])

    def test_lc_typer_result_formats_large_wpm_inside_its_own_buffer(self):
        self.compile_owner('jlcrodrigues--lc-typer/proj/src','states/game_over.c',HERE/'typer-result.c',[('GAME_OVER_SOURCE','states/game_over.c')],flags=['-U_FORTIFY_SOURCE','-D_FORTIFY_SOURCE=2'],args=['wpm'])

    def test_meteor_spawning_retains_a_positive_period_at_twenty_five_seconds(self):
        self.compile_owner('Litone11--LEIC-FEUP/2nd_year/LCOM/proj/src','entities/entities.c',HERE/'meteor-spawn.c',[('ENTITIES_SOURCE','entities/entities.c')],flags=['-fsanitize=integer-divide-by-zero','-fno-sanitize-recover=all'])

if __name__=='__main__':unittest.main()
