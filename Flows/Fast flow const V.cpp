template <int V, typename T>
class HLPP {
    const T INF = numeric_limits<T>::max();
    struct edge {
        int to, rev;
        T f;

        edge() = default;
        edge(int x, int y, T c): to(x), rev(y), f(c) {}
    };

    vector<edge> adj[V];
    vector<int> lst[V], gap[V];
    T ex[V];
    int mxh, h[V], cnt[V], work;

    void updh(int v, int nh) {
        work++;
        if (h[v] != V) cnt[h[v]]--;
        h[v] = nh;
        if (nh == V) return;
        cnt[nh]++, mxh = nh;
        gap[nh].push_back(v);
        if (ex[v] > 0) lst[nh].push_back(v);
    }

    void globalRelabel() {
        work = 0;
        fill(h, h + V, V);
        fill(cnt, cnt + V, 0);
        for (int i = 0; i < mxh; i++) lst[i].clear(), gap[i].clear();
        h[t] = 0;
        queue<int> q({t});
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            for (auto& e : adj[v]) {
                if (h[e.to] == V && adj[e.to][e.rev].f > 0) {
                    q.push(e.to), updh(e.to, h[v] + 1);
                }
            }
            mxh = h[v];
        }
    }

    void push(int v, edge& e) {
        if (ex[e.to] == 0) lst[h[e.to]].push_back(e.to);
        T df = min(ex[v], e.f);
        e.f -= df, adj[e.to][e.rev].f += df;
        ex[v] -= df, ex[e.to] += df;
    }

    void discharge(int v) {
        int nh = V;
        for (auto& e : adj[v]) {
            if (e.f > 0) {
                if (h[v] == h[e.to] + 1) {
                    push(v, e);
                    if (ex[v] <= 0) return;
                } else chmin(nh, h[e.to] + 1);
            }
        }
        if (cnt[h[v]] > 1) updh(v, nh);
        else {
            for (int i = h[v]; i <= mxh; i++) {
                for (auto j : gap[i]) updh(j, V);
                gap[i].clear();
            }
        }
    }

public:
    int s = V - 2, t = V - 1;

    void add_edge(int x, int to, T f, int is_dir) {
        adj[x].emplace_back(to, adj[to].size(), f);
        adj[to].emplace_back(x, adj[x].size() - 1, is_dir ? 0 : f);
    }

    void clear() {
        for (int i = 0; i < V; ++i) {
            adj[i].clear();
            lst[i].clear();
            gap[i].clear();
        }
        mxh = work = 0;
        fill(h, h + V, 0);
        fill(cnt, cnt + V, 0);
        fill(ex, ex + V, 0);
    }

    T calc(int heur_n = V) {
        fill(ex, ex + V, 0);
        ex[s] = INF;
        globalRelabel();
        for (auto& e : adj[s]) push(s, e);
        for (; mxh >= 0; --mxh) {
            while (!lst[mxh].empty()) {
                int v = lst[mxh].back();
                lst[mxh].pop_back();
                discharge(v);
                if (work > 4 * heur_n) {
                    globalRelabel();
                }
            }
        }
        return ex[t];
    }
};