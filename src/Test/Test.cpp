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
}

void Test::test_move_draw_card_from_deck() {
    std::cout << "TEST_CASE: test_move_draw_card_from_deck ";
    TTRController controller;
    controller.start_game(2);
    Game &g = *controller.game;
    Deck &d = controller.game->deck;
    int start_size = d.wagons_deck.size();
    if (g.players[g.active_player].wagon_cards.size() != 4 ||
        d.wagons_deck.size() != start_size) {
        std::cout << "- FAILED\n";
        std::cout << "Turn 1\n";
        return;
    }
    controller.get_card_from_deck();
    if (g.players[g.active_player].wagon_cards.size() != 5 ||
        d.wagons_deck.size() != start_size - 1 ||
        d.last_card.color !=
        g.players[g.active_player].wagon_cards.back().color) {
        std::cout << "- FAILED\n";
        std::cout << "Turn 2\n";
        return;
    }
    controller.get_card_from_deck();
    if (g.players[g.active_player].wagon_cards.size() != 4 ||
        d.wagons_deck.size() != start_size - 2) {
        std::cout << "- FAILED\n";
        std::cout << "Turn 3\n";
        return;
    }
    controller.get_card_from_deck();
    if (g.players[g.active_player].wagon_cards.size() != 5 ||
        d.wagons_deck.size() != start_size - 3 ||
        d.last_card.color !=
        g.players[g.active_player].wagon_cards.back().color) {
        std::cout << "- FAILED\n";
        std::cout << "Turn 4\n";
        return;
    }
    controller.get_card_from_deck();
    if (g.players[g.active_player].wagon_cards.size() != 6 ||
        d.wagons_deck.size() != start_size - 4) {
        std::cout << "- FAILED\n";
        std::cout << "Turn 5\n";
        return;
    }
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
    if (g.players[g.active_player].wagon_cards.size() != 4 ||
        d.wagons_deck.size() != start_size) {
        std::cout << "- FAILED\n";
        std::cout << "Turn 1\n";
        return;
    }
    controller.get_card_from_active(1);
    if (g.players[g.active_player].wagon_cards.size() != 5 ||
        d.wagons_deck.size() != start_size - 1) {
        std::cout << g.active_player << '\n';
        std::cout << g.players[g.active_player].wagon_cards.size() << ' '
                  << d.wagons_deck.size() << ' ' << start_size - 1 << '\n';
        std::cout << "- FAILED\n";
        std::cout << "Turn 2\n";
        return;
    }
    controller.get_card_from_active(1);
    if (g.players[g.active_player].wagon_cards.size() != 4 ||
        d.wagons_deck.size() != start_size - 2) {
        std::cout << "- FAILED\n";
        std::cout << "Turn 3\n";
        return;
    }
    controller.get_card_from_active(2);
    if (g.players[g.active_player].wagon_cards.size() != 5 ||
        d.wagons_deck.size() != start_size - 3) {
        std::cout << "- FAILED\n";
        std::cout << "Turn 4\n";
        return;
    }
    controller.get_card_from_active(1);
    if (g.players[g.active_player].wagon_cards.size() != 6 ||
        d.wagons_deck.size() != start_size - 4) {
        std::cout << "- FAILED\n";
        std::cout << "Turn 5\n";
        return;
    }
    controller.get_card_from_active(3);
    if (g.players[g.active_player].wagon_cards.size() != 7 ||
        d.wagons_deck.size() != start_size - 5) {
        std::cout << "- FAILED\n";
        std::cout << "Turn 6\n";
        return;
    }
    std::cout << "- PASSED\n";
}

void Test::test_move_draw_card_from_both() {
    TTRController controller;
    controller.start_game(2);
}

void Test::test_move_draw_routes() {
    TTRController controller;
    controller.start_game(2);
}
