//code generate by ChatGPT (not tested)
/*
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    void* pool;         // Pointer to the memory pool
    size_t block_size;  // Size of each block
    size_t capacity;    // Total number of blocks in the pool
    size_t used_blocks; // Number of blocks currently in use
} MemoryManager;

// Function to initialize the memory manager
void initializeMemoryManager(MemoryManager* manager, size_t block_size, size_t capacity) {
    manager->pool = malloc(block_size * capacity);
    manager->block_size = block_size;
    manager->capacity = capacity;
    manager->used_blocks = 0;
}

// Function to allocate a block from the memory manager
void* allocateBlock(MemoryManager* manager) {
    if (manager->used_blocks >= manager->capacity) {
        return NULL; // Memory pool is full
    }

    void* block = (char*)manager->pool + (manager->block_size * manager->used_blocks);
    manager->used_blocks++;

    return block;
}

// Function to deallocate a block and make it available for reuse
void deallocateBlock(MemoryManager* manager, void* block) {
    // You can implement your own logic here, depending on your requirements.
    // For example, you could mark the block as unused but keep it in the pool for reuse.
}

// Function to release the memory pool and clean up the memory manager
void releaseMemoryManager(MemoryManager* manager) {
    free(manager->pool);
    manager->pool = NULL;
    manager->block_size = 0;
    manager->capacity = 0;
    manager->used_blocks = 0;
}

int main() {
    MemoryManager manager;
    size_t block_size = 64;     // Size of each block in bytes
    size_t capacity = 100;      // Total number of blocks in the pool

    initializeMemoryManager(&manager, block_size, capacity);

    // Allocate some blocks
    void* block1 = allocateBlock(&manager);
    void* block2 = allocateBlock(&manager);

    // Use the allocated blocks as needed

    // Deallocate a block when it's no longer needed
    deallocateBlock(&manager, block1);

    // Allocate more blocks
    void* block3 = allocateBlock(&manager);

    // Release the memory manager and clean up
    releaseMemoryManager(&manager);

    return 0;
}

*/