#include <bits/stdc++.h>
using namespace std;
int main(){ios::sync_with_stdio(false);cin.tie(nullptr);int T;cin>>T;while(T--){int n;cin>>n;vector<long long>a(n);long long p=0;for(int i=0;i<n;++i){long long w,x;cin>>w>>x;a[i]=w+x;p+=x;}ranges::sort(a);int ans=0;for(auto x:a){if(p<x)break;p-=x;++ans;}cout<<ans<<'\n';}}
