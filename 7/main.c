#include <stdio.h>
int bs(int a[], int l, int r, const int key){
    printf("Searching %d in range [%d, %d].\n", key, l + 1, r + 1);
    if(l <= r){
        int m = (l + r) >> 1;
        if(a[m] == key){
            printf("Found at index %d.\n", m + 1);
            return 1;
        }
        else if(a[m] < key) return bs(a, m + 1, r, key);
        else return bs(a, l, m - 1, key);
    }
    return 0;
}
int main(){
    int n, key;
    scanf("%d%d", &n, &key);
    int a[n];
    for(int i = 0; i < n; i++){
        scanf("%d", &a[i]);
    }

    if(!bs(a, 0, n - 1, key))
        printf("Not found.\n");
}