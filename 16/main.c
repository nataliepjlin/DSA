#include <stdio.h>
#include <string.h>
#define LEN 5000001
char text[LEN];//s
char pattern[LEN];//c
void createLPS(const int len, long long *lps){
    lps[0] = 0;
    int prev = 0;
    for(int cur = 1; cur < len; cur++){
        while(prev > 0 && pattern[prev] != pattern[cur]) prev = lps[prev - 1];
        if(pattern[prev] == pattern[cur]) prev++;
        lps[cur] = prev;
    }
    #ifdef debug
    for(int i = 0; i < len ; i++)
        printf("%lld%c", lps[i], " \n"[i == len - 1]);
    #endif
}
int main(){
    scanf("%s%s", text, pattern);
    const int tlen = strlen(text), plen = strlen(pattern);
    long long lps[plen];
    createLPS(plen, lps);
    for(int cur_p = 1; cur_p <= plen; cur_p++){
        int p = 0, t = 0; long long hit = 0;
        while(t < tlen){
            if(text[t] == pattern[p]){
                t++, p++;
            }
            if(p == cur_p){
                hit++;
                p = lps[p - 1];
            }
            else if(text[t] != pattern[p]){
                if(p != 0) p = lps[p - 1];
                else t++;
            }
        }
        #ifdef debug
        printf("hit = %lld, plag = %lld\n", hit, lps[cur_p - 1] + 1);
        #endif
        printf("%lld\n", hit * (lps[cur_p - 1] + 1));
    }
    return 0;
}