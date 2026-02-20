#ifndef ARENA_H
#define ARENA_H

#include <stddef.h>
#include <unistd.h>

#define PAGE_SIZE getpagesize()
#define ARENA_SIZE (100L * 1024L * 1024L)

/* FreeBlock
    is the structure to store the free block
*/
struct FreeBlock {
  /* next
    is the next free block
    it is used to traverse the free block chain in forward direction
  */
  struct FreeBlock *next;

  /* prev
    is the previous free block
    it is used to traverse the free block chain in backward direction
  */
  struct FreeBlock *prev;

  /* size
    is the size of the free block
  */
  size_t size;

  /* is_busy
    is the busy status of the free block
  */
  int is_busy;

  /* slab_index
    it is used to store the slab index of the lists
  */
  int slab_index;
};

/* lists
    is the structure to store the free block chain
    it is used to find the free block chain in LIFO manner
*/
struct lists {
  /* head
    is the start of the free block chain
    it is used to traverse the free block chain
    it is also used to find the end of the free block chain
  */
  struct FreeBlock *head;

  /* start_address
    is the starting address of the free block chain
  */
  void *start_address;

  /* end_address
    is the ending address of the free block chain
  */
  void *end_address;

  /* free_head
    use data of free block to store next free  block
    Example:
    By putting the * at the very beginning, we are performing an assignment to
    that memory address.

    *(struct FreeBlock **)addr = value; means: "Go to addr, and write the 8-byte
    address value into that spot." Visualizing the "Corruption" Here is why this
    is safe and doesn't destroy the metadata:

    A Free Block in Memory:

    [   Header (Metadata)   ][       Payload (Data Area)       ]
    [ next | prev | size... ][ byte 1 | byte 2 | ... | byte 8  ]
    ^ found                  ^ found + 1 (This is where the user data lives)

    After your "Ghost Link" magic:

    [   Header (Metadata)   ][       Payload (Data Area)       ]
    [ next | prev | size... ][       ADDRESS OF NEXT FREE      ]
    ^ found                  ^ found + 1 (We used the user's bytes!)

    Metadata (Header): Completely untouched! The next and prev pointers for the
    physical chain are safe. Payload: Overwritten with our free_head link. Since
    the block is free, the user doesn't care that we destroyed their data.

    free_head is used to store the recently freed free block chain in LIFO
    manner
  */
  struct FreeBlock *free_head;

  /* tail
    is the tail of the free block chain.
    it is used to find the end of the free block chain and
    allocate new block at the end of the free block chain
  */
  struct FreeBlock *tail;
};

struct Arena {
  /* start_address
    is the starting address of the arena
  */
  void *start_address;

  /* end_address
    is the ending address of the arena
  */
  void *end_address;

  /* free_lists
    is the array of free lists
  */
  struct lists free_lists[9];

  /* next_fresh_page
    is the next fresh page that can be used for allocation by lists
  */
  void *next_fresh_page;

  /* current_page
    is the current page address available for allocation
  */
  int current_page;

  /* total_size
    is the total size of the arena
  */
  size_t total_size;
};

/* rootArena
    is the root arena
*/
extern struct Arena *rootArena;

/* allocate_from_arena
    is the function to allocate memory from the arena
*/
void *allocate_from_arena(size_t size);

/* free_from_arena
    is the function to free memory from the arena
*/
void free_from_arena(void *ptr);

/* arena_free
    is the function to free the arena
*/
void arena_free(struct Arena *arena);

/* display_arena
    is the function to display the arena
*/
void display_arena(struct Arena *arena);

#endif