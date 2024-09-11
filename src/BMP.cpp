#include "BMP.hpp"

#include <iostream>
#include <fstream>

#define BMP_HEADER_SIZE 14
#define BMP_INFO_HEADER_SIZE 40

#define BYTE 8

namespace BMP {
    Color::Color(
        const unsigned char red, 
        const unsigned char green, 
        const unsigned char blue
    ) 
        : r(red) ,
          g(green),
          b(blue)
    {}

    Color::~Color() {}

    Color& Color::operator=(const Color& other) {
        r = other.r;
        g = other.g;
        b = other.b;
    
        return *this;
    }

    const bool Color::operator==(const Color& other) const {
        return (r == other.r && g == other.g && b == other.b);
    }

    Image::Image(const int w, const int h)
        : width(w), height(h), matrix(nullptr)
    {
        createMatrix();
    }

    Image::~Image() {
        destroyMatrix();
    }

    void Image::createMatrix() {
        destroyMatrix();

        matrix = new Color*[height];
        for(int i = 0; i < height; ++i)
            matrix[i] = new Color[width];
    }

    void Image::destroyMatrix() {
        if(!matrix)
            return;

        for(int i = 0; i < height; ++i)
            delete[] matrix[i];
        delete[] matrix;

        matrix = nullptr;
        width = 0;
        height = 0;
    }

    const bool Image::positionIsValid(const int x, const int y) const {
        return (x >= 0 && x < width && y >= 0 && y < height);
    }

    const Color& Image::getColor(const int x, const int y) const {
        if(positionIsValid(x, y))
            return matrix[y][x];
        return matrix[0][0];
    }

    void Image::setColor(const Color& color, const int x, const int y) {
        if(!positionIsValid(x, y))
            return;
        
        matrix[y][x] = color;
    }

    void Image::setColor(
        const unsigned char r, 
        const unsigned char g, 
        const unsigned char b, 
        const int x, 
        const int y
    ) {
        if(!positionIsValid(x, y))
            return;
        
        matrix[y][x].r = r;
        matrix[y][x].g = g;
        matrix[y][x].b = b;
    }

    Image& Image::operator=(const Image& other) {
        destroyMatrix();
        width = other.width;
        height = other.height;
        createMatrix();

        for(int i = 0; i < height; ++i)
            for(int j = 0; j < width; ++j)
                setColor(other.getColor(j, i), j, i);

        return *this;
    }

    const bool Image::save(const char* path) const {
        std::ofstream file;
        file.open(path, std::ios::out | std::ios::binary);

        if(!file.is_open()) {
            std::cout << "failed to open file!\n";
            return false;
        }

        // Every row must occupy an amount of data that can be 
        // divided into chunks of 4 bytes, so, padding is necessary
        // when the number of columns is not divisible by 4
        
        // Each color occupies one byte, since 3 colors are
        // used (red, green and blue), width must be multiplied by 3
        int paddingAmount = (4 - ( (width*3) % 4 )) % 4;
        unsigned char bmpPad[3] = {0, 0, 0};

        const int fileSize = BMP_HEADER_SIZE + BMP_INFO_HEADER_SIZE + height*(width*3 + paddingAmount);
    
        //
        // File header
        //

        unsigned char fileHeader[BMP_HEADER_SIZE];

        // File type
        fileHeader[0] = 'B';
        fileHeader[1] = 'M';
    
        // File size, since a char has only 1 Byte it
        // is necessary split the integer value (4 bytes),
        // into 4 separate bytes
        fileHeader[2] = (char)  fileSize;              // Least significant byte
        fileHeader[3] = (char) (fileSize >> (BYTE));   // Second least significant
        fileHeader[4] = (char) (fileSize >> (2*BYTE)); // Third
        fileHeader[5] = (char) (fileSize >> (3*BYTE)); // Most significant

        // Reserved bytes (not used)
        fileHeader[6] = 0;
        fileHeader[7] = 0;
        fileHeader[8] = 0;
        fileHeader[9] = 0;

        // Pixel data offset
        fileHeader[10] = BMP_HEADER_SIZE + BMP_INFO_HEADER_SIZE;
        fileHeader[11] = 0;
        fileHeader[12] = 0;
        fileHeader[13] = 0;

        //
        // Information header
        //

        unsigned char infoHeader[BMP_INFO_HEADER_SIZE];
    
        // Header size
        infoHeader[0] = BMP_INFO_HEADER_SIZE;
        infoHeader[1] = 0;
        infoHeader[2] = 0;
        infoHeader[3] = 0;

        // Image width
        infoHeader[4] = (char)  width;             
        infoHeader[5] = (char) (width >> (BYTE));  
        infoHeader[6] = (char) (width >> (2*BYTE));
        infoHeader[7] = (char) (width >> (3*BYTE));

        // Image height
        infoHeader[8]  = (char)  height;             
        infoHeader[9]  = (char) (height >> (BYTE));  
        infoHeader[10] = (char) (height >> (2*BYTE));
        infoHeader[11] = (char) (height >> (3*BYTE));
    
        // Planes
        infoHeader[12] = 1;
        infoHeader[13] = 0;

        // Bits per pixel
        infoHeader[14] = 24;
        infoHeader[15] = 0;

        // Compression (no compression)
        infoHeader[16] = 0;
        infoHeader[17] = 0;
        infoHeader[18] = 0;
        infoHeader[19] = 0;

        // Image size (no compression)
        infoHeader[20] = 0;
        infoHeader[21] = 0;
        infoHeader[22] = 0;
        infoHeader[23] = 0;

        // X pixels per meter (not specified)
        infoHeader[24] = 0;
        infoHeader[25] = 0;
        infoHeader[26] = 0;
        infoHeader[27] = 0;

        // Y pixels per meter (not specified)
        infoHeader[28] = 0;
        infoHeader[29] = 0;
        infoHeader[30] = 0;
        infoHeader[31] = 0;

        // Total color (color pallete not used)
        infoHeader[32] = 0;
        infoHeader[33] = 0;
        infoHeader[34] = 0;
        infoHeader[35] = 0;

        // Important colors (generally ignored)
        infoHeader[36] = 0;
        infoHeader[37] = 0;
        infoHeader[38] = 0;
        infoHeader[39] = 0;

        file.write(reinterpret_cast<char*>(fileHeader), BMP_HEADER_SIZE);
        file.write(reinterpret_cast<char*>(infoHeader), BMP_INFO_HEADER_SIZE);

        //
        // Pixel values
        // 

        for(int i = 0; i < height; ++i) {
            for(int j = 0; j < width; ++j) {
                unsigned char r = getColor(j, i).r;
                unsigned char g = getColor(j, i).g;
                unsigned char b = getColor(j, i).b;
                unsigned char color[3] = {b, g, r};
                
                file.write(reinterpret_cast<char*>(color), 3);
            }

            file.write(reinterpret_cast<char*>(bmpPad), paddingAmount);
        }

        file.close();

        std::cout << "BMP image exported successfully!\n";
        return true;
    }
}