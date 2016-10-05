#ifndef _KERNEL_FS_DIRECTORY_H
#define _KERNEL_FS_DIRECTORY_H

#include <kernel/fs/node.h>

namespace FileSystem {

class Directory: public Node {
public:
    Directory (const char *name, mode_t mode, FileSystem *fs):
        Node (name, mode, fs), children (new List<Node> ()) { }

    inline List<Node> *GetChildren () { return this->children; };

private:
    List<Node> *children;
};

}

#endif
