#!/usr/bin/env python3
"""Actual student renderers at source-image and framebuffer allocation boundaries."""
import unittest
from pathlib import Path
import community_a141_regressions as base
HERE=Path(__file__).resolve().parent/'fixtures/community/a143'

class A143RendererRegressions(unittest.TestCase):
    compile_owner=base.ArcadeA141SourceRegressions.compile_owner

    def test_blackjack_screen_copies_each_complete_pixel_without_reading_after_the_image(self):
        self.compile_owner('Coutinho-David--Drivers/proj/src','controllers/graphics.c',HERE/'blackjack-screen.c',[('GRAPHICS_SOURCE','controllers/graphics.c')],flags=['-O2','-ffunction-sections','-fdata-sections'])

    def test_blackjack_number_glyph_copies_complete_rows_without_source_overread(self):
        self.compile_owner('Coutinho-David--Drivers/proj/src','controllers/graphics.c',HERE/'blackjack-screen.c',[('GRAPHICS_SOURCE','controllers/graphics.c')],flags=['-O2','-ffunction-sections','-fdata-sections'],args=['number'])

    def test_meteor_backbuffer_rejects_first_excluded_row_and_column(self):
        self.compile_owner('Litone11--LEIC-FEUP/2nd_year/LCOM/proj/src','devices/video.c',HERE/'meteor-frame.c',[('VIDEO_SOURCE','devices/video.c')],flags=['-O2','-ffunction-sections','-fdata-sections'])

    def test_meteor_menu_paints_before_waiting_and_after_keyboard_info_navigation(self):
        self.compile_owner('Litone11--LEIC-FEUP/2nd_year/LCOM/proj/src','main.c',HERE/'meteor-menu.c',[('MAIN_SOURCE','main.c')],args=['0'])

    def test_meteor_stationary_mouse_click_presents_info_and_returned_menu(self):
        self.compile_owner('Litone11--LEIC-FEUP/2nd_year/LCOM/proj/src','main.c',HERE/'meteor-menu.c',[('MAIN_SOURCE','main.c')],args=['2'])

if __name__=='__main__':unittest.main()
