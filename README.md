# The Amazing Racenion

This project provides a C++ implementation of a team and contestant competition management system developed for the Data Structures 1 course (234218) at the Technion. The system tracks competing teams, contestant registrations, team duels, and team merges under strict worst-case and amortized time complexity constraints.

## Data Structures & Core Architecture

The core of the system combines a dynamic **Hash Table**, generic **AVL Trees**, and a **Union-Find** data structure operating in sync:

* `contestants`: A dynamic Hash Table using chain hashing to map contestant IDs to contestant records, dynamically expanding to maintain an O(1) load factor.
* `teams_by_id`: An AVL Tree mapping team IDs to active team records.
* `teams_by_motivation`: An AVL Tree sorting active teams by their collective motivation (using team ID as a tie-breaker). Each node maintains the size of its subtree to support `select(i)` queries (finding the i-th team) in O(log k) time, where k is the number of active teams.
* `UnionFind`: Each team maintains a reverse-tree structure of its contestants using union-by-size and iterative path compression. Each node stores relative values for missions and skills that are adjusted during unions and path compressions, ensuring that calculations along the search path to the root remain accurate as the tree structure changes (even when a smaller team recruits a larger one).

## Supported Operations & Complexity

Throughout the complexity bounds below, **k** denotes the number of active (non-eliminated) teams currently in the system, and **n** denotes the total number of contestants registered since the system initialized.

* **`add_team` / `remove_team`**: Registers a new team or eliminates an existing active team in O(log k) worst-case time.
* **`add_contestant`**: Registers a new contestant into an active team, updating the hash table, the team's Union-Find structure, and the motivation tree in O(log k) amortized average time.
* **`duel`**: Simulates a match between two active teams based on their collective attributes in O(log k) worst-case time, incrementing the mission count for all members of both teams in O(1) by updating only the roots' relative values.
* **`get_team_experience`**: Retrieves the current experience score of an active team in O(log k) worst-case time.
* **`get_ith_collective_motivation_team`**: Retrieves the ID of the i-th team sorted by collective motivation in O(log k) worst-case time using the motivation tree's `select(i)` operation.
* **`get_contestant_missions_number`**: Computes the total number of missions a contestant has participated in (including after team elimination) in O(log* n) amortized average time by summing relative mission values along the Union-Find search path.
* **`get_partial_team_skill`**: Computes the cumulative product of skills for a contestant and all team members who joined chronologically before them in O(log* n) amortized average time by multiplying relative skill values along the Union-Find search path.
* **`recruit`**: Merges a recruited team into a recruiting team, uniting their Union-Find trees while preserving chronological skill order and individual mission counts in O(log k + log* n) amortized time.