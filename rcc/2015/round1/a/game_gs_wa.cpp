#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

int main()
{
    freopen("game.in", "r", stdin);
    freopen("game.out", "w", stdout);
    int t;
    scanf("%d", &t);
    while (t--)
    {
        int n, l;
        scanf("%d%d", &n, &l);
        vector<int> a(n), b(n);
        for (int i = 0; i < n; i++)
            scanf("%d", &a[i]);
        for (int i = 0; i < n; i++)
            scanf("%d", &b[i]);
        sort(a.begin(), a.end());
        sort(b.begin(), b.end());
        reverse(b.begin(), b.end());
        if (a[0] > b[0])
            printf("YES\n");
        else 
            printf("NO\n");
    }
    return 0;    
}