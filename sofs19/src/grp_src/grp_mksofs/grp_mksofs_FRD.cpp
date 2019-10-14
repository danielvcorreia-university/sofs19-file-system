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
        memset( entryBlockDirs, 0, BlockSize );
        
        /* Filling with "NullReference" Data Field "in" in all SODirEntry dir's */
        for (uint32_t i = 2; i < DPB; i++)
        { entryBlockDirs[i].in = NullReference; }

        /* Setting the Data Field "name" in the first SODirEntry to "." and the second one to ".." */
        memcpy( entryBlockDirs[0].name, ".", 1 );
        memcpy( entryBlockDirs[1].name, "..", 2 );

        /* Writing the block "inodesTotal/InodePerBlock + 1(SuperBlock)" in the raw disk */
        /* The division between itotal and IPB is always with rest 0 */
        soWriteRawBlock( itotal/IPB + 1, entryBlockDirs );
    }
};

