#include "utils.h"
#include "bvh_data.h"
#include "parser.h"
#include "writer.h"
#include "core.h"


void test_specify_nodes()
{
  const char *filename = "../data/input/yanyuan_Tpose.bvh";
  BVH_DATA *data = init_bvh_data();
  parse_bvh(filename, data);
  print_bvh_info(data);
  //int nodes[] = {0, 1, 2, 3, 4, 5};
  //int nodes[] = {0, 1, 2, 6, 7, 8, 9, 10};
  int nodes[] = {0, 1, 2, 11,12, 13, 14, 15};
  //int nodes[] = {0, 16, 17, 18, 19, 20};
  //int nodes[] = {0, 21, 22, 23, 24, 25};
  int print_progress = TRUE;
  inverse_kinematics(data, nodes, 8, 10.0, print_progress);
  const char *out_filename = "../data/output/yanyuan_dongzuo_ik_10.bvh";
  write_bvh(out_filename, data);
  printf("end processing data\n");
}

void test_get_nodes_from_data()
{
  const char *filename = "../data/input/yanyuan_dongzuo.bvh";
  BVH_DATA *data = init_bvh_data();
  parse_bvh(filename, data);

  int part_index = 0;
  int num_of_skeleton_nodes = data->skeleton_part.num_of_skeleton_nodes[part_index], n;
  int nodes[num_of_skeleton_nodes];
  for (n = 0; n < num_of_skeleton_nodes; ++n)
  {
    nodes[n] = data->skeleton_part.skeleton_nodes[part_index][n];
  }
  int print_progress = TRUE;
  inverse_kinematics(data, nodes, num_of_skeleton_nodes, 10, print_progress);
  const char *out_filename = "../data/output/yanyuan_dongzuo_ik_10.bvh";
  write_bvh(out_filename, data);
  printf("end processing data\n");
}


int main(int argc, char *argv[])
{
  //test_get_nodes_from_data();
  test_specify_nodes();
  return 0;
}
