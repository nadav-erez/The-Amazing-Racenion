#include "Team.h"

Team::Team(int teamId) : teamId(teamId), teamMotivation(0), teamExp(0), root(nullptr) {}

int Team::getExp() const {
	return teamExp;
}

int Team::getMotivation() const {
	return teamMotivation;
}

Skill Team::getSkill() const {
	return teamSkill;
}

UnionFind<Contestant *, Team *>::UF_Node *Team::getRoot() {
	return root;
}

void Team::setRoot(UnionFind<Contestant *, Team *>::UF_Node *node) {
	root = node;
}

void Team::win() {
	teamExp += WIN;
}

void Team::draw() {
	teamExp += DRAW;
}

void Team::absorb_team_values(Team *other) {
	if(!other || this == other) {
		return;
	}
	teamMotivation += other->getMotivation();
	teamExp += other->getExp();
	teamSkill *= other->getSkill();
}

void Team::add_contestant_values(int motivation, const Skill &skill) {
	teamMotivation += motivation;
	teamSkill *= skill;
}

int Team::getId() const {
	return teamId;
}










