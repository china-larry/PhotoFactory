// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 TEMPLATE_EDITOR_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// TEMPLATE_EDITOR_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
//#ifdef _TEMPLATE_EDITOR_H_
//#define _TEMPLATE_EDITOR_H_


#ifdef BUILD_DLL
    #define DLL_EXPORT __declspec(dllexport)
#else
    #define DLL_EXPORT __declspec(dllimport)
#endif

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/imgproc/imgproc_c.h>

// 此类是从 template_editor.dll 导出的
class DLL_EXPORT Ctemplate_editor {
public:
	Ctemplate_editor(void);
	// TODO: 在此添加您的方法。
};

extern DLL_EXPORT int ntemplate_editor;
typedef struct _psd_context;
typedef struct _editor_parameter;

DLL_EXPORT int fntemplate_editor(void);
DLL_EXPORT _psd_context *quickrebuild(char * project_path);
DLL_EXPORT _editor_parameter* create_editorpata
														(
														int layer_num,
														int layer_or_photo,
														int x_move,
														int			y_move,
														int			layer_x_move,
														int			layer_y_move,

														int         blend_offset_y,
														int         blend_offset_x,
														int			blend_width,
														int			blend_height,

														float		zoom_scale,
														int			rotation,
														int			blend_mode,
														int			opacity,

														int         load_customer_image,
														IplImage *customer_image,
														int	*		visable,
														int         mask_edit,      // 1为编辑蒙版，0为不编辑
														IplImage *  mask_image      // 编辑后蒙版的opencv对象
														);
DLL_EXPORT void free_editorpata(_editor_parameter *editor_para);
DLL_EXPORT IplImage* image_blend(_psd_context *context,char *project_path ,_editor_parameter *editor_para);
DLL_EXPORT int image_free(_psd_context *context);

//#endif // _TEMPLATE_EDITOR_H_
