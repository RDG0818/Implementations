#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using str = string;
using vll = vector<ll>;
using vvll = vector<vector<ll>>;
#define read(arr); for(ll i=0;i<arr.size();i++) cin>>arr[i];
#define p(num); cout << num << endl;
#define pv(arr); for (ll i : arr) cout << i << endl;
    
ll mod = pow(10, 9) + 7;
vector<bool>sieve(ll n){vector<bool>v(n+1,true);v[0]=false;v[1]=false;for(ll i=2;i<n;i++)if(v[i])for(ll j=pow(i,2);j<n+1;j+=i)v[j]=false;return v;} // Use to check if num is prime
vector<ll>primes(vector<bool>s){vector<ll>v;for(ll i=0;i<s.size();i++)if(s[i])v.push_back(i);return v;} // Use to get array of primes up to num
vector<ll>fib(ll n){vector<ll>v={0,1};for(ll i=0;i<n-2;i++){v.push_back(v[i]+v[i+1]);}return v;} // Use to get n fibonacci num
ll maxSub(vector<ll>v){ll a=0;ll b=0; for(ll i:v){b+=i;if(b>0)a=max(a,b);else b=0;}return a;} // Finds maximum subsequence of an array
ll binexp(ll a,ll b){if(b==0)return 1;if(b%2)return (a*binexp(a,b-1))%mod;return binexp((a*a)%mod,b/2);} // Quickly does a^b % mod.
ll big = LLONG_MAX;
ll small = LLONG_MIN;
str s, s1, s2, s3, s4, s5;
ll a, b, c, n, m, k, q, t, x, y, z;

int main() {
    cin >> n >> k;
    vll v(n);
    read(v);

    sort(v.begin(), v.end());
    ll upper = v[v.size() - 1] + 1;
    ll lower = 1;
    ll midpoint;
    ll iteration = 0;

    while (true) {
        //if (iteration++ > 10) break;
        midpoint = (upper + lower)/2;
        //cout << midpoint << endl;
        ll sum = 0;
        for (ll num : v) {
            sum += ((num-1) / midpoint)+1;
        }
        if (sum <= k) {
            ll midpoint1 = midpoint - 1;
            if (midpoint1 == 0) break;
            ll sum1 = 0;
            for (ll num : v) {
                sum1 += ((num-1) / midpoint1)+1;
            }
            if (sum1 <= k) {
                upper = midpoint;
            }
            else {
                break;
            }
        }
        else {
            lower = midpoint;
        }
        
    }

    cout << midpoint << endl;

    return 0;
}