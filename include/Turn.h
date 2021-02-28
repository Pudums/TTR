#ifndef TTR_TURN_H
#define TTR_TURN_H
#include "Path.h"
#include "Route.h"
#include "WagonCard.h"
struct Turn {
public:
    virtual ~Turn() = default;
};
struct DrawCardFromDeck final : virtual Turn {
public:
    explicit DrawCardFromDeck() = default;
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
    Path path;
    int pos;
    std::vector<WagonCard> cards_to_build;

public:
    explicit BuildPath(const Path &p);
    void setPath(int n);
    void set_wagons(std::vector<WagonCard> w);
    int get_pos();
    std::vector<WagonCard> getWagons();
    ~BuildPath() override = default;
};

#endif  // TTR_TURN_H
