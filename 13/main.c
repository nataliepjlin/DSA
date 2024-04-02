#include <stdio.h>
void swapf(int *a, int *b){
    int t = *a;
    *a = *b, *b = t;
}
int partition(int a[], int l, int r, int *swap, const int b){
    int pivot = a[l];
    int i = l + 1, j = r;
    while(i < j && *swap < b){
        while(i <= r && a[i] <= pivot) i++;
        while(j >= l && a[j] > pivot) j--;
        if(i >= j) break;
        swapf(&a[i], &a[j]);
        *swap += 1;
    }
    if(l < j && a[l] > a[j] && *swap < b){
        swapf(&a[l], &a[j]);
        *swap += 1;
    }
    return j;
}
void qs(int a[], int l, int r, int *swap, const int b){
    if(l < r && *swap < b){
        int m = partition(a, l, r, swap, b);
        qs(a, l, m - 1, swap, b);
        qs(a, m + 1, r, swap, b);
    }
}
int main(){
    int n, b, swap = 0;
    scanf("%d%d", &n, &b);
    int a[n];
    for(int i = 0; i < n; i++) scanf("%d", &a[i]);
    qs(a, 0, n - 1, &swap, b);
    printf("The array is");
    for(int i = 0; i < n; i++) printf(" %d", a[i]);
    printf(" after %d swaps.\n", swap);
}