#ifndef TTR_TTRCONTROLLER_H
#define TTR_TTRCONTROLLER_H
#include <QPixmap>
#include "Game.h"
#include "Server/Client.h"
#include "Server/TTRServer.h"
#include "TTRController_fwd.h"
#include "Turn.h"
struct Game_info {
    int number_of_players = 0;
    int number_of_bots = 0;
};
enum class type_of_game { LOCAL_SERVER, SINGLE_COMPUTER, LOCAL_CLIENT };
class TTRController {
    Game *game = nullptr;
    Turn *current_turn = nullptr;
    GameClient *client = nullptr;
    ttr::LocalServer *server = nullptr;
    int my_id = 0;
    bool started = false;
    type_of_game typeOfGame = type_of_game::SINGLE_COMPUTER;
    Game_info info;

public:
    explicit TTRController() = default;
    void start_game(int number_of_players,
                    int number_of_bots = 0,
                    type_of_game type = type_of_game::SINGLE_COMPUTER);
    void start_game_server();
    void get_card_from_active(int, int id = -1);
    void build_path_initialize(int, int id = -1);
    void set_color_to_build_path(const WagonCard &w, int id = -1);
    void get_routes();
    int get_number_of_players();
    void build_station(const std::string &city);
    std::vector<WagonCard> get_current_player_cards();
    std::vector<Path> get_paths();
    void get_card_from_deck(int id = -1);
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
    void try_bot();
    ~TTRController();
};

#endif  // TTR_TTRCONTROLLER_H
