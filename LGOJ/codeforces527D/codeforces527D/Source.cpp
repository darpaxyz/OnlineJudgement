#include<iostream>
#include<algorithm>
//һ�ε�������ͷ�ļ� 
using namespace std;
struct P
{
	int l, r;
}p[200005];
bool cmp(P a, P b)
{
	return a.r<b.r;
}
int main()
{
	int i, n, x, y;
	cin >> n;
	for (i = 0; i<n; ++i)
	{
		cin >> x >> y;
		p[i].l = x - y, p[i].r = x + y;
	}
	sort(p, p + n, cmp);
	int t = -0x3f3f3f3f, ans = 0;//0x3f3f3f3f�����ڴ��������ĺܺõı��� 
	for (i = 0; i<n; ++i)
	{
		if (p[i].l >= t)
		{
			t = p[i].r; ++ans;
		}
	}
	cout << ans << endl;
	system("pause");
	return 0;
}