/**
 * @file ls1.c
 *
 * Copyright (c) 2015 大前良介 (OHMAE Ryosuke)
 *
 * This software is released under the MIT License.
 * http://opensource.org/licenses/MIT
 *
 * @brief lsコマンド風のプログラムを作るNo.1
 * ディレクトリストリームを読む
 *
 * @author <a href="mailto:ryo@mm2d.net">大前良介 (OHMAE Ryosuke)</a>
 * @date 2015/11/11
 */
#include <stdio.h>
#include <dirent.h>

int main(int argc, char**argv) {
  char *path = "./";
  DIR *dir;
  struct dirent *dent;
  if (argc > 1) {
    path = argv[1];
  }
  dir = opendir(path);
  if (dir == NULL) {
    perror(path);
    return 1;
  }
  while ((dent = readdir(dir)) != NULL) {
    printf("%s\n", dent->d_name);
  }
  closedir(dir);
  return 0;
}
