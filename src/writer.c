#include "writer.h"

void write_left_brace(FILE *file, BVH_DATA *data)
{

}

void write_right_brace(FILE *file, BVH_DATA *data)
{

}

void write_offset(FILE *file, BVH_DATA *data)
{

}

void write_channels(FILE *file, BVH_DATA *data)
{

}

void write_node(FILE *file, BVH_DATA *data)
{

}

void write_endsite(FILE *file, BVH_DATA *data)
{

}

char *current_indent(int depth)
{
  char *indent = (char *) malloc(sizeof(char) * MAX_BUFFER);
  int i;
  for (i = 0; i < depth; ++i)
  {
    strcat(indent, "\t\t");
  }
  return indent;
}

void write_bvh(const char *filename, BVH_DATA *data)
{
  FILE *file = fopen(filename, "w");
  if (file == NULL)
  {
    printf("can't not openning file\n");
    exit(1);
  }
  /* write hierachy-part to file */
  fprintf(file, "%s\n", HIERARCHY);
  int current_occupy = data->hierachy.current_occupy;
  int i;
  for (i = 0; i < current_occupy; ++i)
  {
    int depth = data->hierachy.depth[i];
    if ( strcmp(data->hierachy.type[i], ENDSITE) == 0) {
      fprintf(file, "%s%s\n", current_indent(depth), ENDSITE);
      fprintf(file, "%s%s\n", current_indent(depth), LEFT_BRACE);
      fprintf(
        file, "%sOFFSET %lf %lf %lf\n",
        current_indent(depth+1),
        data->hierachy.offset_from_parent[i][0],
        data->hierachy.offset_from_parent[i][1],
        data->hierachy.offset_from_parent[i][2]
      );
      /*-------*/
      int N;
      if (i == current_occupy - 1) N = depth + 1;
      else N = data->hierachy.depth[i] - data->hierachy.depth[i+1] + 1;
      int j;
      for (j = 0; j < N; ++j)
      {
        int D = depth - j;
        fprintf(file, "%s%s\n",current_indent(D), RIGHT_BRACE);
      }
    } else
    {
      fprintf(file, "%s%s %s\n", current_indent(depth), data->hierachy.type[i], data->hierachy.name[i]);
      fprintf(file, "%s%s\n", current_indent(depth), LEFT_BRACE);
      fprintf(
        file, "%sOFFSET %lf %lf %lf\n",
        current_indent(depth+1),
        data->hierachy.offset_from_parent[i][0],
        data->hierachy.offset_from_parent[i][1],
        data->hierachy.offset_from_parent[i][2]
      );
      if (data->hierachy.channels[i] == 3)
      {
        fprintf(file, "%sCHANNELS %d Zrotation Xrotation Yrotation\n", current_indent(depth+1), 3);
      } else if (data->hierachy.channels[i] == 6) {
        fprintf(file, "%sCHANNELS %d Xposition Yposition Zposition Zrotation Xrotation Yrotation\n", current_indent(depth+1), 6);
      }
    }
  }
  /* write motion-part to file */
  fprintf(file, "%s\n", MOTION);
  fprintf(file, "Frames:    %d\n", data->motion.frames);
  fprintf(file, "Frame Time:    %lf\n", data->motion.frame_time);
  int frames = data->motion.frames, f;
  int channels = data->motion.channels, c;
  for (f = 0; f < frames; ++f)
  {
    char frame_data[MAX_SIZE_PER_LINE] = "";
    for (c = 0; c < channels; ++c)
    {
      char col_data[MAX_SIZE_PER_LINE];
      sprintf(col_data, "%lf", data->motion.frame_datas[f][c]);
      strcat(col_data, BLANK);
      strcat(frame_data, col_data);
    }
    fprintf(file, "%s\n", frame_data);
  }
  /* close the file */
  if (file != NULL) fclose(file);
}

void test_write_bvh(void)
{
  const char *filename = "test.bvh";
  BVH_DATA *data = init_bvh_data();
  write_bvh(filename, data);
}

/*
int main(int argc, char *argv[])
{
  printf("start testing writing bvh file\n");
  test_write_bvh();
  printf("%s hehehe\n", current_indent(2));
  printf("end testing writing bvh file\n");
  return 0;
}
*/
