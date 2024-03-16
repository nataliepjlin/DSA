#include <stdio.h>
#include <stdlib.h>
#define N 100001
int main(){
    int n;
    scanf("%d", &n);
    int next[N];
    int data[N];
    for(int i = 0; i < n; i++){
        scanf("%d%d", &data[i], &next[i]);
        next[i]--;
    }
    int fastIdx = 0, slowIdx = 0;
    while(fastIdx != -1 && next[fastIdx] != -1){
        printf("%d ", data[fastIdx]);
        fastIdx = next[next[fastIdx]]; 
        slowIdx = next[slowIdx];
        if(fastIdx == slowIdx) break;
    }
    if(fastIdx != -1) printf("%d", data[fastIdx]);
    printf("\n");
}