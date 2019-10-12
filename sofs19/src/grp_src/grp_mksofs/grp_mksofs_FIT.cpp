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
        
        /* change the following line by your code */
        
        printf("Inodes: %lu\n", IPB);
        // Número de blocks on Inode Table
        unsigned long nBLocks = itotal/IPB;
        unsigned long nInode = 0;
        // Iterar pela tabela
        for(unsigned long block=1; block<=nBLocks; block++) {
            // Inicializar Bloco
            SOInode inodeBlock[IPB];
            for(unsigned long inode=0; inode<IPB; inode++) {
                // Inicializa cada inode com valores default
                inodeBlock[inode].mode = 0175000;
                inodeBlock[inode].lnkcnt = 0;
                inodeBlock[inode].owner = 0;
                inodeBlock[inode].group = 0;
                inodeBlock[inode].size = 0;
                inodeBlock[inode].blkcnt = 0;
                inodeBlock[inode].ctime = 0;
                inodeBlock[inode].mtime = 0;
                inodeBlock[inode].next = ++nInode;
                inodeBlock[inode].d[0] = 0;
                unsigned long i;
                for(i=0; i<N_DIRECT; i++)
                    inodeBlock[inode].d[i] = NullReference;
                for(i=0; i<N_INDIRECT; i++)
                    inodeBlock[inode].i1[i] = NullReference;
                for(i=0; i<N_DOUBLE_INDIRECT; i++)
                    inodeBlock[inode].i2[i] = NullReference;
            }
            if(block==1) {
                // Primeiro inode aponta para root do filesystem
                /* Octal -> 
                    directory  : 0040000
                    owner  rwx : 00700
                    group  rwx : 00070
                    others r_x : 00005
                */
                inodeBlock[0].mode = 0040775;
                inodeBlock[0].lnkcnt = 2;
                inodeBlock[0].owner = 0;
                inodeBlock[0].group = 0;
                inodeBlock[0].size = BlockSize;
                // "in the newly-formatted disk the root directory occupies a single data block, the one immediately after the inode table"
                inodeBlock[0].blkcnt = 1;
                if(set_date==true) {
                    time_t system_time;
                    time(&system_time);
                    uint32_t time = system_time;
                    inodeBlock[0].ctime=time;
                    inodeBlock[0].mtime=time;
                    inodeBlock[0].atime=time;
                } else {
                    inodeBlock[0].ctime=0;
                    inodeBlock[0].mtime=0;
                    inodeBlock[0].atime=0;
                }
                inodeBlock[0].d[0] = 0;
                unsigned long i;
                for(i=1; i<N_DIRECT; i++)
                    inodeBlock[0].d[i] = NullReference;
                for(i=0; i<N_INDIRECT; i++)
                    inodeBlock[0].i1[i] = NullReference;
                for(i=0; i<N_DOUBLE_INDIRECT; i++)
                    inodeBlock[0].i2[i] = NullReference;
                inodeBlock[0].next = 69;
                //inodeBlock[0].next = NullReference;
            }
            if(nInode==itotal) {
                inodeBlock[IPB-1].next=NullReference;
            }
            soWriteRawBlock(block, inodeBlock);
        }
        //return binFillInodeTable(itotal, set_date);
    }
};

