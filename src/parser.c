
#include "parser.h"

void parse_left_brace(const char *line, BVH_DATA *data)
{
  data->type_count += 1;
}

void parse_right_brace(const char *line, BVH_DATA *data)
{
  data->type_count -= 1;
}

void parse_offset(const char *line, BVH_DATA *data)
{
  int index = data->hierachy.current_occupy - 1;
  char s[MAX_BUFFER];
  sscanf(
    line, "%s %lf %lf %lf",
    s,
    &data->hierachy.offset_from_parent[index][0],
    &data->hierachy.offset_from_parent[index][1],
    &data->hierachy.offset_from_parent[index][2]
  );

  if (strcmp(data->hierachy.type[index], ROOT) == 0)
  {
    data->hierachy.coordinate[index][0] = data->hierachy.offset_from_parent[index][0];
    data->hierachy.coordinate[index][1] = data->hierachy.offset_from_parent[index][1];
    data->hierachy.coordinate[index][2] = data->hierachy.offset_from_parent[index][2];
  } else
  {
    int parent_index = data->hierachy.parent[index];
    data->hierachy.coordinate[index][0] = data->hierachy.coordinate[parent_index][0] +
                                          data->hierachy.offset_from_parent[index][0];
    data->hierachy.coordinate[index][1] = data->hierachy.coordinate[parent_index][1] +
                                          data->hierachy.offset_from_parent[index][1];
    data->hierachy.coordinate[index][2] = data->hierachy.coordinate[parent_index][2] +
                                          data->hierachy.offset_from_parent[index][2];
  }
}

void parse_channels(const char *line, BVH_DATA *data)
{
  int index = data->hierachy.current_occupy - 1;
  if (is_3_channels(line))
  {
    data->hierachy.channels[index] = 3;
    data->hierachy.channel_start[index] = data->motion.channels;
    data->motion.channels += 3;
  }
  else if (is_6_channels(line))
  {
    data->hierachy.channels[index] = 6;
    data->hierachy.channel_start[index] = data->motion.channels;
    data->motion.channels += 6;
  }
  else
  {
    exit(1);
  }
}

void parse_node(const char *line, BVH_DATA *data)
{
  int index = data->hierachy.current_occupy;
  char type[MAX_BUFFER];
  char name[MAX_BUFFER];
  sscanf(line, "%s %s", type, name);
  strcpy(data->hierachy.type[index], type);
  strcpy(data->hierachy.name[index], name);
  data->hierachy.depth[index] = data->type_count;
  data->hierachy.current_occupy += 1;
  data->hierachy.parent[index] = data->type_count - 1;
  if (index > 0)
  {
    if ( strcmp(data->hierachy.type[index - 1], ENDSITE) == 0 )
    data->hierachy.parent[index] = data->type_count - 1;
    else
    data->hierachy.parent[index] = index - 1;
  } else
  {
    data->hierachy.parent[index] = -1;
  }
}

void parse_endsite(const char *line, BVH_DATA *data)
{
  int index = data->hierachy.current_occupy;
  strcpy(data->hierachy.type[index], ENDSITE);
  char s[MAX_BUFFER];
  sscanf(
    line, "%s %lf %lf %lf",
    s,
    &data->hierachy.offset_from_parent[index][0],
    &data->hierachy.offset_from_parent[index][1],
    &data->hierachy.offset_from_parent[index][2]
  );
  data->hierachy.parent[index] = index - 1;
  data->hierachy.depth[index] = data->type_count;
  data->hierachy.channels[index] = 0;
  data->hierachy.current_occupy += 1;
}

void parse_skeleton_parts(BVH_DATA *data)
{
  int current_occupy = data->hierachy.current_occupy, i, p;
  int current_part = 0;
  data->skeleton_part.parts = 0;
  int start[MAX_CAPACITY], end[MAX_CAPACITY];
  for (i  = 0; i < current_occupy; ++i)
  {
    if (i == 0)
    {
      data->skeleton_part.parts += 1;
      start[current_part] = i;
    }
    if (strcmp(data->hierachy.type[i], ENDSITE) == 0)
    {
      end[current_part] = i;
      if (i != current_occupy - 1)
      {
        current_part += 1;
        data->skeleton_part.parts +=1;
        start[current_part] = i+1;
      }
    }
  }
  for (p = 0; p < data->skeleton_part.parts; ++p)
  {
    int s = start[p];
    int e = end[p];
    int N = e - s + 1, n;
    int D = data->hierachy.depth[s], d;
    if (D > 0)
    {
      int parents[D];
      for (d = 0; d < D; ++d)
      {
        if (d == 0)
        {
          parents[D-d-1] = data->hierachy.parent[s];
        } else
        {
          parents[D-d-1] = data->hierachy.parent[parents[d+1]];
        }
      }
      data->skeleton_part.num_of_skeleton_nodes[p] = N + D;
      for (n = 0; n < D; ++n)
      {
        data->skeleton_part.skeleton_nodes[p][n] = parents[n];
      }
      for (n = 0; n < N; ++n)
      {
        data->skeleton_part.skeleton_nodes[p][n+D] = s + n;
      }
    } else
    {
      for (n = 0; n < N; ++n)
      {
        data->skeleton_part.num_of_skeleton_nodes[p] = N;
        data->skeleton_part.skeleton_nodes[p][n] = s + n;
      }
    }
  }
}

void parse_bvh(const char *filename, BVH_DATA *data)
{
  if (data == NULL) data = init_bvh_data();
  FILE *file = fopen(filename, "r");
  char buffer[MAX_SIZE_PER_LINE];
  /* parse hierachy */
  while (fgets(buffer, sizeof(buffer), file))
  {
    char *line = trim(buffer);
    if (is_left_brace(line))
    {
      parse_left_brace(line, data);
    } else if (is_right_brace(line))
    {
      parse_right_brace(line, data);
    } else if (is_offset(line))
    {
      parse_offset(line, data);
    } else if (is_channels(line))
    {
      parse_channels(line, data);
    } else if (is_node(line))
    {
      parse_node(line, data);
    } else if (is_endsite(line))
    {
      fgets(buffer, sizeof(buffer), file);
      fgets(buffer, sizeof(buffer), file);
      char *line = trim(buffer);
      parse_endsite(line, data);
      fgets(buffer, sizeof(buffer), file);
    } else if (is_motion(line))
    {
      break;
    }
  }
  /* parse motion info */
  fgets(buffer, sizeof(buffer), file);
  char *line = trim(buffer);
  char s[MAX_BUFFER];
  sscanf(
    line, "%s\t%d",
    s, &data->motion.frames
  );
  fgets(buffer, sizeof(buffer), file);
  line = trim(buffer);
  sscanf(
    line, "%s %s %lf",
    s, s, &data->motion.frame_time
  );
  data->motion.frame_datas = init_double_2d(data->motion.frames, data->motion.channels);
  /* parse motion params*/
  int rows = data->motion.frames;
  int cols = data->motion.channels;
  int i = 0, j = 0;
  for (i = 0; i < rows; ++i)
  {
    fgets(buffer, sizeof(buffer), file);
    char *line = trim(buffer);
    char *token;
    token = strtok(line, BLANK);
    for (j = 0; j < cols; ++j)
    {
      data->motion.frame_datas[i][j] = atof(token);
      token = strtok(NULL, BLANK);
    }
  }
  parse_skeleton_parts(data);
  /* close file */
  if (file != NULL)
      fclose(file);
}

void test_channels(BVH_DATA *data)
{
  int N = data->hierachy.current_occupy;
  int i;
  for (i = 0; i < N; ++i)
  {
    printf("%d\n", data->hierachy.channels[i]);
  }
}

void test_node(BVH_DATA *data)
{
  int N = data->hierachy.current_occupy;
  int i;
  for (i = 0; i < N; ++i)
  {
    printf("%s and parent : %d and depth: %d\n", data->hierachy.type[i], data->hierachy.parent[i], data->hierachy.depth[i]);
  }
}

void test_offset(BVH_DATA *data)
{
  int N = data->hierachy.current_occupy;
  int i;
  for (i = 0; i < N; ++i)
  {
    printf("%lf %lf %lf\n", data->hierachy.offset_from_parent[i][0], data->hierachy.offset_from_parent[i][1], data->hierachy.offset_from_parent[i][2]);
  }
}

void test_motion_param(BVH_DATA *data)
{
  int rows = data->motion.frames;
  int cols = data->motion.channels;
  int i, j;
  for (i = 0; i < rows; ++i)
  {
    for (j = 0; j < cols; ++j)
    {
      printf("%lf\n", data->motion.frame_datas[i][j]);
    }
  }
}

/*
int main(int argc, char *argv[])
{
  const char *filename = "../data/yanyuan_Tpose.bvh";
  BVH_DATA *data = init_bvh_data();//(BVH_DATA *)malloc(sizeof(BVH_DATA));
  parse_bvh(filename, data);
  test_channels(data);
  test_node(data);
  test_offset(data);
  test_motion_param(data);
  free_bvh_data(data);
  return 0;
}
*/
