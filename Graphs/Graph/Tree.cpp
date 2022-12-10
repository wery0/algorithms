template<typename Data>
struct edge {
    int to;
    Data data;

    edge() = default;
    edge(int to) : to(to) {}
    edge(int to, Data data) : to(to), data(data) {}

    bool operator<(const edge &other) const {return to < other.to;}
    bool operator==(const edge &other) const {return to == other.to && data == other.data;}
};

template<>
struct edge<void> {
    int to;

    edge() = default;
    edge(int to) : to(to) {}

    bool operator<(const edge &other) const {return to < other.to;}
    bool operator==(const edge &other) const {return to == other.to;}
};

template<typename Edge>
struct tree {

    struct edge_range {
        __gnu_cxx::__normal_iterator<Edge*, vector<Edge>> first, last;
        auto begin() const {return first;}
        auto end() const {return last;}

        int size() const {return last - first;}
        Edge& operator[](const int v) {return *(first + v);}
    };

    struct graph {

        int V = -1, E;
        vector<Edge> store;
        vector<int> fir;
        vector<pair<int, Edge>> acc_edge;
        bool is_prepared = 0;

        graph() = default;
        graph(int V) : V(V) {}
        graph(int V, int E) : V(V) {if (E != -1) acc_edge.reserve(E);}

        void prepare() {if (V == -1) {int mxn = -1; for (const auto &[v, e] : acc_edge) {chmax(mxn, v); chmax(mxn, e.to);} V = mxn + 1;} E = acc_edge.size(); store.resize(E); vector<int> deg_cnt(V + 1); for (const auto &p : acc_edge) ++deg_cnt[p.F + 1]; partial_sum(all(deg_cnt), deg_cnt.begin()); fir = deg_cnt; for (const auto &p : acc_edge) store[deg_cnt[p.F]++] = p.S; acc_edge.clear(); acc_edge.shrink_to_fit(); is_prepared = 1;}
        void add_edge(int x, Edge e, const bool is_dir) {if (V != -1) assert(0 <= x && x < V), assert(0 <= e.to && e.to < V); acc_edge.pb({x, e}); if (!is_dir) {swap(x, e.to); acc_edge.pb({x, e});}}
        edge_range operator[](const int v) {assert(is_prepared); return {store.begin() + fir[v], store.begin() + fir[v + 1]};}
    };

    graph g;
    int V = -1;
    bool is_prepared = false;

    void ifs(int v, int p = -1) {
        static int T = 0; T = p == -1 ? 0 : T;
        tin[v] = T++;
        pr[v] = p;
        sz[v] = 1;
        dep[v] = p == -1 ? 0 : dep[p] + 1;
        if (p == -1) {
            jump[v] = v;
        } else if (dep[pr[v]] + dep[jump[jump[pr[v]]]] == dep[jump[pr[v]]] * 2) {
            jump[v] = jump[jump[pr[v]]];
        } else {
            jump[v] = pr[v];
        }
        for (const auto& e : g[v]) {
            if (e.to == p) continue;
            //wdep[e.to] = wdep[v] + e.data;
            ifs(e.to, v);
            sz[v] += sz[e.to];
        }
        tout[v] = T;
    }

    void prepare_and_root(int root_number) {
        assert(!is_prepared);
        g.prepare(); V = g.V;
        assert(g.E == (V - 1) * 2);
        pr.resize(V);
        jump.resize(V);
        dep.resize(V);
        sz.resize(V);
        tin.resize(V);
        tout.resize(V);
        //wdep.resize(V);
        is_prepared = true;
        reroot(root_number);
    }

    vector<int> pr;
    vector<int> jump;
    vector<int> dep;
    vector<int> sz;
    vector<int> tin, tout;
    int root = -1;
    //vector<ll> wdep;

    tree() = default;
    tree(int V): V(V) {g = graph(V, (V - 1) * 2);}


    void add_edge(int x, Edge e) {assert(!is_prepared); g.add_edge(x, e, 0);}
    void reroot(int new_root) {assert(is_prepared); if (root != new_root) ifs(root = new_root);}
    edge_range operator[](const int v) {assert(is_prepared); return g[v];}
    int size() const {return V;}

    //ll weighted_dst(int x, int y, int lc = -1) {if (lc == -1) lc = LCA(x, y); return wdep[x] + wdep[y] - wdep[lc] * 2;}
    int deg(int v) {assert(is_prepared); return g[v].size();}
    int dst(int x, int y, int lc = -1) {if (lc == -1) lc = LCA(x, y); return dep[x] + dep[y] - dep[lc] * 2;}
    int LCA(int x, int y) {assert(is_prepared); if (dep[x] > dep[y]) swap(x, y); y = kth_ancestor(y, dep[y] - dep[x]); if (x == y) return x; for (; pr[x] != pr[y];) {int u1 = jump[x], u2 = jump[y]; if (u1 == u2) x = pr[x], y = pr[y]; else x = u1, y = u2;} return pr[x];}
    int is_descendant(int p, int v) {assert(is_prepared); return tin[p] <= tin[v] && tout[v] <= tout[p];}
    int kth_ancestor(int v, int k) {assert(is_prepared); assert(k <= dep[v]); for (; k;) {int u = jump[v], dfd = dep[v] - dep[u]; if (dfd <= k) v = u, k -= dfd; else v = pr[v], k--;} return v;}
    int get_kth_vertex_on_path(int x, int y, int k, int lc = -1) {if (lc == -1) lc = LCA(x, y); int dst = dep[x] + dep[y] - dep[lc] * 2; if (k > dst) return -1; int d1 = dep[x] - dep[lc]; if (k <= d1) return kth_ancestor(x, k); return kth_ancestor(y, dst - k);}
    int get_centroid() {assert(is_prepared); int v = root; int stv = v, p = -1; for (;;) {int mxsz = -1, h = -1; for (const auto &e : (*this)[v]) {if (e.to == p) continue; if (chmax(mxsz, sz[e.to])) h = e.to;} if (mxsz <= sz[stv] / 2) return v; p = v, v = h;} return -1;}
    int calc_diameter() {assert(is_prepared); int v = max_element(all(dep)) - dep.begin(); auto dfs = [&](auto && dfs, int v, int p) -> int{int mxd = -1; for (const auto &e : g[v]) {if (e.to != p) {int tyt = dfs(dfs, e.to, v); chmax(mxd, tyt);}} return mxd + 1;}; return dfs(dfs, v, -1);}
    vector<int> get_euler_traversal() {assert(is_prepared); vector<int> eul(V); int T = 0; auto dfs = [&](auto && dfs, int v, int p) -> void{eul[T++] = v; for (const auto &e : g[v]) {if (e.to == p) continue; dfs(dfs, e.to, v);}}; dfs(dfs, root, -1); return eul;}
    vector<int> get_path_between_vertexes(int x, int y) {assert(is_prepared); vector<int> fir, sec; while (!is_descendant(x, y)) fir.pb(x), x = pr[x]; while (x != y) sec.pb(y), y = pr[y]; fir.pb(x); reverse(all(sec)); for (int i : sec) fir.pb(i); return fir;}
    vector<int> get_foreach_furthest_vertex_unweighted() {assert(is_prepared); vector<int> deepest1(V), deepest2(V, -1); auto dfs = [&](auto dfs, int v, int p = -1) -> int {deepest1[v] = v; for (const auto &e : g[v]) {if (e.to == p) continue; int tyt = dfs(dfs, e.to, v); if (dep[tyt] >= dep[deepest1[v]]) {deepest2[v] = deepest1[v]; deepest1[v] = tyt;} else if (dep[tyt] > dep[deepest2[v]]) {deepest2[v] = tyt;}} return deepest1[v];}; dfs(dfs, root); vector<int> furthest(V); auto go = [&](auto go, int v, int u = -1, int dst_to_u = -1, int p = -1) -> void {furthest[v] = dst_to_u > dep[deepest1[v]] - dep[v] ? u : deepest1[v]; for (const auto &e : g[v]) {if (e.to == p) continue; int nwu = deepest1[e.to] == deepest1[v] ? deepest2[v] : deepest1[v]; int nwd = nwu == -1 ? -1 : dep[nwu] - dep[v]; if (nwu == -1 || dst_to_u > dep[nwu] - dep[v]) nwu = u, nwd = dst_to_u; go(go, e.to, nwu, nwd + 1, v);}}; go(go, root); return furthest;}
    template<typename D> vector<int> get_foreach_furthest_vertex_weighted() {assert(is_prepared); vector<int> deepest1(V), deepest2(V, -1); vector<D> wdep(V); auto dfs = [&](auto dfs, int v, int p = -1) -> int {deepest1[v] = v; for (const auto &e : g[v]) {if (e.to == p) continue; wdep[e.to] = wdep[v] + e.data; int tyt = dfs(dfs, e.to, v); if (wdep[tyt] >= wdep[deepest1[v]]) {deepest2[v] = deepest1[v]; deepest1[v] = tyt;} else if (wdep[tyt] > wdep[deepest2[v]]) {deepest2[v] = tyt;}} return deepest1[v];}; dfs(dfs, root); vector<int> furthest(V); auto go = [&](auto go, int v, int u = -1, D dst_to_u = -1, int p = -1) -> void {furthest[v] = dst_to_u > wdep[deepest1[v]] - wdep[v] ? u : deepest1[v]; for (const auto &e : g[v]) {if (e.to == p) continue; int nwu = deepest1[e.to] == deepest1[v] ? deepest2[v] : deepest1[v]; D nwd = nwu == -1 ? -1 : wdep[nwu] - wdep[v]; if (nwu == -1 || dst_to_u > wdep[nwu] - wdep[v]) nwu = u, nwd = dst_to_u; go(go, e.to, nwu, nwd + e.data, v);}}; go(go, root); return furthest;}
    ll sum_of_distances_between_all_pairs_of_vrt_unweighted() {ll o = 0; auto dfs = [&](auto && dfs, int v, int pr) -> void {for (const auto &e : (*this)[v]) {if (e.to == pr) continue; dfs(dfs, e.to, v); o += 1ll * sz[e.to] * (V - sz[e.to]);}}; return o;}
    ll sum_of_distances_between_all_pairs_of_vrt_weighted() {ll o = 0; auto dfs = [&](auto && dfs, int v, int pr) -> void {for (const auto &e : (*this)[v]) {if (e.to == pr) continue; dfs(dfs, e.to, v); o += 1ll * e.data * sz[e.to] * (V - sz[e.to]);}}; return o;}
    ll sum_of_distances_between_all_pairs_of_vrt_weighted_mod(const ll mod) {ll o = 0; auto dfs = [&](auto && dfs, int v, int pr) -> void {for (const auto &e : (*this)[v]) {if (e.to == pr) continue; dfs(dfs, e.to, v); o += 1ll * sz[e.to] * (V - sz[e.to]) % mod * e.data % mod;}}; return o % mod;}
    vector<int> get_rooted_isomorphic_subtrees_without_data_guaranteed() {vector<int> ans(V); map<vector<int>, int> mp; int cnt = 1; vector<pii> stq = {{root, 0}}; while (stq.size()) {pii p = stq.back(); const int pr = stq.size() == 1 ? -1 : stq[stq.size() - 2].F; int v = p.F; if (p.S < (*this)[v].size()) {++stq.back().S; if ((*this)[v][p.S].to != pr) stq.pb({(*this)[v][p.S].to, 0});} else {vector<int> tyt; tyt.reserve((*this)[v].size()); for (const auto &e : (*this)[v]) {if (e.to != pr) tyt.pb(ans[e.to]);} sort(all(tyt)); int& i = mp[tyt]; if (!i) i = cnt++; ans[v] = i - 1; stq.pop_back();}} return ans;}
    vector<int> get_prufer_code() {assert(is_prepared && V > 1); vector<int> cnt(V), pr(V); auto dfs = [&](auto && dfs, int v, int p) -> void {pr[v] = p; for (const auto& e : g[v]) {if (e.to != p) dfs(dfs, e.to, v);}}; dfs(dfs, V - 1, -1); for (int q = 0; q < V; ++q) cnt[q] = deg(q); vector<int> ans(V - 2); for (int q = 0, leaf = find(all(cnt), 1) - cnt.begin(), ptr = leaf + 1; q < V - 2; ++q) {int p = pr[leaf]; ans[q] = p; --cnt[leaf]; if (--cnt[p] == 1 && p < ptr) {leaf = p;} else {for (; cnt[ptr] != 1;) ++ptr; leaf = ptr++;}} return ans;}
};