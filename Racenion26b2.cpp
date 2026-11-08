// You can edit anything you want in this file.
// However, you need to implement all public Racenion functions, which are provided below as a template.

#include "Racenion26b2.h"


Racenion::Racenion() {

}

Racenion::~Racenion() {
	teams_by_id.applyToValues(deleteTeam);
	contestants.applyToValues(deleteContestant);
}

StatusType Racenion::add_team(int teamId) {
	if (teamId <= 0) {
		return StatusType::INVALID_INPUT;
	}

	if(teams_by_id.contains(teamId)) {
		return StatusType::FAILURE;
	}

	Team *newTeam = nullptr;
	try {
		newTeam = new Team(teamId);
		teams_by_id.insert(teamId, newTeam);
		teams_by_motivation.insert(MotivationKey(newTeam->getMotivation(), teamId),newTeam);
	} catch (const std::bad_alloc&) {
		teams_by_id.remove(teamId);
		delete newTeam;
		return StatusType::ALLOCATION_ERROR;
	}

	return StatusType::SUCCESS;
}

StatusType Racenion::remove_team(int teamId) {
	if (teamId <= 0) {
		return StatusType::INVALID_INPUT;
	}

	Team **team_ptr_ptr = teams_by_id.find(teamId);
	if(!team_ptr_ptr) {
		return StatusType::FAILURE;
	}

	Team* team_to_delete = *team_ptr_ptr;

	int motivation_of_removed_team = (*team_ptr_ptr)->getMotivation();
	MotivationKey to_remove(motivation_of_removed_team, teamId);

	teams_by_id.remove(teamId);
	teams_by_motivation.remove(to_remove);

	if(team_to_delete->getRoot()) {
		team_to_delete->getRoot()->group = nullptr;
	}


	delete team_to_delete;
	return StatusType::SUCCESS;
}

StatusType Racenion::add_contestant(int contestantId, int teamId,
	const Skill &skill, int motivation, int missionsHad) {

	if (contestantId <= 0 || teamId <= 0 || motivation < 0 ||
		missionsHad < 0 || !skill.isValid()) {
		return StatusType::INVALID_INPUT;
	}

	Contestant **contestant_ptr_ptr = contestants.find(contestantId);
	Team **team_ptr_ptr = teams_by_id.find(teamId);

	if (contestant_ptr_ptr || !team_ptr_ptr) {
		return StatusType::FAILURE;
	}

	Team *target_team = *team_ptr_ptr;

	// save the team's info before adding the new contestant.
	UnionFind<Contestant*, Team*>::UF_Node *root_a = target_team->getRoot();

	Skill total_skill_of_a = target_team->getSkill();
	int oldMotivation = target_team->getMotivation();

	MotivationKey old_motivation_key(oldMotivation, teamId);
	MotivationKey new_motivation_key(oldMotivation + motivation,teamId);

	bool motivation_changed = !(new_motivation_key == old_motivation_key);

	Contestant *c = nullptr;

	try {
		// create the contestant and its UF_Node.
		c = new Contestant(contestantId, skill, motivation);

		UnionFind<Contestant*, Team*>::UF_Node *c_UF_Node =
			new UnionFind<Contestant*, Team*>::UF_Node(c, missionsHad,skill);

		c->set_UF_Node(c_UF_Node);


		// Update the motivation tree only if the key changes
		if (motivation_changed) {
			teams_by_motivation.insert(new_motivation_key,target_team);
		}

		try {
			contestants.insert(contestantId, c);
		} catch (const std::bad_alloc&) {
			if (motivation_changed) {
				teams_by_motivation.remove(new_motivation_key);
			}
			throw;
		}

		// contestant was inserted successfully, so the old motivation key can
		// now be removed.
		if (motivation_changed) {
			teams_by_motivation.remove(old_motivation_key);
		}

		// update the team info
		target_team->add_contestant_values(motivation,skill);

		// add the new contestant to the team's UnionFind tree.
		UnionFind<Contestant*, Team*>::UF_Node *new_root =
			UnionFind<Contestant*, Team*>::unionGroups(root_a,c_UF_Node,
				total_skill_of_a);

		if (new_root) {
			new_root->group = target_team;
		}

		target_team->setRoot(new_root);

	} catch (const std::bad_alloc&) {
		delete c;
		return StatusType::ALLOCATION_ERROR;
	}

	return StatusType::SUCCESS;
}


output_t<int> Racenion::duel(int teamId1, int teamId2) {
	if (teamId1 <= 0 || teamId2 <= 0 || teamId1 == teamId2) {
		return StatusType::INVALID_INPUT;
	}

	Team **team1_ptr_ptr = teams_by_id.find(teamId1);
	Team **team2_ptr_ptr = teams_by_id.find(teamId2);
	if (!team1_ptr_ptr || !team2_ptr_ptr) {
		return StatusType::FAILURE;
	}

	Team *team1 = *team1_ptr_ptr;
	Team *team2 = *team2_ptr_ptr;

	if(!team1->getRoot() || !team2->getRoot()) {
		return StatusType::FAILURE;
	}

	if(team1->getRoot()->size <= 0 || team2->getRoot()->size <= 0) {
		return StatusType::FAILURE;
	}

	team1->getRoot()->missions_delta++;
	team2->getRoot()->missions_delta++;

	if (team1->getExp() + team1->getMotivation() > team2->getExp() + team2->getMotivation()) {
		team1->win();
		return 1;
	}
	if (team1->getExp() + team1->getMotivation() < team2->getExp() + team2->getMotivation()) {
		team2->win();
		return 3;
	}
	if(team1->getSkill() > team2->getSkill()) {
		team1->win();
		return 2;
	}
	if(team1->getSkill() < team2->getSkill()) {
		team2->win();
		return 4;
	}
	team1->draw();
	team2->draw();
	return 0;

}


output_t<int> Racenion::get_contestant_missions_number(int contestantId) {

	if (contestantId <= 0) {
		return StatusType::INVALID_INPUT;
	}

	Contestant ** contestant_ptr_ptr = contestants.find(contestantId);
	if (!contestant_ptr_ptr) {
		return StatusType::FAILURE;
	}

	Contestant *contestant = *contestant_ptr_ptr;

	UnionFind<Contestant*, Team*>::UF_Node *node = contestant->getUF_Node();

	int missions = 0;
	UnionFind<Contestant*, Team*>::find(node, &missions, nullptr);

	return missions;


}

output_t<int> Racenion::get_team_experience(int teamId) {

	if (teamId <= 0) {
		return StatusType::INVALID_INPUT;
	}

	Team **team_ptr_ptr = teams_by_id.find(teamId);
	if(!team_ptr_ptr) {
		return StatusType::FAILURE;
	}

	Team *team = *team_ptr_ptr;

	return team->getExp();

}

output_t<int> Racenion::get_ith_collective_motivation_team(int i) {
	// Select(i) the algorithm from the lecture of the motivation Tree
    if (i < 1 || i > teams_by_motivation.size()) {
	    return StatusType::FAILURE;
    }

	Team** team_ptr_ptr = teams_by_motivation.select(i);

	if (!team_ptr_ptr) {
		return StatusType::FAILURE;
	}

	return (*team_ptr_ptr)->getId();

}

output_t<Skill> Racenion::get_partial_team_skill(int contestantId) {
	if (contestantId <= 0) {
		return StatusType::INVALID_INPUT;
	}

	Contestant ** contestant_ptr_ptr = contestants.find(contestantId);
	if (!contestant_ptr_ptr) {
		return StatusType::FAILURE;
	}

	Contestant *contestant = *contestant_ptr_ptr;
	UnionFind<Contestant*, Team*>::UF_Node *node = contestant->getUF_Node();

	Skill partial_skill;

	UnionFind<Contestant*, Team*>::UF_Node *root =
		UnionFind<Contestant*, Team*>::find(node, nullptr, &partial_skill);

	if(!root->group) {
		return StatusType::FAILURE;
	}

	return partial_skill;

}

StatusType Racenion::recruit(int recruitingTeamId, int recruitedTeamId) {

	if (recruitingTeamId <= 0 || recruitedTeamId <= 0 ||
		recruitingTeamId == recruitedTeamId) {
		return StatusType::INVALID_INPUT;
	}

	Team **team1_ptr_ptr = teams_by_id.find(recruitingTeamId);

	Team **team2_ptr_ptr = teams_by_id.find(recruitedTeamId);

	if (!team1_ptr_ptr || !team2_ptr_ptr) {
		return StatusType::FAILURE;
	}

	Team *team1 = *team1_ptr_ptr;
	Team *team2 = *team2_ptr_ptr;

	UnionFind<Contestant*, Team*>::UF_Node *root_a = team1->getRoot();

	UnionFind<Contestant*, Team*>::UF_Node *root_b = team2->getRoot();


	// empty team cannot recruit
	if (!root_a) {
		return StatusType::FAILURE;
	}


	if (root_b && team1->getExp() + team1->getMotivation() +
		team1->getSkill().getEffectiveSkill()
		<= team2->getExp() + team2->getMotivation() +
		team2->getSkill().getEffectiveSkill()) {
		return StatusType::FAILURE;
	}

	Skill total_skill_of_a = team1->getSkill();

	int oldMotivation1 = team1->getMotivation();
	int oldMotivation2 = team2->getMotivation();

	MotivationKey old_motivation_key1(oldMotivation1, recruitingTeamId);

	MotivationKey old_motivation_key2(oldMotivation2, recruitedTeamId);

	MotivationKey new_motivation_key(oldMotivation1 + oldMotivation2
		, recruitingTeamId);

	//if the recruited team's motivation is 0, team1's key doesn't change.
	bool motivation_changed = !(new_motivation_key == old_motivation_key1);

	if (motivation_changed) {
		try {
			teams_by_motivation.insert(new_motivation_key, team1);
		} catch (const std::bad_alloc&) {
			return StatusType::ALLOCATION_ERROR;
		}
	}

	// update team1 info
	team1->absorb_team_values(team2);

	// merge the UnionFind trees
	UnionFind<Contestant*, Team*>::UF_Node *new_root =
		UnionFind<Contestant*, Team*>::unionGroups(root_a,root_b,total_skill_of_a);

	if (new_root) {
		new_root->group = team1;
	}

	team1->setRoot(new_root);


	// remove team1's previous key if it was replaced by a different key.
	if (motivation_changed) {
		teams_by_motivation.remove(old_motivation_key1);
	}

	// The recruited team always leaves the motivation tree.
	teams_by_motivation.remove(old_motivation_key2);

	// The recruited team is no longer an active team.
	teams_by_id.remove(recruitedTeamId);

	delete team2;

	return StatusType::SUCCESS;
}