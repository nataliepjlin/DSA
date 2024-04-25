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
    long long lps[m];
    createLPS(m, lps);
    for(int cur_p = 1; cur_p <= m; cur_p++){
        int p = 0, t = 0; long long hit = 0;
        while(t < n && p < cur_p){
            if(text[t] == pattern[p]){
                t++, p++;
                if(p == cur_p){
                    hit++;
                    p = lps[p - 1];
                }
            }
            else{
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