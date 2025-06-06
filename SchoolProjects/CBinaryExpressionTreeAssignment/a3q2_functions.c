#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "a3q2_header.h"

/*
 * The downHeap function performs the downheap algorithm.
 * The key at the passed position is swapped with the key of it's child with the smallest value.
 * If the key is smaller, than both of it's children's keys then no swap happens.
 * After swap, the downHeap is called again to the new position of the swapped key and it terminates when it's value is smaller than both children.
*/
void downHeap(int key[20], int pos, int array[20][10]){
    int leftChildIndex = pos * 2 + 1;
    int rightChildIndex = pos * 2 + 2;
    int largestValue = pos;

    if (isParent(pos)) {
        if (key[leftChildIndex] > key[largestValue]) {//change key pos to left child
            largestValue = leftChildIndex;
        }
        if (rightChildIndex < 20 && key[rightChildIndex] > key[largestValue] ) { //change key pos to right child
            largestValue = rightChildIndex;
        }
    }

    if (largestValue != pos) {
        swap(pos,largestValue,key,array);
        downHeap(key,largestValue,array);//recursively iterate through tree array
    }

}

// The isParent function returns true if the passed position has a child or false if there's no child
bool isParent(int keyPos){
    return (keyPos * 2 + 1 < 20);

}

// The swap function swaps 2 rows in the 2D array and the key array. It receives the position of the 2 rows to be swapped, the key array, and the 2D array in its arguements.
void swap(int key1Pos, int key2Pos, int key[20], int array[20][10]){
    int temp;
    temp = key[key1Pos];
    key[key1Pos] = key[key2Pos];
    key[key2Pos] = temp;

    for (int i = 0; i < 10; i++) {
        temp = array[key1Pos][i];
        array[key1Pos][i] = array[key2Pos][i];
        array[key2Pos][i] = temp;
    }

}


