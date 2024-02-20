#include <stdio.h>
int getMinIdx(int l, int r, int arr[]){
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
int sort(int arr[], const int n, const int b){
    int used = 0;
    for(int i = 0; i < n && used < b; i++){
        int m = getMinIdx(i, n - 1, arr);
        if(i != m){
            swap(&arr[i], &arr[m]);
            used++;
        }
    }
    return used;
}
void print(int arr[], int n){
    for(int i = 0; i < n; i++){
        printf("%d ", arr[i]);
    }
}
int main(){
    int n, b;
    scanf("%d%d", &n, &b);
    int arr[n];
    for(int i = 0; i < n; i++)
        scanf("%d", &arr[i]);
    int used = sort(arr, n, b);
    printf("The array is ");
    print(arr, n);
    printf("after %d swaps.\n", used);
    return 0;
}