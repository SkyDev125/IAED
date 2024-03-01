#include <stdio.h>

typedef struct {
  int x, y;
} point;

void point_print(point *new_point) {
  printf("%d", new_point->x);
  printf("%d", new_point->y);
  puts("");
}

int main() {
  point hey = {1, 2};
  point_print(&hey);
  return 0;
}