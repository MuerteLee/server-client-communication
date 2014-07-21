#ifndef _MEMORY_H_
#define _MEMORY_H_
unsigned long kb_main_shared;
unsigned long kb_main_buffers;
unsigned long kb_main_cached;
unsigned long kb_main_free;
unsigned long kb_main_total;
unsigned long kb_main_used;

unsigned long kb_swap_cached; 
unsigned long kb_swap_used;
unsigned long kb_swap_free;
unsigned long kb_swap_total;

unsigned long kb_low_free;
unsigned long kb_low_total;

unsigned long kb_high_free;
unsigned long kb_high_total;

unsigned long kb_active;
unsigned long kb_active_file;
unsigned long kb_active_anon;

unsigned long kb_inactive;
unsigned long kb_inactive_file;
unsigned long kb_inactive_anon;

unsigned long kb_unevicatable;

unsigned long kb_mlocked;

unsigned long kb_dirty;
unsigned long kb_writeback;
unsigned long kb_anon_pages;
unsigned long kb_mapped;
unsigned long kb_shmem;
unsigned long kb_slab;
unsigned long kb_sreclaimable;
unsigned long kb_sunreclaim;

unsigned long kb_kernelstack;

unsigned long kb_pagetables;

unsigned long kb_nfs_unstable;

unsigned long kb_bounce;

unsigned long kb_writebacktmp;

unsigned long kb_commit_limit;
unsigned long kb_committed_as;

unsigned long kb_vmalloc_total;
unsigned long kb_vmalloc_used;
unsigned long kb_vmalloc_chunk;

unsigned long kb_hardware_corrupted;

unsigned long kb_hugepages_total;
unsigned long kb_hugepages_free;
unsigned long kb_hugepages_rsvd;
unsigned long kb_hugepages_surp;
unsigned long kb_hugepagesize;

unsigned long kb_directmap4k;
unsigned long kb_directmap2k;


#define S(X) ( ((unsigned long long)(X) << 10) >> shift)
#define KLONG long long

#ifdef __cplusplus 
extern "C" {
#endif

	int memory(int argc, char* argv[]);

#ifdef __cplusplus
}
#endif

#endif
