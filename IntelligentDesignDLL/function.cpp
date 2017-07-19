#include "function.h"

//找出文件扩展名，并写入extension中
int getFileExtension(char* fileName , char* extension)
{
    int i = 0;
    char* ptr = strrchr(fileName , '.');

    if(ptr && (ptr + 1))
    {
        ptr++;
        strcpy(extension , ptr);
    }

    for(i = 0 ; i < strlen(extension) ; i++)
    {
        extension[i] = tolower(extension[i]);
    }

    return 0;
}

//建立多级文件夹函数
int createFolder(char* folder)
{
    int lengthFolder = 0;
    char temp[2048];

    sprintf(temp , "%s\\" , folder);
    lengthFolder = strlen(temp);

    for(int i = 0 ; i < lengthFolder ; i++)
    {
        if('\\' != folder[i])
        {
            temp[i] = folder[i];
        }
        else
        {
            temp[i] = '\0';
            if(access(temp , 0))
            {
                mkdir(temp);
            }
            temp[i] = '\\';
        }
    }
    if(access(temp , 0))
    {
        mkdir(temp);
    }

    return 0;
}

//提取完整路径中的路径与文件名，返回为fileName在fullPath中的位置
int extractFilenamePosition(char* fullPath , char* path , char* fileName)
{
    int i = 0 , j = 0 , k = 0;
    for(i = strlen(fullPath) - 1 ; i >= 0 ; i--)
    {
        if('\\' == fullPath[i])
            break;
    }

    for(j = 0 ; j < strlen(fullPath) ; j++)
    {
        if(j <= i)
            path[j] = fullPath[j];
        else
        {
            path[j] = '\0';
            fileName[k++] = fullPath[j];
        }
    }
    fileName[k] = '\0';

    return i + 1;
}
