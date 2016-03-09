#ifndef _UTILSH_
#define _UTILSH_

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

#define LEFT_BRACE "{"
#define RIGHT_BRACE "}"
#define MOTION "MOTION"
#define HIERARCHY "HIERARCHY"
#define CHANNELS "CHANNELS"
#define OFFSET "OFFSET"
#define JOINT "JOINT"
#define ROOT "ROOT"
#define ENDSITE "End Site"
#define Zrotation "Zrotation"
#define Xrotation "Xrotation"
#define Yrotation "Yrotation"
#define BLANK "    "
#define INDENT "\t"

/* trim white-space */
char *trim(char *str);

/* is left brace or not */
int is_left_brace(const char *cs);

/* is right brace or not */
int is_right_brace(const char *cs);

/* is motion */
int is_motion(const char *cs);

/* is channels or not */
int is_channels(const char *cs);

/* is off-set or not */
int is_offset(const char *cs);

/* is joint/root or not */
int is_node(const char *cs);

int is_endsite(const char *cs);

int is_zrotation(const char *cs);

int is_xrotation(const char *cs);

int is_yrotation(const char *cs);

int is_3_channels(const char *cs);

int is_6_channels(const char *cs);

#endif
