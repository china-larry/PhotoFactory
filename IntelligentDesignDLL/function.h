#ifndef FUNCTION_H
#define FUNCTION_H

#include "public.h"

//找出文件扩展名，并写入extension中
int getFileExtension(char* fileName , char* extension);
//建立多级文件夹函数
int createFolder(char* folder);
//提取完整路径中的路径与文件名，返回为fileName在fullPath中的位置
int extractFilenamePosition(char* fullPath , char* path , char* fileName);

#endif // FUNCTION_H
