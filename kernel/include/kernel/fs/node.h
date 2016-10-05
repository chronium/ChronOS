#ifndef _KERNEL_FS_NODE_H
#define _KERNEL_FS_NODE_H

#include <sys/types.h>

namespace FileSystem {

class FileSystem;

class Node {
public:
    Node (const char *name, mode_t mode, FileSystem *fs):
        name (name), mode (mode), fs (fs) { }
    
    inline const char *GetName () const { return this->name; };
    
    inline mode_t GetMode () const { return this->mode; };
    inline void SetMode (mode_t val) { this->mode = val; };

    inline FileSystem *GetFileSystem () { return this->fs; };

public:
    const char *name;
    mode_t mode;

    FileSystem *fs;
};

}

#endif
