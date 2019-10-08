#include "grp_mksofs.h"

#include "rawdisk.h"
#include "core.h"
#include "bin_mksofs.h"

#include <string.h>
#include <inttypes.h>

namespace sofs19
{
    /*
       filling in the contents of the root directory:
       the first 2 entries are filled in with "." and ".." references
       the other entries are empty.
       If rdsize is 2, a second block exists and should be filled as well.
    */
    void grpFillRootDir(uint32_t itotal)
    {
        soProbe(606, "%s(%u)\n", __FUNCTION__, itotal);

        /* change the following line by your code */

        /* DPB number of Dir Struct References to fill a Data Block */
        /* SODirEntry buffer */
        SODirEntry entryBlockDirs[DPB];

        /* Filling all the SODirEntry's(Block) with zeros */
        /* Memset() is more eficient than manual changing all data fields "in" to zeros */
        memset( entryBlockDirs, 0, BlockSize );
        
        /* Filling with "NullReference" Data Field "namespace" in all SODirEntry dir's */
        for (uint32_t i = 2; i < DPB; i++)
        { entryBlockDirs[i] -> in = NullReference; }

        /* Setting the first SODirEntry to "." and the second one to ".." */
        entryBlockDirs[0] -> name = ".";
        entryBlockDirs[1] -> name = "..";

        /* Writing the block "inodesTotal/InodePerBlock + 1(SuperBlock)" in the raw disk */
        /* The division between itotal and IPB is always with rest 0 */
        soWriteRawBlock( itotal/IPB + 1, entryBlockDirs );
        
        if (rdsize == 2)
        {
            SODirEntry entryBlock2Dirs[DPB];
            memset( entryBlock2Dirs, 0, BlockSize );
            for(uint32_t i = 0; i < DPB; i++)
            { entryBlock2Dirs[i] -> in = NullReference; }
            soWriteRawBlock( itotal/IPB + 2, entryBlock2Dirs );
        }
    }
};

