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
        //line = malloc(n+1);
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
        if(line)
        {
            delete[] line;//падает при add = 3,4,7,8,9,10,11,12
        }
        line = NULL;
    };
};

//class Line {
//    void* line = NULL;
//public:
//    Line(size_t n) {
//        //line = new BYTE[n];
//        line = malloc(n + 1);
//    };
//
//    void* data() {
//        return line;
//    };
//
//    void operator= (BYTE* line1) {
//        line = line1;
//    }
//    void& operator[] (int i) {
//        return line[i];
//    }
//
//    ~Line() {
//        if (line)
//            delete[] line;//падает при add = 3,4,7,8,9,10,11,12
//        line = NULL;
//    };
//};

int main() {
    setlocale(LC_ALL, "Russian");

    BITMAPFILEHEADER bfh;
    BITMAPINFOHEADER bih;
    RGBTRIPLE rgb;

    pad padding;

    int Add = 1;

    FileWithDes f1("pic3.bmp", "rb");
    FileWithDes f2("pic2.bmp", "wb");

    cout << "пожалуйста, введите количество пикселей, которые будут объеденены в 1 (сторона квадратной области)\n";
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
    int WOst = bih.biWidth % Add;
    cout<<"wost " << WOst << endl;
    bih.biWidth = bih.biWidth / Add;
    if (WOst) ++bih.biWidth;

    LONG HEIGHT = bih.biHeight;
    int HOst = bih.biHeight % Add;
    cout<<"host " << HOst << endl;
    bih.biHeight = bih.biHeight / Add;
    if (HOst) ++bih.biHeight;

    DWORD SIZE = bih.biSizeImage;
    bih.biSizeImage = bih.biHeight* bih.biWidth;

    cout << "стало: " << bih.biHeight << "*" << bih.biWidth << endl;

    padding.before = (4 - (WIGTH * 3) % 4) % 4;
    padding.after = (4 - (bih.biWidth * 3) % 4) % 4;
    bfh.bfSize = sizeof(bfh) + sizeof(bih) + bih.biSizeImage * 3 + padding.after * bih.biHeight * 3;

    fwrite(&bfh, sizeof(bfh), 1, f2.getF());
    fwrite(&bih, sizeof(bih), 1, f2.getF());

    Line line1((WIGTH * 3 + padding.before) * Add);
    Line line2(bih.biWidth * 3 + padding.after);//больше здесь нет паддинга
    
    memset(line2.data() + bih.biWidth * 3, 0, padding.after * sizeof(BYTE));//паддинг line2 черный

    fseek(f1.getF(), bfh.bfOffBits, SEEK_SET);//что за исключение тут?//при повторных запусках не появляется

    cout << "padding.before: " << padding.before << endl;
    cout << "padding.after: " << padding.after << endl;

    double AveregeB;
    double AveregeG;
    double AveregeR;

    auto CreatePixel = [&](int allowH, int allowW, int position) {
        AveregeB = 0;
        AveregeG = 0;
        AveregeR = 0;
        for (int j = 0; j < allowH; ++j) {
            for (int k = 0; k < allowW; ++k) {
                AveregeB += line1[j * (WIGTH * 3 + padding.before) + (position * Add + k) * 3];
                AveregeG += line1[j * (WIGTH * 3 + padding.before) + (position * Add + k) * 3 + 1];
                AveregeR += line1[j * (WIGTH * 3 + padding.before) + (position * Add + k) * 3 + 2];
            }
        }
        AveregeB /= Add;
        AveregeG /= Add;
        AveregeR /= Add;
        line2.data()[position * 3] = byte(AveregeB + 0.5);
        line2.data()[position * 3 + 1] = byte(AveregeG + 0.5);
        line2.data()[position * 3 + 2] = byte(AveregeR + 0.5);
    };

    auto CreateLine2 = [&](int h1, int w2) {//создает розовый, желтый и зелёный
        for (int i = 0; i < w2; ++i) {
            CreatePixel(h1, Add, i);
        }
    };
    auto CreateLastPIxelInLine2 = [&](int h1, int wl2) {//сщздает розовый, желтый и зелёный
        CreatePixel(h1, wl2, bih.biWidth);
    };
    auto CreateAllLine2 = [&](int h1, int w2, int wl2) {
        CreateLine2(h1, w2);
        if (WOst)
        {
            CreateLastPIxelInLine2(h1, wl2);
        }
    };

    for (int i = 0; i < HEIGHT / Add; ++i) {
        fread(line1.data(), (WIGTH * 3 + padding.before) * Add, 1, f1.getF());
        CreateAllLine2(Add, bih.biWidth, WOst);
        fwrite(line2.data(), bih.biWidth * 3 + padding.after, 1, f2.getF());
    }
    if (HOst)
    {
        CreateAllLine2(HOst, bih.biWidth, WOst);
        fwrite(line2.data(), bih.biWidth * 3 + padding.after, 1, f2.getF());
    }

    return 0;
}