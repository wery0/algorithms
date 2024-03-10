//Divides the array of length n into exactly k subsegments, minimizing the cost
//Returns {mincost, borders of subsegments}, if partition exist. If doesn't, returns {0, {}}
//seg_cost must be function in form C(int l, int r, int seg_num)
//<O(kn * T(cost)), O(nk)>
//Requirement: opt is monotonous in both dimension, i. e. opt[i - 1][j] <= opt[i][j] <= opt[i][j + 1]
template<typename C = int64_t>
pair<C, vector<array<int, 2>>> mincost_subsegment_partition_with_answer_restore_knuth(const size_t n, const size_t k, auto seg_cost) {
    static constexpr C inf = numeric_limits<C>::max() / 2;
    assert(0 < k && k <= n);
    vector dp(k, vector<C>(n, inf));
    vector opt(k, vector<int>(n, -1));
    for (size_t r = 0; r < n; ++r) dp[0][r] = seg_cost(0, r, 0), opt[0][r] = 0;
    for (size_t i = 1; i < k; ++i) {
        for (size_t r = n - 1; r >= i; --r) {
            const size_t opt_l = opt[i - 1][r];
            const size_t opt_r = r == n - 1 ? r : opt[i][r + 1];
            for (size_t l = opt_l; l <= opt_r; ++l) {
                C tyt = dp[i - 1][l - 1] + seg_cost(l, r, i);
                if (tyt < dp[i][r]) {
                    dp[i][r] = tyt;
                    opt[i][r] = l;
                }
            }
        }
    }
    if (dp[k - 1][n - 1] == inf) return {0, {}};
    vector<array<int, 2>> borders(k);
    for (int i = k - 1, r = n - 1; i >= 0; r = opt[i][r] - 1, --i) {
        assert(r != -1);
        borders[i] = {opt[i][r], r};
    }
    return {dp[k - 1][n - 1], borders};
}

//Returns mincost of partition. If it doesn't exist, returns 0.
//<O(kn * T(cost)), O(n)>
template<typename C = int64_t>
C mincost_subsegment_partition_without_answer_restore_knuth(const size_t n, const size_t k, auto seg_cost) {
    static constexpr C inf = numeric_limits<C>::max() / 2;
    assert(0 < k && k <= n);
    vector dp(2, vector<C>(n, inf));
    vector opt(2, vector<int>(n, -1));
    for (size_t r = 0; r < n; ++r) dp[0][r] = seg_cost(0, r, 0), opt[0][r] = 0;
    for (size_t i = 1; i < k; ++i) {
        const size_t ii = i & 1;
        fill(dp[ii].begin(), dp[ii].end(), inf);
        fill(opt[ii].begin(), opt[ii].end(), -1);
        for (size_t r = n - 1; r >= i; --r) {
            const size_t opt_l = opt[ii ^ 1][r];
            const size_t opt_r = r == n - 1 ? r : opt[ii][r + 1];
            for (size_t l = opt_l; l <= opt_r; ++l) {
                C tyt = dp[ii ^ 1][l - 1] + seg_cost(l, r, i);
                if (tyt < dp[ii][r]) {
                    dp[ii][r] = tyt;
                    opt[ii][r] = l;
                }
            }
        }
    }
    if (dp[~k & 1][n - 1] == inf) return 0;
    return dp[~k & 1][n - 1];
}
