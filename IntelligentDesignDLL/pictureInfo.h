#ifndef PICTUREINFO_H
#define PICTUREINFO_H

#include "public.h"

extern int gBlock;
extern int gCntBlocks;
extern int gHueLevel;
extern int gSaturationLevel;
extern int gBrightnessLevel;
extern int gWindowSize;
//颜色含量信息
struct ColorContentInfo
{
    double* color;
    int* serialNumber;
};
//图片结构信息
struct PictureInfo
{
    char name[256];				//图片名字（绝对路径）
    double similarity;			//背景相似度得分

    char jsonPosition[4];		//在json文件中的层号

    struct TimeInfo* timeInfo;  //拍摄时间信息

    int windowIndex[16];		//每小块中窗口最大序号

    int* histogramHue[16];		//色调直方图指针(切成16块)
    struct ColorContentInfo blockColorContent[16];	//颜色含量（切成16块）
    struct ColorContentInfo statisticsColorContent;	//某几块（具体自己定义）的颜色含量

    struct PictureInfo* prior;
    struct PictureInfo* next;
};
//照片分类
struct SceneInfo
{
    struct PictureInfo head;		//头结点
    struct PictureInfo* point;	//活动指针

    int count;
};
//psd文件信息
struct PsdInfo
{
    char psdFileName[256];      //PSD文件名（绝对路径）
    struct PictureInfo LayerHead;//头结点
    int LayerCount;             //客照层总数

    struct PsdInfo* prior;
    struct PsdInfo* next;
};

//创建并初始化结构体
struct PictureInfo* createStruct(void);
//释放结构体
int releaseStructure(struct PictureInfo* info);

#endif // PICTUREINFO_H
