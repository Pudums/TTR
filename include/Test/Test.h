//
// Created by timofey on 28.02.2021.
//

#ifndef TTR_TEST_H
#define TTR_TEST_H

#include "Game.h"
#include "TTRController.h"

struct Test {
    static void run_tests();

    // test_game
    // test_moves
    static void test_move_draw_card_from_deck();
    static void test_move_draw_card_from_active();
    static void test_move_draw_card_from_both();
    static void test_move_draw_routes();
    static void test_move_build_path_1();
    static void test_move_build_path_2();
    static void test_move_build_path_3();
    static void test_move_build_path_4();

};

#endif  // TTR_TEST_H
