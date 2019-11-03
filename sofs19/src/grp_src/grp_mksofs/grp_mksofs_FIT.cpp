#include "grp_mksofs.h"

#include "rawdisk.h"
#include "core.h"
#include "bin_mksofs.h"

#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <inttypes.h>

namespace sofs19
{
    void grpFillInodeTable(uint32_t itotal, bool set_date)
    {
        soProbe(604, "%s(%u)\n", __FUNCTION__, itotal);
        
        /*
         uint32_t nBlocks = itotal/IPB;
            SOInode inode[IPB];

            for(uint32_t block_num = 1 ; block_num < 1 + nBlocks ; block_num++)
            {
            	for(uint32_t i = 0 ; i < IPB ; i++)
            	{
                    inode[i].mode = INODE_FREE;
                    inode[i].lnkcnt = 0;
                    inode[i].owner = 0;
                    inode[i].group = 0;
                    inode[i].size = 0;
                    inode[i].blkcnt = 0;

            		for(uint32_t a = 0 ; a < N_DIRECT ; a++)
            		{
            			inode[i].d[a] = NullReference;
            		}

            		for(uint32_t b = 0 ; b < N_INDIRECT ; b++)
            		{
            			inode[i].i1[b] = NullReference;
            		}

            		for(uint32_t c = 0 ; c < N_DOUBLE_INDIRECT ; c++)
            		{
            			inode[i].i2[c] = NullReference;
            		}

                    if(block_num == 1 && i == 0)
                    {
                        inode[0].mode = S_IFDIR | 0775;
                        inode[0].lnkcnt = 2;
                        inode[0].owner = getuid();
                        inode[0].group = getgid();
                        inode[0].atime = time(NULL);
                        inode[0].mtime = time(NULL);
                        inode[0].ctime = time(NULL);
                        inode[0].d[0] = 0;          
                        inode[0].size = BlockSize;
                         
                    }
            	}
            	soWriteRawBlock(block_num, &inode);
            }

            */

        /* change the following line by your code */
        return binFillInodeTable(itotal, set_date);
    }
};

