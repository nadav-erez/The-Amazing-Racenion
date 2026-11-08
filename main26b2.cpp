//
// 234218 Data Structures.
// Semester: 2026 B (Spring).
// Wet Exercise #2 — Racenion.
//
// This main file is necessary to link and run your code.
// This file is READ ONLY: Submitting something else will not result
// in compiling your code with other than this file.
//
// DO NOT use the preprocessors in your own code files.
//

#include "Racenion26b2.h"
#include <string>
#include <iostream>

using namespace std;

void print(string cmd, StatusType res);
void print(string cmd, output_t<int> res);
void print(string cmd, output_t<Skill> res);

int main()
{
	int d1, d2, d3, d4, m00, m01, m10, m11, motivation, missionsHad;

	Racenion *obj = new Racenion();

	string op;
	while (cin >> op)
	{
		if (!op.compare("addTeam")) {
			cin >> d1;
			print(op, obj->add_team(d1));

		} else if (!op.compare("removeTeam")) {
			cin >> d1;
			print(op, obj->remove_team(d1));

		} else if (!op.compare("addContestant")) {
			cin >> d1 >> d2 >> m00 >> m01 >> m10 >> m11 >> motivation >> missionsHad;
			Skill skill(m00, m01, m10, m11);
			print(op, obj->add_contestant(d1, d2, skill, motivation, missionsHad));

		} else if (!op.compare("duel")) {
			cin >> d1 >> d2;
			print(op, obj->duel(d1, d2));

		} else if (!op.compare("getContestantMissionsNumber")) {
			cin >> d1;
			print(op, obj->get_contestant_missions_number(d1));

		} else if (!op.compare("getTeamExperience")) {
			cin >> d1;
			print(op, obj->get_team_experience(d1));

		} else if (!op.compare("getIthCollectiveMotivationTeam")) {
			cin >> d1;
			print(op, obj->get_ith_collective_motivation_team(d1));

		} else if (!op.compare("getPartialTeamSkill")) {
			cin >> d1;
			print(op, obj->get_partial_team_skill(d1));

		} else if (!op.compare("recruit")) {
			cin >> d1 >> d2;
			print(op, obj->recruit(d1, d2));

		} else {
			cout << "Unknown command: " << op << endl;
			break;
		}
		if (cin.fail()) {
			cout << "Invalid input format" << endl;
			break;
		}
	}

	delete obj;
	return 0;
}

static const char *StatusTypeStr[] =
{
	"SUCCESS",
	"ALLOCATION_ERROR",
	"INVALID_INPUT",
	"FAILURE"
};

void print(string cmd, StatusType res)
{
	cout << cmd << ": " << StatusTypeStr[(int) res] << endl;
}

void print(string cmd, output_t<int> res)
{
	if (res.status() == StatusType::SUCCESS) {
		cout << cmd << ": " << StatusTypeStr[(int) res.status()]
		     << ", " << res.ans() << endl;
	} else {
		cout << cmd << ": " << StatusTypeStr[(int) res.status()] << endl;
	}
}

void print(string cmd, output_t<Skill> res)
{
	if (res.status() == StatusType::SUCCESS) {
		cout << cmd << ": " << StatusTypeStr[(int) res.status()]
		     << ", " << res.ans() << endl;
	} else {
		cout << cmd << ": " << StatusTypeStr[(int) res.status()] << endl;
	}
}
