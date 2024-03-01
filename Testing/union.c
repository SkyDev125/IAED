typedef struct {
  int type;
  union {
    int i;
    double d;
    char ch;
  } data;
} mydata;

int main() { mydata test = {1, 32}; }