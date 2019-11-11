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
    bool grpCheckDirEmpty(int ih)
    {
        soProbe(205, "%s(%d)\n", __FUNCTION__, ih);

        /* change the following line by your code */
        //return binCheckDirEmpty(ih);

        SOInode* node = soGetInodePointer(ih);

        uint32_t block = (node->size) / BlockSize;

        SODirEntry buf[DPB];

        uint32_t blockCount = 0;
        uint32_t blockNum = 0;
        uint32_t tmp;

        while(blockCount < block){
            tmp = soGetFileBlock(ih,blockNum);

            if(tmp = NullReference){
                soReadFileBlock(ih,blockNum,buf);

                for(uint32_t i = 2; i < DPB; i++){
                    if(strcmp(buf[i].name, "\0") != 0){
                        return false;
                    }
                }

                blockCount++;
            }

            blockNum++;
        }

        return true;

    }
};

