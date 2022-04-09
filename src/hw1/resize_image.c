#include <math.h>
#include "image.h"

float nn_interpolate(image im, float x, float y, int c)
{
    int x_new = roundf(x);
    int y_new = roundf(y);

    return get_pixel(im, x_new, y_new, c);
}

image nn_resize(image im, int w, int h)
{
    image resized = make_image(w, h, im.c);
    float xa =(float) im.w / w;
    float ya = (float)im.h / h;
    float xb = xa * 0.5 - 0.5;
    float yb = ya * 0.5 - 0.5;
    for(int c = 0; c < im.c; c++) {
        for(int y = 0; y < h; y++) {
            for(int x = 0; x < w; x++) {
                float x_new = xa * x + xb;
                float y_new = ya * y + yb;
                float val = nn_interpolate(im, x_new, y_new, c);
                set_pixel(resized, x, y, c, val);
            }
        }
    }
    return resized;
}

float bilinear_interpolate(image im, float x, float y, int c)
{
    int top_x = ceil(x);
    int top_y = ceil(y);
    int bottom_x = floor(x);
    int bottom_y = floor(y);

    float v1 = get_pixel(im, bottom_x, bottom_y, c);
    float v2 = get_pixel(im, top_x, bottom_y, c);
    float v3 = get_pixel(im, bottom_x, top_y, c);
    float v4 = get_pixel(im, top_x, top_y, c);

    float d1 = x - bottom_x;
    float d2 = top_x - x;
    float d3 = y - bottom_y;
    float d4 = top_y - y;

    float q1 = v1 * d2 + v2 * d1;
    float q2 = v3 * d2 + v4 * d1;

    float q = q1 * d4 + q2 * d3;
    return q;

}

image bilinear_resize(image im, int w, int h)
{
    image resized = make_image(w, h, im.c);
    float xa =(float) im.w / w;
    float ya = (float)im.h / h;
    float xb = xa * 0.5 - 0.5;
    float yb = ya * 0.5 - 0.5;
    for(int c = 0; c < im.c; c++) {
        for(int y = 0; y < h; y++) {
            for(int x = 0; x < w; x++) {
                float x_new = xa * x + xb;
                float y_new = ya * y + yb;
                float val = bilinear_interpolate(im, x_new, y_new, c);
                set_pixel(resized, x, y, c, val);
            }
        }
    }
    return resized;
}

