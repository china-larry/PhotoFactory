#ifndef INTELLIGENTDESIGN_H
#define INTELLIGENTDESIGN_H

#include "public.h"
#include "pictureInfo.h"
#include "cameraInfo.h"
#include <QFile>
#include <QJsonDocument>
#include <QVariantMap>
#include <QDebug>
#include <QJsonValue>
#include <QString>
#include <QStringList>

extern int gBlock;
extern int gCntBlocks;
extern int gHueLevel;
extern int gSaturationLevel;
extern int gBrightnessLevel;
extern int gWindowSize;

//比较背景相似度函数
QByteArray intelligentDesign(QStringList guests , char* templatePath);
//统计每一种色调所占的比例
int computeColorScale(char* fileName , struct PictureInfo* info);
//对各个色调所占的比例进行排序
int sortHue(struct PictureInfo* input);
//分成筛选
int filterPositionArea(struct PictureInfo* sample , struct PictureInfo* info , int imageCount , int rankingArea , int position);
//统计图像中不同块数任意组合所得到的平均色调
int statisticsBlockInfo(struct PictureInfo* sample , int position);
//比较先中块的前rankingArea（1，2，3...）大色调。如果不符合则剔除
bool comparisonHue(struct PictureInfo* guests , struct PictureInfo* sample , int rankingArea);
//把图片的16块分别进行直方图比较，并记录得分
int compareHistogram(struct PictureInfo* sample , struct PictureInfo* pendingInfo);
//交换结构体数据
int swapData(struct PictureInfo* firstInfo , struct PictureInfo* secondInfo);
//比较图片第N大色调
bool comparisonHue(struct PictureInfo* guests , struct PictureInfo* sample , int rankingArea);
//卡方法比较直方图
int compareHistogram(struct PictureInfo* sample , struct PictureInfo* pendingInfo);
//比较直方图窗口
bool comparisonMaxConnectionAreaHue(struct PictureInfo* guests , struct PictureInfo* sample);
//写JSON文件
QByteArray writeInfoToJSONFile(struct PictureInfo*guestsPhoto , int guestsPhotoCount , struct PsdInfo* sampleTemplate  , int sampleTemplateCount);
//读Json文件
int readJsonFile(char* inputJsonFile , struct PictureInfo* head);
//向结构体中写入图片信息
bool writeInfoToStruct(char* fileName , struct PictureInfo* newObject , int gWindowSize , int isStatisticalTime = 0);

#endif // INTELLIGENTDESIGN_H
