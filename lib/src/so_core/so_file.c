/*
*********************************************************************************************************
*
*	文件名称 : so_file.c
*	说    明 : 对设备文件的操作
*				
*   作    者 : Sourcelink
*   日    志 : 
*   日    期 : 2017-11-7
*
*********************************************************************************************************
*/
#include "so_lib.h"
#include "so_file.h"


DEBUG_SET_LEVEL(DEBUG_LEVEL_ERR);


/* 整形 读取 待完善 */
EXPORT int write_value_to_file(const char* fileName, char* buff)
{
    int ret;
    FILE *fp = fopen(fileName, "w");
    if (fp == NULL) {
       ERR("Unable to open file %s", fileName);
        ret = -1;
    } else {
        ret = fwrite(buff, strlen(buff), 1, fp);
        fclose(fp);
    }
    return ret;
}

EXPORT int write_intvalue_to_file(const char* fileName, int value)
{
    char buff[50];
    sprintf(buff, "%d", value);
    return write_value_to_file(fileName, buff);
}

EXPORT int read_value_from_file(const char* fileName, char* buff, int len)
{
    int ret = -1;
    FILE *fp = fopen(fileName,"r");
    if (fp == NULL) {
        ERR("Unable to open file %s",fileName);
        return -1;
    } else {
        ret = fread(buff, sizeof(char), len, fp);
 		fclose(fp);
    }
    return ret;
}

EXPORT int read_intvalue_from_file(const char* fileName)
{
    char buff[255];
    memset(buff, 0, sizeof(buff));
    int ret = read_value_from_file(fileName, buff, sizeof(buff)-1);
    if (ret > 0) {
        return atoi(buff);
    }
    return ret;
}


