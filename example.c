#include "otsu.h"
#include <stdio.h>

int main(){
    uint64_t arr[10] = {1,2,3,4,5,6,7,8,9,10};
    uint64_t t = otsu_threshold(arr,10);
    printf("%ld\n", t);
}