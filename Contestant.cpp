#include "Contestant.h"

Contestant::Contestant(int contestantId, const Skill &skill, int motivation) :
contestantId(contestantId), skill(skill), motivation(motivation), contestant_uf_node(nullptr) {}

void Contestant::set_UF_Node(UnionFind<Contestant*, Team*>::UF_Node *node) {
	contestant_uf_node = node;
}

UnionFind<Contestant*, Team*>::UF_Node *Contestant::getUF_Node() {
	return contestant_uf_node;
}

Contestant::~Contestant() {
	delete contestant_uf_node;
}



