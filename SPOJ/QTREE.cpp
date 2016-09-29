#include <bits/stdc++.h>

using namespace std;

int const MAX = 10000 + 16;

struct segment_tree {
  int n;
  int t[2*MAX];

  void build() {
    for(int i = n-1; i > 0; --i)
      t[i] = max(t[i<<1], t[i<<1|1]);
  }

  void update(int i, int v) {
    for(t[i += n] = v; i; i >>= 1)
      t[i>>1] = max(t[i], t[i^1]);
  }

  int query(int l, int r) {
    int ans = 0;
    for(l += n, r += n; l <= r; l >>= 1, r >>= 1) {
      if(l&1)    ans = max(ans, t[l++]);
      if(!(r&1)) ans = max(ans, t[r--]);
    }
    return ans;
  }
};

int n;
int fa[MAX];
int idx[MAX];
int dist[MAX];
int pref[MAX];
int depth[MAX];
int chain[MAX];
int chain_sz[MAX];
segment_tree tree[MAX];
pair<int, int> edges[MAX];
vector<pair<int,int> > g[MAX];

void dfs(int u = 1, int p = 0) {
  pref[u] = 0;
  chain_sz[u] = 0;
  depth[u] = dist[u];

  for(int i = 0; i < g[u].size(); ++i) {
    int v = g[u][i].first;
    if(v == p) continue;

    dist[v] = dist[u] + 1;
    fa[v] = u;
    dfs(v, u);

    if(depth[v] > depth[ pref[u] ])
      pref[u] = v;

    depth[u] = max(depth[u], depth[ pref[u] ]);
  }
}

void hld1(int u = 1, int p = 0, int ch = 1) {
  chain[u] = ch;
  idx[u] = chain_sz[ch]++;
  for(int i = 0; i < g[u].size(); ++i) {
    int v = g[u][i].first;
    if(v == p) continue;
    hld1(v, u, (v == pref[u]) ? ch : v);
  }
}

void hld2(int u = 1, int p = 0, int cost = 0) {
  int ch = chain[u];
  int n = chain_sz[ch];

  if(idx[u] == 0)
    tree[ch].n = n;

  tree[ch].t[n + idx[u]] = cost;

  if(chain_sz[ch] == idx[u]+1)
    tree[ch].build();

  for(int i = 0; i < g[u].size(); ++i) {
    int v = g[u][i].first;
    int c = g[u][i].second;
    if(v == p) continue;
    hld2(v, u, c);
  }
}

int query(int x, int y) {
  int ans = 0;

  while(chain[x] != chain[y]) {
    if(dist[chain[x]] < dist[chain[y]])
      swap(x, y);
    ans = max(ans, tree[ chain[x] ].query(0, idx[x]));
    x = fa[ chain[x] ];
  }

  if(x == y)
    return ans;

  if(dist[x] > dist[y])
    swap(x, y);

  return max(ans, tree[ chain[x] ].query(idx[x]+1, idx[y]));
}

void clean(int n) {
  for(int i = 1; i <= n; ++i)
    g[i].clear();
}

int main() {
  int TC;
  scanf("%d", &TC);
  while(TC--) {
    scanf("%d", &n);

    for(int i = 1; i < n; ++i) {
      int u, v, c;
      scanf("%d%d%d", &u, &v, &c);
      g[u].push_back(make_pair(v, c));
      g[v].push_back(make_pair(u, c));
      edges[i] = make_pair(u, v);
    }

    dist[0] = -1;
    depth[0] = -1;
    dfs();
    hld1();
    hld2();

    char quest[8];

    while(1) {
      memset(quest, 0, sizeof quest);
      scanf(" %s", quest);

      if('Q' == quest[0]) {
        int u, v;
        scanf("%d%d", &u, &v);
        printf("%d\n", query(u, v));
      } else if('C' == quest[0]) {
        int i, ti;
        scanf("%d%d", &i, &ti);

        int u = edges[i].first, v = edges[i].second;

        // make it (u --> v);
        if(dist[u] > dist[v])
          v = u;

        int ch = chain[v];
        int index = idx[v];
        tree[ch].update(index, ti);
      } else {
        break;
      }
    }

    if(TC) clean(n);
  }
}
