#include "commands.h"


int command_mkdir(const char *dst, const char *name, __mode_t perms)
{
    return 0;
}

int command_mkfile(const char *dst, const char *name, __mode_t perms)
{
    return 0;
}

int command_copy(const char *dst, const char *src, unsigned int byte_rate)
{
    return 0;
}

int command_move(const char *dst, const char *src_path, unsigned int byte_rate)
{
    return 0;
}

int command_rename(const char *new_name, const char *src)
{
    return 0;
}

int command_permissions(__mode_t new_perms, const char *src)
{
    return 0;
}

int command_permissions_recursive(__mode_t new_perms, const char *src, unsigned int depth)
{
    return 0;
}

int command_list(const char *src)
{
    return 0;
}

int command_delete(const char *src)
{
    return 0;
}

