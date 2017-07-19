#ifndef CAMERAINFO_H
#define CAMERAINFO_H

#include "EXIF.h"
#include "public.h"
#include "pictureInfo.h"

struct TimeInfo
{
    char secondString[32];		//秒数，字符串
    int second;					//秒数，整型
    int category;				//类别（按时间段分）
};

//写入拍摄时间函数
int writeTimeToStructure(char* fileName , struct PictureInfo* info);
//按拍摄时间排序
int sortPhotographTime(struct PictureInfo* head);
//照片按时间分类
int photographTimeClassify(struct SceneInfo* category , struct PictureInfo* head , int imageCount);
//相同时间段内不同场景的区分
int sameTimeDifferentSceneClassify(struct SceneInfo* info , int sceneCount);

#endif // CAMERAINFO_H
