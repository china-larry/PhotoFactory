#include "intelligentdesigndll.h"
#include "IntelligentDesign.h"

int gBlock = 15;				//统计块
int gCntBlocks = 16;			//图像分块
int gHueLevel = 6;				//色调级
int gSaturationLevel = 2;		//饱和度级
int gBrightnessLevel = 2;		//亮度级
int gWindowSize = 30;			//滑动窗口大小

//智能设计函数
BUILD_DLL QByteArray IntelligentDesign(QStringList qlistPhotoPath , QString qstrTemplatePath)
{    
    QByteArray qTemplatePath = qstrTemplatePath.toLocal8Bit();
    char* templatePath = qTemplatePath.data();
    //智能设计函数
    QByteArray result = intelligentDesign(qlistPhotoPath , templatePath);

    return result;
}
