#include <stdio.h>
#include <stdlib.h>
typedef struct vector_t{
    long long *presums;
    int cap;
    int size;
}vector_t;
vector_t *vector_new(){
    return (vector_t*)malloc(sizeof(vector_t));
}
void vector_init(vector_t *obj){
    obj->presums = calloc(2, sizeof(long long));
    obj->cap = 1;
    obj->size = 0;
}
void vector_destroy(vector_t *obj){
    free(obj->presums);
    free(obj);
}
void push_back(vector_t *obj, const long long sum){
    #ifdef debug
    printf("sum = %lld\n", sum);
    #endif
    if(obj->size != 0){
        obj->cap += 1;//cap = 1
        obj->presums = realloc(obj->presums, (obj->cap + 1) * sizeof(long long));//real cap = 2(cap + 1)
        #ifdef debug
        printf("obj->size = %d, realloc %d\n", obj->size, obj->cap);
        #endif
    }
    obj->presums[obj->size + 1] = sum + obj->presums[obj->size];
    obj->size += 1;
}

int main(){
    vector_t *myVec = vector_new();
    vector_init(myVec);
    printf("init vec size = %d, cap = %d\n", myVec->size, myVec->cap);
    push_back(myVec, 1);
    push_back(myVec, 2);
    push_back(myVec, 3);
    push_back(myVec, 4);
    push_back(myVec, 5);
    push_back(myVec, 6);
    printf("now vec size = %d, cap = %d\n", myVec->size, myVec->cap);
    for(int i = 0; i <= myVec->size; i++)
        printf("%lld%c", myVec->presums[i], " \n"[i == myVec->size]);
    vector_destroy(myVec);
    return 0;
}