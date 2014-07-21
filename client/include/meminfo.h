#ifndef __MEM_INFO_
#define __MEM_INFO_
	
typedef struct mem_table_struct {
  const char *name;     /* memory type name */
  unsigned long *slot; /* slot in return struct */
} mem_table_struct;

#ifdef __cplusplus
extern "C" {
#endif

	int meminfo(void);

#ifdef __cplusplus
}
#endif

#endif
