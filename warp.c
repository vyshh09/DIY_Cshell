#include "headers.h"


char *expand_path(const char *path, const char *home_dir, const char *pwd, const char *var)
{
    char *expanded = malloc(MAX_INPUT_SIZE);
    if (path[0] == '~')
    {
        snprintf(expanded, MAX_INPUT_SIZE, "%s%s", var, path + 1);
    }
    else if (strcmp(path, "-") == 0)
    {
        snprintf(expanded, MAX_INPUT_SIZE, "%s", pwd);
    }
    else
    {
        snprintf(expanded, MAX_INPUT_SIZE, "%s", path);
    }
    return expanded;
}
