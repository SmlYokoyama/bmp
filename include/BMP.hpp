#pragma once

namespace BMP {
    struct Color {
    public:
        unsigned char r, g, b;

    public:
        Color(
            const unsigned char red   = 0, 
            const unsigned char blue  = 0, 
            const unsigned char green = 0
        );
        ~Color();

        Color& operator=(const Color& other);
        const bool operator==(const Color& other) const;
    };

    class Image {
    private:
        int width;
        int height;
        Color** matrix;
    
    public:
        Image(const int w = 0, const int h = 0);
        ~Image();
    
    private:
        void createMatrix();
        void destroyMatrix();

        const bool positionIsValid(const int x, const int y) const;

    public:
        const Color& getColor(const int x, const int y) const;
        
        void setColor(const Color& color, const int x, const int y);
        void setColor(
            const unsigned char r, 
            const unsigned char g, 
            const unsigned char b, 
            const int x, 
            const int y
        );

        Image& operator=(const Image& other);

        const bool save(const char* path) const;
    };
}