#ifndef _COREH_
#define _COREH_

#include <stdio.h>
#include <math.h>
#include "bvh_data.h"
#include "parser.h"
#include "utils.h"

#define X_Order 1
#define Y_Order 2
#define Z_Order 3
#define PI 3.141592653
#define EPS 0.1
#define DIM 3


#if TRUE

void get_displ_theta(Matrix *displ_all, Matrix *theta_all, const BVH_DATA *data, int *nodes, int num_of_nodes, int index);

void set_displ_theta(BVH_DATA *data, const Matrix *displ_all, const Matrix *theta_all, int *nodes, int num_of_nodes, int index);

void forward_kenematics(Matrix *res, const Matrix *displ_all, const Matrix *theta_all, int *nodes, int num_of_nodes);

void jacobian(Matrix *jacobian, const Matrix *displ_all, Matrix *theta_all, int *nodes, int num_of_nodes);

void inverse_kinematics_per_frame(BVH_DATA *data, int *nodes, int num_of_nodes, int index, double perturbation_degree);

void inverse_kinematics(BVH_DATA *data, int *nodes, int num_of_nodes, double perturbation_degree, int print_progress);

#else

void get_displ_theta(Matrix *displ_all, Matrix *theta_all, const BVH_DATA *data, int start, int end, int index);

void set_displ_theta(BVH_DATA *data, const Matrix *displ_all, const Matrix *theta_all, int start, int end, int index);

void forward_kenematics(Matrix *res, const Matrix *displ_all, const Matrix *theta_all, int start, int end);

void jacobian(Matrix *jacobian, const Matrix *displ_all, Matrix *theta_all, int start, int end);

void inverse_kinematics_per_frame(BVH_DATA *data, int start, int end, int index, double perturbation_degree);

void inverse_kinematics(BVH_DATA *data, int start, int end, double perturbation_degree, int print_progress);

#endif

void transform_matrix(Matrix *T, const double *params, int channels);

void fill_rotation(Matrix *T, double theta, int order);

#endif
