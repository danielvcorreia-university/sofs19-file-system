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
    void grpAddDirEntry(int pih, const char *name, uint32_t cin)
    {
        soProbe(202, "%s(%d, %s, %u)\n", __FUNCTION__, pih, name, cin);

			for(unsigned i=0; name[i]!='\0'; i++){
				if (name[i]=='/') throw SOException(EINVAL, __FUNCTION__);
			}


            if (!strcmp(name, "")) {
            	throw SOException(EINVAL, __FUNCTION__);
            }

            if (strlen(name) > SOFS19_MAX_NAME) {
				throw SOException(ENAMETOOLONG, __FUNCTION__);
			}

            SOInode* pi = soGetInodePointer(pih);
          
            int emptyPlace = -1;
            int emptyPlaceBlockIndex = -1;
            SODirEntry emptyPlaceBlock[DPB];
            SODirEntry d[DPB];
            uint32_t i = 0;
            for (; i < (pi->size / BlockSize); i++ ) {

            	soReadFileBlock(pih, i, d);

				uint32_t j = 0;
				for (; j < DPB; j++) {
					if (emptyPlace < 0 && d[j].name[0] == '\0') {
						soReadFileBlock(pih, i, emptyPlaceBlock);
						emptyPlaceBlockIndex = i;
						emptyPlace = j;
					}

					if (!strcmp(d[j].name, name)) {
						throw SOException(EEXIST,__FUNCTION__);
					}
				}
            }

			if (emptyPlace >= 0) {

				memcpy(emptyPlaceBlock[emptyPlace].name, name, SOFS19_MAX_NAME+1);
				memcpy(&emptyPlaceBlock[emptyPlace].in, &cin, sizeof(uint32_t));
				soWriteFileBlock(pih, emptyPlaceBlockIndex, emptyPlaceBlock);
			}			
			else {

				if (soGetFileBlock(pih, i) == NullReference){
					soAllocFileBlock(pih, i);
				}

				SODirEntry dir[DPB];
				memset(dir,0,BlockSize);
				for(uint32_t i = 0; i < DPB; i++){
					dir[i].in = NullReference;
				}
				memcpy(dir[0].name, name, SOFS19_MAX_NAME+1);
				dir[0].in = cin;
				pi->size += BlockSize;

				soWriteFileBlock(pih, i, dir);
			}

        
        /* change the following line by your code */
       // binAddDirEntry(pih, name, cin);
    }
};

