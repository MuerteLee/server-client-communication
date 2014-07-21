#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include <unistd.h>
#include <fcntl.h>
#include "memory.h"
#include "meminfo.h"

#define BAD_OPEN_MESSAGE					\
"Error: open filename error! \n"				\

#define MEMINFO_FILE "/proc/meminfo"
static int meminfo_fd = -1;

static char buf[2048];

#define FILE_TO_BUF(filename, fd) do{				\
    static int local_n;						\
    if (fd == -1 && (fd = open(filename, O_RDONLY)) == -1) {	\
	fputs(BAD_OPEN_MESSAGE, stderr);			\
	fflush(NULL);						\
	_exit(102);						\
    }								\
    lseek(fd, 0L, SEEK_SET);					\
    if ((local_n = read(fd, buf, sizeof buf - 1)) < 0) {	\
	perror(filename);					\
	fflush(NULL);						\
	_exit(103);						\
    }								\
    buf[local_n] = '\0';					\
}while(0)

static int compare_mem_table_structs(const void *a, const void *b)
{
  return strcmp(((const mem_table_struct*)a)->name,((const mem_table_struct*)b)->name);
}

static int compmi(const void *m1, const void *m2)
{
	struct mem_table_struct *mi1 = (struct mem_table_struct*) m1;
	struct mem_table_struct *mi2 = (struct mem_table_struct*) m2;
  return strcmp(((const mem_table_struct*)m1)->name,((const mem_table_struct*)m2)->name);
}

int meminfo(void)
{
	char* head = NULL;
	char* tail;
	char namebuf[16];
	mem_table_struct findme = {namebuf};
	mem_table_struct *found;

	static mem_table_struct mem_table[]={
		{"MemTotal", 			&kb_main_total			},
		{"MemFree", 	   		&kb_main_free			},
		{"Buffers", 	   		&kb_main_buffers		},
		{"Cached", 		   		&kb_main_cached			},
		{"SwapCached",  		&kb_swap_cached			},
		{"Active",     			&kb_active				},       
		{"Inactive",    		&kb_inactive			},
		{"Active(anon)",    	&kb_active_anon			}, 
		{"Inactive(anon)",  	&kb_inactive_anon		}, 
		{"Active(file)",    	&kb_active_file			}, 
		{"Inactive(file)",  	&kb_inactive_file		}, 
		{"Unevictable",     	&kb_unevicatable		}, 
		{"Mlocked",         	&kb_mlocked				}, 
		{"SwapTotal",       	&kb_swap_total			},
		{"SwapFree",        	&kb_swap_free			},
		{"Dirty",           	&kb_dirty				},
		{"Writeback",       	&kb_writeback			},
		{"AnonPages",       	&kb_anon_pages			},
		{"Mapped",          	&kb_mapped				},
		{"Shmem",           	&kb_shmem				},
		{"Slab",            	&kb_slab				},
		{"SReclaimable",    	&kb_sreclaimable		},
		{"SUnreclaim",      	&kb_sunreclaim			},
		{"KernelStack",     	&kb_kernelstack			},
		{"PageTables",      	&kb_pagetables			},
		{"NFS_Unstable",    	&kb_nfs_unstable		},
		{"Bounce",          	&kb_bounce				},
		{"WritebackTmp",    	&kb_writebacktmp		},
		{"CommitLimit",     	&kb_commit_limit		},
		{"Committed_AS",    	&kb_committed_as		},
		{"VmallocTotal",    	&kb_vmalloc_total		},
		{"VmallocUsed",     	&kb_vmalloc_used		},
		{"VmallocChunk",    	&kb_vmalloc_chunk		},
		{"HardwareCorrupted",	&kb_hardware_corrupted	},
		{"HugePages_Total",  	&kb_hugepages_total		},
		{"HugePages_Free",   	&kb_hugepages_free		},
		{"HugePages_Rsvd",   	&kb_hugepages_rsvd		},
		{"HugePages_Surp",   	&kb_hugepages_surp		},
		{"Hugepagesize",     	&kb_hugepagesize		},
		{"DirectMap4k",      	&kb_directmap4k			},
		{"DirectMap2M",     	&kb_directmap2k			},
	};

	const int mem_table_count = sizeof(mem_table)/(sizeof(mem_table_struct)+1);
	FILE_TO_BUF(MEMINFO_FILE, meminfo_fd);
	head = buf;

	qsort(&mem_table, mem_table_count, sizeof(mem_table_struct), compmi);

	for(;;)
	{
		tail = strchr(head, ':');
		if(!tail) 
			break;
		*tail = '\0';
		if(strlen(head) >= sizeof(namebuf))
		{
			head = tail+1;
			goto nextline;
		}
		strcpy(namebuf,head);

		found = bsearch(&findme, mem_table, mem_table_count,
				sizeof(mem_table_struct), compare_mem_table_structs
				);
		head = tail+1;
		if(!found) 
			goto nextline;
		*(found->slot) = (unsigned long)strtoull(head,&tail,10);
nextline:
		tail = strchr(head, '\n');
		if(!tail) 
			break;
		head = tail+1;
	}
	
	kb_swap_used = kb_swap_total - kb_swap_free;
	kb_main_used = kb_main_total - kb_main_free;
	return 0;
}	
