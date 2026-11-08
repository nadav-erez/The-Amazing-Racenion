#pragma once
#include "wet2util.h"

template <typename Element, typename Group>
class UnionFind {
public:
	struct UF_Node {
		Element element;
		// current element

		Group group;
		// nullptr for every UF_Node that is not a root of an up-tree;
		// for the root of an up-tree, has the group info;

		int size;
		// contains garbage for every UF_Node that is not a root of an up-tree;
		// // for the root of an up-tree, has number of elements in the current up-tree;

		UF_Node *parent;
		// father of the current UF_Node in the up-tree;
		// nullptr for every root in the forest of up-trees;

		int missions_delta;
		// used for get_contestant_missions_number;
		// updated in unions and in path compressions;

		Skill relative_skill;
		// used for get_partial_team_skill;
		// updated in unions and in path compressions;



		UF_Node(const Element& e, int missionsHad, const Skill& skill) :
		element(e), group(nullptr), size(1), parent(nullptr),
		missions_delta(missionsHad), relative_skill(skill) {}
	};


	// this function takes a UF_Node as input, and "climbs" up the up-tree,
	// and returns its root (UF_Node also).
	// in the end, it calls helper function to compress the search path
	static UF_Node* find(UF_Node* start_node, int* missions_res = nullptr, Skill* skill_res = nullptr) {
		if (!start_node) {
			return nullptr;
		}

		UF_Node *root = start_node;
		int temp_sum = 0;
		Skill temp_skill;

		while (root->parent) {
			temp_sum += root->missions_delta;
			temp_skill = root->relative_skill * temp_skill;
			root = root->parent;
		}

		if(missions_res) {
			*missions_res = temp_sum + root->missions_delta;
		}

		if(skill_res) {
			*skill_res = root->relative_skill * temp_skill;
		}

		pathCompression(root, start_node, temp_sum, temp_skill);
		return root;


	}

	static UF_Node* unionGroups(UF_Node *root_a, UF_Node *root_b, const Skill& total_skill_of_a) {
		// handle the edge cases: one of the groups is empty,
		// or trying to union a group with itself
		if(!root_a) {
			return root_b;
		}
		if(!root_b) {
			return root_a;
		}
		if(root_a == root_b) {
			return root_a;
		}

		// normal case: union by size (smaller into larger)
		if(root_a->size >= root_b->size) {
			root_a->size += root_b->size;
			root_b->size = 0;
			root_b->parent = root_a;
			root_b->missions_delta -= root_a->missions_delta;
			root_b->group = nullptr;
			root_b->relative_skill = root_a->relative_skill.inv() * total_skill_of_a * root_b->relative_skill;
			return root_a;
		}
		else {
			root_b->size += root_a->size;
			root_a->size = 0;
			root_a->parent = root_b;
			root_a->missions_delta -= root_b->missions_delta;
			root_a->group = nullptr;
			root_b->relative_skill = total_skill_of_a * root_b->relative_skill;
			root_a->relative_skill = root_b->relative_skill.inv() * root_a->relative_skill;
			return root_b;
		}
	}



private:

	// compress a search path: gets a root of an up-tree and the deepest UF_Node in a search path
	// for each UF_Node in the search path, it changes its parent to be the root to achieve O(log* n)
	static void pathCompression(UF_Node* root, UF_Node* start_node, int sum, const Skill& skill) {
		if(!root || !start_node || root == start_node || start_node->parent == root) {
			return;
		}

		int temp_sum = sum;
		Skill temp_skill = skill;

		UF_Node *next = nullptr;

		while(start_node->parent && start_node->parent != root) {
			next = start_node->parent;
			start_node->parent = root;
			int old_sum = start_node->missions_delta;
			start_node->missions_delta = temp_sum;
			Skill old_skill = start_node->relative_skill;
			start_node->relative_skill = temp_skill;
			temp_sum -= old_sum;
			temp_skill *= old_skill.inv();
			start_node = next;
		}

	}

};
