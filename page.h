#ifndef PAGE_H
#define PAGE_H

#include "global.h"

#define PAGE_SIZE 1024

typedef struct
{
   u32 present    : 1;   // Page present in memory
   u32 rw         : 1;   // Read-only if clear, readwrite if set
   u32 user       : 1;   // Supervisor level only if clear
   u32 accessed   : 1;   // Has the page been accessed since last refresh?
   u32 dirty      : 1;   // Has the page been written to since last refresh?
   u32 unused     : 7;   // Amalgamation of unused and reserved bits
   u32 frame      : 20;  // Frame address (shifted right 12 bits)
} page_table_entry;

typedef struct
{
   page_table_entry pages[PAGE_SIZE];
} page_table_t;

typedef struct
{
   /**
      Array of pointers to pagetables.
   **/
   page_table_t *tables[PAGE_SIZE];
   /**
      Array of pointers to the pagetables above, but gives their *physical*
      location, for loading into the CR3 register.
   **/
   u32 tablesPhysical[PAGE_SIZE];
   /**
      The physical address of tablesPhysical. This comes into play
      when we get our kernel heap allocated and the directory
      may be in a different location in virtual memory.
   **/
   u32 physicalAddr;
} page_directory;

#endif