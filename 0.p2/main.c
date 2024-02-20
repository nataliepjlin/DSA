#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
bool valid(int n, long long trial[]){
    if(n <= 2) return true;
    for(int i = 0; i < n - 2; i++){
        if(((trial[i + 1] - trial[i]) > 0 && (trial[i + 2] - trial[i + 1]) < 0)
        || ((trial[i + 1] - trial[i]) < 0 && (trial[i + 2] - trial[i + 1]) > 0))
            continue;
        return false;
    }
    return true;
}
void perm(int idx, int n, bool used[], long long trial[n][n], const long long arr[], int *anscnt){
    if(!valid(idx, trial[*anscnt])) return;
    if(idx == n){
        *anscnt += 1;
        return;
    }
    for(int i = 0; i < n; i++){
        if(i > 0 && arr[i] == arr[i - 1] && !used[i - 1])
            continue;
        if(!used[i]){
            used[i] = true;
            trial[*anscnt][idx] = arr[i];
            perm(idx + 1, n, used, trial, arr, anscnt);
            trial[*anscnt][idx] = 0;
            used[i] = false;
        }
    }
}
int cmp(const void *a, const void *b){
    int *pa = (int*)a, *pb = (int*)b;
    if(*pa > *pb) return 1;
    else if(*pa < *pa) return -1;
    return 0;
}
int main() {
    int n;
    scanf("%d", &n);
    long long arr[n];
    bool used[n];
    for(int i = 0; i < n; i++){
        scanf("%lld", &arr[i]);
        used[i] = false;
    }
    qsort(arr, n, sizeof(long long), cmp);
    bool flag = false;
    long long trial[n][n];
    int anscnt = 0;
    perm(0, n, used, trial, arr, &anscnt);
    printf("%d\n", anscnt);
    for(int i = 0; i < anscnt; i++){
        for(int j = 0; j < n; j++){
            printf("%lld%c", trial[i][j], " \n"[j == n - 1]);
        }
    }
    return 0;
}