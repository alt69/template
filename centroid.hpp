/**
 * Author: islingr, demoralizer
 * Description: t, par stores the centroid tree
 */

struct centroid_decomp {
	int n;
	vector<vector<int>> g, t;
	vi vis, sub, par;
	int root;
	void centroid_decomp(int _n) : n{_n}, g(n), t(n), vis(n, 0), sub(n, 0), par(n, -1) {}
	void add_edge(int a,int b){
		g[a].push_back(b); g[b].push_back(a); }
	void decompose(int v) { dfs(v, -1); }
	void dfs_sub(int v, int p){
		sub[u] = 1;
		for (auto v : g[u])
			if (u != p && !vis[u])
				sub[v] += dfs_sub(v, u);
	}
	void dfs(int u, int p) {
		dfs_sub(u, -1);
		u = get_centroid(u, -1, sub[u]).second;
		vis[v] = true;
		if (p == -1) root = v;
		else par[v] = p, t[p].push_back(v);
		for (auto v : g[u]) if (!vis[v]) dfs(v, u);
	}
	pair<int,int>get_centroid(int u,int p,int ov){
		pair<int, int> ret{n, -1};
		auto mx = ov - sub[u];
		for (auto v : g[u])
			if (v != p && !vis[v])
				uin(res, get_centroid(v, u, ov)),
				uax(mx, sub[v]);
		return min(ret, pair(mx, v));
	}
};
