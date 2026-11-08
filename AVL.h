#ifndef AVL_H_
#define AVL_H_

#include <stdexcept>
#include <new>


template <typename Key, typename Value>
class AVLTree {
private:
    struct Node {
        Key key;
        Value value;
        Node *left;
        Node *right;
        Node *next_inorder; //each node has a pointer to the 'next_inorder' node
        Node *prev_inorder; //each node has a pointer to the 'prev_inorder' node
        int height;
        int size; // the number of nodes in the subtree including itself

        Node(const Key& k, const Value& v)
            : key(k), value(v), left(nullptr), right(nullptr),
        next_inorder(nullptr), prev_inorder(nullptr), height(0), size(1) {}
    };

    struct keyAndVal {
        Key key;
        Value val;
    };

    Node *root;
    Node *min;
    Node *next_node_to_visit;
    Key last_visited_key;
    bool has_visited; // indicates if last_visited_key has garbage or valid info
    int node_count;

    static int nodeSize(const Node* n) {
        return n ? n->size : 0;
    }
    //
    static void updateSize(Node* n) {
        n->size = 1 + nodeSize(n->left) + nodeSize(n->right);
    }
    static int nodeHeight(const Node* n) {
        return n ? n->height : -1;
    }

    static int maxInt(int a, int b) {
        return a > b ? a : b;
    }

    static void updateHeight(Node* n) {
        n->height = 1 + maxInt(nodeHeight(n->left), nodeHeight(n->right));
    }

    static int balanceFactor(const Node* n) {
        return n ? nodeHeight(n->left) - nodeHeight(n->right) : 0;
    }


    static Node* rotateRight(Node* d) {
        if (!d || !d->left) {
            return d;
        }
        Node* new_root = d->left;

        d->left = new_root->right;
        new_root->right = d;

        updateHeight(d);
        updateHeight(new_root);
        updateSize(d);
        updateSize(new_root);

        return new_root;
    }

    static Node* rotateLeft(Node* b) {
        if (!b || !b->right) {
            return b;
        }

        Node* new_root = b->right;

        b->right = new_root->left;
        new_root->left = b;

        updateHeight(b);
        updateHeight(new_root);
        updateSize(b);
        updateSize(new_root);

        return new_root;

    }

    static Node* rebalance(Node *n) {
        updateHeight(n);
        updateSize(n);
        int bf = balanceFactor(n);

        if (bf >= 2) {
            //LL
            if (balanceFactor(n->left) >= 0) {
                return rotateRight(n);
            }
            //LR
            else {
                n->left = rotateLeft(n->left);
                return rotateRight(n);
            }
        }

        if (bf <= -2) {
            //RR
            if(balanceFactor(n->right) <= 0) {
                return rotateLeft(n);
            }
            //RL
            else {
                n->right = rotateRight(n->right);
                return rotateLeft(n);
            }
        }

        return n;
    }


    Node* insertRec(Node* n, const Key& key, const Value& value, bool* inserted,
        Node *next, Node *prev) {
        if (!n) {
            *inserted = true;
            Node *new_node = new Node(key, value);
            new_node->next_inorder = next;
            new_node->prev_inorder = prev;
            if(next) {
                next->prev_inorder = new_node;
            }
            if(prev) {
                prev->next_inorder = new_node;
            }
            else {
                min = new_node;
            }

            if (has_visited) {
                if (key > last_visited_key) {
                    if (!next_node_to_visit || key < next_node_to_visit->key) {
                        next_node_to_visit = new_node;
                    }
                }
            } else {
                if (next_node_to_visit && key < next_node_to_visit->key) {
                    next_node_to_visit = new_node;
                }
            }
            return new_node;
        }

        if (key == n->key) {
            *inserted = false;
            return n;
        }

        if (key < n->key) {
            n->left = insertRec(n->left, key, value, inserted, n, prev);
        } else {
            n->right = insertRec(n->right, key, value, inserted, next, n);
        }


        if (!*inserted) {
            return n;
        }

        return rebalance(n);
    }

    Node* removeRec(Node* n, const Key& key, bool* removed) {
        if (!n) {
            *removed = false;
            return nullptr;
        }

        if (key < n->key) {
            n->left = removeRec(n->left, key, removed);
        } else if (n->key < key) {
            n->right = removeRec(n->right, key, removed);
        } else {
            // we need to remove this node
            *removed = true;
            if (n->left == nullptr || n->right == nullptr) {
                Node* temp = n->left ? n->left : n->right;
                if(n->next_inorder) {
                    n->next_inorder->prev_inorder = n->prev_inorder;
                }
                if(n->prev_inorder) {
                    n->prev_inorder->next_inorder = n->next_inorder;
                }
                else {
                    min = n->next_inorder;
                }
                delete n;
                return temp;
            }

            Node* succ = n->next_inorder;

            if (next_node_to_visit == n) {
                next_node_to_visit = succ;
            } else if (next_node_to_visit == succ) {
                next_node_to_visit = n;
            }

            n->key = succ->key;
            n->value = succ->value;

            Key succ_key = succ->key;
            bool dummy = false;
            n->right = removeRec(n->right, succ_key, &dummy);
        }

        if (!*removed) {
            return n;
        }
        return rebalance(n);
    }

    static Node* findNode(Node* n, const Key& key) {
        while (n) {
            if (key == n->key) {
                return n;
            }
            n = key < n->key ? n->left : n->right;
        }
        return nullptr;
    }

    static void destroyRec(Node* n) {
        if (!n) {
            return;
        }
        destroyRec(n->left);
        destroyRec(n->right);
        delete n;
    }

    template<typename Function>
    static void applyPostOrderRec(Node* n, Function& action) {
        if (!n) return;
        applyPostOrderRec(n->left, action);
        applyPostOrderRec(n->right, action);
        action(n->value);
    }

    static keyAndVal* mergeSortedArrays(keyAndVal *a1, int size1, keyAndVal *a2,
        int size2, keyAndVal *res) {

        int i_a = 0, i_b = 0, i_c = 0;

        while (i_a < size1 && i_b < size2) {
            if (a1[i_a].key < a2[i_b].key) {
                res[i_c++] = a1[i_a++];
            }
            else {
                res[i_c++] = a2[i_b++];
            }
        }

        while (i_a < size1) {
            res[i_c++] = a1[i_a++];
        }

        while(i_b < size2) {
            res[i_c++] = a2[i_b++];
        }
        return res;
    }

    //build a balanced avl Tree from sorted array
    static Node* buildTreeRec(const keyAndVal* arr, int start, int end) {
        if (start > end) {
            return nullptr;
        }

        int mid = (start + end) / 2;

        Node *n = new Node(arr[mid].key, arr[mid].val);

        try {
            n->left = buildTreeRec(arr, start, mid - 1);
            n->right = buildTreeRec(arr, mid + 1, end);
        } catch (std::bad_alloc&) {
            destroyRec(n->left);
            destroyRec(n->right);
            delete n;
            throw;
        }

        updateHeight(n);
        updateSize(n);
        return n;
    }


    //update next_inorder and prev_inorder for each node after building an
    //avl tree from sorted array
    static void updateNextAndPrev(Node* n, Node*& prev_node, Node*& new_min) {
        if (!n) {
            return;
        }
        updateNextAndPrev(n->left, prev_node, new_min);

        n->prev_inorder = prev_node;

        if (prev_node != nullptr) {
            prev_node->next_inorder = n;
        } else {
            new_min = n;
        }

        prev_node = n;

        updateNextAndPrev(n->right, prev_node, new_min);
    }




public:
    AVLTree() : root(nullptr), min(nullptr), next_node_to_visit(nullptr),
    last_visited_key(Key()), has_visited(false), node_count(0) {}

    ~AVLTree() {
        destroyRec(root);
    };

    AVLTree(const AVLTree& other) = delete;
    AVLTree& operator=(const AVLTree& other) = delete;

    bool empty() const {
        return node_count == 0 && !root;
    }

    int size() const {
        return node_count;
    }

    int height() const {
        return nodeHeight(root);
    }

    bool insert(const Key& key, const Value& value) {
        bool inserted = false;
        Node *next = nullptr;
        Node *prev = nullptr;
        root = insertRec(root, key, value, &inserted, next, prev);
        if (inserted) {
            ++node_count;
        }
        return inserted;
    }

    bool remove(const Key& key) {
        //if the next_node_to_visit is the key to remove, we move it 1 step ahead
        if (next_node_to_visit && next_node_to_visit->key == key) {
            next_node_to_visit = next_node_to_visit->next_inorder;
        }

        bool removed = false;
        root = removeRec(root, key, &removed);
        if (removed) {
            --node_count;
        }
        return removed;
    }

    bool contains(const Key& key) const {
        return findNode(root, key) != nullptr;
    }

    Value* find(const Key& key) {
        Node* n = findNode(root, key);
        return n ? &n->value : nullptr;
    }

    const Value* find(const Key& key) const {
        Node* n = findNode(root, key);
        return n ? &n->value : nullptr;
    }

    template <typename Function>
    void applyToValues(Function action) {
        applyPostOrderRec(root, action);
    }

    const Key* goToNextNode() {
        if (!next_node_to_visit) {
            next_node_to_visit = min;
        }
        if (!next_node_to_visit) {
            return nullptr;
        }

        //we need to return a pointer to the key of this node, and then advance it
        const Key* ret_key = &next_node_to_visit->key;

        last_visited_key = *ret_key;
        has_visited = true;

        next_node_to_visit = next_node_to_visit->next_inorder;

        return ret_key;
    }

    //build an array of key and values sorted by the key, from a tree
    keyAndVal* TreeToSortedArr() const {
        if (node_count <= 0) {
            return nullptr;
        }
        keyAndVal *arr = new keyAndVal[node_count];
        Node *it = min;
        for (int i = 0; i < node_count; ++i) {
            arr[i].key = it->key;
            arr[i].val = it->value;
            it = it->next_inorder;
        }
        return arr;
    }

    //get an array of values from a tree (without the keys)
    Value* getValuesArray() const {
        if (node_count == 0) {
            return nullptr;
        }
        Value* arr = new Value[node_count];
        int i = 0;
        for (Node* it = min; it != nullptr; it = it->next_inorder) {
            arr[i++] = it->value;
        }
        return arr;
    }

    //merge other tree into this tree
    void mergeWith(AVLTree& other) {
        int total_nodes = this->node_count + other.node_count;

        if (total_nodes == 0) {
            return;
        }

        keyAndVal *arr1 = nullptr;
        keyAndVal *arr2 = nullptr;
        keyAndVal *merged_arr = nullptr;

        try {
            arr1 = this->TreeToSortedArr();
            arr2 = other.TreeToSortedArr();
            merged_arr = new keyAndVal[total_nodes];
        } catch (std::bad_alloc&) {
            delete[] arr1;
            delete[] arr2;
            delete[] merged_arr;
            throw;
        }

        mergeSortedArrays(arr1, this->node_count, arr2, other.node_count, merged_arr);

        Node* new_root = nullptr;
        try {
            new_root = buildTreeRec(merged_arr, 0, total_nodes - 1);
        } catch (std::bad_alloc&) {
            destroyRec(new_root);
            delete[] arr1;
            delete[] arr2;
            delete[] merged_arr;
            throw;
        }

        destroyRec(this->root);
        destroyRec(other.root);

        this->root = new_root;
        this->node_count = total_nodes;

        Node* prev = nullptr;
        Node* new_min = nullptr;
        updateNextAndPrev(this->root, prev, new_min);

        if (prev) {
            prev->next_inorder = nullptr;
        }

        this->min = new_min;
        this->next_node_to_visit = nullptr;
        this->last_visited_key = Key();
        this->has_visited = false;

        delete[] arr1;
        delete[] arr2;
        delete[] merged_arr;

        other.root = nullptr;
        other.min = nullptr;
        other.next_node_to_visit = nullptr;
        other.node_count = 0;
        other.has_visited = false;
        other.last_visited_key = Key();
    }

    Value* select(int i) {
        // return the ith element in the tree sorted by key
        if (i < 1 || i > node_count) {
            return nullptr;
        }
        Node* n = root;
        while (n) {
            int left_subtree_size = nodeSize(n->left);
            if (i == left_subtree_size + 1) {
                return &n->value;
            } 
            if (i <= left_subtree_size) {
                n = n->left;
            } 
            else {
                i -= left_subtree_size + 1;
                n = n->right;
                }
            }
            return nullptr;
        }
};

#endif // AVL_H_
