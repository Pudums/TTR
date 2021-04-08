//
// Created by timofey on 28.02.2021.
//
#include "Test.h"

void Test::run_tests() {
    // test_deck

    // test_game
    // test_moves
    test_move_draw_card_from_deck();
    test_move_draw_card_from_active();
    test_move_draw_card_from_both();
    test_move_draw_routes();
    test_move_build_path_1();
    test_move_build_path_2();
    test_move_build_path_3();
    test_move_build_path_4();
    test_lots_turns();
}

void Test::test_move_draw_card_from_deck() {
    std::cout << "TEST_CASE: test_move_draw_card_from_deck ";
    TTRController controller;
    controller.start_game(2);
    Game &g = *controller.game;
    Deck &d = controller.game->deck;
    int start_size = d.wagons_deck.size();
    std::cout << "Turn 1 is running...\n";
    if (g.players[g.active_player].wagon_cards.size() != 4 ||
        d.wagons_deck.size() != start_size) {
        std::cout << "- FAILED\n";
        std::cout << "Turn 1\n";
        return;
    }
    std::cout << "Turn 2 is running...\n";
    controller.get_card_from_deck();
    if (g.players[g.active_player].wagon_cards.size() != 5 ||
        d.wagons_deck.size() != start_size - 1 ||
        d.last_card.color !=
            g.players[g.active_player].wagon_cards.back().color) {
        std::cout << "- FAILED\n";
        std::cout << "Turn 2\n";
        return;
    }
    std::cout << "Turn 3 is running...\n";
    controller.get_card_from_deck();
    if (g.players[g.active_player].wagon_cards.size() != 4 ||
        d.wagons_deck.size() != start_size - 2) {
        std::cout << "- FAILED\n";
        std::cout << "Turn 3\n";
        return;
    }
    std::cout << "Turn 4 is running...\n";
    controller.get_card_from_deck();
    if (g.players[g.active_player].wagon_cards.size() != 5 ||
        d.wagons_deck.size() != start_size - 3 ||
        d.last_card.color !=
            g.players[g.active_player].wagon_cards.back().color) {
        std::cout << "- FAILED\n";
        std::cout << "Turn 4\n";
        return;
    }
    std::cout << "Turn 5 is running...\n";
    controller.get_card_from_deck();
    if (g.players[g.active_player].wagon_cards.size() != 6 ||
        d.wagons_deck.size() != start_size - 4) {
        std::cout << "- FAILED\n";
        std::cout << "Turn 5\n";
        return;
    }
    std::cout << "Turn 6 is running...\n";
    controller.get_card_from_deck();
    if (g.players[g.active_player].wagon_cards.size() != 7 ||
        d.wagons_deck.size() != start_size - 5 ||
        d.last_card.color !=
            g.players[g.active_player].wagon_cards.back().color) {
        std::cout << "- FAILED\n";
        std::cout << "Turn 6\n";
        return;
    }
    std::cout << "- PASSED\n";
}

void Test::test_move_draw_card_from_active() {
    std::cout << "TEST_CASE: test_move_draw_card_from_active ";
    TTRController controller;
    controller.start_game(2);
    Game &g = *controller.game;
    Deck &d = controller.game->deck;
    int start_size = d.wagons_deck.size();
    std::cout << "Turn 1 is running...\n";
    if (g.players[g.active_player].wagon_cards.size() != 4 ||
        d.wagons_deck.size() != start_size) {
        std::cout << "- FAILED\n";
        std::cout << "Turn 1\n";
        return;
    }
    std::cout << "Turn 2 is running...\n";
    controller.get_card_from_active(1);
    //std::cout << g.players[g.active_player].wagon_cards.size() << ' ' << d.wagons_deck.size() << ' ' << start_size << '\n';
    if (g.players[g.active_player].wagon_cards.size() != 4 ||
        d.wagons_deck.size() != start_size - 1) {
        std::cout << g.players[g.active_player].wagon_cards.size() << ' ' << d.wagons_deck.size() << '\n';
        std::cout << "- FAILED\n";
        std::cout << "Turn 2\n";
        return;
    }
    std::cout << "Turn 3 is running...\n";
    controller.get_card_from_active(1);
    if (g.players[g.active_player].wagon_cards.size() != 5 ||
        d.wagons_deck.size() != start_size - 2) {
        std::cout << "- FAILED\n";
        std::cout << "Turn 3\n";
        return;
    }
    std::cout << "Turn 4 is running...\n";
    controller.get_card_from_active(2);
    if (g.players[g.active_player].wagon_cards.size() != 5 ||
        d.wagons_deck.size() != start_size - 3) {
        std::cout << "- FAILED\n";
        std::cout << "Turn 4\n";
        return;
    }
    std::cout << "Turn 5 is running...\n";
    controller.get_card_from_active(1);
    if (g.players[g.active_player].wagon_cards.size() != 6 ||
        d.wagons_deck.size() != start_size - 4) {
        std::cout << "- FAILED\n";
        std::cout << "Turn 5\n";
        return;
    }
    std::cout << "Turn 6 is running...\n";
    controller.get_card_from_active(3);
    if (g.players[g.active_player].wagon_cards.size() != 6 ||
        d.wagons_deck.size() != start_size - 5) {
        std::cout << "- FAILED\n";
        std::cout << "Turn 6\n";
        return;
    }
    std::cout << "- PASSED\n";
}

void Test::test_move_draw_card_from_both() {
    std::cout << "TEST_CASE: test_move_draw_card_from_both ";
    TTRController controller;
    controller.start_game(2);
    Game &g = *controller.game;
    Deck &d = controller.game->deck;
    int start_size = d.wagons_deck.size();
    std::cout << "Turn 1 is running...\n";
    if (g.players[g.active_player].wagon_cards.size() != 4 ||
        d.wagons_deck.size() != start_size) {
        std::cout << "- FAILED\n";
        std::cout << "Turn 1\n";
        return;
    }
    std::cout << "Turn 2 is running...\n";
    controller.get_card_from_active(1);
    if (g.players[g.active_player].wagon_cards.size() != 4 ||
        d.wagons_deck.size() != start_size - 1 ||
        d.last_card.color != d.active_wagons[1].color) {
        std::cout << g.active_player << '\n';
        std::cout << "- FAILED\n";
        std::cout << "Turn 2\n";
        return;
    }
    std::cout << "Turn 3 is running...\n";
    controller.get_card_from_deck();
    if (g.players[g.active_player].wagon_cards.size() != 5 ||
        d.wagons_deck.size() != start_size - 2) {
        std::cout << "- FAILED\n";
        std::cout << "Turn 3\n";
        return;
    }
    std::cout << "Turn 4 is running...\n";
    controller.get_card_from_deck();
    if (g.players[g.active_player].wagon_cards.size() != 5 ||
        d.wagons_deck.size() != start_size - 3) {
        std::cout << "- FAILED\n";
        std::cout << "Turn 4\n";
        return;
    }
    std::cout << "Turn 5 is running...\n";
    controller.get_card_from_active(1);
    if (g.players[g.active_player].wagon_cards.size() != 6 ||
        d.wagons_deck.size() != start_size - 4 ||
        d.last_card.color != d.active_wagons[1].color) {
        std::cout << "- FAILED\n";
        std::cout << "Turn 5\n";
        return;
    }
    std::cout << "Turn 6 is running...\n";
    controller.get_card_from_active(3);
    if (g.players[g.active_player].wagon_cards.size() != 6 ||
        d.wagons_deck.size() != start_size - 5 ||
        d.last_card.color != d.active_wagons[3].color) {
        std::cout << "- FAILED\n";
        std::cout << "Turn 6\n";
        return;
    }
    std::cout << "- PASSED\n";
}

void Test::test_move_draw_routes() {
    std::cout << "TEST_CASE: test_move_draw_routes ";
    TTRController controller;
    controller.start_game(2);
    Game &g = *controller.game;
    Deck &d = controller.game->deck;
    controller.get_routes();
    std::cout << "Turn 1 is running...\n";
    if (g.players[g.active_player].active_routes.size() != 4) {
        std::cout << g.active_player << ' ';
        std::cout << g.players[g.active_player].active_routes.size() << '\n';
        std::cout << "- FAILED\n";
        std::cout << "Turn 1\n";
        return;
    }
    std::cout << "Turn 2 is running...\n";
    controller.get_routes();
    if (g.players[g.active_player].active_routes.size() != 7) {
        std::cout << g.active_player << ' ';
        std::cout << g.players[g.active_player].active_routes.size() << '\n';
        std::cout << "- FAILED\n";
        std::cout << "Turn 2\n";
        return;
    }
    std::cout << "- PASSED\n";
}

void Test::test_move_build_path_1() {
    std::cout << "TEST_CASE: test_move_build_path_1 ";
    TTRController controller;
    controller.start_game(2);
    Game &g = *controller.game;
    std::cout << "Turn 1 is running...\n";
    controller.build_path_initialize(0);
    controller.set_color_to_build_path(WagonCard("White"));
    if (g.last_path_is_built) {
        std::cout << "- FAILED\n";
        std::cout << "Turn 1\n";
        return;
    }
    for (int i = 0; i < 4; i++) {
        g.players[g.active_player].wagon_cards.emplace_back("White");
    }
    std::cout << "Turn 2 is running...\n";
    controller.build_path_initialize(0);
    controller.set_color_to_build_path(WagonCard("White"));
    if (!g.last_path_is_built) {
        std::cout << "- FAILED\n";
        std::cout << "Turn 2\n";
        return;
    }
    std::cout << "Turn 3 is running...\n";
    controller.build_path_initialize(0);
    controller.set_color_to_build_path(WagonCard("White"));
    if (g.last_path_is_built) {
        std::cout << "- FAILED\n";
        std::cout << "Turn 3\n";
        return;
    }
    for (int i = 0; i < 4; i++) {
        g.players[g.active_player].wagon_cards.emplace_back("Multicolored");
    }
    std::cout << "Turn 4 is running...\n";
    controller.build_path_initialize(0);
    controller.set_color_to_build_path(WagonCard("White"));
    if (!g.last_path_is_built) {
        std::cout << "- FAILED\n";
        std::cout << "Turn 4\n";
        return;
    }
    std::cout << "- PASSED\n";
}

void Test::test_move_build_path_2() {
    std::cout << "TEST_CASE: test_move_build_path_2 ";
    TTRController controller;
    controller.start_game(1);
    Game &g = *controller.game;
    g.players[0].wagon_cards = std::vector<WagonCard>{
        WagonCard("Red"), WagonCard("Red"), WagonCard("Red"), WagonCard("Red")};
    controller.build_path_initialize(92);
    controller.set_color_to_build_path(WagonCard("Red"));
    if (!g.last_path_is_built || g.board.paths[92].owner == -1) {
        std::cout << "- FAILED\n";
        std::cout << "Turn 1\n";
        return;
    }
    g.players[0].wagon_cards = std::vector<WagonCard>{
        WagonCard("Red"), WagonCard("Red"), WagonCard("Red")};
    controller.build_path_initialize(92);
    controller.set_color_to_build_path(WagonCard("Red"));
    if (g.last_path_is_built || g.players[0].wagon_cards.size() != 3) {
        std::cout << "- FAILED\n";
        std::cout << "Turn 2\n";
        return;
    }
    g.players[0].wagon_cards.clear();
    g.players[0].wagon_cards = std::vector<WagonCard>{
        WagonCard("Multicolored"), WagonCard("Multicolored"),
        WagonCard("Multicolored"), WagonCard("Multicolored")};
    controller.build_path_initialize(92);
    controller.set_color_to_build_path(WagonCard("Multicolored"));
    if (!g.last_path_is_built || !g.players[0].wagon_cards.empty()) {
        std::cout << "- FAILED\n";
        std::cout << "Turn 3\n";
        return;
    }
    g.players[0].wagon_cards.clear();
    g.players[0].wagon_cards = std::vector<WagonCard>{
        WagonCard("Multicolored"), WagonCard("Multicolored"), WagonCard("Red"),
        WagonCard("Red")};
    controller.build_path_initialize(92);
    controller.set_color_to_build_path(WagonCard("Multicolored"));
    if (g.last_path_is_built) {
        std::cout << "- FAILED\n";
        std::cout << "Turn 4\n";
        return;
    }
    g.players[0].wagon_cards.clear();
    g.players[0].wagon_cards = std::vector<WagonCard>{
        WagonCard("Multicolored"), WagonCard("Multicolored"), WagonCard("Red"),
        WagonCard("Red")};
    controller.build_path_initialize(92);
    controller.set_color_to_build_path(WagonCard("Red"));
    if (!g.last_path_is_built || !g.players[0].wagon_cards.empty()) {
        std::cout << "- FAILED\n";
        std::cout << "Turn 5\n";
        return;
    }
    g.players[0].wagon_cards.clear();
    g.players[0].wagon_cards = std::vector<WagonCard>{
        WagonCard("Multicolored"), WagonCard("Multicolored"), WagonCard("Red"),
        WagonCard("Red")};
    controller.build_path_initialize(92);
    controller.set_color_to_build_path(WagonCard("Green"));
    if (g.last_path_is_built) {
        std::cout << "- FAILED\n";
        std::cout << "Turn 6\n";
        return;
    }
    g.players[0].wagon_cards.clear();
    g.players[0].wagon_cards = std::vector<WagonCard>{
        WagonCard("Multicolored"), WagonCard("Multicolored"),
        WagonCard("Multicolored"), WagonCard("Multicolored"),
        WagonCard("Red"),          WagonCard("Red")};
    controller.build_path_initialize(92);
    controller.set_color_to_build_path(WagonCard("Green"));
    if (!g.last_path_is_built || g.players[0].wagon_cards.size() != 2) {
        std::cout << "- FAILED\n";
        std::cout << "Turn 7\n";
        return;
    }
    g.players[0].wagon_cards.clear();
    std::cout << "- PASSED\n";
}

void Test::test_move_build_path_3() {
    std::cout << "TEST_CASE: test_move_build_path_3 ";
    TTRController controller;
    controller.start_game(1);
    Game &g = *controller.game;
    g.players[0].wagon_cards =
        std::vector<WagonCard>{WagonCard("Purple"), WagonCard("Purple"),
                               WagonCard("Purple"), WagonCard("Purple")};
    controller.build_path_initialize(61);
    controller.set_color_to_build_path(WagonCard("Purple"));
    if (!g.last_path_is_built || g.players[0].wagon_cards.size() != 1) {
        std::cout << "- FAILED\n";
        std::cout << "Turn 1\n";
        return;
    }
    g.players[0].wagon_cards.clear();
    g.players[0].wagon_cards = std::vector<WagonCard>{
        WagonCard("Purple"), WagonCard("Purple"), WagonCard("Multicolored"),
        WagonCard("Multicolored")};
    controller.build_path_initialize(61);
    controller.set_color_to_build_path(WagonCard("Purple"));
    if (!g.last_path_is_built || g.players[0].wagon_cards.size() != 1 ||
        g.players[0].wagon_cards[0].color != "Multicolored") {
        std::cout << "- FAILED\n";
        std::cout << "Turn 2\n";
        return;
    }
    g.players[0].wagon_cards.clear();
    std::cout << "- PASSED\n";
}

void Test::test_move_build_path_4() {
    std::cout << "TEST_CASE: test_move_build_path_4 ";
    TTRController controller;
    controller.start_game(1);
    Game &g = *controller.game;
    g.players[0].wagon_cards =
        std::vector<WagonCard>{WagonCard("Yellow"), WagonCard("Yellow"),
                               WagonCard("Yellow"), WagonCard("Yellow")};
    controller.build_path_initialize(86);
    controller.set_color_to_build_path(WagonCard("Yellow"));
    if (g.last_path_is_built) {
        std::cout << "- FAILED\n";
        std::cout << "Turn 1\n";
        return;
    }
    g.players[0].wagon_cards.clear();
    g.players[0].wagon_cards =
        std::vector<WagonCard>{WagonCard("Yellow"), WagonCard("Yellow"),
                               WagonCard("Yellow"), WagonCard("Multicolored")};
    controller.build_path_initialize(86);
    controller.set_color_to_build_path(WagonCard("Yellow"));
    if (!g.last_path_is_built) {
        std::cout << "- FAILED\n";
        std::cout << "Turn 2\n";
        return;
    }
    g.players[0].wagon_cards.clear();
    std::cout << "- PASSED\n";
}

void Test::test_lots_turns() {
    std::cout << "TEST_CASE: test_lots_of_turns ";
    std::cout.flush();
    TTRController controller;
    controller.start_game(2);
    Game &g = *controller.game;
    Deck &d = controller.game->deck;
    for (int i = 0; i < 50; i ++) {
        int j = 0;
        while (d.active_wagons[j].color == Multicolored) {
            j++;
        }
        controller.get_card_from_active(j);
    }
    std::cout << "PASSED";
}
