#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>
using namespace std;

typedef unsigned __int16 WORD;//2 �����
//typedef unsigned int DWORD;//4 �����
typedef long LONG;//4 �����
typedef unsigned char BYTE;//1 ����

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
            cout << "���� " << A << " �� ���������� ��� �� ������� �������\n";
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
    size_t n = 0;
    BYTE* line = NULL;
public:
    Line(size_t n)
        :n(n), line(new BYTE[n])
    {};

    Line(BYTE* line1)
    {
        line = line1;
    };

    BYTE* data() {
        return line;
    };

    void operator= (BYTE* line1) {
        line = line1;
    }
    BYTE& operator[] (int i) {
        if (i < 0 || i >= n)
            throw 1;
        return line[i];
    }

    ~Line() {
        if(line)
        {
            delete[] line;//[] �� ������ �� ������
        }
        //������ ��� add = 3,4,7,8,9,10,11,12  �� 17*13
        //������ ��� add = 3,5,7,9,10,11...  �� 256*256
        line = NULL;
    };
};

int main() {
    setlocale(LC_ALL, "Russian");

    BITMAPFILEHEADER bfh;
    BITMAPINFOHEADER bih;
    RGBTRIPLE rgb;

    pad padding;

    int Add = 1;

    FileWithDes f1("pic1.bmp", "rb");
    FileWithDes f2("pic2.bmp", "wb");

    cout << "����������, ������� ���������� ��������, ������� ����� ���������� � 1 (������� ���������� �������)\n";
    cin >> Add;

    fread(&bfh, sizeof(bfh), 1, f1.getF());
    fread(&bih, sizeof(bih), 1, f1.getF());

    if ((Add + 1 > bih.biWidth) || (Add + 1 > bih.biHeight))
    {
        cout << "���������� ������������ �������� ������� ������� (����������� �������� � �������� 1*1)";
        return 0;
    }

    cout << "����: " << bih.biHeight << "*" << bih.biWidth << endl;

    LONG WIGTH = bih.biWidth;
    int WOst = bih.biWidth % Add;
    bih.biWidth = bih.biWidth / Add;
    if (WOst) ++bih.biWidth;

    LONG HEIGHT = bih.biHeight;
    int HOst = bih.biHeight % Add;
    bih.biHeight = bih.biHeight / Add;
    if (HOst) ++bih.biHeight;

    DWORD SIZE = bih.biSizeImage;
    bih.biSizeImage = bih.biHeight* bih.biWidth;

    cout<<"host " << HOst << endl;
    cout<<"wost " << WOst << endl;
    cout << "�����: " << bih.biHeight << "*" << bih.biWidth << endl;

    padding.before = (4 - (WIGTH * 3) % 4) % 4;
    padding.after = (4 - (bih.biWidth * 3) % 4) % 4;
    bfh.bfSize = sizeof(bfh) + sizeof(bih) + bih.biSizeImage * 3 + padding.after * bih.biHeight * 3;

    fwrite(&bfh, sizeof(bfh), 1, f2.getF());
    fwrite(&bih, sizeof(bih), 1, f2.getF());

    Line line1((WIGTH * 3 + padding.before) * Add);
    Line line2(bih.biWidth * 3 + padding.after);//������ ����� ��� ��������
    
    memset(line2.data() + bih.biWidth * 3, 0, padding.after * sizeof(BYTE));//������� line2 ������

    fseek(f1.getF(), bfh.bfOffBits, SEEK_SET);//��� �� ���������� ���?//��� ��������� �������� �� ����������

    cout << "padding.before: " << padding.before << endl;
    cout << "padding.after: " << padding.after << endl;

    double AveregeB;
    double AveregeG;
    double AveregeR;

    auto CreatePixel = [&](int allowH, int allowW, int position) {//������ ������ ��������� �������
        int count = allowH * allowW;
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
        AveregeB /= count;
        AveregeG /= count;
        AveregeR /= count;
        line2.data()[position * 3] = byte(AveregeB + 0.5);
        line2.data()[position * 3 + 1] = byte(AveregeG + 0.5);
        line2.data()[position * 3 + 2] = byte(AveregeR + 0.5);
    };

    auto CreateLine2 = [&](int h1, int w2) {//������ ��� ������ ����� �������
        for (int i = 0; i < w2; ++i) {
            CreatePixel(h1, Add, i);
        }
    };
    auto CreateAllLine2 = [&](int h1) {
        if (WOst)
        {
            CreateLine2(h1, bih.biWidth - 1);//��� -1 ������� �����, ���� �� ���� �� ���������
            CreatePixel(h1, WOst, bih.biWidth - 1);//��������� ������� � ������
        }
        else
        {
            CreateLine2(h1, bih.biWidth);//������ ��� ������
        }
    };

    for (int i = 0; i < HEIGHT / Add; ++i) {
        fread(line1.data(), (WIGTH * 3 + padding.before) * Add, 1, f1.getF());
        CreateAllLine2(Add);
        fwrite(line2.data(), bih.biWidth * 3 + padding.after, 1, f2.getF());
    }
    if (HOst)
    {
        CreateAllLine2(HOst);
        fwrite(line2.data(), bih.biWidth * 3 + padding.after, 1, f2.getF());
    }
    return 0;
}