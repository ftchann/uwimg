#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "image.h"


int adjust_dim(int a, int d)
{
    if (a < 0) return 0;
    if (a >= d) return d - 1;
    return a;
}

float get_pixel(image im, int x, int y, int c)
{
    int x_new = adjust_dim(x, im.w);
    int y_new = adjust_dim(y, im.h);
    int index = x_new + im.w * y_new + im.w * im.h * c;
    return im.data[index];
}

void set_pixel(image im, int x, int y, int c, float v)
{
    if (x < 0 || y < 0 || c < 0 || x >= im.w || y >= im.h || c >= im.c) return;
    int index = x + im.w*y + im.w*im.h*c;
    im.data[index] = v;
}

image copy_image(image im)
{
    image copy = make_image(im.w, im.h, im.c);
    memcpy(copy.data, im.data, im.h*im.w*im.c*sizeof(float));
    return copy;
}

image rgb_to_grayscale(image im)
{
    assert(im.c == 3);
    image gray = make_image(im.w, im.h, 1);
    for (int y = 0; y < im.h; ++y) {
        for (int x = 0; x < im.w; ++x) {
            float r = get_pixel(im, x, y, 0);
            float g = get_pixel(im, x, y, 1);
            float b = get_pixel(im, x, y, 2);
            float gray_value = 0.299 * r + 0.587 * g + 0.114 * b;
            set_pixel(gray, x, y, 0, gray_value);
        }
    }

    return gray;
}

void shift_image(image im, int c, float v)
{
    for (int y = 0; y < im.h; ++y) {
        for (int x = 0; x < im.w; ++x) {
            int index = x + im.w * y+ im.w * im.h * c;
            im.data[index] += v;
        }
    }
}

void clamp_image(image im)
{
    for (int k = 0; k < im.c; ++k) {
        for (int y = 0; y < im.h; ++y) {
            for (int x = 0; x < im.w; ++x) {
                int index = x + im.w * y+ im.w * im.h * k;
                float value = im.data[index];
                if (value < 0) im.data[index] = 0;
                if (value > 1) im.data[index] = 1;
            }
        }
    }
}


// These might be handy
float three_way_max(float a, float b, float c)
{
    return (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

float three_way_min(float a, float b, float c)
{
    return (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

void rgb_to_hsv(image im)
{
    float r, g, b, h, s, v, m, c, ha;
    for (int y = 0; y < im.h; ++y) {
        for (int x = 0; x < im.w; ++x) {
            r = get_pixel(im, x, y, 0);
            g = get_pixel(im, x, y, 1);
            b = get_pixel(im, x, y, 2);

            v = three_way_max(r,g,b);
            m = three_way_min(r,g,b);
            c = v - m;
            s = (v == 0) ? 0 : c / v;

            if (c == 0) {
                ha = 0;
            } else if (v == r) {
                ha = ((g - b) / c);
            } else if (v == g) {
                ha = ((b - r) / c) + 2;
            } else {
                ha = ((r - g) / c) + 4;
            }

            if (ha < 0) {
                h = ha / 6 + 1;
            } else {
                h = ha / 6;
            }

            set_pixel(im, x, y, 0, h);
            set_pixel(im, x, y, 1, s);
            set_pixel(im, x, y, 2, v);

        }
    }
}

void hsv_to_rgb(image im)
{
    float r, g, b, h, s, v, m, c, ha, xv;
    for (int y = 0; y < im.h; ++y) {
        for (int x = 0; x < im.w; ++x) {
            h = get_pixel(im, x, y, 0);
            s = get_pixel(im, x, y, 1);
            v = get_pixel(im, x, y, 2);

            c = v * s;
            ha = h * 6;
            xv= c * (1 - fabs(fmod(ha, 2) - 1));

            if (ha >= 0 && ha < 1) {
                r = c;
                g = xv;
                b = 0;
            } else if (ha >= 1 && ha < 2) {
                r = xv;
                g = c;
                b = 0;
            } else if (ha >= 2 && ha < 3) {
                r = 0;
                g = c;
                b = xv;
            } else if (ha >= 3 && ha < 4) {
                r = 0;
                g = xv;
                b = c;
            } else if (ha >= 4 && ha < 5) {
                r = xv;
                g = 0;
                b = c;
            } else {
                r = c;
                g = 0;
                b = xv;
            }

            m = v - c;

            r = r + m;
            g = g + m;
            b = b + m;

            set_pixel(im, x, y, 0, r);
            set_pixel(im, x, y, 1, g);
            set_pixel(im, x, y, 2, b);
        }
    }
}
