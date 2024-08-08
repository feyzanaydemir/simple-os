#ifndef VMM_H
#define VMM_H

/*************************************************************
 * Includes
 ************************************************************/
#include <stdint.h>


/*************************************************************
 * Macros
 ************************************************************/
#define PAGES_PER_TABLE 1024
#define PAGES_PER_DIR 1024


/*************************************************************
 * Enums
 ************************************************************/
enum PAGE_DIRECTORY_ENTRY_FLAGS {
	I86_PDE_PRESENT			=	1,
	I86_PDE_WRITABLE		=	2,
	I86_PDE_USER			=	4,
	I86_PDE_PWT				=	8,
	I86_PDE_PCD				=	0x10,
	I86_PDE_ACCESSED		=	0x20,
	I86_PDE_DIRTY			=	0x40,
	I86_PDE_4MB				=	0x80,
	I86_PDE_CPU_GLOBAL		=	0x100,
	I86_PDE_LV4_GLOBAL		=	0x200,
   	I86_PDE_FRAME			=	0x7FFFF000
};

enum PAGE_TABLE_ENTRY_FLAGS {
	I86_PTE_PRESENT			=	1,
	I86_PTE_WRITABLE		=	2,
	I86_PTE_USER			=	4,
	I86_PTE_WRITETHOUGH		=	8,
	I86_PTE_NOT_CACHEABLE	=	0x10,
	I86_PTE_ACCESSED		=	0x20,
	I86_PTE_DIRTY			=	0x40,
	I86_PTE_PAT				=	0x80,
	I86_PTE_CPU_GLOBAL		=	0x100,
	I86_PTE_LV4_GLOBAL		=	0x200,
   	I86_PTE_FRAME			=	0x7FFFF000
};


/*************************************************************
 * Typedefs and Structs
 ************************************************************/
typedef uint32_t directory_entry;
typedef uint32_t table_entry;

typedef struct page_directory {
	directory_entry entries[PAGES_PER_DIR];
} page_directory_t;

typedef struct page_table {
	table_entry entries[PAGES_PER_TABLE];
} page_table_t;


/*************************************************************
 * Function Prototypes
 ************************************************************/
extern void directory_entry_add_attrib (directory_entry* entry, uint32_t attrib);
extern void directory_entry_del_attrib (directory_entry* entry, uint32_t attrib);
extern void directory_entry_set_frame (directory_entry* entry, uint32_t phys_addr);
extern int directory_entry_is_present (directory_entry entry);
extern int directory_entry_is_user (directory_entry entry);
extern int directory_entry_is_4mb (directory_entry entry);
extern int directory_entry_is_writable (directory_entry entry);
extern uint32_t directory_entry_pfn (directory_entry entry);

extern void table_entry_add_attrib (table_entry* entry, uint32_t attrib);
extern void table_entry_del_attrib (table_entry* entry, uint32_t attrib);
extern void table_entry_set_frame (table_entry* entry, uint32_t phys_addr);
extern int table_entry_is_present (table_entry entry);
extern int table_entry_is_writable (table_entry entry);
extern uint32_t table_entry_pfn (table_entry entry);

extern void init_paging(void);
extern void *map_physical_to_virtual(void *uint32_tess);
extern void *get_uint32_tess(void *virtual_address);

extern int switch_directory(page_directory_t* directory);
extern void *alloc_page(void);
extern void free_page(table_entry page);
extern void *map_page(void *block_address);
extern void unmap_page(table_entry page);

#endif
