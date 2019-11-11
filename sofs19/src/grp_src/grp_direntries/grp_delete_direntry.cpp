#include "direntries.h"

#include "core.h"
#include "dal.h"
#include "fileblocks.h"
#include "bin_direntries.h"

#include <errno.h>
#include <string.h>
#include <sys/stat.h>

namespace sofs19
{
    uint32_t grpDeleteDirEntry(int pih, const char *name)
    {

        SODirEntry dirEntrys[DPB];

        soProbe(203, "%s(%d, %s)\n", __FUNCTION__, pih, name);
        SOInode *inode = soGetInodePointer(pih);

        uint32_t ret;

        for(uint32_t i = 0; i < inode->blkcnt; i++){
            soReadFileBlock(pih, i, dirEntrys);
            for(uint32_t j = 0; j < DPB; j++){
                if(strcmp(dirEntrys[j].name, name) == 0){
                    for(uint32_t k = 0; k < SOFS19_MAX_NAME; k++) {
                        dirEntrys[j].name[k] = '\0';
                    }
                    ret = dirEntrys[j].in;
                    dirEntrys[j].in = NullReference;
                    soWriteFileBlock(pih, i, dirEntrys);
                    return ret;
                }
            }
        }

        throw SOException(ENOENT, __FUNCTION__);
    }
};

