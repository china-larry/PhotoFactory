#include "EXIF.h"

#define SOI  0xD8 // 图像开始
#define APP0 0xE0 // JFIF应用数据块
#define APP1 0xE1 // Exif数据块(APP1)

#define MAX_WORD 65535

ExifReader::ExifReader(string filePath) : in(filePath.c_str(), ios_base::in | ios_base::binary)
{
    jpegFault = false;
    APP0Fault = false;
    ExifFault = false;

    index = 0;
    stringData = NULL;
    isLittleEndian = true;

    initJPEGInfo();

    if (!readSOI())
    {
       jpegFault = true;
       return;
    }

    if (!readAPP0())
       APP0Fault = true;

    if (!readExif())
       ExifFault = true;

    in.close();
}

void ExifReader::initJPEGInfo(void)
{
    info.xResolution.a = info.xResolution.b = 0;
    info.yResolution.a = info.yResolution.b = 0;
    info.resolutionUnit = 0;
    info.YCbCrPosition = 0;
    info.compression = 0;
    info.exposureTime.a = info.exposureTime.b = 0;
    info.fNumber.a = info.fNumber.b = 0;
    info.exposureProgram = 0;
    info.ISOSpeedRatings = 0;
    info.compressBit.a = info.compressBit.b = 0;
    info.shutterSpeed.a = info.shutterSpeed.b = 0;
    info.aperture.a = info.aperture.b = 0;
    info.exposureBias.a = info.exposureBias.b = 0;
    info.maxAperture.a = info.maxAperture.b = 0;
    info.meteringMode = 0;
    info.lightSource = 0;
    info.flash = 0;
    info.focalLength.a = info.focalLength.b = 0;
    info.colorSpace = 0;
    info.width = 0;
    info.height = 0;
    info.exposureIndex.a = info.exposureIndex.b = 0;
    info.sensingMethod = 0;
}

ExifReader::~ExifReader()
{
    for (unsigned int i = 0; i < entries.size(); i++)
       delete entries[i];

    delete[] stringData;
}

JPEGInfo* ExifReader::getJPEGInfo(void)
{
    if (jpegFault || ExifFault)
       return NULL;

    return new JPEGInfo(info);
}

WORD ExifReader::stringToWORD(const char* str)
{
    if (isLittleEndian)
       return ((BYTE)str[1] << 8) + (BYTE)str[0];
    else
       return ((BYTE)str[0] << 8) + (BYTE)str[1];
}

DWORD ExifReader::stringToDWORD(const char* str)
{
    if (isLittleEndian)
    {
       return ((BYTE)str[3] << 24) + ((BYTE)str[2] << 16) +
         ((BYTE)str[1] << 8) + (BYTE)str[0];
    }
    else
    {
       return ((BYTE)str[0] << 24) + ((BYTE)str[1] << 16) +
         ((BYTE)str[2] << 8) + (BYTE)str[3];
    }
}


// 读取SOI段
bool ExifReader::readSOI(void)
{
    char title[2];

    in.read(title, 2);
    if ((BYTE)title[0] != 0xFF || (BYTE)title[1] != SOI)
       return false;
    else
       return true;
}


// 读取APP0段
bool ExifReader::readAPP0(void)
{
    char len[2];
    char title[2];

    in.read(title, 2);
    if ((BYTE)title[0] != 0xFF || (BYTE)title[1] != APP0)
    {
       in.seekg(-2, ios_base::cur );
       return false;
    }

    int   length = 0; // APP0段长度
    char* data = NULL;

    in.read(len, 2);
    length = ((BYTE)len[0] << 8) + len[1];
    data = new char[length - 2];
    in.read(data, length - 2);

    // 处理APP0的数据(TIFF格式)
    // ......

    delete[] data;

    return true;
}


// 读取APP1段
bool ExifReader::readExif(void)
{
    char len[2];
    char title[2];

    in.read(title, 2);
    if ((BYTE)title[0] != 0xFF || (BYTE)title[1] != APP1)
       return false;

    int   length = 0; // APP1段长度

    in.read(len, 2);
    length = ((BYTE)len[0] << 8) + (BYTE)len[1];

    if (stringData != NULL)
       delete[] stringData;

    stringData = new char[length - 2];
    in.read(stringData, length - 2);

    char data[6];
    getString(data, 6);   // 获得EXIF Header

    // TIFF Image File Header开始

    getString(data, 2, 0);   // 读取字节顺序方式
    if (data[0] == 'I' && data[1] == 'I')
       isLittleEndian = true;
    else
       isLittleEndian = false;

    getString(data, 2, 2);   // Flag(0x2A)
    getString(data, 4, 4);   // 第一个IFD的偏移量

    DWORD offset = stringToDWORD(data);

    readIFD(offset);

    TIFDEntry* entry = NULL;
    while (entries.size() != 0)
    {
       entry = entries[entries.size() - 1];
       entries.pop_back();
       analyseTIFD(entry);
       delete entry;
    }

    return true;
}

void ExifReader::getString(char* out, int size)
{
    memcpy(out, stringData + index, size);
    index += size;
}

void ExifReader::getString(char* out, int size, int offset)
{
    memcpy(out, stringData + index + offset, size);
}

TIFDEntry* ExifReader::createFromIndex(int offset)
{
    TIFDEntry* entry = new TIFDEntry;
    char data[4];

    getString(data, 2, offset);
    entry->tag = stringToWORD(data);

    getString(data, 2, offset + 2);
        entry->type = stringToWORD(data);
    getString(data, 4, offset + 4);
        entry->size = stringToDWORD(data);
    getString(data, 4, offset + 8);
        entry->val = stringToDWORD(data);

    return entry;
}

void ExifReader::readIFD(int offset)
{
    char data[6];

    getString(data, 2, offset); // Entry个数

    WORD numOfIFD = stringToWORD(data);

    for (int i = 0; i < numOfIFD; i++)
    {
       entries.push_back(createFromIndex(offset + 2 + 12*i));
    }

    getString(data, 4, offset + 2 + 12 * numOfIFD);

    DWORD offsetOfNext = stringToDWORD(data);

    if (offsetOfNext != 0)
       readIFD(offsetOfNext);
}

string ExifReader::getEntryASCII(TIFDEntry* entry)
{
    string ret;

    if (entry->type != 2)
       return ret;

    char* des = new char[entry->size];

    getString(des, entry->size, entry->val);
    ret = des;
    delete des;
    return ret;
}

string ExifReader::getEntryUndefined(TIFDEntry* entry)
{
    string ret;

    if (entry->type != 7)
       return ret;

    char* des = new char[entry->size];

    if (entry->size > 4)
    {
       getString(des, entry->size, entry->val);
       ret = des;
    }

    delete des;
    return ret;
}

SRational ExifReader::getEntrySRational(TIFDEntry* entry)
{
    SRational ret;
    ret.a = 0;
    ret.b = 1;

    if (entry->type != 10)
       return ret;

    char data[4];

    getString(data, 4, entry->val);
    ret.a = (data[3] << 24) + (data[2] << 16) + (data[1] << 8) + data[0];
    getString(data, 4, entry->val + 4);
    ret.b = (data[3] << 24) + (data[2] << 16) + (data[1] << 8) + data[0];
    return ret;
}

Rational ExifReader::getEntryRational(TIFDEntry* entry)
{
    Rational ret;
    ret.a = 0;
    ret.b = 1;

    if (entry->type != 5)
       return ret;

    char data[4];

    getString(data, 4, entry->val);
    ret.a = stringToDWORD(data);
    getString(data, 4, entry->val + 4);
    ret.b = stringToDWORD(data);
    return ret;
}

void ExifReader::analyseTIFD(TIFDEntry* entry)
{
    switch (entry->tag)
    {
    case 0x010E:   // 图像说明
       info.description = getEntryASCII(entry);
       break;

    case 0x010F:   // 制造厂商
       info.maker = getEntryASCII(entry);
       break;

    case 0x0110:   // 型号
       info.model = getEntryASCII(entry);
       break;

    case 0x011A:   // x分辩率
       info.xResolution = getEntryRational(entry);
       break;

    case 0x011B:   // y分辩率
       info.yResolution = getEntryRational(entry);
       break;

    case 0x0128:   // 分辩率单位
       info.resolutionUnit = (WORD)entry->val;
       break;

    case 0x0131:   // 创建软件名称
       info.software = getEntryASCII(entry);
       break;

    case 0x0132:   // 创建时间
       info.createTime = getEntryASCII(entry);
       break;

    case 0x0213:   // YCbCr位置
       info.YCbCrPosition = (WORD)entry->val;
       break;

    case 0x8298:   // 版权信息
       info.copyright = getEntryASCII(entry);
       break;

    case 0x8769:   // Exif末尾
       readIFD(entry->val);
       break;

    case 0x0103:   // 压缩信息
       info.compression = (WORD)entry->val;
       break;

    case 0x829A:   // 曝光时间
       info.exposureTime = getEntryRational(entry);
       break;

    case 0x829D:   // F-值
       info.fNumber = getEntryRational(entry);
       break;

    case 0x8822:   // 曝光设定
       info.exposureProgram = (WORD)entry->val;
       break;

    case 0x8827:   // ISO速率
       info.ISOSpeedRatings = (WORD)entry->val;
       break;

    case 0x9003:   // 拍摄时间
       info.orgTime = getEntryASCII(entry);
       break;

    case 0x9004:   // 被软件修改的时间
       info.digTime = getEntryASCII(entry);
       break;

    case 0x9102:   // 每像素压缩位数
       info.compressBit = getEntryRational(entry);
       break;

    case 0x9201:   // 快门速度
       info.shutterSpeed = getEntrySRational(entry);
       break;

    case 0x9202:   // 光圈值
       info.aperture = getEntryRational(entry);
       break;

    case 0x9204:   // 曝光补偿值
       info.exposureBias = getEntrySRational(entry);
       break;

    case 0x9205:   // 最大光圈
       info.maxAperture = getEntryRational(entry);
       break;

    case 0x9207:   // 测光模式
       info.meteringMode = entry->val;
       break;

    case 0x9208:   // 光源
       info.lightSource = entry->val;
       break;

    case 0x9209:   // 闪光灯
       info.flash = entry->val;
       break;

    case 0x920a:   // 焦距
       info.focalLength = getEntryRational(entry);
       break;

    /* case 0x927c:
       makerNote = getEntryUndefined(entry);
       cout << makerNote << endl;
       break;*/

    case 0xa001:   // 色彩空间
       info.colorSpace = (WORD)entry->val;
       break;

    case 0xa002:   // Exif宽度
       info.width = entry->val;
       break;

    case 0xa003:   // Exif高度
       info.height = entry->val;
       break;

    case 0xa215:   // 曝光指数
       info.exposureIndex = getEntryRational(entry);
       break;

    case 0xa217:
       info.sensingMethod = entry->val;
       break;

    }
}

