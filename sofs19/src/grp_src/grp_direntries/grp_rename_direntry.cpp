#include "direntries.h"

#include "core.h"
#include "dal.h"
#include "fileblocks.h"
#include "bin_direntries.h"

#include <string.h>
#include <errno.h>
#include <sys/stat.h>

namespace sofs19
{
    void grpRenameDirEntry(int pih, const char *name, const char *newName)
    {
        soProbe(204, "%s(%d, %s, %s)\n", __FUNCTION__, pih, name, newName);

        SOInode* ip = soGetInodePointer(pih);
        SODirEntry dir[DPB];

        bool exists = false;
        
        for(uint32_t i = 0; i < ip->size / BlockSize; i++) {
            
            soReadFileBlock(pih, i, dir);
            
            for(uint32_t entry_ind = 0; entry_ind < DPB; entry_ind++) {
                if(strcmp(dir[entry_ind].name, name) == 0 && !exists) {
                    memcpy(dir[entry_ind].name, newName, SOFS19_MAX_NAME);
                    soWriteFileBlock(pih, i, dir);
                    exists = true;
                }
            }
        }    
        if(!exists)
            throw SOException(ENOENT, __FUNCTION__);    
    }
};

