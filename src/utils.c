#include "utils.h"

char *trim(char *str)
{
  char *end;
  // Trim leading space
  while(isspace(*str)) str++;
  if(*str == 0)  // All spaces?
    return str;
  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace(*end)) end--;
  // Write new null terminator
  *(end+1) = 0;
  return str;
}

int is_left_brace(const char *cs)
{
  if (strstr(cs, LEFT_BRACE) != NULL) return TRUE;
  else return FALSE;
}

int is_right_brace(const char *cs)
{
  if (strstr(cs, RIGHT_BRACE) != NULL) return TRUE;
  else return FALSE;
}

int is_motion(const char *cs)
{
  if (strstr(cs, MOTION) != NULL ) return TRUE;
  else return FALSE;
}

int is_channels(const char *cs)
{
  if (strstr(cs, CHANNELS) != NULL) return TRUE;
  else return FALSE;
}

int is_offset(const char *cs)
{
  if (strstr(cs, OFFSET) != NULL) return TRUE;
  else return FALSE;
}

int is_node(const char *cs)
{
  if (strstr(cs, JOINT) != NULL || strstr(cs, ROOT) != NULL)
  return TRUE;
  else
  return FALSE;
}

int is_endsite(const char *cs)
{
  if (strstr(cs, ENDSITE) != NULL) return TRUE;
  else return FALSE;
}

int is_zrotation(const char *cs)
{
  if (strstr(cs, Zrotation) != NULL) return TRUE;
  else return FALSE;
}

int is_xrotation(const char *cs)
{
  if (strstr(cs, Xrotation) != NULL) return TRUE;
  else return FALSE;
}

int is_yrotation(const char *cs)
{
  if (strstr(cs, Yrotation) != NULL) return TRUE;
  else return FALSE;
}

int is_3_channels(const char *cs)
{
  if (strstr(cs, CHANNELS) != NULL && strstr(cs, "3") != NULL)
      return TRUE;
  else
      return FALSE;
}

int is_6_channels(const char *cs)
{
  if (strstr(cs, CHANNELS) != NULL && strstr(cs, "6") != NULL)
      return TRUE;
  else
      return FALSE;
}

void print_separate_line(void)
{
  printf("\n");
  int i;
  for (i = 0; i < 10; ++i)
      printf(" - ");
  printf("\n");
}

void test_is(void)
{
  print_separate_line();
  if (is_left_brace("{"))
      printf("is left brace\n");
  if (! is_left_brace("}"))
      printf("is not left brace\n");
  print_separate_line();
  if (is_right_brace("}"))
      printf("is right brace\n");
  if (! is_right_brace("asdf"))
      printf("is not right brace");
  print_separate_line();
  if (is_motion("MOTION"))
      printf("is motion\n");
  if (! is_motion("asdfasf"))
      printf("is not motion");
  print_separate_line();
  if (is_channels("CHANNELS"))
      printf("is channels\n");
  if (! is_channels("sdafasdfa"))
      printf("is not channels\n");
  print_separate_line();
  if (is_offset("OFFSET"))
      printf("is offset\n");
  if (! is_offset("asdfasfd"))
      printf("is not offset\n");
  print_separate_line();
  if (is_node("JOINT") && is_node("ROOT"))
      printf("is node\n");
  if (! is_node("asdfasd"))
      printf("is not node\n");
  print_separate_line();
  if (is_endsite(ENDSITE))
      printf("is end site\n");
  if (! is_endsite("asdfasdf"))
      printf("is not end site\n");
}
