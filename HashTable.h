#pragma once

template <typename Key, typename Value>
class HashTable {
private:
	struct Node {
		Key key;
		Value value;
		Node *next;

		Node(const Key& k, const Value& v) : key(k), value(v), next(nullptr) {}
	};

	static const int INITIAL_SIZE = 29;
	static const int EXPAND_FACTOR = 3;

	int m;
	// the current size of hash table
	// the hash function is x mod m.
	// m = INITIAL_SIZE * (EXPAND_FACTOR ^ i), where i is the number of expands

	int n;
	// the number of elements in the hash table

	Node **hashTable;
	// hashTable with chain hashing

	void expand() {
		int old_size = m, updated_size =  m * EXPAND_FACTOR;
		Node **updated = new Node *[updated_size];
		m = updated_size;

		for (int i = 0; i < m; ++i) {
			updated[i] = nullptr;
		}


		for (int i = 0; i < old_size; ++i) {
			Node* temp = hashTable[i];
			while(temp) {
				Node* to_add = temp;
				temp = temp->next;
				int new_hash_result = to_add->key % m;
				to_add->next = updated[new_hash_result];
				updated[new_hash_result] = to_add;
			}
		}
		delete[] hashTable;
		hashTable = updated;
	}


public:
	HashTable() : m(INITIAL_SIZE), n(0), hashTable(new Node *[INITIAL_SIZE]) {
		for (int i = 0; i < m; ++i) {
			hashTable[i] = nullptr;
		}
	}

	HashTable(const HashTable& other) = delete;
	HashTable& operator=(const HashTable& other) = delete;

	~HashTable() {
		for (int i = 0; i < m; ++i) {
			Node *temp = hashTable[i];
			while(temp) {
				Node* to_delete = temp;
				temp = temp->next;
				delete to_delete;
			}
		}

		delete[] hashTable;
	}

	Value* find(const Key& key) const {
		Node *temp = hashTable[key % m];
		while(temp && temp->key != key) {
			temp = temp->next;
		}
		if(!temp) {
			return nullptr;
		}
		return &temp->value;
	}

	void insert(const Key& key, const Value& value) {
		if(!find(key)) {
			Node *temp = hashTable[key % m];
			Node *to_add = new Node(key, value);
			to_add->next = temp;
			hashTable[key % m] = to_add;
			++n;
			if (n==m) {
				expand();
			}
		}
	}

	template <typename Function>
	void applyToValues(Function action) {
		for (int i = 0; i < m; ++i) {
			Node *temp = hashTable[i];
			while(temp) {
				action(temp->value);
				temp = temp->next;
			}
		}
	}








};