#include <stdio.h>
int arr[100001];
int getMinIdx(int l, int r){
    int m = l;
    for(int i = l + 1; i <= r; i++){
        if(arr[m] > arr[i])
            m = i;
    }
    return m;
}
void swap(int *a, int *b){
    int t = *a;
    *a = *b;
    *b = t;
}
int sort(const int n, const int b){
    int used = 0;
    for(int i = 0; i < n; i++){
        int m = getMinIdx(i, n - 1);
        if(i != m){
            swap(&arr[i], &arr[m]);
            used++;
            if(used == b) break;
        }
    }
    return used;
}
void print(int n){
    for(int i = 0; i < n; i++){
        printf("%d ", arr[i]);
    }
}
int main(){
    int n, b;
    scanf("%d%d", &n, &b);
    for(int i = 0; i < n; i++)
        scanf("%d", &arr[i]);
    int used = sort(n, b);
    printf("The array is ");
    print(n);
    printf("after %d swaps.\n", used);
    return 0;
}