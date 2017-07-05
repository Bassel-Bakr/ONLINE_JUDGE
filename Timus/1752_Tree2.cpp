#include <bits/stdc++.h>

using namespace std;

int const N = 100 * 1000 + 16;

int n, q;
int fa[N];
int sz[N];
bool done[N];
vector<int> g[N];
map<int, vector<int>> dist[N];
int e, tour[N+N], tin[N], depth[N], st[18][N+N];

void euler_tour(int u, int p) {
  tin[u] = e;
  tour[e++] = u;
  depth[u] = depth[p] + 1;

  for(int v : g[u]) {
    if(v == p)
      continue;

    euler_tour(v, u);
    tour[e++] = u;
  }
}

void build_lca_table() {
  for(int i = 0; i < e; ++i)
    st[0][i] = tour[i];

  for(int j = 1; (1 << j) <= e; ++j) {
    int b = 1 << j-1;
    int lim = b+b-1;
    for(int i = 0; i + lim < e; ++i) {
      int u = st[j-1][i];
      int v = st[j-1][i+b];
      st[j][i] = (depth[u] < depth[v] ? u : v);
    }
  }
}

int find_lca(int u, int v) {
  int l = tin[u];
  int r = tin[v];

  if(l > r)
    swap(l, r);

  int b = ((sizeof(int)) << 3) - __builtin_clz(r - l + 1) - 1;
  int x = st[b][l];
  int y = st[b][r-(1<<b)+1];
  return (depth[x] < depth[y] ? x : y);
}

void dfs(int u, int p) {
  sz[u] = 1;
  for(int v : g[u]) {
    if(v == p || done[v])
      continue;

    dfs(v, u);
    sz[u] += sz[v];
  }
}

void build_dist(int u, int p, int node, int d) {
  dist[node][d].emplace_back(u);
  for(int v : g[u])
    if(not done[v] && v != p)
      build_dist(v, u, node, d+1);
}

int centroid(int u) {
  dfs(u, 0);
  int prev = u;
  int size = sz[u];
  do {
    prev = u;
    for(int v : g[u]) {
      if(done[v] || sz[v] >= sz[u])
        continue;

      if(2 * sz[v] > size) {
        u = v;
        break;
      }
    }
  } while(u != prev);
  done[u] = true;
  build_dist(u, 0, u, 0);
  for(auto& p : dist[u])
    sort(p.second.begin(), p.second.end());
  for(int v : g[u])
    if(not done[v])
      fa[centroid(v)] = u;
  return u;
}

int query(int v, int d) {
  if(d == 0)
    return v;

  int p = v;
  int node = n+n;
  while(p) {
    int distance = depth[p] + depth[v] - 2 * depth[find_lca(p, v)];
    
    if(distance == d) {
      node = min(node, p);
    } else if(distance < d) {
      auto level = dist[p].find(d - distance);
      if(level != dist[p].end())
        for(int u : level->second) {
          if(d != depth[u] + depth[v] - 2 * depth[find_lca(u, v)])
            continue;
          node = min(node, u);
          break;
        }
    }

    p = fa[p];
  }
  return (node <= n ? node : 0);
}

int main() {
  cin.tie(0);
  cin.sync_with_stdio(0);

  cin >> n >> q;

  for(int i = 1; i < n; ++i) {
    int u, v;
    cin >> u >> v;
    g[u].emplace_back(v);
    g[v].emplace_back(u);
  }

  euler_tour(1, 0);
  build_lca_table();
  centroid(1);

  while(q--) {
    int v, d;
    cin >> v >> d;
    cout << query(v, d);
    if(q)
      cout << '\n';
  }
}
