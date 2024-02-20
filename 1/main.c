#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#define N 21
bool valid(int n, long long trial[]) {
    if (n <= 2)
        return true;
    for (int i = 0; i < n - 2; i++) {
        if (((trial[i + 1] - trial[i]) > 0 && (trial[i + 2] - trial[i + 1]) < 0) ||
            ((trial[i + 1] - trial[i]) < 0 && (trial[i + 2] - trial[i + 1]) > 0))
            continue;
        return false;
    }
    return true;
}

void perm(int idx, int n, bool used[], long long trial[N], const long long arr[], int ans[N][N], int *cnt) {
    if (!valid(idx, trial))
        return;
    if (idx == n) {
        for(int i = 0; i < n; i++)
            ans[*cnt][i] = trial[i];
        *cnt += 1;
        return;
    }
    for (int i = 0; i < n; i++) {
        if (i > 0 && arr[i] == arr[i - 1] && !used[i - 1])
            continue;

        if (!used[i]) {
            used[i] = true;
            trial[idx] = arr[i];
            perm(idx + 1, n, used, trial, arr, ans, cnt);
            trial[idx] = 0;
            used[i] = false;
        }
    }
}

int cmp(const void *a, const void *b) {
    long long *pa = (long long *)a, *pb = (long long *)b;
    if (*pa > *pb)
        return 1;
    else if (*pa < *pb)
        return -1;
    return 0;
}

int main() {
    int n;
    scanf("%d", &n);
    long long arr[N];
    bool used[N];
    for (int i = 0; i < n; i++) {
        scanf("%lld", &arr[i]);
        used[i] = false;
    }
    qsort(arr, n, sizeof(long long), cmp);

    int cnt = 0;
    long long trial[N];
    int ans[N][N];
    perm(0, n, used, trial, arr, ans, &cnt);
    printf("%d\n", cnt);
    for(int i = 0; i < cnt; i++){
        for(int j = 0; j < n; j++)
            printf("%d%c", ans[i][j], " \n"[j == n - 1]);
    }
    return 0;
}
