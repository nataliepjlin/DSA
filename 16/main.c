#include <stdio.h>
#include <string.h>
#define LEN 5000001
char text[LEN];//s
char pattern[LEN];//c
int createLPS(const int len, long long *lps, long long *plag){
    lps[0] = 0, plag[0] = 1;
    int prev = 0, cur = 1, cnt = 0;
    while(cur < len){
        if(pattern[prev] == pattern[cur]){
            lps[cur] = prev + 1;
            plag[cur] = plag[ lps[cur] - 1 ] + 1;
            prev++, cur++;
        }
        else{
            if(prev != 0) prev = lps[prev - 1];
            else{
                lps[cur] = 0;
                plag[cur] = 1;
                cur++;
            }
        }
    }
}
int main(){
    scanf("%s%s", text, pattern);
    const int n = strlen(text), m = strlen(pattern);
    long long lps[m], plag[m], hits = 0;
    createLPS(m, lps, plag);
    int idx[n], cnt = 0;
    for(int i = 0; i < n; i++){
        if(text[i] == pattern[0]){
            idx[cnt++] = i;
            hits++;
        }
    }
    #ifdef debug
    printf("hits = %lld, plag = %lld, hits * plag = ", hits, plag[0]);
    #endif
    printf("%lld\n", hits * plag[0]);
    for(int j = 1; j < m; j++){
        int newcnt = 0; hits = 0;
        for(int c = 0; c < cnt; c++){
            if(idx[c] < n - 1 && text[ idx[c]+1 ] == pattern[j]){
                idx[newcnt++] = idx[c] + 1;
                hits++;
            }
        }
        cnt = newcnt;
        #ifdef debug
        printf("hits = %lld, plag = %lld, hits * plag = ", hits, plag[j]);
        #endif
        printf("%lld\n", hits * plag[j]);
    }
    return 0;
}