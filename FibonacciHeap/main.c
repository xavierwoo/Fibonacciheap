//
//  main.c
//  FibonacciHeap
//
//  Created by Xinyun on 2016/12/6.
//  Copyright © 2016年 Xavier Woo. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "FibonacciHeap.h"

int cmp(int *a, int *b){
    if(*a < *b){
        return -1;
    }else if(*a > *b){
        return 1;
    }else{
        return 0;
    }
}

int main(int argc, const char * argv[]) {
    int *value;
    int i;
    int r;
    //FibonacciHeapNode* node;
    FibonacciHeap* h = FibonacciHeap_new((FibonacciHeapNode_compare)cmp);
    //srand(6);
    
    FibonacciHeapNode* fn = NULL;
    FibonacciHeapNode* fn2 = NULL;
    
    for(i=0; i<30000; i++){
        value = (int*)malloc(sizeof(int));
        *value = rand()%100;
        FibonacciHeap_insert(h, NULL, value);
        
        
    }
    
    for(i=0;h->num > 10&&i < 1000;++i){
        r = rand()%3;
        switch (r) {
            case 0:
                value = (int*)malloc(sizeof(int));
                *value = rand()%100;
                fn = FibonacciHeap_insert(h, NULL, value);
                if(FibonacciHeap_error(h) > 0){
                    printf("\n");
                }
                break;
            case 1:
                
                fn2 = FibonacciHeap_exactMin(h);
                
                //FibonacciHeap_printHeapValue(h);
                
                
                if(FibonacciHeap_error(h) > 0){
                    printf("\n");
                }
                FibonacciHeapNode_destroy(fn2, NULL, free);
                if(fn2 == fn){
                    fn = NULL;
                }
                break;
            case 2:
                if(fn != NULL){
                    value = (int*)malloc(sizeof(int));
                    *value = *(int*)fn->value / 2;
                    FibonacciHeap_decreaseKey(h, fn, value, free);
                    if(FibonacciHeap_error(h) > 0){
                        printf("\n");
                    }
                    fn = NULL;
                }
            default:
                break;
        }
//        FibonacciHeap_printHeapValue(h);
//        printf("\n");
    
    }
    
    FibonacciHeap_destroy(h, NULL, free);
    


    return 0;
}
