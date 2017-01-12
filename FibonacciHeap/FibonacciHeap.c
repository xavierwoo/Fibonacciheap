//
//  FibonacciHeap.c
//  FibonacciHeap
//
//  Created by Xinyun on 2016/12/6.
//  Copyright © 2016 Xavier Woo. All rights reserved.
//

#include "FibonacciHeap.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>


#ifdef FibonacciHeap_debug

void FibonacciHeapNode_countRecursive(FibonacciHeapNode* node, FibonacciHeapNode* stopN, int* count){
    if(node != stopN){
        FibonacciHeapNode_countRecursive(node->right, stopN, count);
    }
    if(node->child != NULL){
        FibonacciHeapNode_countRecursive(node->child, node->child->left, count);
    }
    ++*count;
}

int FibonacciHeap_error(FibonacciHeap* heap){
    
    int count = 0;
    
    if(heap->num == 0 && heap->min == NULL)return 0;
    
    FibonacciHeapNode_countRecursive(heap->min, heap->min->left, &count);
    
    if(count != heap->num)return 1;
    
    return 0;
}

void PrintFDEBUG ( const char * format, ... )
{
    //char buffer[256];
    va_list arg;
    va_start(arg, format);
    vfprintf(stderr, format, arg);
    va_end(arg);}

void FibonacciHeapNode_printNodeValue(FibonacciHeapNode* node){
    FibonacciHeapNode* currNode;
    PrintFDEBUG(" %d", *(int*)node->value);
    
    if(node->isMarked) {
        PrintFDEBUG("* ");
    }else{
        PrintFDEBUG(" ");
    }
    
    currNode = node->child;
    
    if(currNode != NULL) {
        PrintFDEBUG("{");
        do{
            FibonacciHeapNode_printNodeValue(currNode);
            currNode = currNode->right;
        }while(currNode != node->child);
        PrintFDEBUG("}");
    }
}

void FibonacciHeap_printHeapValue(FibonacciHeap* heap){
    FibonacciHeapNode* currNode;
    currNode = heap->min;
    do{
        FibonacciHeapNode_printNodeValue(currNode);
        currNode = currNode->right;
    }while(currNode != heap->min);
    PrintFDEBUG("\n\n");
}
#endif

DegreePointers* DegreePointers_new(){
    DegreePointers* dps = (DegreePointers*)malloc(sizeof(DegreePointers));
    dps->pointers = (FibonacciHeapNode**)calloc(4, sizeof(FibonacciHeapNode*));
    dps->len = 4;
    return dps;
}

void FibonacciHeapNode_insertToLeft(FibonacciHeapNode* node1, FibonacciHeapNode* node2){
    FibonacciHeapNode *tmpL;
    tmpL = node1->left;
    node2->right = node1;
    node2->left = tmpL;
    tmpL->right = node2;
    node1->left = node2;
    node2->parent = node1->parent;
}

void FibonacciHeapNode_addChild(FibonacciHeapNode_compare cmp, FibonacciHeapNode* parent, FibonacciHeapNode* child){
    if(parent->degree == 0){
        parent->child = child;
        child->parent = parent;
        child->left = child;
        child->right = child;
    }else{
        FibonacciHeapNode_insertToLeft(parent->child, child);
        parent->child = cmp(parent->child->value, child->value) < 0 ? parent->child : child;
    }
    ++parent->degree;
}

FibonacciHeapNode* FibonacciHeapNode_merge(FibonacciHeap* h, FibonacciHeapNode_compare cmp, FibonacciHeapNode* node1, FibonacciHeapNode* node2){
    FibonacciHeapNode *bigger, *smaller;
    if(cmp(node1->value, node2->value) <= 0){
        bigger = node2;
        smaller = node1;
    }else{
        bigger = node1;
        smaller = node2;
    }
    

    bigger->left->right = bigger->right;
    bigger->right->left = bigger->left;
    

    FibonacciHeapNode_addChild(cmp, smaller, bigger);
    if(h->min == bigger){
        h->min = smaller;
    }
    return smaller;
}

FibonacciHeapNode* DegreePointers_get(DegreePointers* dps, int degree){
    int newLen;
    if(degree >= dps->len){
        newLen = degree * 2;
        dps->pointers = (FibonacciHeapNode**)realloc(dps->pointers, sizeof(FibonacciHeapNode*) * newLen);
        
        memset(dps->pointers+dps->len, 0, sizeof(FibonacciHeapNode*) * (newLen - dps->len));
        
        //        for(i= dps->len; i< newLen; ++i){
        //            dps->pointers[i] = NULL;
        //        }
        dps->len = newLen;
    }
    
    return dps->pointers[degree];
}

void DegreePointers_set(FibonacciHeap* h,FibonacciHeapNode_compare cmp, DegreePointers* dps, FibonacciHeapNode* node){
    FibonacciHeapNode* preNode;
    FibonacciHeapNode* newNode;
    preNode = DegreePointers_get(dps, node->degree);
    
    if(preNode == NULL){
        dps->pointers[node->degree] = node;
        node->parent = NULL;
        return;
    }
    dps->pointers[preNode->degree] = NULL;
    
    
    newNode = FibonacciHeapNode_merge(h,cmp, preNode, node);
    DegreePointers_set(h, cmp, dps, newNode);
}


void DegreePointers_destroy(DegreePointers* dps){
    free(dps->pointers);
    free(dps);
    
}

FibonacciHeap* FibonacciHeap_new(FibonacciHeapNode_compare cmp){
    FibonacciHeap* heap = (FibonacciHeap*)malloc(sizeof(FibonacciHeap));
    heap->min = NULL;
    heap->num = 0;
    heap->compare = cmp;
    heap->dps = DegreePointers_new();
    return heap;
}


FibonacciHeapNode* FibonacciHeapNode_new(void* key, void* value){
    FibonacciHeapNode* node = (FibonacciHeapNode*)malloc(sizeof(FibonacciHeapNode));
    node->child = NULL;
    node->degree = 0;
    node->parent = NULL;
    node->parent = NULL;
    node->left = NULL;
    node->right = NULL;
    node->isMarked = 0;
    node->key = key;
    node->value = value;
    return node;
}



FibonacciHeapNode* FibonacciHeap_insert(FibonacciHeap* heap, void* key, void* value){
    FibonacciHeapNode* node = FibonacciHeapNode_new(key, value);
    //FibonacciHeapNode *tmpL;
    if(heap->min == NULL){
        heap->min = node;
        node->left = node;
        node->right = node;
    }else{
        FibonacciHeapNode_insertToLeft(heap->min, node);
        if(heap->compare(node->value, heap->min->value) < 0){
            heap->min = node;
        }
    }
    ++heap->num;
    return node;
}

//FibonacciHeapNode* FibonacciHeapNode_union(FibonacciHeapNode_compare cmp, FibonacciHeapNode* node1, FibonacciHeapNode* node2){
//    FibonacciHeapNode *n1R, *n2L;
//    n1R = node1->right;
//    n2L = node2->left;
//    node1->right = node2;
//    node2->left = node1;
//    n1R->left = n2L;
//    n2L->right = n1R;
//
//    return cmp(node1->value, node2->value) < 0 ? node1 : node2;
//}



FibonacciHeapNode* FibonacciHeap_extractMin(FibonacciHeap* heap){
    FibonacciHeapNode *end, *current, *next = NULL;
    FibonacciHeapNode* preMin = heap->min;
    int continueFlag = 1;
    if(heap->num == 0) return NULL;
    if(heap->num == 1){
        --heap->num;
        heap->min=NULL;
        return preMin;
    }
    --heap->num;
    
    memset(heap->dps->pointers, 0, sizeof(FibonacciHeapNode*) * heap->dps->len);
    
    if(preMin->left != preMin){
        if(preMin->degree > 0){
            preMin->left->right = preMin->child;
            preMin->right->left = preMin->child->left;
            preMin->child->left->right = preMin->right;
            preMin->child->left = preMin->left;
            heap->min = preMin->child;
        }else{
            preMin->left->right = preMin->right;
            preMin->right->left = preMin->left;
            heap->min = preMin->right;
        }
        end = preMin->left;
        
        //current = heap->min;
    }else{
        end = preMin->child->left;
        heap->min = preMin->child;
        
    }
    current = heap->min;
    while(continueFlag){
        current->parent = NULL;
        
        if(current != end){
            next = current->right;
        }else{
            continueFlag = 0;
        }

        DegreePointers_set(heap,heap->compare, heap->dps, current);

        if(continueFlag != 0){
            current = next;
        }
        heap->min = heap->compare(current->value, heap->min->value) < 0 ? current : heap->min;
    }
    
    preMin->parent = NULL;
    preMin->child = NULL;
    preMin->left = NULL;
    preMin->right = NULL;
    
    return preMin;
}


void FibonacciHeapNode_destroy(FibonacciHeapNode* node, FibonacciHeap_destructor keyDestructor, FibonacciHeap_destructor valueDestructor){
    //printf("%d destroyed\n", *(int*)node->value);
    if(keyDestructor != NULL){
        keyDestructor(node->key);
    }
    if(valueDestructor != NULL){
        valueDestructor(node->value);
    }
    
    free(node);
}

void FibonacciHeapNode_destroyRecursive(FibonacciHeapNode* node, FibonacciHeap_destructor keyDestructor, FibonacciHeap_destructor valueDestructor, FibonacciHeapNode* stopN){
    
    if(node != stopN){
        FibonacciHeapNode_destroyRecursive(node->right, keyDestructor, valueDestructor, stopN);
    }
    if(node->child != NULL){
        FibonacciHeapNode_destroyRecursive(node->child, keyDestructor, valueDestructor, node->child->left);
    }
    FibonacciHeapNode_destroy(node, keyDestructor, valueDestructor);
}

void FibonacciHeap_destroy(FibonacciHeap* heap, FibonacciHeap_destructor keyDestructor, FibonacciHeap_destructor valueDestructor){
    if(heap->num>0){
        FibonacciHeapNode_destroyRecursive(heap->min, keyDestructor, valueDestructor, heap->min->left);
    }
    free(heap->dps->pointers);
    free(heap->dps);
    free(heap);
}

int FibonacciHeapNode_decreaseKeyCase(FibonacciHeapNode* node, FibonacciHeapNode_compare cmp){
    if(node->parent == NULL){
        return -1;
    }else if(cmp(node->value, node->parent->value) >= 0){
        return 0;
    }else if(node->parent->isMarked == 0){
        return 1;
    }else{
        return 2;
    }
}

void FibonacciHeap_decreaseKeyCaseM1(FibonacciHeap* heap, FibonacciHeapNode* node){
    heap->min = heap->compare(heap->min->value, node->value) <= 0 ? heap->min : node;
}

void FibonacciHeapNode_removeChild(FibonacciHeapNode* parent, FibonacciHeapNode* child, FibonacciHeapNode_compare cmp){
    FibonacciHeapNode* currNode;
    if(parent->degree == 1){
        parent->child = NULL;
    }else if(parent->child == child){
        parent->child = child->right;
        currNode = child->right->right;
        while(currNode != child){
            if(cmp(currNode->value, parent->child->value) < 0){
                parent->child = currNode;
            }
            currNode = currNode ->right;
        }
    }
    
    child->left->right = child->right;
    child->right->left = child->left;
    child->left = NULL;
    child->right = NULL;
    child->parent = NULL;
    --parent->degree;
}

void FibonacciHeap_decreaseKeyCase1(FibonacciHeap* heap, FibonacciHeapNode* node){
    node->parent->isMarked = 1;
    FibonacciHeapNode_removeChild(node->parent, node, heap->compare);
    FibonacciHeapNode_insertToLeft(heap->min, node);
    heap->min = heap->compare(heap->min->value, node->value) <= 0 ? heap->min : node;
}

void FibonacciHeap_decreaseKeyCase2(FibonacciHeap* heap, FibonacciHeapNode* node){
    FibonacciHeapNode *parent, *child;
    child = node;
    parent = child->parent;
    
    for(;;){
        if(parent==NULL){
            break;
        }else if(parent->isMarked == 0){
            parent->isMarked = 1;
            break;
        }else{
            FibonacciHeapNode_removeChild(parent, child, heap->compare);
            FibonacciHeapNode_insertToLeft(heap->min, child);
            heap->min = heap->compare(heap->min->value, child->value) <= 0 ? heap->min : child;
            parent->isMarked = 0;
            child = parent;
            parent = parent->parent;
        }
    }
}

void FibonacciHeap_decreaseKey(FibonacciHeap* heap, FibonacciHeapNode* node, void* value, FibonacciHeap_destructor valueDestructor){
    int dCase;
    if(heap->compare(node->value, value) < 0){
        fprintf(stderr, "decreaseKey Error!\n");
        exit(EXIT_FAILURE);
    }
    if(valueDestructor != NULL){
        valueDestructor(node->value);
    }
    node->value = value;
    
    dCase = FibonacciHeapNode_decreaseKeyCase(node, heap->compare);
    switch (dCase) {
        case -1:
            FibonacciHeap_decreaseKeyCaseM1(heap, node);
            break;
        case 0:
            //It is perfect, nothing is needed.
            break;
        case 1:
            FibonacciHeap_decreaseKeyCase1(heap, node);
            break;
        case 2:
            FibonacciHeap_decreaseKeyCase2(heap, node);
            break;
        default:
            break;
    }
    
}




