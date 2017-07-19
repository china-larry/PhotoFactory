#ifndef EXIF_H
#define EXIF_H

#include <fstream>
#include <vector>

using namespace std;

typedef unsigned char   BYTE;
typedef unsigned short   WORD;
typedef unsigned long   DWORD;

// 表示一个无符号有理数
struct Rational
{
    DWORD a;   // 分子
    DWORD b;   // 分母
};

// 表示一个有符号有理数
struct SRational
{
    long a;   // 分子
    long b;   // 分母
};

// JPEG文件信息
struct JPEGInfo
{
    string    description; // 图像描述
    string    maker;    // 制造厂商
    string    model;    // 照相机型号
    Rational   xResolution; // x分辨率
    Rational   yResolution; // y分辨率
    WORD    resolutionUnit; // 分辨率单位, 参数含义详见TIFF标准
    string    software;   // 创建该图像的软件名和软件版本
    string    createTime;   // 创建时间,格式为YYYY:MM:DD HH:MM:SS
    WORD    YCbCrPosition; // YCbCr位置, 参数含义详见TIFF标准
    string    copyright;   // 版权信息
    WORD    compression; // 压缩信息, 参数含义详见TIFF标准
    Rational   exposureTime; // 曝光时间
    Rational   fNumber;   // F-值, 例如F2.8
    WORD    exposureProgram;// 曝光设定
    WORD    ISOSpeedRatings;// ISO速率
    string    orgTime;   // 拍摄时间
    string    digTime;   // 图像被软件修改的时间
    Rational   compressBit; // 每像素压缩位数
    SRational   shutterSpeed; // 快门速度，例如1/30秒
    Rational   aperture;   // 光圈值, 例如F2.8
    SRational   exposureBias; // 曝光补偿值
    Rational   maxAperture; // 最大光圈
    WORD    meteringMode; // 测光模式
    WORD    lightSource; // 光源
    WORD    flash;    // 闪光灯
    Rational   focalLength; // 焦距
    WORD    colorSpace;   // 色彩空间
    DWORD    width;    // 图像宽度
    DWORD    height;    // 图像高度
    Rational   exposureIndex; // 曝光指数
    WORD    sensingMethod; // 感光方式
};

struct TIFDEntry
{
    WORD tag;   // Tag的含义可以查阅TIFF的规范文档
    WORD type;   // 指明此Entry中记录的数据类型，TIFF规范只定义了五种类型，EXIF增加了三种
    DWORD size;   // 大小
    DWORD val;   // 取值, 根据type含义会改变
};

class ExifReader
{
public:
    //ExifReader(std::string filePath);
    ExifReader(std::string filePath);
    ~ExifReader();

    /******************************************************************************
    * 作用:   获取JPEG的Exif信息
    * 返回值:   如果返回值为NULL则表示当前JPEG文件不含有Exif信息
    * 备注:   使用者需要调用delete
    ******************************************************************************/
    JPEGInfo* getJPEGInfo(void);

private:
    void   initJPEGInfo(void);
    bool   readSOI(void);
    bool   readAPP0(void);
    bool   readExif(void);
    void   getString(char* out, int size);
    void   getString(char* out, int size, int offset);
    WORD   stringToWORD(const char* str);
    DWORD   stringToDWORD(const char* str);
    TIFDEntry* createFromIndex(int offset);
    void   readIFD(int offset);
    void   analyseTIFD(TIFDEntry* entry);

    string   getEntryASCII(TIFDEntry* entry);
    Rational getEntryRational(TIFDEntry* entry);
    SRational getEntrySRational(TIFDEntry* entry);
    string   getEntryUndefined(TIFDEntry* entry);

    bool    jpegFault;
    bool    APP0Fault;
    bool    ExifFault;

    ifstream in;
    int     index;    // 记录当前指针(stringData中使用)
    char*    stringData;   // 已经读取的字符串内容
    bool    isLittleEndian; // 低字节是否在前
    vector<TIFDEntry*> entries; // 保存TIFD信息, 详见TIFF标准
    JPEGInfo   info;    // JPEG文件信息
};

#endif // EXIF_H
