#include "core.h"

void fill_rotation(Matrix *T, double theta, int order)
{
  double angle = (theta / 360) * 2 * PI;
  double sin_theta = sin(angle);
  double cos_theta = cos(angle);
  if (order == 1)
  {
    T->data[1][1] = cos_theta;
    T->data[1][2] = -sin_theta;
    T->data[2][1] = sin_theta;
    T->data[2][2] = cos_theta;
  } else if (order == 2)
  {
    T->data[0][0] = cos_theta;
    T->data[0][2] = sin_theta;
    T->data[2][0] = -sin_theta;
    T->data[2][2] = cos_theta;
  } else
  {
    T->data[0][0] = cos_theta;
    T->data[0][1] = -sin_theta;
    T->data[1][0] = sin_theta;
    T->data[1][1] = cos_theta;
  }
}

/* default roration order: z, x, y */
void transform_matrix(Matrix *T, const double *displ, const double *theta)
{

  Matrix Tz, Tx, Ty;
  eye(&Tz, 4, 4);
  eye(&Tx, 4, 4);
  eye(&Ty, 4, 4);
  double displ_x = displ[0];
  double displ_y = displ[1];
  double displ_z = displ[2];
  double theta_z = theta[0];
  double theta_x = theta[1];
  double theta_y = theta[2];
  fill_rotation(&Tz, theta_z, Z_Order);
  fill_rotation(&Tx, theta_x, X_Order);
  fill_rotation(&Ty, theta_y, Y_Order);
  Matrix TzTx;
  multiply(&TzTx, &Tz, &Tx);
  multiply(T, &TzTx, &Ty);
  T->data[0][3] = displ_x;
  T->data[1][3] = displ_y;
  T->data[2][3] = displ_z;
  T->data[3][3] = 1.0;
}

void get_displ_theta(Matrix *displ_all, Matrix *theta_all, const BVH_DATA *data, int start, int end, int index)
{
  zeros(displ_all, (end-start)*DIM, 1);
  zeros(theta_all, (end-start)*DIM, 1);
  int i;
  for (i = start; i < end; ++i)
  {
    int channels = data->hierachy.channels[i];
    int channel_start = data->hierachy.channel_start[i];
    if (channels != 0)
    {
      // channels = 3 or 6
      double *params;
      if (channels == 3)
      {
        params = get_motion_data(channel_start, channels, data->motion.frame_datas[index]);
        displ_all->data[i*3 + 0][0] = data->hierachy.offset_from_parent[i][0];
        displ_all->data[i*3 + 1][0] = data->hierachy.offset_from_parent[i][1];
        displ_all->data[i*3 + 2][0] = data->hierachy.offset_from_parent[i][2];
        theta_all->data[i*3 + 0][0] = params[0];
        theta_all->data[i*3 + 1][0] = params[1];
        theta_all->data[i*3 + 2][0] = params[2];
      } else
      {
        params = get_motion_data(channel_start, channels, data->motion.frame_datas[index]);
        displ_all->data[i*3 + 0][0] = params[0];
        displ_all->data[i*3 + 1][0] = params[1];
        displ_all->data[i*3 + 2][0] = params[2];
        theta_all->data[i*3 + 0][0] = params[3];
        theta_all->data[i*3 + 1][0] = params[4];
        theta_all->data[i*3 + 2][0] = params[5];
      }
    } else
    {
      // if channels = 0 , this is the end-site
      displ_all->data[i*3 + 0][0] = data->hierachy.offset_from_parent[i][0];
      displ_all->data[i*3 + 1][0] = data->hierachy.offset_from_parent[i][1];
      displ_all->data[i*3 + 2][0] = data->hierachy.offset_from_parent[i][2];
      theta_all->data[i*3 + 0][0] = 0.0;
      theta_all->data[i*3 + 1][0] = 0.0;
      theta_all->data[i*3 + 2][0] = 0.0;
    }
  }
}

void get_displ_theta(Matrix *displ_all, Matrix *theta_all, const BVH_DATA *data, int *nodes, int num_of_nodes, int index)
{
  int N = num_of_nodes;
  zeros(displ_all, N*DIM, 1);
  zeros(theta_all, N*DIM, 1);
  int i, n;
  for (n = 0; n < N; ++n)
  {
    i = nodes[n];
    int channels = data->hierachy.channels[i];
    int channel_start = data->hierachy.channel_start[i];
    if (channels != 0)
    {
      // channels = 3 or 6
      double *params;
      if (channels == 3)
      {
        params = get_motion_data(channel_start, channels, data->motion.frame_datas[index]);
        displ_all->data[i*3 + 0][0] = data->hierachy.offset_from_parent[i][0];
        displ_all->data[i*3 + 1][0] = data->hierachy.offset_from_parent[i][1];
        displ_all->data[i*3 + 2][0] = data->hierachy.offset_from_parent[i][2];
        theta_all->data[i*3 + 0][0] = params[0];
        theta_all->data[i*3 + 1][0] = params[1];
        theta_all->data[i*3 + 2][0] = params[2];
      } else
      {
        params = get_motion_data(channel_start, channels, data->motion.frame_datas[index]);
        displ_all->data[i*3 + 0][0] = params[0];
        displ_all->data[i*3 + 1][0] = params[1];
        displ_all->data[i*3 + 2][0] = params[2];
        theta_all->data[i*3 + 0][0] = params[3];
        theta_all->data[i*3 + 1][0] = params[4];
        theta_all->data[i*3 + 2][0] = params[5];
      }
    } else
    {
      // if channels = 0 , this is the end-site
      displ_all->data[i*3 + 0][0] = data->hierachy.offset_from_parent[i][0];
      displ_all->data[i*3 + 1][0] = data->hierachy.offset_from_parent[i][1];
      displ_all->data[i*3 + 2][0] = data->hierachy.offset_from_parent[i][2];
      theta_all->data[i*3 + 0][0] = 0.0;
      theta_all->data[i*3 + 1][0] = 0.0;
      theta_all->data[i*3 + 2][0] = 0.0;
    }
  }
}

void set_displ_theta(BVH_DATA *data, const Matrix *displ_all, const Matrix *theta_all, int start, int end, int index)
{
  int i;
  for (i = start; i < end; ++i)
  {
    int channels = data->hierachy.channels[i];
    int channel_start = data->hierachy.channel_start[i];
    if (channels != 0)
    {
      // channels = 3 or 6
      if (channels == 3)
      {
        data->motion.frame_datas[index][channel_start + 0] = theta_all->data[i * DIM + 0][0];
        data->motion.frame_datas[index][channel_start + 1] = theta_all->data[i * DIM + 1][0];
        data->motion.frame_datas[index][channel_start + 2] = theta_all->data[i * DIM + 2][0];
      } else
      {
        data->motion.frame_datas[index][channel_start + 3] = theta_all->data[i * DIM + 0][0];
        data->motion.frame_datas[index][channel_start + 4] = theta_all->data[i * DIM + 1][0];
        data->motion.frame_datas[index][channel_start + 5] = theta_all->data[i * DIM + 2][0];
      }
    } else
    {
      // if channels = 0 , this is the end-site
      // do nothiing
    }
  }
}

void set_displ_theta(BVH_DATA *data, const Matrix *displ_all, const Matrix *theta_all, int *nodes, int num_of_nodes, int index)
{
  int N = num_of_nodes;
  int i, n;
  for (n = 0; n < N; ++n)
  {
    i = nodes[n];
    int channels = data->hierachy.channels[i];
    int channel_start = data->hierachy.channel_start[i];
    if (channels != 0)
    {
      // channels = 3 or 6
      if (channels == 3)
      {
        data->motion.frame_datas[index][channel_start + 0] = theta_all->data[i * DIM + 0][0];
        data->motion.frame_datas[index][channel_start + 1] = theta_all->data[i * DIM + 1][0];
        data->motion.frame_datas[index][channel_start + 2] = theta_all->data[i * DIM + 2][0];
      } else
      {
        data->motion.frame_datas[index][channel_start + 3] = theta_all->data[i * DIM + 0][0];
        data->motion.frame_datas[index][channel_start + 4] = theta_all->data[i * DIM + 1][0];
        data->motion.frame_datas[index][channel_start + 5] = theta_all->data[i * DIM + 2][0];
      }
    } else
    {
      // if channels = 0 , this is the end-site
      // do nothiing
    }
  }
}

void forward_kenematics(Matrix *res, const Matrix *displ_all, const Matrix *theta_all, int start, int end)
{
  int i;
  Matrix T_final;
  eye(&T_final, 4, 4);
  for (i = start; i < end; ++i)
  {
    Matrix T;
    double displ[] = {
      displ_all->data[i*3 + 0][0],
      displ_all->data[i*3 + 1][0],
      displ_all->data[i*3 + 2][0]
    };
    double theta[] = {
      theta_all->data[i*3 + 0][0],
      theta_all->data[i*3 + 1][0],
      theta_all->data[i*3 + 2][0]
    };
    transform_matrix(&T, displ, theta);
    multiply(&T_final, &T_final, &T);
    /*--------------*/
    zeros(res, 3, 1);
    res->data[0][0] = T_final.data[0][3];
    res->data[1][0] = T_final.data[1][3];
    res->data[2][0] = T_final.data[2][3];
  }
}

void forward_kenematics(Matrix *res, const Matrix *displ_all, const Matrix *theta_all, int *nodes, int num_of_nodes)
{
  int N = num_of_nodes;
  int i, n;
  Matrix T_final;
  eye(&T_final, 4, 4);
  for (n = 0; n < N; ++n)
  {
    i = nodes[n];
    Matrix T;
    double displ[] = {
      displ_all->data[i*3 + 0][0],
      displ_all->data[i*3 + 1][0],
      displ_all->data[i*3 + 2][0]
    };
    double theta[] = {
      theta_all->data[i*3 + 0][0],
      theta_all->data[i*3 + 1][0],
      theta_all->data[i*3 + 2][0]
    };
    transform_matrix(&T, displ, theta);
    multiply(&T_final, &T_final, &T);
    /*--------------*/
    zeros(res, 3, 1);
    res->data[0][0] = T_final.data[0][3];
    res->data[1][0] = T_final.data[1][3];
    res->data[2][0] = T_final.data[2][3];
  }
}

void jacobian(Matrix *jacobian, const Matrix *displ_all, Matrix *theta_all, int start, int end)
{
  int M = (end - start) * DIM;
  zeros(jacobian, DIM, M);
  Matrix orig;
  forward_kenematics(&orig, displ_all, theta_all, start, end);
  int i;
  for (i = 0; i < M; ++i)
  {
    Matrix dest;
    theta_all->data[i][0] += EPS;
    forward_kenematics(&dest, displ_all, theta_all, start, end);
    theta_all->data[i][0] -= EPS;
    jacobian->data[0][i] = (dest.data[0][0] - orig.data[0][0]) / EPS;
    jacobian->data[1][i] = (dest.data[1][0] - orig.data[1][0]) / EPS;
    jacobian->data[2][i] = (dest.data[2][0] - orig.data[2][0]) / EPS;
  }
}

void jacobian(Matrix *jacobian, const Matrix *displ_all, Matrix *theta_all, int *nodes, int num_of_nodes)
{
  int N = num_of_nodes;
  int M = N * DIM;
  zeros(jacobian, DIM, M);
  Matrix orig;
  forward_kenematics(&orig, displ_all, theta_all, nodes, N);
  int i;
  for (i = 0; i < M; ++i)
  {
    Matrix dest;
    theta_all->data[i][0] += EPS;
    forward_kenematics(&dest, displ_all, theta_all, nodes, N);
    theta_all->data[i][0] -= EPS;
    jacobian->data[0][i] = (dest.data[0][0] - orig.data[0][0]) / EPS;
    jacobian->data[1][i] = (dest.data[1][0] - orig.data[1][0]) / EPS;
    jacobian->data[2][i] = (dest.data[2][0] - orig.data[2][0]) / EPS;
  }
}

void inverse_kinematics_per_frame(BVH_DATA *data, int start, int end, int index, double perturbation_degree)
{
  double beta = 0.5, lambda = 0.1;
  Matrix displ, theta, theta_dst, orig;
  get_displ_theta(&displ, &theta, data, start, end, index);
  copy_matrix(&theta_dst, &theta);
  for (int i = 0; i < theta.rows - 3; ++i)
  {
    for (int j = 0; j < theta.cols; ++j)
    {
      theta_dst.data[i][j] += perturbation_degree;
    }
  }
  forward_kenematics(&orig, &displ, &theta, start, end);
  Matrix regular;
  eye(&regular, DIM, DIM);
  multiply_scala(&regular, lambda, &regular);
  double p, threshold = 0.00001;
  for (int j = 0; j < 1000; ++j)
  {
    Matrix dest, diff_pos;
    forward_kenematics(&dest, &displ, &theta_dst, start, end);
    minus_matrix(&diff_pos, &orig, &dest);
    Matrix J, J_t;
    jacobian(&J, &displ, &theta_dst, start, end);
    J.cols -= 3;
    theta_dst.rows -= 3;
    transpose(&J_t, &J);
    Matrix grad;
    multiply(&grad, &J, &J_t);
    add_matrix(&grad, &grad, &regular);
    inverse(&grad, &grad);
    multiply(&grad, &J_t, &grad);
    multiply_scala(&grad, beta, &grad);
    multiply(&grad, &grad, &diff_pos);
    add_matrix(&theta_dst, &theta_dst, &grad);
    theta_dst.rows += 3;
    p = norm2_1d(&diff_pos);
    printf("%lf\n", p);
    if (p < threshold)
    {
      set_displ_theta(data, &displ, &theta_dst, start, end, index);
      break;
    }
  }
}

void inverse_kinematics_per_frame(BVH_DATA *data, int *nodes, int num_of_nodes, int index, double perturbation_degree)
{
  double beta = 0.5, lambda = 0.1;
  Matrix displ, theta, theta_dst, orig;
  get_displ_theta(&displ, &theta, data, nodes, num_of_nodes, index);
  copy_matrix(&theta_dst, &theta);
  for (int i = 0; i < theta.rows - 3; ++i)
  {
    for (int j = 0; j < theta.cols; ++j)
    {
      theta_dst.data[i][j] += perturbation_degree;
    }
  }
  forward_kenematics(&orig, &displ, &theta, nodes, num_of_nodes);
  Matrix regular;
  eye(&regular, DIM, DIM);
  multiply_scala(&regular, lambda, &regular);
  double p, threshold = 0.00001;
  for (int j = 0; j < 1000; ++j)
  {
    Matrix dest, diff_pos;
    forward_kenematics(&dest, &displ, &theta_dst, nodes, num_of_nodes);
    minus_matrix(&diff_pos, &orig, &dest);
    Matrix J, J_t;
    jacobian(&J, &displ, &theta_dst, nodes, num_of_nodes);
    J.cols -= 3;
    theta_dst.rows -= 3;
    transpose(&J_t, &J);
    Matrix grad;
    multiply(&grad, &J, &J_t);
    add_matrix(&grad, &grad, &regular);
    inverse(&grad, &grad);
    multiply(&grad, &J_t, &grad);
    multiply_scala(&grad, beta, &grad);
    multiply(&grad, &grad, &diff_pos);
    add_matrix(&theta_dst, &theta_dst, &grad);
    theta_dst.rows += 3;
    p = norm2_1d(&diff_pos);
    printf("%lf\n", p);
    if (p < threshold)
    {
      set_displ_theta(data, &displ, &theta_dst, nodes, num_of_nodes, index);
      break;
    }
  }
}

void inverse_kinematics(BVH_DATA *data, int start, int end, double perturbation_degree, int print_progress)
{
  int frames = data->motion.frames, ff;
  for (ff = 0; ff < frames; ++ff)
  {
    if (print_progress) printf("process frame %d / total frame is : %d\n", ff + 1, frames);
    inverse_kinematics_per_frame(data, start, end, ff, perturbation_degree);
  }
}

void inverse_kinematics(BVH_DATA *data, int *nodes, int num_of_nodes, double perturbation_degree, int print_progress)
{
  int N = num_of_nodes;
  if (data->hierachy.parent[nodes[0]] >= 0)
  {
    printf("the first node is not root\n");
    exit(-1);
  }
  if ( strcmp(data->hierachy.type[nodes[N-1]], ENDSITE) != 0)
  {
    printf("the last node is not end-site\n");
    exit(1);
  }
  int frames = data->motion.frames, ff;
  for (ff = 0; ff < frames; ++ff)
  {
    if (print_progress) printf("process frame %d / total frame is : %d\n", ff + 1, frames);
    inverse_kinematics_per_frame(data, nodes, N, ff, perturbation_degree);
  }
}
