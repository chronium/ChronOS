#include <dirent.h>
#include <fcntl.h>

DIR *opendir (const char *name) {
    return fdopendir (open (name, O_RDONLY));
}
