/**
 * @file ls3.c
 *
 * Copyright (c) 2015 大前良介 (OHMAE Ryosuke)
 *
 * This software is released under the MIT License.
 * http://opensource.org/licenses/MIT
 *
 * @brief lsコマンド風のプログラムを作るNo.3
 * リストするファイルのフィルタリングを行う
 *
 * @author <a href="mailto:ryo@mm2d.net">大前良介 (OHMAE Ryosuke)</a>
 * @date 2015/11/11
 */
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <getopt.h>

/**
 * 隠しファイルの表示方針
 */
enum {
  FILTER_DEFAULT, /**< '.'から始まるもの以外を表示する */
  FILTER_ALMOST,  /**< '.'と'..'以外を表示する */
  FILTER_ALL,     /**< すべて表示する */
};

static char *parse_cmd_args(int argc, char**argv);
static void list_dir(const char *base_path);

/**
 * 隠しファイルの表示方針
 */
static int filter = FILTER_DEFAULT;

/**
 * @brief コマンドライン引数をパースする
 * @param[IN] argc 引数の数
 * @param[IN/OUT] argv 引数配列
 * @return パス
 */
static char *parse_cmd_args(int argc, char**argv) {
  char *path = "./";
  int opt;
  const struct option longopts[] = {
      { "all", no_argument, NULL, 'a' },
      { "almost-all", no_argument, NULL, 'A' },
  };
  while ((opt = getopt_long(argc, argv, "aA", longopts, NULL)) != -1) {
    switch (opt) {
      case 'a':
        filter = FILTER_ALL;
        break;
      case 'A':
        filter = FILTER_ALMOST;
        break;
      default:
        return NULL;
    }
  }
  if (argc > optind) {
    path = argv[optind];
  }
  return path;
}

static void list_dir(const char *base_path) {
  DIR *dir;
  struct dirent *dent;
  dir = opendir(base_path);
  if (dir == NULL) {
    perror(base_path);
    return;
  }
  while ((dent = readdir(dir)) != NULL) {
    const char *name = dent->d_name;
    if (filter != FILTER_ALL
        && name[0] == '.'
        && (filter == FILTER_DEFAULT
            || name[1 + (name[1] == '.')] == '\0')) {
      continue;
    }
    printf("%s\n", name);
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
