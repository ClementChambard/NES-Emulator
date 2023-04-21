#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <iostream>
#include <GL/glew.h>

class Texture {

    public:
        /**
         *  Constructor : loads texture from file
         *   - filename (std::string) : the image file from which to load the texture
         */
        Texture(std::string filename);

        /**
         *  Constructor : create empty texture
         *   - w (int) : the width of the new texture
         *   - h (int) : the height of the new texture
         */
        Texture(int w, int h);

        /**
         *  Copy constructor
         */
        Texture(Texture& tex);

        /**
         *  Move constructor
         */
        Texture(Texture&& tex);

        /**
         *  Destructor
         */
        ~Texture();

        /**
         *  Assignment operator
         */
        Texture& operator=(Texture& tex);
        Texture& operator=(Texture&& tex);

        /**
         *  Binds the texture for drawing
         */
        virtual void bind();

        /**
         *  Unbinds the texture for drawing
         */
        virtual void unbind();

        /**
         *  Returns the size of the texture
         *   - w (int&) : the width of the texture
         *   - h (int&) : the height of the texture
         */
        void getSize(int& w, int& h) const { w = m_w; h = m_h; }
        int getWidth() const { return m_w; }
        int getHeight() const { return m_h; }

        /**
         *  Returns the OpenGL ID of the texture
         */
        GLuint getID() const { return texID; }
    private:
        GLuint texID;
        int m_w, m_h;
};

#endif // TEXTURE_H_
