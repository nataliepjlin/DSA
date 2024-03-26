#include <stdio.h>
#include <limits.h>

int arr[1000005];

#define PARENT(i) (i / 2)

void swap(int *a, int *b){
    int t = *a;
    *a = *b, *b = t;
}

void heapIncreaseKey(int i, int key){
    arr[i] = key;
    while(i > 1 && arr[PARENT(i)] < arr[i]){
        swap(&arr[PARENT(i)], &arr[i]);
        i = PARENT(i);
    }
}

int main(){
    int n, key;
    scanf("%d", &n);
    for(int i = 1; i <= n; i++){
        scanf("%d", &key);
        heapIncreaseKey(i, key);
    }
    for(int i = 1; i <= n; i++)
        printf("%d%c", arr[i], " \n"[i == n]);
    return 0;
}
