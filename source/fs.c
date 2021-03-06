/*
*   fs.c
*/

#include <stddef.h>
#include "fs.h"
#include "fatfs/ff.h"

static FATFS fs;

int mountSD()
{
    if (f_mount(&fs, "0:", 1) != FR_OK) {
        //printF("Failed to mount SD card!");
        return 1;
    }
    //printF("Mounted SD card");
    return 0;
}

int unmountSD()
{
    if (f_mount(NULL, "0:", 1) != FR_OK) {
        //printF("Failed to mount SD card!");
        return 1;
    }
    //printF("Unmounted SD card");
    return 0;
}

int fileReadOffset(u8 *dest, const char *path, u32 size, u32 offset){
    FRESULT fr;
    FIL fp;
    u32 br = 0;

    fr = f_open(&fp, path, FA_READ);
    if (fr != FR_OK)goto error;
    
    if (!size) size = f_size(&fp);
    if (offset) {
        fr = f_lseek(&fp, offset);
        if (fr != FR_OK) goto error;
    }

    fr = f_read(&fp, dest, size, &br);
    if (fr != FR_OK) goto error;

    fr = f_close(&fp);
    if (fr != FR_OK) goto error;
    return 0;

error:
    f_close(&fp);
    return fr;
}

int fileRead(u8 *dest, const char *path, u32 size){
    return fileReadOffset(dest, path, size, 0);
}

int fileWrite(const u8 *buffer, const char *path, u32 size){
    FRESULT fr;
    FIL fp;
    u32 br = 0;

    if(f_open(&fp, path, FA_WRITE | FA_OPEN_ALWAYS) == FR_OK){
        fr = f_write(&fp, buffer, size, &br);
        f_close(&fp);
        if (fr == FR_OK && br == size) return 0;
    }
    return fr;
}