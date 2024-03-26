#include <stdio.h>
#include <limits.h>
#define PARENT(i) i / 2
void swap(int *a, int *b){
    int t = *a;
    *a = *b, *b = t;
}
void heapIncreaseKey(int arr[], int i, int key){
    arr[i] = key;
    while(i > 1 && arr[PARENT(i)] < arr[i]){
        swap(&arr[PARENT(i)], &arr[i]);
        i = PARENT(i);
    }
}
void maxHeapInsert(int arr[], int *size, int key){
    *size += 1;
    arr[*size] = INT_MIN;
    heapIncreaseKey(arr, *size, key);
}
int main(){
    int n, size = 0, key;
    scanf("%d", &n);
    int arr[100005];
    for(int i = 0; i < n; i++){
        scanf("%d", &key);
        maxHeapInsert(arr, &size, key);
    }
    for(int i = 1; i <= n; i++)
        printf("%d%c", arr[i], " \n"[i == n]);
    return 0;
}