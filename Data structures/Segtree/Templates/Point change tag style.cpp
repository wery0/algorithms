template<typename T>
class segtree_point_upd {

    struct tag {

        tag() = default;

        friend inline bool is_neutral(const tag& t) {
            //Write neutral if need, or remove it
            return false;
        }

        friend inline void merge(const tag& l, const tag& r, tag& res) {
            if (is_neutral(l)) {res = r; return;} if (is_neutral(r)) {res = l; return;}
            //Write merge
        }

        friend inline tag merge(const tag& l, const tag& r) {
            tag res;
            merge(l, r, res);
            return res;
        }

//        friend inline void merge_to_left(tag& l, const tag& r) {
//            if (is_neutral(l)) {l = r; return;}if (is_neutral(r)) return;
//
//        }
//
//        friend inline void merge_to_right(const tag& l, tag& r) {
//            if (is_neutral(l)) return;if (is_neutral(r)) {r = l; return;}
//
//        }
    };

    tag neutral_tag;    //Init neutral tag

    const uint a, U;
    vector<tag> m;

    inline void upd(uint v) {
        merge(m[v << 1], m[v << 1 | 1], m[v]);
    }

public:
    segtree_point_upd() = default;
    template<typename I>
    segtree_point_upd(I first, I last): a(last - first), U(geq_pow2(a)) {
        m.resize(U * 2);
        for (uint q = 0; q < a; ++q) {
            tag& t = m[U + q];
            const T val = *(first + q);
            //Write init of last layer
        }
        for (uint q = U; --q;) {
            const tag &l = m[q << 1], &r = m[q << 1 | 1];
            merge(l, r, m[q]);
        }
    }
    template<typename T_arr>
    segtree_point_upd(T_arr& n) {
        (*this) = segtree_point_upd<T>(n.begin(), n.end());
    }
    segtree_point_upd(uint a) {
        vector<T> m(a);
        (*this) = segtree_point_upd<T>(m);
    }

    inline tag seg_statistic(uint ql, uint qr) {
        ql += U, qr += U;
        tag lt = neutral_tag;
        tag rt = neutral_tag;
        for (; ql <= qr; ql = (ql + 1) >> 1, qr = (qr - 1) >> 1) {
            if (ql & 1) lt = merge(lt, m[ql]);
            if (~qr & 1) rt = merge(m[qr], rt);
//            if (ql & 1) merge_to_left(lt, m[ql]);
//            if (~qr & 1) merge_to_right(m[qr], rt);
        }
        return merge(lt, rt);
    }

    inline void point_change(uint p, tag t) {
        p += U;
        m[p] = t;
        for (p >>= 1; p; p >>= 1) {
            upd(p);
        }
    }
};
//Bottom-up segtree, supports only point updates