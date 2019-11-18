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
    uint32_t grpGetDirEntry(int pih, const char *name)
    {
        soProbe(201, "%s(%d, %s)\n", __FUNCTION__, pih, name);

        for(uint32_t i = 0; name[i] != '\0'; i++){
            if(name[i] == '/'){
                throw SOException(EINVAL, __FUNCTION__);
            }
        }

        if(strcmp(name, "") == 0){
            throw  SOException(EINVAL, __FUNCTION__);
        }

        SOInode* ip = soGetInodePointer(pih);
        SODirEntry d[DPB];

        for(uint32_t i = 0; i<= (ip -> size/ BlockSize); i++){
            soReadFileBlock(pih,i,d);
            for(uint32_t j = 0; j < DPB; j++){
                if(strcmp(d[j].name, name) == 0){
                    return d[j].in;
                }
            }
        }

        return NullReference;
    }
};

