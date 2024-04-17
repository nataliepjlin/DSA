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
bool realMatch(const int k, const int m, const int n, const int i_j, 
const char tstr[k][n + 1], const char pstr[k][m + 1]){
    for(int c = 0; c < m; c++){
        for(int r = 0; r < k; r++){
            if(tstr[r][c + i_j] != pstr[r][c]) return false;            
        }
    }
    return true;
}
void createLPS(const long long T[], const int m, int *lps){
    lps[0] = 0;
    int prev = 0, cur = 1;
    while(cur < m){
        if(T[prev] == T[cur]){
            lps[cur] = prev + 1;
            prev++, cur++;
        }
        else{
            if(prev != 0) prev = lps[prev - 1];
            else{
                lps[cur] = 0;
                cur++;
            }
        }
    }
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
    int lps[m], i = 0, j = 0, cnt = 0, hits[n - m + 1];
    createLPS(T, m, lps);
    bool has = false;
    while(i < n){
        if(T[i] == P[j]){
            i++, j++;
        }
        else{
            if(j != 0) j = lps[j - 1];
            else i++;
        }

        if(j == m){
            printf("%d ", i - j);
            if(!realMatch(k, m, n, i - j, tstr, pstr)){
                hits[cnt++] = i - j;
            }
            j = lps[j - 1];
            has = true;
        }
    }
    if(!has) printf("-1");
    printf("\n");
    if(!cnt) printf("-1\n");
    else{
        for(int i = 0; i < cnt; i++) printf("%d%c", hits[i], " \n"[i == cnt - 1]);
    }
    return 0;
}