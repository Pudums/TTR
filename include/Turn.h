#ifndef TTR_TURN_H
#define TTR_TURN_H
#include "Path.h"
#include "Route.h"
#include "WagonCard.h"

struct Turn {
public:
    static inline int num = 0;
    virtual ~Turn() = default;
};

struct DrawCardFromDeck final : virtual Turn {
public:
    explicit DrawCardFromDeck();
    ~DrawCardFromDeck() override = default;
};

struct DrawCardFromActive final : virtual Turn {
    std::size_t number;

public:
    explicit DrawCardFromActive(std::size_t id);
    ~DrawCardFromActive() override = default;
};

struct TakeRoutes final : virtual Turn {
public:
    TakeRoutes() = default;
    ~TakeRoutes() override = default;
};

struct BuildPath final : virtual Turn {
private:
    int pos;
    std::vector<WagonCard> cards_to_build;

public:
    explicit BuildPath(int);
    void set_wagons(const std::vector<WagonCard> &w);
    int get_pos();
    std::vector<WagonCard> getWagons();
    ~BuildPath() override = default;
};

#endif  // TTR_TURN_H