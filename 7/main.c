#include <stdio.h>
void bs(int a[], int l, int r, const int key){
    printf("Searching %d in range [%d, %d].\n", key, l + 1, r + 1);
    if(l > r){
        printf("Not found.\n");
        return;
    } 
    int m = (l + r) >> 1;
    if(a[m] == key){
        printf("Found at index %d.\n", m + 1);
        return;
    }
    else if(a[m] < key) bs(a, m + 1, r, key);
    else bs(a, l, m - 1, key);
}
int main(){
    int n, key;
    scanf("%d%d", &n, &key);
    int a[n];
    for(int i = 0; i < n; i++){
        scanf("%d", &a[i]);
    }

    bs(a, 0, n - 1, key);
    return 0;
}