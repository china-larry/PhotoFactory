#ifndef INTELLIGENTDESIGNDLL_H
#define INTELLIGENTDESIGNDLL_H

#include <QString>
#include <QStringList>

#define BUILD_DLL

#ifdef BUILD_DLL
#define BUILD_DLL __declspec(dllexport)
#else
#define BUILD_DLL __declspec(dllimport)
#endif

//智能设计函数
BUILD_DLL QByteArray IntelligentDesign(QStringList qlistPhotoPath , QString qstrTemplatePath);

#endif // INTELLIGENTDESIGNDLL_H

