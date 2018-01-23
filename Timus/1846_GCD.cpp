#include <bits/stdc++.h>

using namespace std;
using pii = pair <int,int>;

struct sq
{
	int n, m;
	vector<int> a, b, c;
	set<pii, greater<pii>> index;

	sq()
	{
		n = 0;
		m = 316;
		a.reserve(1e5);
		b.assign(320, 0);
		c.assign(320, 0);
	}

	void rebuild(int block)
	{
		int l = block * m;
		int r = min(l + m, n) - 1;
		b[block] = 0;
		for (int i = l; i <= r; ++i)
			b[block] = __gcd(b[block], a[i]);
	}

	int add(int x)
	{
		int i = n++;
		a.emplace_back(x);
		index.emplace(x, i);
		if (i % m == 0)
			b[i / m] = 0;
		i /= m;
		b[i] = __gcd(b[i], x);
		c[i + 1] = __gcd(b[i], c[i]);
		return c[i + 1];
	}

	int rem(int x)
	{
		--n;
		auto v1 = index.lower_bound(make_pair(x, n));
		int idx = v1->second;
		index.erase(v1);
		if (x != a[n])
		{
			auto v2 = index.lower_bound(make_pair(a[n], n));
			index.erase(v2);
			index.emplace(a[n], idx);
		}
		swap(a[idx], a[n]);
		a.pop_back();
		int bl = idx / m;
		int br = (n - 1) / m;
		// rebuild blocks
		rebuild(bl);
		if (bl != br)
			rebuild(br);
		// rebuild prefix gcd
		for (int i = bl; i <= br; ++i)
			c[i + 1] = __gcd(c[i], b[i]);
		return max(1, c[br + 1]);
	}
};

int main()
{
	cin.tie(0);
	cin.sync_with_stdio(0);

	sq s;

	int q;
	cin >> q;

	while (q--)
	{
		char c;
		int x;
		cin >> c >> x;
		if (c == '+')
			cout << s.add(x) << '\n';
		else
			cout << s.rem(x) << '\n';
	}

	cout.flush();
}
