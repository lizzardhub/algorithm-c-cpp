#include <bits/stdc++.h>

#define ll long long int

using namespace std;

int flow = 0;
int sflow = 0;
struct TNode {
    bool terminal;

    int size;
    TNode *arcs;
    char *symbol;

    TNode *suffix;
    TNode *term;
    int id; // Id is individual for each node
    int sid; // sid is index of string in set
};

TNode* new_node() {
    TNode *node = new TNode();
    node->terminal = false;

    node->size = 0;
    node->arcs = (TNode*) malloc(0);
    node->symbol = (char*) malloc(0);

    node->suffix = nullptr;
    node->term = nullptr;
    node->id = flow; flow++;
    return node;
}

void addarc(TNode *node, char c) {
    node->size++;
    node->arcs = (TNode *) realloc(node->arcs, node->size * sizeof(TNode));
    node->symbol = (char *) realloc(node->symbol, node->size * sizeof(char));

    TNode &v = node->arcs[node->size - 1];
    v.terminal = false;

    v.size = 0;
    v.arcs = (TNode*) malloc(0);
    v.symbol = (char*) malloc(0);

    v.suffix = nullptr;
    v.term = nullptr;
    v.id = flow; flow++;

    node->symbol[node->size - 1] = c;
}

TNode* getnode(TNode *node, char c) {
    int p = 0;
    while (p < node->size && node->symbol[p] != c) p++;
    if (p < node->size) return &node->arcs[p];
    else return nullptr;
}

void trie_add(TNode *root, string &s) {
    TNode *v = root;
    int pos = 0;
    while (pos < s.length() && getnode(v, s[pos])) {
        v = getnode(v, s[pos]);
        pos++;
    }
    while (pos < s.length()) {
        addarc(v, s[pos]);
        v = getnode(v, s[pos]);
        pos++;
    }
    v->terminal = true;
    v->sid = sflow; sflow++;
}

TNode* make_trie(vector<string> &S) {
    TNode *root = new_node();
    for (int i = 0; i < S.size(); i++) {
        trie_add(root, S[i]);
    }
    return root;
}

void build_suffix(TNode *root) {
    root->suffix = root;
    root->term = root;

    stack<TNode*> g;
    //g.push(root);

    // Especially for root
    for (int c = 0; c < root->size; c++) {
        g.push(&root->arcs[c]);
        root->arcs[c].suffix = root;
        root->arcs[c].term = root;
    }

    while (!g.empty()) {
        TNode *u = g.top();
        g.pop();
        for (int cidx = 0; cidx < u->size; cidx++) { // Char index
            char c = u->symbol[cidx];
            TNode *v = &u->arcs[cidx];
            g.push(v);

            // Operate vertex u:
            TNode *voyage = u->suffix;

            while (voyage != voyage->suffix && !getnode(voyage, c)) {
                voyage = voyage->suffix;
            }

            if (getnode(voyage, c)) {
                // A normal link
                v->suffix = getnode(voyage, c);
            } else {
                // A link to root
                v->suffix = voyage;
            }

            if (v->suffix->terminal) {
                v->term = v->suffix;
            } else {
                v->term = v->suffix->term;
            }
        }
    }
}

void automate(TNode *automaton, string &s, vector<string> &S) {
    TNode *v = automaton;
    for (int i = 0; i < s.length(); i++) {
        char c = s[i];
        while (v != v->suffix && !getnode(v, c)) {
            v = v->suffix;
        }
        if (getnode(v, c)) {
            v = getnode(v, c);
        }
        if (v->terminal || v->term->terminal) {
            cout << "Found match at: " << i + 1 << "\n";
            if (v->terminal) {
                cout << "Str: " << S[v->sid] << "\n\n";
            } else {
                cout << "Str: " << S[v->term->sid] << "\n\n";
            }
        }
    }
}

void print(TNode *root) {
    cout << "# " << root->id << endl;
    cout << "Children: ";
    for (int idx = 0; idx < root->size; idx++) {
        char a = root->symbol[idx];
        cout << a << " " << root->arcs[idx].id << "  ";
    }
    cout << endl;
    cout << "Is terminal: " << root->terminal << endl;
    cout << "Suffix: " << root->suffix->id << endl;
    cout << "Terminal: " << root->term->id << endl;

    for (int idx = 0; idx < root->size; idx++) {
        print(&root->arcs[idx]);
    }
}

int main() {
    vector<string> S;
    int n;
    cin >> n;
    for (int i = 0; i < n; i++) {
        S.push_back("");
        cin >> S[S.size() - 1];
    }

    TNode *root = make_trie(S);
    build_suffix(root);
    string s;
    getline(cin, s);
    getline(cin, s);
    cout << s << "\n";
    //print(root);
    automate(root, s, S);

    return 0;
}
