
#include "cameraInfo.h"

//写入拍摄时间函数
int writeTimeToStructure(char* fileName , struct PictureInfo* info)
{
    int second;
    char* point;
    char takePhotoTime[256];
    ExifReader* temp = new ExifReader(fileName);
    JPEGInfo* tmp = temp->getJPEGInfo();

    //如果取不到拍摄时间，则默认置为25:00:00
    if(NULL == tmp)
    {
        info->timeInfo->second = 90000;
        strcpy(info->timeInfo->secondString , "25:00:00");

        delete temp;

        return 1;
    }

    strcpy(takePhotoTime , tmp->orgTime.c_str());

    second = ((takePhotoTime[11] - '0') * 10 + (takePhotoTime[12] - '0')) * 3600 + ((takePhotoTime[14] - '0') * 10 + (takePhotoTime[15] - '0')) * 60 + ((takePhotoTime[17] - '0') * 10 + (takePhotoTime[18] - '0'));

    point = &takePhotoTime[11];
    strcpy(info->timeInfo->secondString , point);

    info->timeInfo->second = second;

    delete temp;

    return 0;
}
//按拍摄时间排序
int sortPhotographTime(struct PictureInfo* head)
{
    struct PictureInfo* minPointer;
    struct PictureInfo* pointer;
    struct PictureInfo* tempPointer;

    struct PictureInfo* tempHead = (struct PictureInfo*)malloc(sizeof(struct PictureInfo));

    tempHead->next = head->next;
    head->next->prior = tempHead;
    head->next = NULL;
    //
    tempPointer = head;

    while(tempHead->next != NULL)
    {
        minPointer = tempHead->next;
        for(pointer = tempHead->next ; pointer != NULL ; pointer = pointer->next)
        {
            if(pointer->timeInfo->second < minPointer->timeInfo->second)
            {
                minPointer = pointer;
            }
        }

        minPointer->prior->next = minPointer->next;
        if(NULL != minPointer->next)
            minPointer->next->prior = minPointer->prior;

        tempPointer->next = minPointer;
        minPointer->prior = tempPointer;
        minPointer->next = NULL;

        tempPointer = tempPointer->next;
    }

    free(tempHead);

    return 0;
}
//照片按时间分类
int photographTimeClassify(struct SceneInfo* category , struct PictureInfo* head , int imageCount)
{
    int cntCategory = 0;
    int sameClassPicNum = 1;

    struct PictureInfo* point;

    //把链从head上摘下链到category[]上
    category[cntCategory].head.next = head->next;
    head->next->prior = &category[cntCategory].head;

    //指针指向头结点
    category[cntCategory].point = &category[cntCategory].head;

    for(point = head->next ; point->next != NULL ; point = point->next)
    {
        if((point->next->timeInfo->second) - (point->timeInfo->second) > 600)	//拍摄时间大于10分钟则分成另一类
        {
            category[cntCategory].count = sameClassPicNum;		//本类中照片数目

            sameClassPicNum = 1;

            cntCategory++;

            category[cntCategory].head.next = point->next;
            point->next->prior = &category[cntCategory].head;

            category[cntCategory].point = &category[cntCategory].head;
        }
        else
        {
            sameClassPicNum++;
        }
    }
    category[cntCategory++].count = sameClassPicNum;		//最后一段跳出循环时，记录其图片张数

    return cntCategory;
}
//相同时间段内不同场景的区分
int sameTimeDifferentSceneClassify(struct SceneInfo* info , int sceneCount)
{
    int hueType;				//本张照片主色调类型
    int previousHueType = 0;	//上一张照片主色调类型
    int sameHueCount = 1;		//色调相同照片的张数

    int isFirstSection = 1;		//是每个场景的第一段

    struct PictureInfo* point;
    struct PictureInfo* tempPoint;

    for(int i = 0 ; i < sceneCount ; i++)
    {
        point = info[i].head.next;
        tempPoint = point;

        previousHueType = 0;	//上一张照片主色调类型
        sameHueCount = 1;		//色调相同照片的张数
        isFirstSection = 1;		//是每个场景的第一段

        int isTail = 1;

        for(int k = 0 ; k < info[i].count ; k++)
        {
            hueType = point->statisticsColorContent.serialNumber[0] / (gSaturationLevel * gBrightnessLevel) + 1;

            if(hueType > 6)
                hueType = 7;

            if(0 == previousHueType)
                previousHueType = hueType;
            else
            {
                if(previousHueType == hueType)	//此张与前一张为同一色调
                    sameHueCount++;
                else
                {
                    if(sameHueCount < 5)
                    {
                        tempPoint = point;
                    }
                    else
                    {
                        if(isFirstSection)
                        {
                            isFirstSection = 0;
                            tempPoint = point;

                            sameHueCount = 1;
                            previousHueType = hueType;
                        }
                        else
                        {
                            int previousCount = info[i].count;
                            info[i].count = k;

                            info[sceneCount].head.next = tempPoint;
                            info[sceneCount].count = previousCount - k;

                            sceneCount++;

                            isTail = 0;	//没有扫描到尾部就中断进行下一次

                            break;
                        }
                    }
                }
            }
            point = point->next;
        }
        if(sameHueCount >= 5 && !isFirstSection && isTail)
        {
            int previousCount = info[i].count;
            info[i].count = previousCount - sameHueCount;

            info[sceneCount].head.next = tempPoint;
            info[sceneCount].count = sameHueCount;

            sceneCount++;

            isTail = 1;
        }
    }

    return sceneCount;
}
