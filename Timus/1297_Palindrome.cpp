#include <bits/stdc++.h>

using namespace std;
using ll = long long;

int const N = 4096 + 16;
int const inf = N;

int n;
string w;
string ver;
string rev;

int sa[N];
int dp[N];
int lcp[N];
int len[N];
int L[N];
int R[N];
int t[N][54];

inline int get(char c) {
	if(not isalpha(c))
		return 52 + (c == '$');
	else if(c <= 'Z')
		return c-'A';
	else
		return c-'a'+26;
}

int id;
void dfs(int p, int x) {
	int cnt = 0;
	for(int i = 0; i < 54; ++i) {
		int k = t[p][i];
		if(k == 0)
			continue;
		cnt++;
		dfs(k, x + R[p] - L[p] + 1);
	}

	if(cnt == 0) {
		sa[id] = L[p] - x;
		len[n - sa[id]] = id;
		id++;
	}
}

void build() {
	int rem = 0;
	int x = 0;
	int edge = 0;
	int used = 1;
	R[0] = -1;
	for(int i = 0; i < n; ++i) {
		rem++;
		while(rem) {
			int p = 0;
			int e = edge;
			while((e - edge) + R[p] - L[p] + 1 < x) {
				e += R[p] - L[p] + 1;
				p = t[p][get(w[e])];
			}

			int idx = x - (e - edge);

			if(L[p] + idx > R[p]) {
				if(t[p][get(w[i])]) {
					edge = L[t[p][get(w[i])]] - x++;
					break;
				}
				t[p][get(w[i])] = used;;
				L[used] = i;
				R[used++] = inf;
			} else {
				int idx2 = L[p] + idx;
				if(w[idx2] == w[i]) {
					x++;
					break;
				} else {
					copy_n(t[p], 54, t[used]);
					fill_n(t[p], 54, 0);

					L[used] = idx2;
					R[used] = R[p];
					t[p][get(w[idx2])] = used++;

					L[used] = i;
					R[used] = inf;
					t[p][get(w[i])] = used++;

					R[p] = idx2-1;
				}
			}
			if(--rem == 0)
				break;
			x--;
			edge++;
		}
	}

	dfs(0, 0);

	for(int i = n, j = 0; i > 0; --i, j -= j > 0) {
		int idx = len[i];
		if(idx == n - 1)
			continue;
		while(w[sa[idx] + j] == w[sa[idx+1] + j])
			j++;
		lcp[idx] = j;
	}
}

int st[13][2048];

int main() {
	cin.tie(0);
	cin.sync_with_stdio(0);

	cin >> ver;

	rev.resize(ver.length());
	copy(ver.begin(), ver.end(), rev.begin());
	reverse(rev.begin(), rev.end());

	w = ver + "$" + rev + "#";
	n = w.length();
	build();

	for(int i = 0; i < n; ++i)
		st[0][i] = lcp[i];

	for(int j = 1; (1 << j) <= n; ++j) {
		int b = 1 << (j - 1);
		int lim = b + b - 1;
		for(int i = 0; i + lim < n; ++i)
			st[j][i] = min(st[j-1][i], st[j-1][i+b]);
	}

	auto query = [] (int l, int r) {
		int b = 31 - __builtin_clz(r - l + 1);
		return min(st[b][l], st[b][r-(1<<b)+1]);
	};

	// odd
	for(int i = 0; i+i < n; ++i) {
		int l = len[i+2];
		int r = len[n-i];
		if(l > r)
			swap(l, r);
		int x = query(l, r-1);
		dp[i-x+1] = max(dp[i-x+1], (x << 1) - 1);
	}

	// even
	for(int i = 1; i+i < n; ++i) {
		if(w[i-1] != w[i])
			continue;
		int l = len[i+1];
		int r = len[n-i];
		if(l > r)
			swap(l, r);
		int x = query(l, r-1);
		dp[i-x] = max(dp[i-x], x << 1);
	}

	int best = max_element(dp, dp+n) - dp;
	w.resize(best + dp[best]);
	cout << w.c_str()+best << "\n";
}
