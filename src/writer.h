#ifndef _WRITERH_
#define _WRITERH_

#include "bvh_data.h"
#include "utils.h"

/* max size of character per line */
#define MAX_SIZE_PER_LINE 10000

void write_bvh(const char *filename, BVH_DATA *data);

void write_left_brace(FILE *file, BVH_DATA *data);

void write_right_brace(FILE *file, BVH_DATA *data);

void write_offset(FILE *file, BVH_DATA *data);

void write_channels(FILE *file, BVH_DATA *data);

void write_node(FILE *file, BVH_DATA *data);

void write_endsite(FILE *file, BVH_DATA *data);

char *current_indent(int depth);

#endif
