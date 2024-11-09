#include "array.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

typedef struct {
    int capacity;  // Total allocated slots for elements
    int size;      // Current number of elements in the array
    char data[];   // Flexible array member for the actual data
} ArrayHeader;

// Internal function to get the data pointer
void *get_data_pointer(ArrayHeader *header) {
    return header ? header->data : NULL;
}

// Ensure array can hold `count` more items, resizing if necessary
void *array_hold(void *array, int count, int item_size) {
    ArrayHeader *header;
	if (array == NULL) {
	    header = NULL;
	} else {
	    // If the array exists, calculate the header pointer from the array pointer
	    header = (ArrayHeader *)((char *)array - offsetof(ArrayHeader, data));
	}

	int required_size = ((header != NULL) ? header->size + count : count);


    if (header == NULL) {
        // Allocate initial memory for a new array
        int initial_capacity = count > 4 ? count : 4;  // Minimum initial capacity
        int raw_size = sizeof(ArrayHeader) + (item_size * initial_capacity);
        header = (ArrayHeader *)malloc(raw_size);
        header->capacity = initial_capacity;
        header->size = count;
        return get_data_pointer(header);
    }

    if (required_size <= header->capacity) {
        // No resizing needed; just increase the size
        header->size += count;
        return get_data_pointer(header);
    }

    // Resize array if needed
    int new_capacity = header->capacity * 2;
    if (required_size > new_capacity) {
        new_capacity = required_size;
    }

    int raw_size = sizeof(ArrayHeader) + (item_size * new_capacity);
    header = (ArrayHeader *)realloc(header, raw_size);
    header->capacity = new_capacity;
    header->size = required_size;

    return get_data_pointer(header);
}

// Return the number of items in the array
int array_length(void *array) {
    if (array == NULL) {
        return 0;
    }
    ArrayHeader *header = (ArrayHeader *)((char *)array - offsetof(ArrayHeader, data));
    return header->size;
}

// Free array memory
void array_free(void *array) {
    if (array != NULL) {
        ArrayHeader *header = (ArrayHeader *)((char *)array - offsetof(ArrayHeader, data));
        free(header);
    }
}

