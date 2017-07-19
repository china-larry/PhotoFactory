// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� TEMPLATE_EDITOR_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// TEMPLATE_EDITOR_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
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

// �����Ǵ� template_editor.dll ������
class DLL_EXPORT Ctemplate_editor {
public:
	Ctemplate_editor(void);
	// TODO: �ڴ�������ķ�����
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
														int         mask_edit,      // 1Ϊ�༭�ɰ棬0Ϊ���༭
														IplImage *  mask_image      // �༭���ɰ��opencv����
														);
DLL_EXPORT void free_editorpata(_editor_parameter *editor_para);
DLL_EXPORT IplImage* image_blend(_psd_context *context,char *project_path ,_editor_parameter *editor_para);
DLL_EXPORT int image_free(_psd_context *context);

//#endif // _TEMPLATE_EDITOR_H_
