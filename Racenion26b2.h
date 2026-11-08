//
// 234218 Data Structures 1.
// Semester: 2026 B (Spring).
// Wet Exercise #2 - Racenion.
//
// The following header file contains all methods we expect you to implement.
// You MAY add private methods and fields of your own.
// DO NOT erase or modify the signatures of the public methods.
// DO NOT modify the preprocessors in this file.
// DO NOT use the preprocessors in your other code files.
//

#ifndef RACENION26B2_H_
#define RACENION26B2_H_
#include "wet2util.h"
#include "AVL.h"
#include "HashTable.h"
#include "UnionFind.h"
#include "Contestant.h"
#include "Team.h"


class Racenion {
private:

	struct MotivationKey {
		int teamMotivation;
		int teamId;

		MotivationKey() : teamMotivation(0), teamId(0) {}
		MotivationKey(int motivation, int teamId) : teamMotivation(motivation), teamId(teamId) {}

		bool operator<(const MotivationKey& other) const {
			if(teamMotivation == other.teamMotivation) {
				return teamId < other.teamId;
			}
			return teamMotivation < other.teamMotivation;
		}

		bool operator>(const MotivationKey& other) const {
			return other < *this;
		}

		bool operator==(const MotivationKey& other) const {
			return teamMotivation == other.teamMotivation
			&& teamId == other.teamId;
		}

	};

	static void deleteTeam(Team* t) {
		delete t;
	}

	static void deleteContestant(Contestant* c) {
		delete c;
	}


	HashTable<int, Contestant*> contestants;
	AVLTree<int, Team*> teams_by_id;
	AVLTree<MotivationKey, Team*> teams_by_motivation;




public:
	// <DO-NOT-MODIFY> {

	Racenion();
	virtual ~Racenion();

	StatusType add_team(int teamId);

	StatusType remove_team(int teamId);

	StatusType add_contestant(int contestantId,
	                          int teamId,
	                          const Skill &skill,
	                          int motivation,
	                          int missionsHad);

	output_t<int> duel(int teamId1, int teamId2);

	output_t<int> get_contestant_missions_number(int contestantId);

	output_t<int> get_team_experience(int teamId);

	output_t<int> get_ith_collective_motivation_team(int i);

	output_t<Skill> get_partial_team_skill(int contestantId);

	StatusType recruit(int recruitingTeamId, int recruitedTeamId);

	// } </DO-NOT-MODIFY>
};

#endif // RACENION26B2_H_
