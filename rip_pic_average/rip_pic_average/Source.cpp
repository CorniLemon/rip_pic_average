#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>
using namespace std;

typedef unsigned __int16 WORD;//2 байта
//typedef unsigned int DWORD;//4 байта
typedef long LONG;//4 байта
typedef unsigned char BYTE;//1 байт

struct pad {
    size_t before = 0;
    size_t after = 0;
};

class FileWithDes {
    FILE* f = NULL;
public:
    FileWithDes(const char A[], const char B[]) {
        f = fopen(A, B);
        if (!f) {
            cout << "файл " << A << " не существует или не удалось создать\n";
            throw;
        }
    };

    FILE* getF() {
        return f;
    };

    ~FileWithDes() {
        fclose(f);
        f = NULL;
    }
};

class Line {
    BYTE* line = NULL;
public:
    Line(size_t n) {
        line = new BYTE[n];
    };

    BYTE* data() {
        return line;
    };

    void operator= (BYTE* line1) {
        line = line1;
    }
    BYTE& operator[] (int i) {
        return line[i];
    }

    ~Line() {
        delete[] line;
        line = NULL;
    };
};

//class Pixel {
//    BYTE* line;
//    size_t ch;
//public:
//    Pixel(BYTE* line, size_t ch)
//        : line(line),
//        ch(ch)
//    {
//    }
//
//    BYTE& operator[] (int k) {
//        return line[k];
//    }
//};
//
//class Line {
//    BYTE* line;
//    size_t w, ch;
//public:
//    Line(BYTE *line, size_t w, size_t ch)
//        : line(line),
//        w(w), ch(ch)
//    {
//    }
//
//    Pixel operator[] (int j) {
//        return Pixel(line + j * ch, ch);
//    }
//};
//
//class Matr {
//    BYTE* line = NULL;
//    size_t h, w, ch;
//public:
//    Matr(size_t h, size_t w, size_t ch)
//        : line(new BYTE[h * w * ch]),
//        h(h), w(w), ch(ch)
//    {
//    }
//
//    BYTE* data() {
//        return line;
//    }
//
//    void operator= (BYTE* line1) {
//        line = line1;
//    }
//    Line operator[] (int i) {
//        return Line(line + i * w * ch, w, ch);
//    }
//
//    BYTE& at(size_t i, size_t j, size_t k)
//    {
//        return line[(i * w + j) * ch + k];
//    }
//
//    ~Matr() {
//        delete[] line;
//        line = NULL;
//    }
//
//    operator void* ()
//    {
//        return line;
//    }
//};

//void f()
//{
//    Matr m(100, 50, 3);
//    m[0][0][0] = 5;
//    fread(m, ...);
//}

//void CreateLine2(BYTE* line1, BYTE* line2, int Add, int WightInPix, FileWithDes f2, int lenghtInBytes, int lenghtForF2) {
//    RGBTRIPLE Averege;
//    for (int j = 0; j < WightInPix; ++j) {//скорее всего косяк здесь
//        Averege.rgbtBlue = 0;
//        Averege.rgbtGreen = 0;
//        Averege.rgbtRed = 0;
//        for (int i = 0; i < Add; ++i) {
//            Averege.rgbtBlue += line1[3 * i* WightInPix +j];
//            Averege.rgbtGreen += line1[3 * i * lenghtInBytes + j+1];
//            Averege.rgbtRed += line1[3 * i * lenghtInBytes + j+2];
//        }
//        Averege.rgbtBlue /= Add;
//        Averege.rgbtGreen /= Add;
//        Averege.rgbtRed /= Add;
//        line2[3 * j] = Averege.rgbtBlue;
//        line2[3 * j + 1] = Averege.rgbtGreen;
//        line2[3 * j + 2] = Averege.rgbtRed;
//    }
//    fwrite(line2, lenghtForF2, 1, f2.getF());
//    //return line2;
//}

int main() {
    setlocale(LC_ALL, "Russian");

    BITMAPFILEHEADER bfh;
    BITMAPINFOHEADER bih;
    RGBTRIPLE rgb;

    pad padding;

    int Add = 1;

    FileWithDes f1("pic1.bmp", "rb");
    FileWithDes f2("pic2.bmp", "wb");

    cout << "пожалуйста, введите количество пикселей, которые будут объеденены в 1 (сторона квадратной области)\nпока что не обрабатывает последнюю строку и столбец\n";
    cin >> Add;

    fread(&bfh, sizeof(bfh), 1, f1.getF());
    fread(&bih, sizeof(bih), 1, f1.getF());

    if ((Add + 1 > bih.biWidth) || (Add + 1 > bih.biHeight))
    {
        cout << "количество обьединяемых пикселей слишком большое (изображение сожмется в картинку 1*1)";
        return 0;
    }

    cout << "было: " << bih.biHeight << "*" << bih.biWidth << endl;

    LONG WIGTH = bih.biWidth;
    bih.biWidth /= Add;
    LONG HEIGHT = bih.biHeight;
    bih.biHeight /= Add;
    DWORD SIZE = bih.biSizeImage;
    bih.biSizeImage /= Add * Add;

    cout << "стало: " << bih.biHeight << "*" << bih.biWidth << endl;

    padding.before = (4 - (WIGTH * 3) % 4) % 4;
    padding.after = (4 - (bih.biWidth * 3) % 4) % 4;
    bfh.bfSize = sizeof(bfh) + sizeof(bih) + bih.biSizeImage * sizeof(rgb) + padding.after * bih.biHeight * sizeof(rgb);

    fwrite(&bfh, sizeof(bfh), 1, f2.getF());
    fwrite(&bih, sizeof(bih), 1, f2.getF());

    Line line1((WIGTH * 3 + padding.before) * Add);
    Line line2(bih.biWidth * 3 + padding.after);//больше здесь нет паддинга

    memset(line2.data() + bih.biWidth * 3, 0, padding.after * sizeof(BYTE));//паддинг line2 черный

    BYTE trash = 0;//мусорные данные для выравнивания

    fseek(f1.getF(), bfh.bfOffBits, SEEK_SET);//что за исключение тут?//при повторных запусках не появляется

    cout << "padding.before: " << padding.before << endl;
    cout << "padding.after: " << padding.after << endl;

    double AveregeB;
    double AveregeG;
    double AveregeR;
    for (int i = 0; i < HEIGHT / Add; ++i) {
        fread(line1.data(), (WIGTH * 3 + padding.before) * Add, 1, f1.getF());

        for (int i = 0; i < bih.biWidth; ++i) {
            AveregeB = 0;
            AveregeG = 0;
            AveregeR = 0;
            for (int j = 0; j < Add; ++j) {
                for (int k = 0; k < Add; ++k) {
                    AveregeB += line1[j * (WIGTH * 3 + padding.before) + (i * Add + k) * 3];
                    AveregeG += line1[j * (WIGTH * 3 + padding.before) + (i * Add + k) * 3 + 1];
                    AveregeR += line1[j * (WIGTH * 3 + padding.before) + (i * Add + k) * 3 + 2];
                }
            }
            AveregeB /= Add;
            AveregeG /= Add;
            AveregeR /= Add;
            line2.data()[i * 3] = byte(AveregeB + 0.5);
            line2.data()[i * 3 + 1] = byte(AveregeG + 0.5);
            line2.data()[i * 3 + 2] = byte(AveregeR + 0.5);
        }
        fwrite(line2.data(), bih.biWidth * 3 + padding.after, 1, f2.getF());
        //line2=CreateLine2(line1.data(), line2.data(), Add, bih.biWidth, f2, (WIGTH) * 3 + padding.before);
        //CreateLine2(line1.data(), line2.data(), Add, bih.biWidth, f2, (WIGTH) * 3 + padding.before, bih.biWidth * 3 + padding.after);
        //fwrite(line2.data(), bih.biWidth * 3 + padding.after, 1, f2.getF());
    }

    return 0;
}