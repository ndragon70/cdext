#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main_list(int argc, char **argv);
int main_ccd(int argc, char **argv);
int main_save(int argc, char **argv);
int main_delete(int argc, char **argv);
int main_setcd(int argc, char **argv);

int main(int argc, char **argv)
{
    int ret = 0;
    if (argc < 2) {
        fprintf(stderr, "usage: %s [ccd|list|save|delete|setcd]\n", argv[0]);
        exit(1);
    }
    if (strcmp(argv[1], "list") == 0) {
        ret = main_list(argc, argv);
    } else if (strcmp(argv[1], "ccd") ==0) {
        ret = main_ccd(argc, argv);
    } else if (strcmp(argv[1], "save") ==0) {
        ret = main_save(argc, argv);
    } else if (strcmp(argv[1], "delete") ==0) {
        ret = main_delete(argc, argv);
    } else if (strcmp(argv[1], "setcd") ==0) {
        ret = main_setcd(argc, argv);
    } else {
        fprintf(stderr, "Unknown sub command: %s\n", argv[1]);
        exit(1);
    }
    exit(ret);
}
