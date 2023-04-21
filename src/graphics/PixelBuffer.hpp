#ifndef PIXELBUFFER_H_
#define PIXELBUFFER_H_

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Texture.hpp"

typedef glm::vec<4, uint8_t> Pixel;

class PixelBuffer : public Texture {

    public:
        PixelBuffer(int w, int h);
        PixelBuffer(PixelBuffer& pb);
        PixelBuffer(PixelBuffer&& pb);
        ~PixelBuffer();

        PixelBuffer& operator=(PixelBuffer& pb);
        PixelBuffer& operator=(PixelBuffer&& pb);

        void bind() override;
        void unbind() override;

        void write(Pixel* data, int data_w, int data_h, int x_start, int y_start);
        void write(PixelBuffer const& buf, int data_h, int x_start, int y_start);

        void setPixel(int x, int y, Pixel const& pixel);

    private:
        Pixel* m_data;
        bool m_changed = false;

};

#endif // PIXELBUFFER_H_
