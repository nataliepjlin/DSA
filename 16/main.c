#include <stdio.h>
#include <string.h>
#define LEN 5000001
char text[LEN];//s
char pattern[LEN];//c
void createLPS(const int len, long long *lps){
    lps[0] = 0;
    int prev = 0, cur = 1;
    while(cur < len){
        if(pattern[prev] == pattern[cur]){
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
    #ifdef debug
    for(int i = 0; i < len ; i++)
        printf("%lld%c", lps[i], " \n"[i == len - 1]);
    #endif
}
int main(){
    scanf("%s%s", text, pattern);
    const int n = strlen(text), m = strlen(pattern);
    long long lps[m], hits = 0;
    createLPS(m, lps);
    int idx[n], cnt = 0;
    for(int i = 0; i < n; i++){
        if(text[i] == pattern[0]){
            idx[cnt++] = i;
            hits++;
        }
    }
    printf("%lld\n", hits * (lps[0] + 1));
    for(int j = 1; j < m; j++){
        int newcnt = 0; hits = 0;
        for(int c = 0; c < cnt; c++){
            if(idx[c] < n - 1 && text[ idx[c]+1 ] == pattern[j]){
                idx[newcnt++] = idx[c] + 1;
                hits++;
            }
        }
        cnt = newcnt;
        printf("%lld\n", hits * (lps[j] + 1));
    }
    return 0;
}