#ifndef _BVH_DATAH_
#define _BVH_DATAH_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_BUFFER 100
#define MAX_CAPACITY 100
#define MAX_CHIL 20


typedef struct Matrix
{
  double data[MAX_CAPACITY][MAX_CAPACITY];
  int rows;
  int cols;
} Matrix;


typedef struct Hierachy
{
  char type[MAX_CAPACITY][MAX_BUFFER];
  char name[MAX_CAPACITY][MAX_BUFFER];
  int depth[MAX_CAPACITY];
  int parent[MAX_CAPACITY];
  double offset_from_parent[MAX_CAPACITY][3];
  double coordinate[MAX_CAPACITY][3];
  int channels[MAX_CAPACITY];
  int channel_start[MAX_CAPACITY];
  int order[MAX_CAPACITY][3];
  double head[MAX_CAPACITY][3];
  double tail[MAX_CAPACITY][3];
  int current_occupy;
} Hierachy;

typedef struct Motion
{
  int frames;
  int channels;
  double frame_time;
  double **frame_datas;
} Motion;

typedef struct SkeletonPart
{
  int parts;
  int num_of_skeleton_nodes[MAX_CAPACITY];
  int skeleton_nodes[MAX_CAPACITY][MAX_CAPACITY];
} SkeletonPart;

typedef struct BVH_DATA
{
  int type_count;
  Hierachy hierachy;
  Motion motion;
  SkeletonPart skeleton_part;
} BVH_DATA;

void print_bvh_info(const BVH_DATA *data);

/* init bvh data */
BVH_DATA *init_bvh_data();

/* free bvh data */
void free_bvh_data(BVH_DATA *data);

/* init 2d double */
double **init_double_2d(int rows, int cols);

/* free 2d double */
void free_double_2d(double **data, int rows, int cols);

/* getting  */
double *get_motion_data(int channel_start, int channels, double *frame_data);

/* transpose of matrix */
void transpose(Matrix *res, const Matrix *orig);

/* inverse of matrix */
void inverse(Matrix *inv, const Matrix *mat);

/* matrix multiply */
void multiply(Matrix *res, const Matrix *left, const Matrix *right);

/* matrix multiply constant */
void multiply_scala(Matrix *res, double s, const Matrix *mat);

/* init zeros matrix */
void zeros(Matrix *mat, int rows, int cols);

void eye(Matrix *mat, int rows, int cols);

void copy_matrix(Matrix *dst, const Matrix *src);

double norm2_1d(Matrix *mat);

void add_matrix(Matrix *dst, const Matrix *left, const Matrix *right);

void minus_matrix(Matrix *dst, const Matrix *left, const Matrix *right);

void print_matrix(Matrix *mat);
#endif
