/**
 * @file ls2.c
 *
 * Copyright (c) 2015 大前良介 (OHMAE Ryosuke)
 *
 * This software is released under the MIT License.
 * http://opensource.org/licenses/MIT
 *
 * @brief lsコマンド風のプログラムを作るNo.2
 * 関数化
 *
 * @author <a href="mailto:ryo@mm2d.net">大前良介 (OHMAE Ryosuke)</a>
 * @date 2015/11/11
 */
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

static char *parse_cmd_args(int argc, char**argv);
static void list_dir(const char *base_path);

/**
 * @brief コマンドライン引数をパースする
 * @param[IN] argc 引数の数
 * @param[IN/OUT] argv 引数配列
 * @return パス
 */
static char *parse_cmd_args(int argc, char**argv) {
  char *path = "./";
  if (argc > 1) {
    path = argv[1];
  }
  return path;
}

/**
 * @brief 指定パスのディレクトリエントリをリストする
 * @param[IN] base_path パス
 */
static void list_dir(const char *base_path) {
  DIR *dir;
  struct dirent *dent;
  dir = opendir(base_path);
  if (dir == NULL) {
    perror(base_path);
    return;
  }
  while ((dent = readdir(dir)) != NULL) {
    printf("%s\n", dent->d_name);
  }
  closedir(dir);
}

int main(int argc, char**argv) {
  char *path = parse_cmd_args(argc, argv);
  if (path == NULL) {
    return EXIT_FAILURE;
  }
  list_dir(path);
  return EXIT_SUCCESS;
}
