#ifndef SPRITEFONT_H_
#define SPRITEFONT_H_

#include "Texture.hpp"
#include "VertexBatch.hpp"
#include <vector>

class SpriteFont {

    public:

        struct Glyph {
            char c;
            glm::vec2 size;
            glm::vec2 offset;
            glm::vec4 uvs;
            float x_advance;
        };

        enum align {
            LEFT,
            RIGHT,
            CENTER,
            TOP,
            BOTTOM
        };

        SpriteFont(std::string fileName);
        ~SpriteFont();

        void renderText(std::string text, glm::vec2 pos, glm::vec2 charSize, glm::vec<4, uint8_t> col = {255,255,255,255});

        void renderAllText();

        void set_halign(align a) {
            if (a == TOP || a == BOTTOM) return;
            halign = a;
        }
        void set_valign(align a) {
            if (a == LEFT || a == RIGHT) return;
            valign = a;
        }

        static SpriteFont* last_created_instance;
    private:
        Texture* font_atlas;
        std::vector<Glyph> glyphs;
        VertexBatch* fontBatch;
        align halign = CENTER;
        align valign = CENTER;
};

std::istream& operator>>(std::istream& l, SpriteFont::Glyph& r);

#endif // SPRITEFONT_H_
