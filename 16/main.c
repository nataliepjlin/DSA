#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#define LEN 5000001
char text[LEN];//s
char pattern[LEN];//c
void prefix_ftn(const int len, long long *lps, long long *plag, long long *hits){
    lps[0] = -1, plag[0] = 1, hits[0] = 0;
    int prev = -1;
    for(int cur = 1; cur < len; cur++){
        hits[cur] = 0;
        while(prev >= 0 && pattern[cur] != pattern[prev + 1]) prev = lps[prev];
        if(pattern[cur] == pattern[prev + 1]){
            prev++;
            lps[cur] = prev;
            plag[cur] = plag[ lps[cur] ] + 1;
        }
        else{
            lps[cur] = -1, plag[cur] = 1;
        }
    }
    #ifdef debug
    for(int i = 0; i < len; i++){
        printf("%lld%c", lps[i], " \n"[i == len - 1]);
    }
    #endif
}
int min(int a, int b){
    return a < b ? a : b;
}
int main(){
    scanf("%s%s", text, pattern);
    const int n = strlen(text), m = strlen(pattern);
    long long lps[m], plag[m], hits[m + 1];
    hits[m] = 0;
    prefix_ftn(m, lps, plag, hits);
    int t = 0, p = -1;
    while(t < n){
        if(text[t] == pattern[p + 1]){
            t++, p++;
            hits[p + 1]++;
            #ifdef debug
            printf("hits[%d]++\n", p+1);
            #endif
            if(p == m - 1) p = lps[p];
        }
        else{
            if(p >= 0) p = lps[p];
            else t++;
        }
    }
    #ifdef debug
    printf("raw hits: ");
    for(int i = 1; i <= m; i++){
        printf("%lld%c", hits[i], " \n"[i == m]);
    }
    #endif
    for(int i = min(n, m) - 1; i >= 0; i--){
        hits[ lps[i] + 1 ] += hits[i + 1];
    }
    for(int i = 0; i < m; i++){
        #ifdef debug
        printf("i = %d, plag = %lld, hit = %lld, plag * hit = ", i, plag[i], hits[i + 1]);
        #endif
        printf("%lld\n", plag[i] * hits[i + 1]);
    }
    return 0;
}