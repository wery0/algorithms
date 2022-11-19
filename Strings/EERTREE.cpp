template<const int ALPHABET = 26>
struct EERTREE {

    struct Node {
        int len = 0;
        Node *sl = 0;
        Node *m[ALPHABET] = {0};

        Node() = default;
        Node(int d): len(d) {}
    };

    str t;
    Node *r0 = new Node(0);
    Node *r1 = new Node(-1);
    Node *lst = r0;

    Node* go(Node *n) {
        for (; n != r1; n = n->sl) {
            int cur = t.size() - 1;
            int pr = cur - 1 - n->len;
            if (pr >= 0 && t[pr] == t[cur]) break;
        }
        return n;
    }

    EERTREE() {r0->sl = r1->sl = r1;}

    int vertex_count = 0;

    void add_char(char c) {
        assert('a' <= c && c < 'a' + ALPHABET);
        c -= 'a';
        t += c;
        lst = go(lst);
        if (!lst->m[c]) {
            vertex_count++;
            lst->m[c] = new Node(lst->len + 2);
            lst->m[c]->sl = lst == r1 ? r0 : go(lst->sl)->m[c];
        }
        lst = lst->m[c];
    }
};