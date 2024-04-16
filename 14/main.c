#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#define d 52
void getStr(const int k, const int n, char str[k][n + 1]){
    for(int r = 0; r < k; r++){
        scanf("%s", str[r]);
    }
}
void genHashPrint(const int k, const int n, const char str[k][n + 1], long long T[], const int q){
    for(int c = 0; c < n; c++){
        T[c] = 0;
        for(int r = 0; r < k; r++){
            T[c] = (T[c] * d) % q + ((isupper(str[r][c])) ? (str[r][c] - 'A') : (str[r][c] - 'a' + 26)) % q;
            T[c] %= q;
        }
        printf("%lld%c", T[c], " \n"[c == n - 1]);
    }
}
bool realMatch(const int k, const int m, const int s, const int n, 
const char tstr[k][n + 1], const char pstr[k][m + 1]){
    for(int c = 0; c < m; c++){
        for(int r = 0; r < k; r++){
            if(tstr[r][s + c] != pstr[r][c]) return false;
        }
    }
    return true;
}
int main(){
    int k, n, m, q;
    scanf("%d%d%d%d", &k, &n, &m, &q);
    char tstr[k][n + 1], pstr[k][m + 1];
    long long T[n], P[m];
    getStr(k, n, tstr);
    getStr(k, m, pstr);
    genHashPrint(k, n, tstr, T, q);
    genHashPrint(k, m, pstr, P, q);
    long long h = 1;
    for(int i = 0; i < m - 1; i++) h = (h * d) % q;
    bool has = false;
    int cnt = 0, hits[n - m + 1];
    long long p = 0, t = 0;
    for(int i = 0; i < m; i++){
        p = ((p * d) % q + P[i]) % q;
        t = ((t * d) % q + T[i]) % q;
        // printf("t = %lld, p = %lld\n", t, p);
    }
    for(int s = 0; s <= n - m; s++){
        if(p == t){
            printf("%d ", s);
            if(!realMatch(k, m, s, n, tstr, pstr)){
                hits[cnt] = s;
                cnt++;
            }
            has = true;
        }
        if(s < n - m)
            t = ((d * (t - (T[s] * h) % q)) % q + T[s + m]) % q;
        // printf("s = %d, t = %lld, p = %lld\n", s, t, p);
    }
    if(!has) printf("-1");
    printf("\n");
    if(!cnt) printf("-1\n");
    else{
        for(int i = 0; i < cnt; i++) printf("%d%c", hits[i], " \n"[i == cnt - 1]);
    }
    return 0;
}