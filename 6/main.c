#include <stdio.h>
#include <stdlib.h>
void insert(int *arr, int m, int *cnt, const int b){
    int data = arr[m];
    int i = m - 1;
    while(i >= 0 && arr[i] > data && *cnt < b){
        arr[i + 1] = arr[i];
        i--;
        *cnt += 1;
    }
    arr[i + 1] = data;
}
void insertionSort(int *arr, int n, int *cnt, const int b){
    for(int i = 1; i < n; i++){
        insert(arr, i, cnt, b);
    }
}
void printArr(int *arr, int n){
    for(int i = 0; i < n; i++)
        printf(" %d", arr[i]);
}
int main(){
    int n, b;
    scanf("%d%d", &n, &b);
    int *arr = (int*)malloc(sizeof(int) * n);
    for(int i = 0; i < n; i++)
        scanf("%d", &arr[i]);
    int cnt = 0;
    insertionSort(arr, n, &cnt, b);
    printf("The array is");
    printArr(arr, n);
    printf(" after %d back-moves.\n", cnt);
    free(arr);
    return 0;
}