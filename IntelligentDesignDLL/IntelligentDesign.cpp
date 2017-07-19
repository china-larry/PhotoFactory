#include "IntelligentDesign.h"

//对各色调所占比例排序
int sortHue(struct ColorContentInfo* sample)
{
    int i = 0 , j = 0 , k = 0;
    int maxNumber = 0;
    double maxElement = 0.0;
    double tempSort[512];
    //将信息写入临时数组
    for(k = 0 ; k < gHueLevel * gSaturationLevel * gBrightnessLevel + 6 ; k++)
    {
        tempSort[i++] = sample->color[k];
    }

    for(i = 0 ; i < gHueLevel * gSaturationLevel * gBrightnessLevel + 6 ; i++)
    {
        maxNumber = 0;
        maxElement = tempSort[0];
        for(j = 0 ; j < gHueLevel * gSaturationLevel * gBrightnessLevel + 6 ; j++)
        {
            if(tempSort[j] > maxElement)
            {
                maxElement = tempSort[j];
                maxNumber = j;
            }
        }
        sample->serialNumber[i] = maxNumber;					//写入排序序号
        tempSort[maxNumber] = -1.0;								//因为要保持相对位置不变，所以排序后不删除元素
    }

    return 0;
}
//统计要计算的方块内各颜色的平均色调
int statisticsBlockInfo(struct PictureInfo* sample , int position)
{
    int i , j , k;
    int cntBlock = 0;

    memset(sample->statisticsColorContent.color , 0 , (gHueLevel * gSaturationLevel * gBrightnessLevel + 6) * sizeof(double));//相应位置清零，留待加和
    //统计
    for(i = 0 ; i < 16 ; i++)
    {
        if((position >> i) & 1)
        {
            for(j = 0 ; j < gHueLevel * gSaturationLevel * gBrightnessLevel + 6 ; j++)
            {
                sample->statisticsColorContent.color[j] += sample->blockColorContent[i].color[j];
            }
            cntBlock++;
        }
    }
    //求平均
    for(k = 0 ; k < gHueLevel * gSaturationLevel * gBrightnessLevel + 6 ; k++)
    {
        sample->statisticsColorContent.color[k] /= cntBlock;
    }
    //对结构中各个颜色分量排序结果写入serialNumber字段中
    sortHue(&sample->statisticsColorContent);

    return 0;
}
//直方图窗口滑动统计
int slidingWindow(struct PictureInfo* sample , int window)
{
    int i = 0 , j = 0 , k = 0;
    int low = 0 , high = window - 1;							//直方图窗口上下端
    int windowSum = 0;
    int windowsMaxSum = 0;
    int maxIndex = 0;
    int* tempData = (int*)malloc(sizeof(int) * (360 + window));	//对H直方图做周期延拓（延window大小）

    for(i = 0 ; i < 16 ; i++)
    {
        //复制直方图到tempData并做window大小的周期延拓
        memmove(tempData , sample->histogramHue[i] , sizeof(int) * 360);
        memmove(&tempData[360] , sample->histogramHue[i] , sizeof(int) * window);

        windowSum = 0;
        low = 0;
        high = window - 1;
        //初始窗口内大小
        for(k = 0 ; k < window ; k++)
        {
            windowSum += tempData[k];
        }
        //初始化最大区域的和及其对应序号
        maxIndex = 0;
        windowsMaxSum = windowSum;

        for(j = 1 ; j < 360 ; j++)
        {
            windowSum += tempData[++high];
            windowSum -= tempData[low++];

            if(windowSum > windowsMaxSum)
            {
                windowsMaxSum = windowSum;
                maxIndex = j;
            }
        }
        sample->windowIndex[i] = maxIndex;
    }

    free(tempData);

    return 0;
}
//计算各色调所占比例
int computeColorScale(char* fileName , struct PictureInfo* info)
{
    int i = 0 , j = 0 , k = 0 , row = 0 , col = 0;
    int width , height , totlePixel;					//图像宽高及总像素数
    int hueLevel , saturationLevel , brightnessLevel;	//色度、饱和、亮度级数
    int brightnessSegmentation , saturationSegmentation;//同色调不同亮度分段，同色调不同亮度分段
    int histogramData;									//直方图数据（0-255）
    float* hsvPixel;
    unsigned char* imagePixel , *lightPixel , *yuvPixel , *pixelRGB;
    IplImage* srcImage , *image , *grayImage , *yuvImage , *normalizedImage , *normalizedHSVImage;

    double scale = 1.0;
    srcImage = cvLoadImage(fileName , 1);
    if(srcImage->height > 500)
    {
        scale = srcImage->height / 500.0;
        image = cvCreateImage(cvSize(srcImage->width / scale , 500) , srcImage->depth , srcImage->nChannels);
        cvResize(srcImage , image , CV_INTER_AREA);
    }
    else
    {
        image = cvCreateImage(cvGetSize(srcImage) , srcImage->depth , srcImage->nChannels);
        cvCopy(srcImage , image);
    }
    grayImage = cvCreateImage(cvGetSize(image) , 8 , 1);
    yuvImage = cvCreateImage(cvGetSize(image) , 8 , 3);
    normalizedImage = cvCreateImage(cvGetSize(image) , 32 , image->nChannels);
    normalizedHSVImage = cvCreateImage(cvGetSize(image) , 32 , image->nChannels);

    cvConvertScale(image , normalizedImage , 1.0/255.0 , 0);		//将原图归一化到0-1之间
    cvCvtColor(normalizedImage , normalizedHSVImage , CV_BGR2HSV);
    cvCvtColor(image , grayImage , CV_BGR2GRAY);
    cvCvtColor(srcImage , yuvImage , CV_RGB2YUV);

    strcpy(info->name , fileName);									//保存文件名
    width = image->width;
    height = image->height;
    totlePixel = width * height;

    //对色度亮度饱和度分级
    hueLevel = gHueLevel;
    brightnessLevel = gBrightnessLevel;
    saturationLevel = gSaturationLevel;

    for(row = 0 ; row < height ; row++)
    {
        for(col = 0 ; col < width ; col++)
        {
            lightPixel = &((unsigned char*)(grayImage->imageData + grayImage->widthStep * row))[col];
            hsvPixel = &((float*)(normalizedHSVImage->imageData + normalizedHSVImage->widthStep * row))[col * 3];
            imagePixel = &((unsigned char*)(image->imageData + image->widthStep * row))[col * 3];
            yuvPixel = &((unsigned char*)(yuvImage->imageData + yuvImage->widthStep * row))[col * 3];
            //直方图数据
            histogramData = (int)(hsvPixel[0] + 0.5);
            info->histogramHue[((4 * col / width) == 4) ? 3 : (4 * col / width) + ((4 * row / height) == 4) ? 12 : (16 * row / height)][histogramData]++;	//将图片分块放入直方图

            //同色调不同饱和度与亮度分段
            brightnessSegmentation = *lightPixel / (256 / brightnessLevel);
            saturationSegmentation = ((int)(hsvPixel[1] * 256)) / (256 / saturationLevel);
            //统计16块的颜色信息，写入相应结构中
            if(imagePixel[0] > 220 && imagePixel[1] > 220 && imagePixel[2] > 220)				//白色 if((hsvPixel[2] > 0.7 && hsvPixel[1] < 0.15)|| (imagePixel[0] > 240 && imagePixel[1] > 240 && imagePixel[2] > 240 && (max(max(imagePixel[0] , imagePixel[1]) , imagePixel[2]) - min(min(imagePixel[0] , imagePixel[1]) , imagePixel[2])) < 5))
            {
                if((hsvPixel[0] >= 0.0 && hsvPixel[0] <= 30.0) || (hsvPixel[0] > 330.0 && hsvPixel[0] <= 360.0))
                {
                    info->blockColorContent[(row * 4 / height) * 4 + (col * 4 / width)].color[hueLevel * saturationLevel * brightnessLevel + RED]++;
                    continue;
                }
                if(hsvPixel[0] > 30.0 && hsvPixel[0] <= 90.0)
                {
                    info->blockColorContent[(row * 4 / height) * 4 + (col * 4 / width)].color[hueLevel * saturationLevel * brightnessLevel + YELLOW]++;
                    continue;
                }
                if(hsvPixel[0] > 90.0 && hsvPixel[0] <= 150.0)
                {
                    info->blockColorContent[(row * 4 / height) * 4 + (col * 4 / width)].color[hueLevel * saturationLevel * brightnessLevel + GREEN]++;
                    continue;
                }
                if(hsvPixel[0] > 150.0 && hsvPixel[0] <= 210.0)
                {
                    info->blockColorContent[(row * 4 / height) * 4 + (col * 4 / width)].color[hueLevel * saturationLevel * brightnessLevel + CYAN]++;
                    continue;
                }
                if(hsvPixel[0] > 210.0 && hsvPixel[0] <= 270.0)
                {
                    info->blockColorContent[(row * 4 / height) * 4 + (col * 4 / width)].color[hueLevel * saturationLevel * brightnessLevel + BLUE]++;
                    continue;
                }
                if(hsvPixel[0] > 270.0 && hsvPixel[0] <= 330.0)
                {
                    info->blockColorContent[(row * 4 / height) * 4 + (col * 4 / width)].color[hueLevel * saturationLevel * brightnessLevel + MAGENTA]++;
                    continue;
                }
            }

            if(yuvPixel[2] >= 77 && yuvPixel[2] < 127 && yuvPixel[1] >= 133 && yuvPixel[1] < 173 && hsvPixel[0] >= 0.0 && hsvPixel[0] <= 30.0 && hsvPixel[1] >= 0.12 && hsvPixel[1] <= 0.77 && hsvPixel[2] >= 0.32)		//皮肤色块(不统计)
            {
                pixelRGB = &((unsigned char*)(image->imageData + image->widthStep * row))[col * 3];
                if((((pixelRGB[2] - pixelRGB[1]) > 15) || ((pixelRGB[2] - pixelRGB[1] < -15))) && (pixelRGB[2] > pixelRGB[1]) && (pixelRGB[2] > pixelRGB[0]) && (MAX(MAX(pixelRGB[0] , pixelRGB[1]) , pixelRGB[2]) - MIN(MIN(pixelRGB[0] , pixelRGB[1]) , pixelRGB[2]) > 15))
                {
                    continue;
                }
            }
            if((hsvPixel[0] >= 0.0 && hsvPixel[0] <= 30.0) || (hsvPixel[0] > 330.0 && hsvPixel[0] <= 360.0))
            {
                info->blockColorContent[(row * 4 / height) * 4 + (col * 4 / width)].color[RED * brightnessLevel * saturationLevel + saturationSegmentation * brightnessLevel + brightnessSegmentation]++;
                continue;
            }
            if(hsvPixel[0] > 30.0 && hsvPixel[0] <= 90.0)											//yellow & green
            {
                if((imagePixel[1] > imagePixel[2]) && (imagePixel[1] > imagePixel[0]))				//green > red & blue
                {
                    info->blockColorContent[(row * 4 / height) * 4 + (col * 4 / width)].color[GREEN * brightnessLevel * saturationLevel + saturationSegmentation * brightnessLevel + brightnessSegmentation]++;
                    continue;
                }
                else
                {
                    info->blockColorContent[(row * 4 / height) * 4 + (col * 4 / width)].color[YELLOW * brightnessLevel * saturationLevel + saturationSegmentation * brightnessLevel + brightnessSegmentation]++;
                    continue;
                }
            }
            if(hsvPixel[0] > 90.0 && hsvPixel[0] <= 150.0)
            {
                info->blockColorContent[(row * 4 / height) * 4 + (col * 4 / width)].color[GREEN * brightnessLevel * saturationLevel + saturationSegmentation * brightnessLevel + brightnessSegmentation]++;
                continue;
            }
            if(hsvPixel[0] > 150.0 && hsvPixel[0] <= 210.0)
            {
                info->blockColorContent[(row * 4 / height) * 4 + (col * 4 / width)].color[CYAN * brightnessLevel * saturationLevel + saturationSegmentation * brightnessLevel + brightnessSegmentation]++;
                continue;
            }
            if(hsvPixel[0] > 210.0 && hsvPixel[0] <= 270.0)
            {
                info->blockColorContent[(row * 4 / height) * 4 + (col * 4 / width)].color[BLUE * brightnessLevel * saturationLevel + saturationSegmentation * brightnessLevel + brightnessSegmentation]++;
                continue;
            }
            if(hsvPixel[0] > 270.0 && hsvPixel[0] <= 330.0)
            {
                info->blockColorContent[(row * 4 / height) * 4 + (col * 4 / width)].color[MAGENTA * brightnessLevel * saturationLevel + saturationSegmentation * brightnessLevel + brightnessSegmentation]++;
                continue;
            }
        }
    }

    for(i = 0 ; i < gCntBlocks ; i++)
    {
        //统计各块的颜色比例
        for(j = 0 ; j < hueLevel * saturationLevel * brightnessLevel + 6 ; j++)
        {
            info->blockColorContent[i].color[j] /= (totlePixel / 16);
        }

        info->histogramHue[i][0] += info->histogramHue[i][360];	//将四舍五入的第361（数据下标为360）维的数据放入第0维
        info->histogramHue[i][360] = 0;
        //直方图归一化
        for(k = 0 ; k < 360 ; k++)
        {
            info->histogramHue[i][k] = info->histogramHue[i][k] * 65536 * 4 / totlePixel;
        }
        //对各块颜色含量进行排序
        sortHue(&info->blockColorContent[i]);
    }

    cvReleaseImage(&srcImage);
    cvReleaseImage(&image);
    cvReleaseImage(&grayImage);
    cvReleaseImage(&yuvImage);
    cvReleaseImage(&normalizedImage);
    cvReleaseImage(&normalizedHSVImage);

    return 0;
}
//向结构体中写入图片信息
bool writeInfoToStruct(char* fileName , struct PictureInfo* newObject , int gWindowSize , int isStatisticalTime)
{
    //统计各块色调比例
    computeColorScale(fileName , newObject);
    //测试统计直方图最大区域
    slidingWindow(newObject , gWindowSize);
    //统计要计算的方块内各颜色的平均色调（15代表前四块）
    statisticsBlockInfo(newObject , gBlock);
    //如果需要统计拍摄时间，则调用函数写入
    if(isStatisticalTime)
    {
        writeTimeToStructure(fileName , newObject);
    }

    return true;
}
//读Json文件
int readJsonFile(char* inputJsonFile , struct PictureInfo* head)
{
    int imageCount = 0;
    char extension[8];
    char fileName[1024];
    char inputPath[1024];
    struct PictureInfo* point = head , *newObject;

    strcpy(inputPath , inputJsonFile);
    inputPath[strlen(inputJsonFile) - 16] = '\0';       //取相对基地址

    QFile descFile(inputJsonFile);

    if(!descFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return -1;
    }
    descFile.seek(0);
    QByteArray fileByteArray = descFile.readAll();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(fileByteArray);
    QVariantMap result = jsonDoc.toVariant().toMap();

    QVariantMap psdDataMap = result["psd"].toMap();

    foreach(QString qstrLayerID , psdDataMap.keys())
    {
        // layer数据
        QVariantMap layerDataMap = psdDataMap[qstrLayerID].toMap();

        if(1 == layerDataMap["type"].toInt())
        {
            char* picture_site;
            QString qPicture_site = layerDataMap["picture_site"].toString();
            QByteArray ba = qPicture_site.toLatin1();
            picture_site = ba.data();
            //判断文件类型
            getFileExtension(picture_site , extension);
            if(0 != strcmp("png" , extension) && 0 != strcmp("jpg" , extension) && 0 != strcmp("jpeg" , extension))
                continue;

            sprintf(fileName , "%s%s" , inputPath , picture_site);
            //新建结构体并初始化
            newObject = createStruct();
            //写入图片所在图层位置
            char* jsonPosition;
            QByteArray tmpNum = qstrLayerID.toLatin1();
            jsonPosition = tmpNum.data();
            strcpy(newObject->jsonPosition , jsonPosition);

            //将图片信息写入结构中
            writeInfoToStruct(fileName , newObject , gWindowSize);

            newObject->prior = point;
            newObject->next = NULL;
            point->next = newObject;
            point = newObject;

            imageCount++;
        }
    }
    descFile.close();

    return imageCount;
}
//比较图片第N大色调
bool comparisonHue(struct PictureInfo* guests , struct PictureInfo* sample , int rankingArea)
{
    for(int i = 0 ; i < rankingArea + 1 ; i++)
    {
        if(guests->statisticsColorContent.serialNumber[i] != sample->statisticsColorContent.serialNumber[i])
            return false;
    }
    return true;
}
//比较最大颜色分布范围
bool comparisonMaxConnectionAreaHue(struct PictureInfo* guests , struct PictureInfo* sample)
{
    int i = 0;
    int isRight = 0;
    //测试，目前比较前四块
    for(i = 0 ; i < 4 ; i++)
    {
        if(abs(guests->windowIndex[i] - sample->windowIndex[i]) >= (gWindowSize / 2))	//两个最大连接区域无交集
            ;
        else
            isRight++;
    }

    if(isRight >= 4)
        return true;
    else
        return false;
}
//卡方法比较直方图
int compareHistogram(struct PictureInfo* sample , struct PictureInfo* pendingInfo)
{
    double result[16] = {0.0};
    for(int i = 0 ; i < 16 ; i++)
    {
        for(int j = 0 ; j < 360 ; j++)
        {
            double v1 = sample->histogramHue[i][j];
            double v2 = pendingInfo->histogramHue[i][j];

            if((v1 + v2) != 0)
                result[i] += (fabs(v1 - v2) / (v1 + v2));
        }
    }

    pendingInfo->similarity = 0.0;											//相似度统计
    for(int i = 0 ; i < 16 ; i++)
        pendingInfo->similarity += (result[i] / 16);

    pendingInfo->similarity = 100 - pendingInfo->similarity;

    return 0;
}
//写JSON文件
QByteArray writeInfoToJSONFile(struct PictureInfo*guestsPhoto , int guestsPhotoCount , struct PsdInfo* sampleTemplate  , int sampleTemplateCount)
{
    int i = 0 , j = 0 , k = 0 , p = 0;
    struct PictureInfo* pointGuestsPhoto = NULL;
    struct PictureInfo* maxPicInfo = NULL;                       //得分匹配值最大的一个
    struct PsdInfo* maxPsdInfo = NULL;                       //得分匹配值最大的一个PSD
    struct PsdInfo* pointSampleTemplate = NULL;
    struct PictureInfo* pointSamplePhoto = NULL;

    QVariantMap theme , layer_dataMap , numMap , psdPageMap;
    QJsonDocument json;
    QByteArray jsonByte;

    //客/样照的第一个结点
    pointGuestsPhoto = guestsPhoto->next;
    pointSampleTemplate = sampleTemplate->next;
    //每张客照去找相应的照片
    for(i = 0 ; i < guestsPhotoCount ; i++)
    {
        //默认指向第一个结点（第一个PSD文件）
        pointSampleTemplate = sampleTemplate->next;
        //最大相似图片默认为第一个PSD的第一张
        maxPicInfo = pointSampleTemplate->LayerHead.next;
        //最大相似图片默认对应第一个PSD文件
        maxPsdInfo = pointSampleTemplate;
        //逐个PSD遍历
        for(j = 0 ; j < sampleTemplateCount ; j++)
        {
            //第三层的活动指针
            pointSamplePhoto = pointSampleTemplate->LayerHead.next;
            //逐个图层遍历
            for(k = 0 ; k < pointSampleTemplate->LayerCount ; k++)
            {
                //如果两张图片不在同一颜色区域内
                if(false == comparisonHue(pointGuestsPhoto , pointSamplePhoto , FIRST))
                {
                    pointSamplePhoto = pointSamplePhoto->next;
                    continue;
                }
                if(false == comparisonMaxConnectionAreaHue(pointGuestsPhoto , pointSamplePhoto))
                {
                    pointSamplePhoto = pointSamplePhoto->next;
                    continue;
                }
                //比较与保存得出结果，直方图比较
                compareHistogram(pointGuestsPhoto , pointSamplePhoto);
                //记录最相似的值
                if(maxPicInfo->similarity < pointSamplePhoto->similarity)
                {
                    maxPicInfo = pointSamplePhoto;
                    maxPsdInfo = pointSampleTemplate;
                }

                pointSamplePhoto = pointSamplePhoto->next;
            }
            //模板下一张psd
            pointSampleTemplate = pointSampleTemplate->next;
        }

        //每一张客照找样照结束，写入Json文件
        QVariantMap layerMap;
        PictureInfo* writeInfo = maxPsdInfo->LayerHead.next;
        for(p = 0 ; p < maxPsdInfo->LayerCount ; p++)
        {
            QVariantMap pathMap;
            QString qResultFilePath = QString::fromLocal8Bit(pointGuestsPhoto->name);
            pathMap.insert("path" , qResultFilePath);                   //Path:XXX
            layerMap.insert(writeInfo->jsonPosition , pathMap);         //"4":path

            writeInfo = writeInfo->next;
            guestsPhotoCount--;       //匹配客照后相应客照链减1
            if(pointGuestsPhoto->next)//如果客照没结尾
            {
                pointGuestsPhoto = pointGuestsPhoto->next;
            }
        }
        //
        numMap.insert("template_path" , maxPsdInfo->psdFileName);//"layer_data":"F:\xxx.psd"
        numMap.insert("layer_data" , layerMap);

        QString qstrIndex = QString::number(i);
        psdPageMap.insert(qstrIndex , numMap);

        maxPsdInfo->prior->next = maxPsdInfo->next;

        //如果maxPsdInfo为尾结点（代表模板已经用光，直接跳出）
        if(NULL == maxPsdInfo->next)
            break;
        else
            maxPsdInfo->next->prior = maxPsdInfo->prior;

        sampleTemplateCount--;//模板数也相应减1

        free(maxPsdInfo);
    }

    theme.insert("theme" , psdPageMap);

    json = QJsonDocument::fromVariant(theme);
    jsonByte = json.toJson();

    return jsonByte;
}
//扫描模板库函数
int traverseTemplateLibrary(char* tmplateLibraryPath , struct PsdInfo* head)
{
    int psdCount = 0;
    long handle;
    char to_search[1024];
    char inputJsonFile[1024];
    struct _finddata_t fileinfo;
    struct PsdInfo* pstrPointer = head , *newPSDObject = NULL;
    //查找用
    sprintf(to_search , "%s\\*" , tmplateLibraryPath);
    //第一次查找
    handle = _findfirst(to_search , &fileinfo);

    if(-1 == handle)
    {
        printf("查找失败或无文件！\n");
        return -1;
    }

    do{
        //如果是文件夹
        if(strcmp("." , fileinfo.name) && strcmp(".." , fileinfo.name) && 16 == fileinfo.attrib)
        {
            sprintf(inputJsonFile , "%s\\%s\\psd_release.desc" , tmplateLibraryPath , fileinfo.name);
            //如果描述文件信息不存在
            if(access(inputJsonFile , 0))
                continue;
            //创建新的psd信息结点
            newPSDObject = (struct PsdInfo*)malloc(sizeof(struct PsdInfo));
            //读单个Json文件
            newPSDObject->LayerCount = readJsonFile(inputJsonFile , &newPSDObject->LayerHead);
            //写入PSD文件完整路径
            sprintf(newPSDObject->psdFileName , "%s\\%s" , tmplateLibraryPath , fileinfo.name);
            //指向下一个
            pstrPointer->next = newPSDObject;
            newPSDObject->prior = pstrPointer;
            newPSDObject->next = NULL;
            pstrPointer = newPSDObject;

            psdCount++;
        }
        else
            continue;
    }while(!_findnext(handle , &fileinfo));//循环查找其他符合的文件,直到找不到其他的为止

    _findclose(handle);

    return psdCount;
}
//读入客照并写入结构中
int readGuestsPhotos(QStringList guests , struct PictureInfo* head)
{
    int isStatisticalTime = 1;
    char fileName[1024];
    int imageCount = guests.size();
    struct PictureInfo* newPicObject , *pointer = head;

    for(int i = 0 ; i < imageCount ; i++)
    {
        QByteArray qFileName = guests[i].toLocal8Bit();
        char* guestsFilename = qFileName.data();

        strcpy(fileName , guestsFilename);
        //新建结构体并初始化
        newPicObject = createStruct();
        //写入图片所在图层位置(客照默认为-1)
        strcpy(newPicObject->jsonPosition , "-1");
        //将图片信息写入结构中
        writeInfoToStruct(fileName , newPicObject , gWindowSize , isStatisticalTime);

        newPicObject->prior = pointer;
        newPicObject->next = NULL;
        pointer->next = newPicObject;
        pointer = newPicObject;
    }

    return imageCount;
}
//比较背景相似度函数
QByteArray intelligentDesign(QStringList guests , char* templatePath)
{
    int sampleTemplateCount = 0 , guestsPhotoCount = 0 , sceneCount = 0;
    struct PictureInfo guestsPhoto;
    struct SceneInfo category[16];
    struct PsdInfo sampleTemplate;

    //扫描模板库与客照链表
    sampleTemplateCount = traverseTemplateLibrary(templatePath , &sampleTemplate);
    guestsPhotoCount = readGuestsPhotos(guests , &guestsPhoto);

    //对样照进行按时间排序
    sortPhotographTime(&guestsPhoto);
    //照片按时间分类
    //sceneCount = photographTimeClassify(category , &guestsPhoto , guestsPhotoCount);
    //相同时间段内不同场景的区分
    //sameTimeDifferentSceneClassify(category , sceneCount);

    QByteArray result = writeInfoToJSONFile(&guestsPhoto , guestsPhotoCount , &sampleTemplate , sampleTemplateCount);

    return result;
}
