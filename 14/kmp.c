#include <stdio.h>
void createLPS(const int P[], const int m, int *lps){
    lps[0] = 0;
    int prev = 0;
    for(int cur = 1; cur < m; cur++){
        while(prev > 0 && P[prev] != P[cur]){
            printf("prev = %d->", prev);
            prev = lps[prev - 1];
            printf("%d\n", prev);
        }
        if(P[prev] == P[cur]) prev++;
        lps[cur] = prev;
    }
    for(int i = 0; i < m ; i++)
        printf("%d%c", lps[i], " \n"[i == m - 1]);
}
void matcher(const int T[], const int P[], const int n, const int m){
    int lps[m];
    createLPS(P, m, lps);
    int q = 0;
    for(int i = 0; i < n; i++){
        printf("\ni = %d\n", i);
        while(q > 0 && P[q] != T[i]){
            printf("q = %d->", q);
            q = lps[q - 1];
            printf("%d, ", q);
        }
        if(P[q] == T[i]) q++;
        if(q == m) printf("Pattern occurs with shift %d\n", i - m);
    }
}
int main(){
    const int T[] = {5, 2, 5, 2, 6, 1, 5, 2, 7, 5, 2, 5, 2, 5};
    const int P[] = {5, 2, 5, 2, 5};
    matcher(T, P, 14, 5);
}