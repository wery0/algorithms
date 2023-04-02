template<typename T, ll N>      //Works with coordinates [0; N]
class implicit_segtree {
    static constexpr T NO = -1;     //Change, if set value can be -1
    static constexpr T INF = numeric_limits<T>::max();
    struct Node {
        Node* l = 0;
        Node* r = 0;
        T ps_set = NO;
        T sm = 0;
        T mx = -INF;

        Node() = default;
    };

    Node* root = new Node();

    T gsm(Node* n) {return n ? n->sm : 0;}
    T gmx(Node* n) {return n ? n->mx : -INF;}

    void setval(Node* n, ll val, ll sz) {
        n->ps_set = val;
        n->sm = sz * val;
        n->mx = val;
    }

    void push(Node* n, ll sz) {
        if (!n->l) n->l = new Node();
        if (!n->r) n->r = new Node();
        setval(n->l, n->ps_set, (sz + 1) >> 1);
        setval(n->r, n->ps_set, sz >> 1);
        n->ps_set = NO;
    }

    void upd(Node* n) {
        n->sm = gsm(n->l) + gsm(n->r);
        n->mx = max(gmx(n->l), gmx(n->r));
    }

    void seg_set(ll ql, ll qr, T val, ll l, ll r, Node*& n) {
        if (qr < l || r < ql) return;
        if (!n) n = new Node();
        if (ql <= l && r <= qr) {
            setval(n, val, r - l + 1);
            return;
        }
        if (n->ps_set != NO) push(n, r - l + 1);
        const ll md = (l + r) >> 1;
        seg_set(ql, qr, val, l, md, n->l);
        seg_set(ql, qr, val, md + 1, r, n->r);
        upd(n);
    }

    T seg_sum(ll ql, ll qr, ll l, ll r, Node*& n) {
        if (!n || qr < l || r < ql) return 0;
        if (n->ps_set != NO) return (max(ql, l) - min(qr, r) + 1) * n->ps_set;
        if (ql <= l && r <= qr) return n->sm;
        const ll md = (l + r) >> 1;
        return seg_sum(ql, qr, l, md, n->l) +
               seg_sum(ql, qr, md + 1, r, n->r);
    }

    T seg_max(ll ql, ll qr, ll l, ll r, Node*& n) {
        if (!n || qr < l || r < ql) return -INF;
        if (n->ps_set != NO) return n->ps_set;
        if (ql <= l && r <= qr) return n->mx;
        const ll md = (l + r) >> 1;
        return max(seg_max(ql, qr, l, md, n->l),
                   seg_max(ql, qr, md + 1, r, n->r));
    }

    void destroy(Node* n){
        if(!n) return;
        destroy(n->l);
        destroy(n->r);
        delete n;
    }

public:
    implicit_segtree() = default;
    ~implicit_segtree() {destroy(root);}

    T seg_sum(ll ql, ll qr) {return seg_sum(ql, qr, 0, N, root);}
    T seg_max(ll ql, ll qr) {return seg_max(ql, qr, 0, N, root);}
    void seg_set(ll ql, ll qr, T val) {seg_set(ql, qr, val, 0, N, root);}
};