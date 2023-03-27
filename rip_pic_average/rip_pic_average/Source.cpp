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

//class Line {//надо менять на массив массивов
//    RGBTRIPLE** line = NULL;
//    int Add;
//public:
//    Line(size_t n, int Add) {
//        line = new RGBTRIPLE *[Add];
//        for(int i=0; i<Add; ++i)
//            line [i] = new RGBTRIPLE[n];
//        this->Add = Add;
//    };
//
//    Line(size_t n) {
//        line = new RGBTRIPLE * [1];
//        line[0] = new RGBTRIPLE[n];
//        this->Add = 1;
//    };
//
//    RGBTRIPLE* data(int i) {
//        return line[i];
//    };
//
//    RGBTRIPLE* operator[](int i) {
//        return line[i];
//    };
//
//    ~Line() {
//        for (int i = 0; i < Add; ++i)
//            delete[] line[i];
//        delete[] line;
//        line = NULL;
//    };
//};
//
//void CreateLine2(Line line1, Line line2, int Add, LONG biWidth, int AAA, int num) {
//    RGBTRIPLE Averege;
//    int count=Add*Add;
//    for (int i = 0; i < biWidth-AAA; ++i) {
//        Averege.rgbtBlue = 0;
//        Averege.rgbtGreen = 0;
//        Averege.rgbtRed = 0;
//        for (int a = 0; a < Add; ++a)
//            for (int b = 0; b < Add; ++b){
//                Averege.rgbtBlue += line1[a][b].rgbtBlue;
//                Averege.rgbtGreen += line1[a][b].rgbtGreen;
//                Averege.rgbtRed += line1[a][b].rgbtRed;
//            }
//        Averege.rgbtBlue /= count;
//        Averege.rgbtGreen /= count;
//        Averege.rgbtRed /= count;
//        line2[0][i] = Averege;
//    }
//    if (AAA){
//        int count = Add * num;
//        for (int a = 0; a < num; ++a)
//            for (int b = 0; b < Add; ++b) {
//                Averege.rgbtBlue += line1[a][b].rgbtBlue;
//                Averege.rgbtGreen += line1[a][b].rgbtGreen;
//                Averege.rgbtRed += line1[a][b].rgbtRed;
//            }
//        Averege.rgbtBlue /= count;
//        Averege.rgbtGreen /= count;
//        Averege.rgbtRed /= count;
//        line2[0][biWidth] = Averege;
//    }
//}
//
//void CreateLastLine2(Line line1, Line line2, int Add, LONG biWidth, int AAA, int numLine, int numColumn) {
//    RGBTRIPLE Averege;
//    int count = numColumn * Add;
//    for (int i = 0; i < biWidth - AAA; ++i) {
//        Averege.rgbtBlue = 0;
//        Averege.rgbtGreen = 0;
//        Averege.rgbtRed = 0;
//        for (int a = 0; a < numLine; ++a)
//            for (int b = 0; b < Add; ++b){
//                Averege.rgbtBlue += line1[a][b].rgbtBlue;
//                Averege.rgbtGreen += line1[a][b].rgbtGreen;
//                Averege.rgbtRed += line1[a][b].rgbtRed;
//            }
//        Averege.rgbtBlue /= count;
//        Averege.rgbtGreen /= count;
//        Averege.rgbtRed /= count;
//        line2[0][i] = Averege;
//    }
//    if (AAA) {
//        int count = numColumn * numLine;
//        for (int a = 0; a < numLine; ++a)
//            for (int b = 0; b < numColumn; ++b) {
//                Averege.rgbtBlue += line1[a][b].rgbtBlue;
//                Averege.rgbtGreen += line1[a][b].rgbtGreen;
//                Averege.rgbtRed += line1[a][b].rgbtRed;
//            }
//        Averege.rgbtBlue /= count;
//        Averege.rgbtGreen /= count;
//        Averege.rgbtRed /= count;
//        line2[0][biWidth] = Averege;
//    }
//}

class Line2 {
public:
    RGBTRIPLE* line = NULL;
    int Add;
    Line2(size_t n) {//конструктор line2
        line = new RGBTRIPLE[n];
        this->Add = 1;
    };
    RGBTRIPLE operator[](int i) {
        return line[i];
    };
    RGBTRIPLE* data() {
        return line;
    }
    ~Line2() {
        delete[] line;
        line = NULL;
    };
};

class Line {
public:
    RGBTRIPLE** line = NULL;
    int Add;
    Line(size_t n, int Add) {//конструктор line1
        line = new RGBTRIPLE * [Add];
        for (int i = 0; i < Add; ++i)
            line[i] = new RGBTRIPLE[n];
        this->Add = Add;
    };
    //Line(size_t n) {//конструктор line2
    //    line = new RGBTRIPLE * [1];
    //    line[0] = new RGBTRIPLE[n];
    //    this->Add = 1;
    //};
    RGBTRIPLE* operator[](int i) {
        return line[i];
    };
    RGBTRIPLE** data() {
        return line;
    }
    RGBTRIPLE data(int a, int b) {
        return line[a][b];
    }
    ~Line() {
        for (int i = 0; i < Add; ++i)
            delete[] line[i];
        delete[] line;
        line = NULL;
    };
};

void CreateLine2(Line line1, Line2 line2, int Add, LONG biWidth, int AAA, int num) {
    RGBTRIPLE Averege;
    int count=Add*Add;
    for (int i = 0; i < biWidth-AAA; ++i) {
        Averege.rgbtBlue = 0;
        Averege.rgbtGreen = 0;
        Averege.rgbtRed = 0;
        for (int a = 0; a < Add; ++a)
            for (int b = 0; b < Add; ++b){
                Averege.rgbtBlue += line1.data(a,b).rgbtBlue;
                Averege.rgbtGreen += line1.data(a, b).rgbtGreen;
                Averege.rgbtRed += line1.data(a, b).rgbtRed;
            }
        Averege.rgbtBlue /= count;
        Averege.rgbtGreen /= count;
        Averege.rgbtRed /= count;
        line2[i] = Averege;
    }
    if (AAA){
        int count = Add * num;
        for (int a = 0; a < num; ++a)
            for (int b = 0; b < Add; ++b) {
                Averege.rgbtBlue += line1[a][b].rgbtBlue;
                Averege.rgbtGreen += line1[a][b].rgbtGreen;
                Averege.rgbtRed += line1[a][b].rgbtRed;
            }
        Averege.rgbtBlue /= count;
        Averege.rgbtGreen /= count;
        Averege.rgbtRed /= count;
        line2[biWidth] = Averege;
    }
}

void CreateLastLine2(Line line1, Line2 line2, int Add, LONG biWidth, int AAA, int numLine, int numColumn) {
    RGBTRIPLE Averege;
    int count = numColumn * Add;
    for (int i = 0; i < biWidth - AAA; ++i) {
        Averege.rgbtBlue = 0;
        Averege.rgbtGreen = 0;
        Averege.rgbtRed = 0;
        for (int a = 0; a < numLine; ++a)
            for (int b = 0; b < Add; ++b){
                Averege.rgbtBlue += line1[a][b].rgbtBlue;
                Averege.rgbtGreen += line1[a][b].rgbtGreen;
                Averege.rgbtRed += line1[a][b].rgbtRed;
            }
        Averege.rgbtBlue /= count;
        Averege.rgbtGreen /= count;
        Averege.rgbtRed /= count;
        line2[i] = Averege;
    }
    if (AAA) {
        int count = numColumn * numLine;
        for (int a = 0; a < numLine; ++a)
            for (int b = 0; b < numColumn; ++b) {
                Averege.rgbtBlue += line1[a][b].rgbtBlue;
                Averege.rgbtGreen += line1[a][b].rgbtGreen;
                Averege.rgbtRed += line1[a][b].rgbtRed;
            }
        Averege.rgbtBlue /= count;
        Averege.rgbtGreen /= count;
        Averege.rgbtRed /= count;
        line2[biWidth] = Averege;
    }
}

int main() {

    setlocale(LC_ALL, "Russian");

    BITMAPFILEHEADER bfh;
    BITMAPINFOHEADER bih;
    pad padding;

    int Add = 1;

    FileWithDes f1("pic1.bmp", "rb");
    FileWithDes f2("pic2.bmp", "wb");

    fread(&bfh, sizeof(bfh), 1, f1.getF());
    fread(&bih, sizeof(bih), 1, f1.getF());

    cout << "пожалуйста, введите количество пикселей, которые будут объеденены в 1 (сторона квадратной области)\n";
    cin >> Add;

    if ((Add + 1 > bih.biWidth) || (Add + 1 > bih.biHeight))
    {
        cout << "количество пропускаемых символов слишком большое";
        return 0;
    }

    cout << "было: " << bih.biHeight << "*" << bih.biWidth << endl;

    int AAA = (bih.biWidth % Add) ? 1 : 0;//вероятно, стоит убрать ?: для оптимизации
    LONG WIGTH = bih.biWidth;
    bih.biWidth /= Add;
    bih.biWidth += AAA;
    LONG HEIGHT = bih.biHeight;
    bih.biHeight /= Add;
    bih.biHeight += (bih.biHeight % Add) ? 1 : 0;//вероятно, стоит убрать ?: для оптимизации
    DWORD SIZE = bih.biSizeImage;
    bih.biSizeImage = bih.biWidth * bih.biHeight;

    cout << "стало: " << bih.biHeight << "*" << bih.biWidth << endl;

    padding.before = (4 - (WIGTH * 3) % 4) % 4;
    padding.after = (4 - (bih.biWidth * 3) % 4) % 4;

    cout << "padding.before: " << padding.before << endl;
    cout << "padding.after: " << padding.after << endl;

    bfh.bfSize = sizeof(bfh) + sizeof(bih) + bih.biSizeImage * 3 + padding.after * bih.biHeight * 3;

    fwrite(&bfh, sizeof(bfh), 1, f2.getF());
    fwrite(&bih, sizeof(bih), 1, f2.getF());

    Line line1(WIGTH /** 3*/, Add);//в пикселях
    Line2 line2(bih.biWidth /** 3 + padding.after*/);

    //memset(&line2 + (bih.biWidth /** 3*/)*Add, 0, padding.after * sizeof(BYTE));//устанавливает выравнивание 0. sizeof(BYTE)==1?//теперь выравнивания нет, потому что храним стуктуры а не байты

    fseek(f1.getF(), bfh.bfOffBits, SEEK_SET);//на начало чтения
    BYTE trash = 0;//мусор для выравнивания
    for (int i = 0; i < HEIGHT/Add; ++i) {
        for (int j = 0; j < Add; ++j) {
            fread(line1.data(), WIGTH * 3, 1, f1.getF());//считали line 1
        }
        CreateLine2(line1, line2, Add, bih.biWidth, AAA, bih.biWidth % Add);
        fwrite(line2.data(), bih.biWidth * 3 /*+ padding.after*/, 1, f2.getF());
        fwrite(&trash, padding.after, 1, f2.getF());
    }
    if(bih.biHeight % Add){
        CreateLastLine2(line1, line2, Add, bih.biWidth, AAA, bih.biWidth % Add, bih.biHeight % Add);
        fwrite(line2.data(), bih.biWidth * 3 + padding.after, 1, f2.getF());
        fwrite(&trash, padding.after, 1, f2.getF());
    }
    return 0;
}