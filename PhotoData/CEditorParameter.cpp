#include "CEditorParameter.h"

CEditorParameter::CEditorParameter()
    : layer_num(0)
    , layer_or_photo(0)
    , x_move(0)
    , y_move(0)
    , layer_x_move(0)
    , layer_y_move(0)
    , blend_offset_y(0)
    , blend_offset_x(0)
    , blend_width(0)
    , blend_height(0)
    , zoom_scale(1.0f)
    , rotation(0)
    , blend_mode(0)
    , opacity(0)
    , load_customer_image(0)
    , customer_image(NULL)
    , mask_edit(0)
    , mask_image(NULL)
{    
}

void CEditorParameter::set_layer_num(int layer_num)
{
    this->layer_num = layer_num;
}

void CEditorParameter::set_layer_or_photo(int layer_or_photo)
{
    this->layer_or_photo = layer_or_photo;
}

void CEditorParameter::set_x_move(int x_move)
{
    this->x_move = x_move;
}

void CEditorParameter::set_y_move(int y_move)
{
    this->y_move = y_move;
}
void CEditorParameter::set_layer_x_move(int layer_x_move)
{
    this->layer_x_move = layer_x_move;
}

void CEditorParameter::set_layer_y_move(int layer_y_move)
{
    this->layer_y_move = layer_y_move;
}

void CEditorParameter::set_blend_offset_y(int blend_offset_y)
{
    this->blend_offset_y = blend_offset_y;
}

void CEditorParameter::set_blend_offset_x(int blend_offset_x)
{
    this->blend_offset_x = blend_offset_x;
}

void CEditorParameter::set_blend_width(int blend_width)
{
    this->blend_width = blend_width;
}

void CEditorParameter::set_blend_height(int blend_height)
{
    this->blend_height = blend_height;
}

void CEditorParameter::set_zoom_scale(float zoom_scale)
{
    this->zoom_scale = zoom_scale;
}

void CEditorParameter::set_rotation(int rotation)
{
    this->rotation = rotation;
}

void CEditorParameter::set_blend_mode(int blend_mode)
{
    this->blend_mode = blend_mode;
}

void CEditorParameter::set_opacity(int opacity)
{
    this->opacity = opacity;
}

void CEditorParameter::set_load_customer_image(int load_customer_image)
{
    this->load_customer_image = load_customer_image;
}

void CEditorParameter::set_customer_image(IplImage *pImage)
{
    this->customer_image = pImage;
}


void CEditorParameter::set_visable(int *visable)
{
    this->visable = visable;
}

void CEditorParameter::set_mask_edit(int mask_edit)
{
    this->mask_edit = mask_edit;
}

void CEditorParameter::set_mask_image(IplImage *pImage)
{
    this->mask_image = pImage;
}
