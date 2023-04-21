#include "PixelBuffer.hpp"
#include <memory.h>

PixelBuffer::PixelBuffer(int width, int height) : Texture(width, height)
{
    m_data = new Pixel[width * height];
}
PixelBuffer::PixelBuffer(PixelBuffer& pb) : Texture(pb)
{
    m_data = new Pixel[getWidth() * getHeight()];
    memcpy(m_data, pb.m_data, getWidth() * getHeight());
}
PixelBuffer::PixelBuffer(PixelBuffer&& pb) : Texture(pb), m_data(pb.m_data)
{
    pb.m_data = nullptr;
}

PixelBuffer& PixelBuffer::operator=(PixelBuffer& pb) {
    Texture::operator=(pb);
    m_data = new Pixel[getWidth() * getHeight()];
    memcpy(m_data, pb.m_data, getWidth() * getHeight());
    return *this;
}

PixelBuffer& PixelBuffer::operator=(PixelBuffer&& pb) {
    Texture::operator=(pb);
    m_data = pb.m_data;
    pb.m_data = nullptr;
    return *this;
}

PixelBuffer::~PixelBuffer()
{
    delete[] m_data;
}

void PixelBuffer::bind() {
    Texture::bind();
    if (m_changed) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, getWidth(), getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, (uint8_t*)m_data);
        m_changed = false;
    }
}

void PixelBuffer::unbind() {
    Texture::unbind();
}

void PixelBuffer::write(PixelBuffer const& buf, int data_h, int x_start, int y_start)
{
    write(buf.m_data, buf.getWidth(), buf.getHeight(), x_start, y_start);
}

void PixelBuffer::write(Pixel* data, int data_w, int data_h, int x_start, int y_start)
{
    for (int x = 0; x < data_w; x++)
    {
        for (int y = 0; y < data_h; y++)
        {
            setPixel(x_start + x, y_start + y, data[x + y * data_w]);
        }
    }
}

void PixelBuffer::setPixel(int x, int y, Pixel const& pix) {
    if (x < 0 || x >= getWidth() || y < 0 || y >= getHeight()) return;
    if (m_data[x + y * getWidth()] == pix) return;

    m_changed = true;
    m_data[x + y * getWidth()] = pix;
}
