#pragma once

#include <QOpenGLFunctions_3_3_Core>

class Texture2D : protected QOpenGLFunctions_3_3_Core{
public:
    GLuint id = 0;

    Texture2D(char* file_path, GLenum internal_format);
    Texture2D(uint8_t* data, uint32_t width, uint32_t height, GLenum image_format, GLenum internal_format);
    ~Texture2D();
    void fill(char* file_path, GLenum internal_format);
    void fill(uint8_t* data, uint32_t width, uint32_t height, GLenum image_format, GLenum internal_format);
    void bind();
    void bind_texture_unit(GLenum texture_unit);

private:
    int texture_width;
    int texture_height;
    int color_channels_count;
};
