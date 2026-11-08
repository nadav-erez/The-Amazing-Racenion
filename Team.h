#include "UnionFind.h"
#include "wet2util.h"

class Contestant;

class Team {
private:
    int teamId;
    int teamMotivation;
    Skill teamSkill;
    int teamExp;
    UnionFind<Contestant*, Team*>::UF_Node *root;

    static const int WIN = 3;
    static const int DRAW = 1;


public:
    explicit Team(int teamId);

    Team(const Team& other) = delete;
    Team& operator=(const Team& other) = delete;
    ~Team() = default;

    int getId() const;
    int getMotivation() const;
    int getExp() const;
    Skill getSkill() const;
    UnionFind<Contestant*, Team*>::UF_Node* getRoot();
    void setRoot(UnionFind<Contestant*, Team*>::UF_Node *node);

    void win();
    void draw();

    void absorb_team_values(Team* other);
    void add_contestant_values(int motivation, const Skill& skill);

};
