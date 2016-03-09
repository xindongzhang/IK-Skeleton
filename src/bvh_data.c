#include "bvh_data.h"

void print_bvh_info(const BVH_DATA *data)
{
  printf("current_occupy: %d\n", data->hierachy.current_occupy);
  printf("frames: %d\n", data->motion.frames);
  printf("channels: %d\n", data->motion.channels);
  int parts = data->skeleton_part.parts, p;
  printf("skeleton parts: %d\n", parts);
  for (p = 0; p < parts; ++p)
  {
    int num_of_skeleton_nodes = data->skeleton_part.num_of_skeleton_nodes[p], n;
    printf("number of skeleton nodes is : %d , { ", num_of_skeleton_nodes);
    for (n = 0; n < num_of_skeleton_nodes; ++n)
    {
      printf("%d ", data->skeleton_part.skeleton_nodes[p][n]);
    }
    printf("}\n");
  }
}

/* init bvh data */
BVH_DATA *init_bvh_data()
{
  BVH_DATA *data = (BVH_DATA *) malloc(sizeof(BVH_DATA));
  data->type_count = 0;
  data->hierachy.current_occupy = 0;
  data->motion.channels = 0;
  return data;
}

/* free bvh data */
void free_bvh_data(BVH_DATA *data)
{
  free_double_2d(data->motion.frame_datas, data->motion.frames, data->motion.channels);
  if (data != NULL)
  {
    free(data);
  }
}

/* getting  motion params */
double *get_motion_data(int channel_start, int channels, double *frame_data)
{
  double *res;
  if (channels == 6)
  {
    res = (double *) malloc(sizeof(double) * 6);
    int i;
    for (i = 0; i < channels; ++i)
    {
      res[i] = frame_data[channel_start + i];
    }
  } else if (channels == 3)
  {
    res = (double *) malloc(sizeof(double) * 3);
    int i;
    for (i = 0; i < channels; ++i)
    {
      res[i] = frame_data[channel_start + i];
    }
  } else
  {
    printf("the channels is not 3 or 6\n");
    exit(-1);
  }
  return res;
}

/* init 2d double data */
double **init_double_2d(int rows, int cols)
{
  double **data = (double **) malloc(sizeof(double *) * rows);
  int i;
  for (i = 0; i < rows; ++i)
  {
    data[i] = (double *) malloc(sizeof(double) * cols);
    if (data[i] == NULL)
    {
      printf("out of memory\n");
      exit(1);
    } else
    {
      int j;
      for (j = 0; j < cols; ++j)
      {
        data[i][j] = 0.0;
      }
    }
  }
  return data;
}

/* free 2d double data */
void free_double_2d(double **data, int rows, int cols)
{
  int i;
  for (i = 0; i < rows; ++i)
  {
    free(data[i]);
  }
  free(data);
}

void transpose(Matrix *res, const Matrix *orig)
{
  int rows = orig->rows, i;
  int cols = orig->cols, j;
  for (i = 0; i < rows; ++i)
  {
    for (j = 0; j < cols; ++j)
    {
      res->data[j][i] = orig->data[i][j];
    }
  }
  res->rows = cols;
  res->cols = rows;
}

/* matrix multiply */
void multiply(Matrix *res, const Matrix *left_o, const Matrix *right_o)
{
  Matrix left, right;
  copy_matrix(&left, left_o);
  copy_matrix(&right, right_o);
  int left_rows = left.rows;
  int left_cols = left.cols;
  //int right_rows = right->rows;
  int right_cols = right.cols;
  int i,j,k;
  for(i = 0; i < left_rows; ++i)
  {
    for (j = 0; j < right_cols; ++j)
    {
      res->data[i][j] = 0.0;
      for (k = 0; k < left_cols; ++k)
      {
        res->data[i][j] += left.data[i][k] * right.data[k][j];
      }
    }
  }
  res->rows = left_rows;
  res->cols = right_cols;
}

/* matrix multiply constant */
void multiply_scala(Matrix *res, double s, const Matrix *mat)
{
  copy_matrix(res, mat);
  int rows = res->rows, i;
  int cols = res->cols, j;
  for(i = 0; i < rows; ++i)
  {
    for (j = 0; j < cols; ++j)
    {
      res->data[i][j] *= s;
    }
  }
}

void zeros(Matrix *mat, int rows, int cols)
{
  mat->rows = rows;
  mat->cols = cols;
  int i,j;
  for (i = 0; i < rows; ++i)
  {
    for (j = 0; j < cols; ++j)
    {
      mat->data[i][j] = 0.0;
    }
  }
}

void eye(Matrix *mat, int rows, int cols)
{
  mat->rows = rows;
  mat->cols = cols;
  zeros(mat, rows, cols);
  int N, i;
  rows > cols? N = cols: N = rows;
  for (i = 0; i < N; ++i)
  {
    mat->data[i][i] = 1.0;
  }
}

double norm2_1d(Matrix *mat)
{
  double norm = 0.0;
  int rows = mat->rows, i;
  int cols = mat->cols, j;
  if (rows == 1)
  {
    for (j = 0; j < cols; ++j)
    {
      norm += mat->data[0][j] * mat->data[0][j];
    }
  } else if (cols == 1)
  {
    for (i = 0; i < rows; ++i)
    {
      norm += mat->data[i][0] * mat->data[i][0];
    }
  } else
  {
    printf("this is not 1d-array\n");
    exit(-1);
  }
  return sqrt(norm);
}

void add_matrix(Matrix *dst, const Matrix *left_o, const Matrix *right_o)
{
  Matrix left, right;
  copy_matrix(&left, left_o);
  copy_matrix(&right, right_o);
  int rows = left.rows, i;
  int cols = right.cols, j;
  for(i = 0; i < rows; ++i)
  {
    for (j = 0; j < cols; ++j)
    {
      dst->data[i][j] = left.data[i][j] + right.data[i][j];
    }
  }
  dst->rows = rows;
  dst->cols = cols;
}

void minus_matrix(Matrix *dst, const Matrix *left_o, const Matrix *right_o)
{
  Matrix left, right;
  copy_matrix(&left, left_o);
  copy_matrix(&right, right_o);
  int rows = left.rows, i;
  int cols = right.cols, j;
  for(i = 0; i < rows; ++i)
  {
    for (j = 0; j < cols; ++j)
    {
      dst->data[i][j] = left.data[i][j] - right.data[i][j];
    }
  }
  dst->rows = rows;
  dst->cols = cols;
}

void print_matrix(Matrix *mat)
{
  int rows = mat->rows, i;
  int cols = mat->cols, j;
  for (i = 0; i < rows; ++i)
  {
    for (j = 0; j < cols; ++j)
    {
      printf("%lf\t", mat->data[i][j]);
    }
    printf("\n");
  }
}

void copy_matrix(Matrix *dst, const Matrix *src)
{
  int rows = src->rows, i;
  int cols = src->cols, j;
  dst->rows = rows;
  dst->cols = cols;
  for (i = 0; i < rows; ++i)
  {
    for (j = 0; j < cols; ++j)
    {
      dst->data[i][j] = src->data[i][j];
    }
  }
}

/* inverse of matrix */
void inverse(Matrix *inv, const Matrix *mat)
{
  int rows = mat->rows, i;
  int cols = mat->cols, j, k;
  inv->rows = rows;
  inv->cols = cols;
  copy_matrix(inv, mat);
  double ratio,a;
  for (i = 0; i < rows; ++i)
  {
    for (j = cols; j < 2 * cols; ++j)
    {
      if(i == (j-rows))
          inv->data[i][j] = 1.0;
      else
          inv->data[i][j] = 0.0;
    }
  }
  for(i = 0; i < rows; i++)
  {
      for(j = 0; j < cols; j++)
      {
          if(i!=j)
          {
              ratio = inv->data[j][i] / inv->data[i][i];
              for(k = 0; k < 2*cols; k++)
              {
                  inv->data[j][k] -= ratio * inv->data[i][k];
              }
          }
      }
  }
  for(i = 0; i < rows; i++)
  {
      a = inv->data[i][i];
      for(j = 0; j < 2*cols; j++)
      {
          inv->data[i][j] /= a;
      }
  }
  for(i = 0; i < rows; i++)
  {
      for(j = cols; j < 2*cols; j++)
      {
        inv->data[i][j-cols] = inv->data[i][j];
      }
  }
}


void test_matrix()
{
  Matrix *orig = (Matrix *) malloc(sizeof(Matrix));
  Matrix *trans = (Matrix *) malloc(sizeof(Matrix));
  orig->rows = 2;
  orig->cols = 3;
  orig->data[1][0] = 10;
  transpose(trans, orig);
  print_matrix(orig);
  print_matrix(trans);
  printf("-----------------\n");
  Matrix res;
  Matrix left, right;
  left.rows = 2;
  left.cols = 2;
  left.data[0][0] = 1.0;left.data[1][1] = 2.0;
  right.rows = 2;
  right.cols = 2;
  right.data[0][0] = 1.0;right.data[1][1] = 2.0;
  multiply(&res, &right, &left);
  print_matrix(&res);
  printf("------------------\n");
  Matrix zero;
  zeros(&zero, 3, 3);
  Matrix eyes;
  eye(&eyes, 4, 4);
  print_matrix(&zero);
  print_matrix(&eyes);
}

/*
int main(void)
{
  test_matrix();
  return 0;
}
*/
