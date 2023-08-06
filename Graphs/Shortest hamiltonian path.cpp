//Calculates shortest hamiltonian path in graph with adjacency matrix m
//Returns {length_of_shortest_path, path_itself} if it exists, of {NO, {}} otherwise
//Edge i -> j absence should be marked as m[i][j] == NO
//<O(2^n * n^2), O(2^n * n)>
template<typename T>
pair<T, vector<size_t>> shortest_hamiltonian_path(vector<vector<T>> m, const T NO = -1) {
    static_assert(is_signed_v<T>);
    static constexpr T INF = numeric_limits<T>::max();
    const size_t n = m.size();
    if (n == 0) return {0, {}};
    const size_t all = 1 << n;
    for (const auto& row : m) assert(row.size() == n);
    vector<size_t> has_edge_mask(n);
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            has_edge_mask[i] |= (m[i][j] != NO ? 1 : 0) << j; 
        }
    }
    vector<vector<T>> dp(all, vector<T>(n));
    for (size_t mask = 1; mask < all; ++mask) {
        if (!(mask & (mask - 1))) continue;
        for (size_t cm = mask; cm; cm &= cm - 1) {
            size_t first = __builtin_ctzll(cm);
            T min_len = INF;
            for (size_t nm = (mask ^ (1 << first)) & has_edge_mask[first]; nm; nm &= nm - 1) {
                size_t next = __builtin_ctzll(nm);
                if (dp[mask ^ (1 << first)][next] == INF) continue;
                min_len = min(min_len, dp[mask ^ (1 << first)][next] + m[first][next]);
            }
            dp[mask][first] = min_len;
        }
    }
    auto it = min_element(dp[all - 1].begin(), dp[all - 1].end());
    if (*it == INF) return {NO, {}};
    vector<size_t> ans(n);
    for (size_t mask = all - 1, first = it - dp[all - 1].begin(), i = 0; i < n; ++i) {
        ans[i] = first;
        mask ^= 1 << first;
        T min_len = INF;
        size_t best_next = 0;
        for (size_t next = 0; next < n; ++next) {
            if ((~mask >> next & 1) || dp[mask][next] == INF || m[first][next] == NO) continue;
            T tyt = dp[mask][next] + m[first][next];
            if (tyt < min_len) {
                min_len = tyt;
                best_next = next;
            }
        }
        first = best_next;
    }
    return {*it, ans};
}
