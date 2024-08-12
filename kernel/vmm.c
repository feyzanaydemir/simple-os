/*************************************************************
 * Includes
 ************************************************************/
#include "../include/vmm.h"
#include "../include/pmm.h"
#include "../include/string.h"
#include "../include/stdlib.h"
#include "../include/screen.h"


/*************************************************************
 * Macros
 ************************************************************/
#define PAGE_SIZE 0x1000 /* 4KB*/
#define PAGE_DIRECTORY_INDEX(x) (((x) >> 22) & 0x3ff)
#define PAGE_TABLE_INDEX(x) (((x) >> 12) & 0x3ff)
#define PAGE_GET_PHYSICAL_ADDRESS(x) (*x & ~0xfff)


/*************************************************************
 * Global Variables
 ************************************************************/
page_directory_t *current_directory = 0;
uint32_t current_directory_phys_addr = 0;


/*************************************************************
 * Public Function Definitions
 ************************************************************/
void init_paging(void) {
    page_table_t *table = (page_table_t*)alloc_block();
    if (!table) {
        return;
    }

    page_table_t *kernel_table = (page_table_t*)alloc_block();
    if (!kernel_table) {
        return;
    }

    /* Clear tables */
    memset(table, 0, sizeof(page_table_t));
    memset(kernel_table, 0, sizeof(page_table_t));

    /* First 4mb identity mapped */
    uint32_t i = 0, phys = 0x0, virt = 0x00000000;
    while (i < 1024) {
        /* Create new page */
        table_entry page = 0;
        table_entry_add_attrib(&page, I86_PTE_PRESENT);
        table_entry_set_frame(&page, phys);
        kernel_table->entries[PAGE_TABLE_INDEX(virt)] = page;

        i++;
        phys += 0x1000; /* 4096 */
        virt += 0x1000;
    }

    /* Map 1mb to 3gb */
    i = 0, phys = 0x100000, virt = 0xc0000000;
    while (i < 1024) {
        /* Create new page */
        table_entry page = 0;
        table_entry_add_attrib(&page, I86_PTE_PRESENT);
        table_entry_add_attrib(&page, I86_PTE_WRITABLE);
        table_entry_set_frame(&page, phys);
        table->entries[PAGE_TABLE_INDEX(virt)] = page;

        i++;
        phys += 0x1000; /* 4096 */
        virt += 0x1000;
    }

    /* create default directory table */
    page_directory_t *directory = (page_directory_t*)malloc(3 * PAGE_SIZE);
    if (!directory) {
        return;
    }

    /* clear directory table and set it as current */
    memset(directory, 0, sizeof(page_directory_t));
    
    directory_entry *entry1 = &directory->entries[PAGE_DIRECTORY_INDEX(0xc0000000)];
    directory_entry_add_attrib(entry1, I86_PDE_PRESENT);
    directory_entry_add_attrib(entry1, I86_PDE_WRITABLE);
    directory_entry_set_frame(entry1, (uint32_t)table);

    directory_entry *entry2 = &directory->entries[PAGE_DIRECTORY_INDEX(0x00000000)];
    directory_entry_add_attrib(entry2, I86_PDE_PRESENT);
    directory_entry_add_attrib(entry2, I86_PDE_WRITABLE);
    directory_entry_set_frame(entry2, (uint32_t)kernel_table);
    
    switch_directory(directory);

    /* Enable paging by setting the PG (bit 31) and PE (bit 0) bits in the CR0 register */
    uint32_t cr0;
    asm volatile ("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000;
    asm volatile ("mov %0, %%cr0" : : "r"(cr0));

    kprint("Initilized paging.\n");
}

int switch_directory(page_directory_t* directory) {
	if (!directory)
		return 0;
    
	current_directory = directory;
    current_directory_phys_addr = (uint32_t)&directory->entries;
    asm volatile("mov %0, %%cr3":: "r"(current_directory_phys_addr));

    return 1;
}

/* sets a flag in the page table entry */
void directory_entry_add_attrib (directory_entry* entry, uint32_t attrib) {
    *entry |= attrib;
};
/* clears a flag in the page table entry */
void directory_entry_del_attrib (directory_entry* entry, uint32_t attrib) {
    *entry &= ~attrib;
};
/* sets a frame to page table entry */
void directory_entry_set_frame (directory_entry* entry, uint32_t physical_address) {
    *entry = (*entry & ~I86_PDE_FRAME) | physical_address;
};
/* test if page is present */
int directory_entry_is_present (directory_entry entry) {
    return entry & I86_PDE_PRESENT;
};
/* test if directory is user mode */
int directory_entry_is_user (directory_entry entry) {
    return entry & I86_PDE_USER;
};
/* test if directory contains 4mb pages */
int directory_entry_is_4mb (directory_entry entry) {
    return entry & I86_PDE_4MB;
};
/* test if page is writable */
int directory_entry_is_writable (directory_entry entry) {
    return entry & I86_PDE_WRITABLE;
};
/* get page table entry frame address */
uint32_t directory_entry_pfn (directory_entry entry) {
    return entry & I86_PDE_FRAME;
};


/* sets a flag in the page table entry */
void table_entry_add_attrib (table_entry* entry, uint32_t attrib) {
    *entry |= attrib;
};
/* clears a flag in the page table entry */
void table_entry_del_attrib (table_entry* entry, uint32_t attrib) {
    *entry &= ~attrib;
};
/* sets a frame to page table entry */
void table_entry_set_frame (table_entry* entry, uint32_t physical_address) {
    *entry = (*entry & ~I86_PTE_FRAME) | physical_address;
};
/* test if page is present */
int table_entry_is_present (table_entry entry) {
    return entry & I86_PTE_PRESENT;
};
/* test if page is writable */
int table_entry_is_writable (table_entry entry) {
    return entry & I86_PTE_WRITABLE;
};
/* get page table entry frame address */
uint32_t table_entry_pfn (table_entry entry) {
    return entry & I86_PTE_FRAME;
};

// void *alloc_page(void) {}
// void free_page(table_entry page) {}
// void *map_page(void *block_address) {}
// void unmap_page(table_entry page) {}
