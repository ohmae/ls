/**
 * @file ls5.c
 *
 * Copyright (c) 2015 大前良介 (OHMAE Ryosuke)
 *
 * This software is released under the MIT License.
 * http://opensource.org/licenses/MIT
 *
 * @brief lsコマンド風のプログラムを作るNo.5
 * ロングフォーマット出力：モード文字列を表示
 *
 * @author <a href="mailto:ryo@mm2d.net">大前良介 (OHMAE Ryosuke)</a>
 * @date 2015/11/23
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <dirent.h>
#include <getopt.h>
#include <sys/stat.h>

#define PATH_MAX 4096

#ifndef S_IXUGO
#define S_IXUGO (S_IXUSR | S_IXGRP | S_IXOTH)
#endif

/**
 * 隠しファイルの表示方針
 */
enum {
  FILTER_DEFAULT, /**< '.'から始まるもの以外を表示する */
  FILTER_ALMOST,  /**< '.'と'..'以外を表示する */
  FILTER_ALL,     /**< すべて表示する */
};

static char *parse_cmd_args(int argc, char**argv);
static void get_mode_string(mode_t mode, char *str);
static void print_type_indicator(mode_t mode);
static void list_dir(const char *base_path);

/**
 * 隠しファイルの表示方針
 */
static int filter = FILTER_DEFAULT;
/**
 * 属性を示す文字を表示する
 */
static bool classify = false;
/**
 * ロングフォーマットで表示する
 */
static bool long_format = false;

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
      { "classify", no_argument, NULL, 'F' },
      { "long-format", no_argument, NULL, 'l' },
  };
  while ((opt = getopt_long(argc, argv, "aAFl", longopts, NULL)) != -1) {
    switch (opt) {
      case 'a':
        filter = FILTER_ALL;
        break;
      case 'A':
        filter = FILTER_ALMOST;
        break;
      case 'F':
        classify = true;
        break;
      case 'l':
        long_format = true;
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

/**
 * @brief モード文字列を作成する
 * @param[IN]  mode モードパラメータ
 * @param[OUT] str  文字列の出力先、11バイト以上のバッファを指定
 */
static void get_mode_string(mode_t mode, char *str) {
  str[0] = (S_ISBLK(mode))  ? 'b' :
           (S_ISCHR(mode))  ? 'c' :
           (S_ISDIR(mode))  ? 'd' :
           (S_ISREG(mode))  ? '-' :
           (S_ISFIFO(mode)) ? 'p' :
           (S_ISLNK(mode))  ? 'l' :
           (S_ISSOCK(mode)) ? 's' : '?';
  str[1] = mode & S_IRUSR ? 'r' : '-';
  str[2] = mode & S_IWUSR ? 'w' : '-';
  str[3] = mode & S_ISUID ? (mode & S_IXUSR ? 's' : 'S') : (mode & S_IXUSR ? 'x' : '-');
  str[4] = mode & S_IRGRP ? 'r' : '-';
  str[5] = mode & S_IWGRP ? 'w' : '-';
  str[6] = mode & S_ISGID ? (mode & S_IXGRP ? 's' : 'S') : (mode & S_IXGRP ? 'x' : '-');
  str[7] = mode & S_IROTH ? 'r' : '-';
  str[8] = mode & S_IWOTH ? 'w' : '-';
  str[9] = mode & S_ISVTX ? (mode & S_IXOTH ? 't' : 'T') : (mode & S_IXOTH ? 'x' : '-');
  str[10] = '\0';
}

/**
 * @brief ファイルタイプ別のインジケータを出力する
 * @param[IN] mode モードパラメータ
 */
static void print_type_indicator(mode_t mode) {
  if (S_ISREG(mode)) {
    if (mode & S_IXUGO) {
      putchar('*');
    }
  } else {
    if (S_ISDIR(mode)) {
      putchar('/');
    } else if (S_ISLNK(mode)) {
      putchar('@');
    } else if (S_ISFIFO(mode)) {
      putchar('|');
    } else if (S_ISSOCK(mode)) {
      putchar('=');
    }
  }
}

/**
 * @brief 指定パスのディレクトリエントリをリストする
 * @param[IN] base_path パス
 */
static void list_dir(const char *base_path) {
  DIR *dir;
  struct dirent *dent;
  char path[PATH_MAX + 1];
  size_t path_len;
  dir = opendir(base_path);
  if (dir == NULL) {
    perror(base_path);
    return;
  }
  path_len = strlen(base_path);
  if (path_len >= PATH_MAX - 1) {
    fprintf(stderr, "too long path\n");
    return;
  }
  strncpy(path, base_path, PATH_MAX);
  if (path[path_len - 1] != '/') {
    path[path_len] = '/';
    path_len++;
    path[path_len] = '\0';
  }
  while ((dent = readdir(dir)) != NULL) {
    struct stat dent_stat;
    const char *name = dent->d_name;
    if (filter != FILTER_ALL
        && name[0] == '.'
        && (filter == FILTER_DEFAULT
            || name[1 + (name[1] == '.')] == '\0')) {
      continue;
    }
    strncpy(&path[path_len], dent->d_name, PATH_MAX - path_len);
    if (lstat(path, &dent_stat) != 0) {
      perror(path);
      continue;
    }
    if (long_format) {
      char mode_str[11];
      get_mode_string(dent_stat.st_mode, mode_str);
      printf("%s ", mode_str);
    }
    printf("%s", name);
    if (classify) {
      print_type_indicator(dent_stat.st_mode);
    }
    putchar('\n');
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
