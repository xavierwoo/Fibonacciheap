//
//  FibonacciHeap.h
//  FibonacciHeap
//
//  Created by Xinyun on 2016/12/6.
//  Copyright © 2016年 Xavier Woo. All rights reserved.
//

#ifndef FibonacciHeap_h
#define FibonacciHeap_h

#define FibonacciHeap_debug

typedef int(*FibonacciHeapNode_compare)(void*, void*);
typedef void(*FibonacciHeap_destructor)(void*);


typedef struct FibonacciHeapNode{
    void* key;
    void* value;
    
    int isMarked;
    int degree;
    struct FibonacciHeapNode* parent;
    struct FibonacciHeapNode* left;
    struct FibonacciHeapNode* right;
    struct FibonacciHeapNode* child;
}FibonacciHeapNode;

typedef struct DegreePointers{
    FibonacciHeapNode** pointers;
    int len;
}DegreePointers;

typedef struct FibonacciHeap{
    FibonacciHeapNode* min;
    int num;
    FibonacciHeapNode_compare compare;
    DegreePointers* dps;
}FibonacciHeap;



FibonacciHeap* FibonacciHeap_new(FibonacciHeapNode_compare cmp);

FibonacciHeapNode* FibonacciHeap_insert(FibonacciHeap* heap, void* key, void* value);

FibonacciHeapNode* FibonacciHeap_exactMin(FibonacciHeap* heap);

void FibonacciHeap_decreaseKey(FibonacciHeap* heap, FibonacciHeapNode* node, void* value, FibonacciHeap_destructor valueDestructor);

void FibonacciHeapNode_destroy(FibonacciHeapNode* node, FibonacciHeap_destructor keyDestructor, FibonacciHeap_destructor valueDestructor);

void FibonacciHeap_destroy(FibonacciHeap* heap, FibonacciHeap_destructor keyDestructor, FibonacciHeap_destructor valueDestructor);


#ifdef FibonacciHeap_debug
void FibonacciHeap_printHeapValue(FibonacciHeap* heap);
int FibonacciHeap_error(FibonacciHeap* heap);
#endif

#endif /* FibonacciHeap_h */
