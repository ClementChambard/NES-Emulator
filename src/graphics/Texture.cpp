#include "Texture.hpp"

#include <SDL2/SDL_image.h>

Texture::Texture(std::string filename)
{
    SDL_Surface* img = IMG_Load(filename.c_str());
    SDL_Surface* rgbImg = SDL_ConvertSurfaceFormat(img, SDL_PIXELFORMAT_RGBA32, 0);
    SDL_FreeSurface(img);

    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, rgbImg->w, rgbImg->h, 0,
                   GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)rgbImg->pixels);
      glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    m_w = rgbImg->w;
    m_h = rgbImg->h;

    SDL_FreeSurface(rgbImg);
}

Texture::Texture(int w, int h)
{
    this->m_w = w;
    this->m_h = h;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0,
                   GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture(Texture& tex) {
    m_w = tex.m_w, m_h = tex.m_h;
    uint8_t* data = new uint8_t[m_w*m_h];
    glBindTexture(GL_TEXTURE_2D, tex.texID);
    glReadPixels(0, 0, m_w, m_h, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_w, m_h, 0,
                   GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)data);
      glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    delete[] data;
}

Texture::Texture(Texture&& tex) {
    m_w = tex.m_w; tex.m_w = 0;
    m_h = tex.m_h; tex.m_h = 0;
    texID = tex.texID; tex.texID = 0;
}

Texture& Texture::operator=(Texture& tex) {
    m_w = tex.m_w, m_h = tex.m_h;
    uint8_t* data = new uint8_t[m_w*m_h];
    glBindTexture(GL_TEXTURE_2D, tex.texID);
    glReadPixels(0, 0, m_w, m_h, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_w, m_h, 0,
                   GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)data);
      glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    delete[] data;
    return *this;
}

Texture& Texture::operator=(Texture&& tex) {
    m_w = tex.m_w; tex.m_w = 0;
    m_h = tex.m_h; tex.m_h = 0;
    texID = tex.texID; tex.texID = 0;
    return *this;
}

Texture::~Texture()
{
    glDeleteTextures(1, &texID);
}

void Texture::bind()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texID);
}

void Texture::unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
