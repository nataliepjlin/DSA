#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>
#define d 52
void getStr(const int k, const int n, char str[k][n + 1]){
    for(int r = 0; r < k; r++){
        scanf("%s", str[r]);
        #ifdef debug
        printf("str[%d] = %s\n", r, str[r]);
        #endif
    }
}
void genHashPrint(const int k, const int n, const char str[k][n + 1], int t[], const int q){
    for(int c = 0; c < n; c++){
        t[c] = 0;
        for(int r = 0; r < k; r++){
            t[c] = t[c] * d + ((isupper(str[r][c])) ? (str[r][c] - 'A') : (str[r][c] - 'a' + 26));
            t[c] %= q;
        }
        printf("%d%c", t[c], " \n"[c == n - 1]);
    }
}
bool realMatch(const int k, const int m, const int s, const int n, 
const char tstr[k][n + 1], const char pstr[k][m + 1]){
    for(int r = 0; r < k; r++){
        for(int c = 0; c < m; c++){
            if(tstr[r][s + c] != pstr[r][c]) return false;
        }
    }
    return true;
}
int main(){
    int k, n, m, q;
    scanf("%d%d%d%d", &k, &n, &m, &q);
    char tstr[k][n + 1], pstr[k][m + 1];
    int t[n], p[m];
    const int pre = (int)pow(d, k - 1);
    getStr(k, n, tstr);
    getStr(k, m, pstr);
    genHashPrint(k, n, tstr, t, q);
    genHashPrint(k, m, pstr, p, q);
    int spuCnt = 0, spu[n - m], has = 0;
    for(int s = 0; s <= n - m; s++){
        if(t[s] == p[m]){
            printf("%d ", s);
            if(!realMatch(k, m, s, n, tstr, pstr)){
                spu[spuCnt] = s;
                spuCnt++;
            }
            has = 1;
        }
    }
    if(!has) printf("-1");
    printf("\n");
    if(!spuCnt) printf("-1\n");
    else{
        for(int i = 0; i < spuCnt; i++) printf("%d%c", spu[i], " \n"[i == spuCnt - 1]);
    }
    return 0;
}