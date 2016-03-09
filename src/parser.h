#ifndef _PARSERH_
#define _PARSERH_

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "utils.h"
#include "bvh_data.h"

/* max size of character per line */
#define MAX_SIZE_PER_LINE 10000

void parse_bvh(const char* filename, BVH_DATA *data);

void parse_left_brace(const char *line, BVH_DATA *data);

void parse_right_brace(const char *line, BVH_DATA *data);

void parse_offset(const char *line, BVH_DATA *data);

void parse_channels(const char *line, BVH_DATA *data);

void parse_node(const char *line, BVH_DATA *data);

void parse_endsite(const char *line, BVH_DATA *data);

void parse_skeleton_parts(BVH_DATA *data);

#endif
