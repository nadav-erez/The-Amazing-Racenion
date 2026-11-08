#pragma once
#include "UnionFind.h"
#include "wet2util.h"

class Team;


class Contestant {
private:
    int contestantId;
    Skill skill;
    int motivation;
    UnionFind<Contestant*, Team*>::UF_Node *contestant_uf_node;

public:
    Contestant(int contestantId, const Skill& skill, int motivation);

    ~Contestant();

    Contestant(const Contestant& other) = delete;
    Contestant& operator=(const Contestant& other) = delete;

    void set_UF_Node(UnionFind<Contestant*, Team*>::UF_Node *node);
    UnionFind<Contestant*, Team*>::UF_Node* getUF_Node();

};
