# ifndef __PEEK_H
# define __PEEK_H
int is_symbol(const char *path);
int entry_compare(const void *a, const void *b);
void list_directory(const char *path, int show_hidden, int show_details);
# endif