/*********************************************************************************************************
**
**                                    中国软件开源组织
**
**                                   嵌入式实时操作系统
**
**                                       SylixOS(TM)
**
**                               Copyright  All Rights Reserved
**
**--------------文件信息--------------------------------------------------------------------------------
**
** 文   件   名: run_test_suite.c
**
** 创   建   人: Lu.Zhenping (卢振平)
**
** 文件创建日期: 2016 年 04 月 14 日
**
** 描        述: 运行测试用例的主函数.
*********************************************************************************************************/
#ifdef SYLIXOS
#include <gjbext.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <dirent.h>
#include <sys/wait.h>
#include <spawn.h>
#include <mman.h>
#include <semaphore.h>
/*********************************************************************************************************
  全局变量
*********************************************************************************************************/
static  int     allcase         = 0;
static  char   *dir             = NULL;
static  char   *file            = NULL;
static  int     err_count       = 0;
static  int     count           = 0;
/*********************************************************************************************************
  测试黑名单
*********************************************************************************************************/
static  char   *blacklist[] = {
        ".", "..", "run_test_suite", "target-6"
};
/*********************************************************************************************************
** 函数名称: run_one_case
** 功能描述: 运行一个测试 CASE
** 输　入  : NONE
** 返回  值: NONE
** 全局变量:
** 调用模块:
*********************************************************************************************************/
void  run_one_case (char *filename)
{
    int         result = 0;
    pid_t       pid;
    char       *argv[4] = {NULL};
    FILE       *fp;

    if (file) {
        fp = fopen(file, "a+");
    } else {
        fp = stdout;
    }

    fprintf(fp, "exec %s\n", filename);
    fflush(fp);

    if (strstr(filename, ".ko")) {
        /*
         *  如果是内核模块, 我们将直接返回, 因为已经做其他处理
         */
        fclose(fp);

        return;
    } else {
        if (file) {
            fclose(fp);
        }

        count++;

        argv[0] = filename;
        argv[1] = file;
        argv[2] = NULL;

        posix_spawnp(&pid, argv[0], NULL, NULL, argv, NULL);

        waitpid(pid, &result, 0);
    }

    if (file) {
        fp = fopen(file, "a+");
    } else {
        fp = stdout;
    }

    switch (result) {

    case 0:
        if (fp == stdout) {
            fprintf(fp, "PASS\r\n");
        } else {
            fprintf(fp, "PASS\r\n");
        }
        break;

    default:
        err_count++;
        if (fp == stdout) {
            fprintf(fp, "FAILED\r\n");
        } else {
            fprintf(fp, "FAILED\r\n");
        }

        break;

    }

    if (file) {
        fclose(fp);
    }
}
/*********************************************************************************************************
** 函数名称: run_dir_case
** 功能描述: 运行指定目录的测试 CASE
** 输　入  : NONE
** 返回  值: NONE
** 全局变量:
** 调用模块:
*********************************************************************************************************/

#define ARRAY_SIZE(array)       (sizeof(array) / sizeof(array[0]))

void  run_dir_case (char  *dirname)
{
    DIR            *dir;
    struct dirent   ent;
    struct dirent  *res;
    int             ret;
    char            test_path[PATH_MAX];
    struct stat     st;
    int             i;
    int             find = 0;

    dir = opendir(dirname);

    for (;;) {
        ret = readdir_r(dir, &ent, &res);
        if (ret < 0 || res == NULL) {
            break;
        }

        for (i = 0; i < ARRAY_SIZE(blacklist); ++i) {
            if (!strcmp(res->d_name, blacklist[i])) {
                find = 1;
                break;
            }
        }

        if (find) {
            find = 0;
            continue;
        }

        snprintf(test_path, PATH_MAX, "%s/%s", dirname, res->d_name);

        if (res->d_type == DT_REG) {
            run_one_case(test_path);
        } else if (res->d_type == DT_UNKNOWN) {
            ret = lstat(test_path, &st);
            if (ret < 0) {
                fprintf(stderr, "failed to lstat %s\n", test_path);
                break;
            }
            if (S_ISREG(st.st_mode)) {
                run_one_case(test_path);
            } else if (S_ISDIR(st.st_mode)) {
                run_dir_case(test_path);
            }

        } else if (res->d_type == DT_DIR) {
            run_dir_case(test_path);
        }
    }
}
/*********************************************************************************************************
** 函数名称: run_all_case
** 功能描述: 运行所有测试 CASE
** 输　入  : NONE
** 返回  值: NONE
** 全局变量:
** 调用模块:
*********************************************************************************************************/
void  run_all_case (void)
{
    FILE   *fp;

    run_dir_case(".");
    if (file) {
        fp = fopen(file, "a+");
    } else {
        fp = stdout;
    }

    fprintf(fp, "Test result:\n"
            "\tTotal test case: %d.\n"
            "\tSuccess test case: %d\n"
            "\tFailed total test case: %d\n", count, count - err_count, err_count);

    if (file) {
        fclose(fp);
    }
}
/*********************************************************************************************************
** 函数名称: help
** 功能描述: 帮助信息
** 输　入  : NONE
** 返回  值: NONE
** 全局变量:
** 调用模块:
*********************************************************************************************************/
void  help  (void)
{
    printf("Usage:  \n"
            "./run_posix_suite [options] [parameter].\n"
            "   -a          --all           test all case.\n"
            "   -d          --dir [name]    test specify directory case.\n"
            "   -f          --file [name]   save test result file.\n"
            "   -h          --help          print help infomation.\n");
}
/*********************************************************************************************************
** 函数名称: args_parse
** 功能描述: 参数解析
** 输　入  : argc, argv[]
** 输　出  : 无
** 全局变量:
** 调用模块:
*********************************************************************************************************/
void  args_parse (int argc, char *argv[])
{
    int      err = 0;

    if (argc < 2) {
        help();
        exit(EXIT_FAILURE);
    }

    for (;;) {
        int                   idx = 0;
        int                   c;
        static struct option  optionLong[] = {
            {"all",  no_argument,       NULL, 'a'},
            {"dir",  required_argument, NULL, 'd'},
            {"file",  required_argument, NULL, 'f'},
            {"help", no_argument,       NULL, 'h'},
            {NULL, 0, NULL, 0}
        };

        c = getopt_long(argc, argv, "d:f:ah", optionLong, &idx);
        if (c == -1) {
            break;
        }

        switch (c) {

        case 'a':
            allcase = 1;
            break;

        case 'd':
            dir = optarg;
            break;

        case 'f':
            file = optarg;
            break;

        case 'h':
        case '?':
            err = 1;
            break;
        }
    }

    if (err) {
        help();
        exit(EXIT_SUCCESS);
    }
}
/*********************************************************************************************************
** 函数名称: main
** 功能描述: 主函数
** 输　入  : argc, argv[]
** 输　出  : ERROR
** 全局变量:
** 调用模块:
*********************************************************************************************************/
int main (int argc, char *argv[])
{
    args_parse(argc, argv);

    if (file) {
        unlink(file);
    }

    if (allcase) {
        fprintf(stdout, "running all test case...\n");
        run_all_case();
    }

    if (dir) {
        fprintf(stdout, "running directory %s test case...\n", dir);
        run_dir_case(dir);
    }

    return  (0);
}
/*********************************************************************************************************
  END
*********************************************************************************************************/
