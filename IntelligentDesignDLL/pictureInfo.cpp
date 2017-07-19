#include "pictureInfo.h"

//初始化结构体
struct PictureInfo* createStruct(void)
{
    int i = 0;
    int mallocSize;						//申请链表长度
    struct PictureInfo* newObject;		//新申请的结点信息

    //新申请结点信息
    newObject = (struct PictureInfo*)malloc(sizeof(struct PictureInfo));
    //申请链表长度
    mallocSize = gHueLevel * gSaturationLevel * gBrightnessLevel + 6;

    for(i = 0 ; i < gCntBlocks ; i++)											//目前固定16块
    {
        //360维直方图，申请361大小。用于四舍五入，将下标360放入下标为0中。
        newObject->histogramHue[i] = (int*)malloc(361 * sizeof(int));
        memset(newObject->histogramHue[i] , 0 , 361 * sizeof(int));

        newObject->blockColorContent[i].color = (double*)malloc(sizeof(double) * mallocSize);
        newObject->blockColorContent[i].serialNumber = (int*)malloc(sizeof(int) * mallocSize);
        memset(newObject->blockColorContent[i].color , 0 , sizeof(double) * mallocSize);
        memset(newObject->blockColorContent[i].serialNumber , 0 , sizeof(int) * mallocSize);
    }

    newObject->statisticsColorContent.color = (double*)malloc(sizeof(double) * mallocSize);
    newObject->statisticsColorContent.serialNumber = (int*)malloc(sizeof(int) * mallocSize);
    memset(newObject->statisticsColorContent.color , 0 , sizeof(double) * mallocSize);
    memset(newObject->statisticsColorContent.serialNumber , 0 , sizeof(int) * mallocSize);

    newObject->prior = NULL;
    newObject->next = NULL;

    return newObject;
}
//释放结构体函数
int releaseStructure(struct PictureInfo* object)
{
    int i = 0;
    for(i = 0 ; i < 16 ; i++)
    {
        free(object->histogramHue[i]);
        free(object->blockColorContent[i].color);
        free(object->blockColorContent[i].serialNumber);
    }

    free(object->statisticsColorContent.color);
    free(object->statisticsColorContent.serialNumber);
    free(object);

    return 0;
}

