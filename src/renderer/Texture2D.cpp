#include <Texture2D.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture2D::Texture2D(char* file_path, GLenum internal_format){
    initializeOpenGLFunctions();
    glGenTextures(1, &id);
    fill(file_path, internal_format);
}
Texture2D::Texture2D(uint8_t* data, uint32_t width, uint32_t height, GLenum image_format, GLenum internal_format){
    initializeOpenGLFunctions();
    glGenTextures(1, &id);
    fill(data, width, height, image_format, internal_format);
}
Texture2D::~Texture2D(){
    if(id != 0){
        puts("Log: Texture2D: Killing Texture2D");
        glDeleteTextures(1, &id);
        id = 0;
    }
}
void Texture2D::fill(char* file_path, GLenum internal_format){
    //load texture
    uint8_t* data = stbi_load(file_path, &texture_width, &texture_height, &color_channels_count, 0);
    if(!data){
        printf("Err: Texture2D: Failed to load texture image at path %s\n", file_path);
        throw std::runtime_error("^");
    }
    if(color_channels_count != 3 && color_channels_count != 4){
        printf("Err: Texture2D: Invalid color channels count for texture image at path %s. Expected 3-4, got %d\n", file_path, color_channels_count);
        throw std::runtime_error("^");
    }
    fill(data, texture_width, texture_height, (color_channels_count == 4) ? GL_RGBA : GL_RGB, internal_format);
    stbi_image_free(data);
}
void Texture2D::fill(uint8_t* data, uint32_t width, uint32_t height, GLenum image_format, GLenum internal_format){
    if(internal_format == 0){
        internal_format = image_format;
    }
    bind();
    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, image_format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
}
void Texture2D::bind(){
    glBindTexture(GL_TEXTURE_2D, id);
}
void Texture2D::bind_texture_unit(GLenum texture_unit){
    glActiveTexture(texture_unit);
    glBindTexture(GL_TEXTURE_2D, id);
}
