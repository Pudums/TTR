#ifndef TTR_TTRCONTROLLER_H
#define TTR_TTRCONTROLLER_H
#include <QPixmap>
#include "Game.h"
#include "Server/Client.h"
#include "Server/TTRServer.h"
#include "TTRController_fwd.h"
#include "Turn.h"
struct Game_info{
    int number_of_players = 0;
    int number_of_bots = 0;
};
class TTRController {
    bool is_local = false;
    Game *game = nullptr;
    Turn *current_turn = nullptr;
    GameClient *client = nullptr;
    ttr::LocalServer* server = nullptr;
    int my_id = 0;
    bool single_computer = true;
    bool started = false;
    Game_info info;
public:
    explicit TTRController() = default;
    void start_game(int number_of_players, int number_of_bots = 0, bool is_local_ = true, bool single_computer_ = true);
    void start_game_server();
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
    bool is_game_started() const;
    void end_game();
    int get_current_player_id();
    std::vector<Path> get_all_paths();
    std::vector<int> get_results();
    std::vector<std::pair<std::string, Circle>> get_stations();
    ~TTRController();
};

#endif  // TTR_TTRCONTROLLER_H
