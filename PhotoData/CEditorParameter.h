/**
 * 合成图层接口数据，传递图层那些数据项修改操作
 *
 */
#ifndef CEDITORPARAMETER_H
#define CEDITORPARAMETER_H

#include <opencv/cv.h>

class CEditorParameter
{
public:
    CEditorParameter();
public:
    void set_layer_num(int layer_num);
    void set_layer_or_photo(int layer_or_photo);
    void set_x_move(int x_move);
    void set_y_move(int y_move);
    void set_layer_x_move(int layer_x_move);
    void set_layer_y_move(int layer_y_move);
    void set_blend_offset_y(int blend_offset_y);
    void set_blend_offset_x(int blend_offset_x);
    void set_blend_width(int blend_width);
    void set_blend_height(int blend_height);
    void set_zoom_scale(float zoom_scale);
    void set_rotation(int rotation);
    void set_blend_mode(int blend_mode);
    void set_opacity(int opacity);
    void set_load_customer_image(int load_customer_image);
  //  void set_customer_image_path(char* customer_image_path);
    void set_customer_image(IplImage* pImage);
    void set_visable(int visable[]);
    void set_mask_edit(int mask_edit);
    void set_mask_image(IplImage* pImage);
public:
    int			layer_num;      // 操作的图层号 0、1、2...
    int        layer_or_photo; // 对图层操作为1，对人像照片操作为0
    int			x_move;         // 人像照片的移动位置 x ，默认值为0
    int			y_move;		    // 人像照片的移动位置 y ，默认值为0
    int			layer_x_move;   // 图层的移动位置 x ，默认值为0
    int			layer_y_move;   // 图层的移动位置 y ，默认值为0

    int         blend_offset_y; // 操作区域的左上角坐标 y
    int         blend_offset_x;	// 操作区域的左上角坐标 x
    int			blend_width;    // 操作区域的宽度
    int			blend_height;   // 操作区域的高度

    float		zoom_scale;	    // 缩放比例（与原图相比的比例）, 默认值为1
    int			rotation;       // 旋转角度   0...360  实际角度，非增量值
    int			blend_mode;     // 混合模式   0、1、2...
    int			opacity;	    // 透明度	  0...255

    int         load_customer_image; // 载入客照时为1，否则为0
//    char *    customer_image_path; // 载入客照路径
    IplImage* customer_image;
    int			 *visable;	    // 所有图层的可视情况
    int             mask_edit;     // 1为编辑蒙版，0为不编辑
    IplImage*   mask_image;     // 编辑后蒙版的opencv对象
};

#endif // CEDITORPARAMETER_H
