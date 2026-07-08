#include "hashmap.h"
#include <stdio.h>
#include <unistd.h>

#define CLSC "\033[2J\033[H"

int power(int a, int b) {
  int r = 1;
  for (int i = 0; i < b; i++) {
    r *= a;
  }
  return r;
}

int _stoi(char *chr) {
  int i = 0;
  int digits = -1;
  int di = 0;
  char cur = chr[i];
  int r = 0;
  while (chr[di] <= '9' && chr[di] >= '0') {
    digits++;
    di++;
  }
  while (cur >= '0' && cur <= '9') {
    r += power(10, digits - i) * (cur - '0');
    i++;
    cur = chr[i];
  }
  return r;
}

int main() {
  Hashmap *map = init_hashmap(hashstr, cmpstr);
  while (1) {
    char key[STR_SIZE];
    int size = 1;
    int idx = 0;
    int *val = (int *)malloc(sizeof(int));
    printf("%s 키를 입력하세요 (종료시 q) > ", CLSC);
    fgets(key, STR_SIZE, stdin);
    if (key[0] == 'q' && key[1] == '\n') {
      free(val);
      break;
    }
    int *getValue = map->get(map, (unsigned char *)key);
    if (getValue == NULL) {
      while (1) {
        char vc[STR_SIZE];
        int v = 0;
        printf("%s 값을 입력하세요 (값 입력 종료시 q) > ", CLSC);
        fgets(vc, STR_SIZE, stdin);
        v = _stoi(vc);
        if (vc[0] == 'q' && vc[1] == '\n') {
          break;
        }
        if (idx >= size - 1) {
          size *= 2;
          val = (int *)realloc(val, size * sizeof(int));
          for (int i = idx; i < size; i++) {
            val[i] = -1;
          }
        }
        val[idx] = v;
        idx++;
      }
      val[idx] = -1;
      map->put(map, (unsigned char *)key, val);
    } else {
      free(val);
      int i = 0;
      printf("이미 있는데요? 뭔지 보여드림;;\n");
      while (getValue[i] != -1) {
        printf("%d,", getValue[i]);
        i++;
      }
      printf("\n");
      usleep(1000 * 1000);

      char deleteCheck[STR_SIZE];
      printf("%s 혹시 삭제 해드릴까? (y/n) > ", CLSC);
      fgets(deleteCheck, STR_SIZE, stdin);
      if (deleteCheck[0] == 'y' && deleteCheck[1] == '\n') {
        map->remove(map, (unsigned char *)key);
      }
    }
  }

  return 0;
}
