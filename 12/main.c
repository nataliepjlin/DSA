#include<stdio.h>
int PARENT(int i){
    return i / 2;
}
int LEFT(int i){
    return 2 * i;
}
int RIGHT(int i){
    return 2 * i + 1;
}
int getLargest(int A[], int i, int heapSize){
    int l = LEFT(i), r = RIGHT(i);
    int largest;

    if(l <= heapSize && A[l] > A[i])
        largest = l;
    else largest = i;

    if(r <= heapSize && A[r] > A[largest])
        largest = r;

    return largest;
}

void maxHeapify(int A[], int i, int heapSize){
    int largest = getLargest(A, i, heapSize);
    while(largest != i){
        int tmp = A[i];
        A[i] = A[largest];
        A[largest] = tmp;

        i = largest;
        largest = getLargest(A, i, heapSize);
    }
}

void buildMaxHeap(int A[], int heapSize){
    for(int i = heapSize / 2; i >= 1; i--){
        maxHeapify(A, i, heapSize);
    }
}

int main(){
    int n;
    int arr[1000001];
    scanf("%d", &n);

    for(int i = 1; i <= n; i++){
        scanf("%d", &arr[i]);
    }

    buildMaxHeap(arr, n);

    for(int i = 1; i <= n; i++){
        printf("%d%c", arr[i], " \n"[i == n]);
    }
    return 0;
}