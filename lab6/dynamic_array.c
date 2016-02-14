
#include <stdlib.h>
#include <string.h>
#include "dynamic_array.h"

int CAPACITY_INIT = 1;
int GROWTH_FACTOR = 2;

int DArray_init(DArray* arr){
    arr->array = (int *) malloc(CAPACITY_INIT*sizeof(int));
    if (arr->array == NULL) {
        return -1;
    }
    arr->upto = 0;
    arr->size = CAPACITY_INIT;
    return 1;
}

int append(DArray* arr, int i){
    int *iptr;
    if (arr->upto >= arr->size) {
        /* tmp pointer needed below as if allocation failed, original array would be lost */
        iptr = (int *) realloc(arr->array, arr->size*GROWTH_FACTOR*sizeof(int));
        if (iptr == NULL) {
            return -1;
        }
        arr->array = iptr;
        arr->size *= GROWTH_FACTOR;
    }
    arr->array[arr->upto] = i;
    return arr->upto++;
    
        
}

int get(DArray* arr, int index) {
    if (index >= arr->upto || index < 0) {
        return -1;
    }
    return arr->array[index];
}

int get_index(DArray* arr, int value) {
    int i;
    for (i=0; i< arr->upto; i++){
        if (arr->array[i]==value) {
            return i;
        }
    }
    return -1;
}

void DArray_free(DArray *arr) {
  free(arr->array);
}

int set(DArray* arr, int index, int value)
{
    if (index < 0 || index > arr->upto)
        return -1;
    arr->array[index] = value;
    return 0;
}

int delete(DArray* arr, int value)
{
    int index = get_index(arr,value);      //get the index of the value
    int* iptr;
    if (index > arr->upto || index<0)      //sanity check
        return -1;
    for (int i = index; i < arr->upto; i++)
    {
        arr->array[i] = arr->array[i+1];   //moving elements forward
    }
    arr->array[arr->upto] = 0;

    if ((arr->upto-1) <= (arr->size)/GROWTH_FACTOR) //reallocate to save space
    {
        iptr = (int*)realloc(arr->array,arr->size/(GROWTH_FACTOR)*sizeof(int));
        if (iptr == NULL)
            return -1;
        arr->array = iptr;
        arr->size /= GROWTH_FACTOR;
    }
    arr->upto--;

    return arr->upto;
    
}