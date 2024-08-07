# include "headers.h"

int is_symbol(const char *path)
{
    return (strcmp(path, ".") == 0 || strcmp(path, "..") == 0 || path[0] == '~' || strcmp(path, "-") == 0);
}

int entry_compare(const void *a, const void *b)
{
    return strcmp(((struct Entry *)a)->name, ((struct Entry *)b)->name);
}


void list_directory(const char *path, int show_hidden, int show_details)
{
    DIR *dir = opendir(path);
    if (dir == NULL)
    {
        printf("empty directory\n");
        return;
    }

    struct dirent *entry;
    int entry_count = 0;
    struct Entry *entries = NULL;

    while ((entry = readdir(dir)) != NULL)
    {
        if (!show_hidden && entry->d_name[0] == '.')
            continue;

        char full_path[MAX_INPUT_SIZE];
        snprintf(full_path, MAX_INPUT_SIZE, "%s/%s", path, entry->d_name);

        struct stat file_info;
        if (lstat(full_path, &file_info) == 0)
        {
            entry_count++;
            entries = realloc(entries, sizeof(struct Entry) * entry_count);
            strcpy(entries[entry_count - 1].name, entry->d_name);
            entries[entry_count - 1].file_info = file_info;
        }
    }

    closedir(dir);

    qsort(entries, entry_count, sizeof(struct Entry), entry_compare);

    for (int i = 0; i < entry_count; i++)
    {
        char *color_code = "\033[0m";

        if (S_ISDIR(entries[i].file_info.st_mode))
        {
            color_code = "\033[1;34m";
        }
        else if (S_ISREG(entries[i].file_info.st_mode) && (entries[i].file_info.st_mode & S_IXUSR))
        {
            color_code = "\033[1;32m";
        }

        printf("%s%s%s", color_code, entries[i].name, "\033[0m");

        if (show_details)
        {
            struct passwd *pw = getpwuid(entries[i].file_info.st_uid);
            struct group *grp = getgrgid(entries[i].file_info.st_gid);
            struct tm *timeinfo = localtime(&entries[i].file_info.st_mtime);

            printf("  %s  %s  %lld  ", pw->pw_name, grp->gr_name, (long long)entries[i].file_info.st_size);

            char time_buffer[80];
            strftime(time_buffer, sizeof(time_buffer), "%b %d %H:%M", timeinfo);
            printf("%s", time_buffer);
        }

        printf("\n");
    }

    free(entries);
}
