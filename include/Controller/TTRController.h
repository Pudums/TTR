#ifndef TTR_TTRCONTROLLER_H
#define TTR_TTRCONTROLLER_H
#include <QPixmap>
#include "Game.h"
#include "Server/Client.h"
#include "Server/TTRServer.h"
#include "Turn.h"
class TTRController {
    bool is_local = false;
    Game *game = nullptr;
    Turn *current_turn = nullptr;
    GameClient *client = nullptr;
    ttr::LocalServer* server = nullptr;
public:
    explicit TTRController() = default;
    void start_game(int num, bool is_local_ = true);
    void get_card_from_active(int);
    void build_path_initialize(int);
    void set_color_to_build_path(const WagonCard &w);
    void get_routes();
    void build_station(const std::string &city);
    const std::vector<WagonCard> &get_current_player_cards();
    std::vector<Path> get_paths();
    void get_card_from_deck();
    std::map<std::string, int> get_count_by_color();
    std::vector<WagonCard> get_active_cards();
    std::vector<Player> get_players();
    int is_game_end();
    void end_game();
    int get_current_player_id();
    std::vector<Path> get_all_paths();
    std::vector<int> get_results();
    std::vector<std::pair<std::string, Circle>> get_stations();
    ~TTRController();
};

#endif  // TTR_TTRCONTROLLER_H
