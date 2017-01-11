//
//  main.cpp
//  Golomb_1_lab
//
//  Created by Alesya on 20.09.16.
//  Copyright © 2016 Alesya. All rights reserved.
//

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <math.h>
#include "/Users/alesya/Desktop/Study/code/WriteToFile.cpp"
#include "/Users/alesya/Desktop/Study/code/WriteToFile.hpp"
#include "/Users/alesya/Desktop/Study/code/ReadFromFile.cpp"
#include "/Users/alesya/Desktop/Study/code/ReadFromFile.hpp"


using namespace std;

int cur;
long int cur1;
long int cur2;
int sym;
int max_n;
int* in_file = new int[8];
int k_file;

const char *msg = "/Users/alesya/Desktop/Study/code/msg.txt";
const char *code = "/Users/alesya/Desktop/Study/code/code.txt";
const char *decode = "/Users/alesya/Desktop/Study/code/decode.txt";

//получение числа, котрое будет закодировано.

int get_n(double p)
{
    double i = 0;
    int k = 0;
    while (i == 0)
    {
        i = (double)rand()/RAND_MAX;
        if (i < p)
            i = 1;
        else
        {
            i = 0;
            k++;
        }
    }
    return k;
}

int test_m(int m)
{
    int res = 0;
    int a = 2;
    
    for (int i = 0; i < 15; i++)
        if (a == m)
            return 1;
        else
            a = a*2;
    return 0;
}

void putbits(int number[3])
{
    int i = 19;
    int bits[20];
    
    while(number[1] > 1)
    {
        bits[i] = number[1]%2;
        number[1] = number[1]/2;
        i--;
    }
    bits[i] = number[1];
    
    while ((20 - i) < number[2])
    {
        i--;
        bits[i] = 0;
    }
    
    i--;
    bits[i] = 1;
    i--;
    
    for (int j = 0; j < number[0]; j++, i--)
        bits[i] = 0;
    
    FILE *f = fopen(code,"rb+");
    fseek(f, 0, SEEK_END);
    for (i++; i < 20; i++)
    {
        fprintf(f,"%d ", bits[i]);
        in_file[k_file] = bits[i] - '0';
        if (k_file == 7)
        {
            k_file = 0;
            CWriteToFile("/Users/alesya/Desktop/Study/code/code_1.txt", in_file, 8);
            for (int y = 0; y < 8; y++)
                in_file[y] = -1;
        }
        else
            k_file++;
        sym++;
    }
    fclose(f);
}


int codeGolomb(int m)
{
    int n = 0;
    int kol_word = 0;
    
    FILE *f = fopen(code,"wb");
    fprintf(f,"%d\n", m);
    fclose(f);
    
    FILE *f_in = fopen (msg, "rb");
    fscanf(f_in, "%d ", &n);
    while(n != -1)
    {
        kol_word++;
        int number[3]; // 0 - целое число, 1 - остаток, который будет кодироваться, 2 - количество бит для кодирования остатка
        number[0] = n/m;
        number[1] = n%m;
        int b = (int)(log2(m) + 1);
        double c = (double)(pow(2,b) - m);
        
        if (n > max_n)
            max_n = n;
        
        if (test_m(m) == 0)
            if (number[1] < c)
                number[2] = b - 1;
            else
            {
                number[1] = number[1] + c;
                number[2] = b;
            }
            else
                number[2] = log2(m);
        
        putbits(number);
        fscanf(f_in, "%d ", &n);
    }
    
    FILE *f1 = fopen(code,"rb+");
    fseek(f1, 0, SEEK_END);
    n = -1;
    fprintf(f1,"%d ", n);
    fclose(f1);
    return kol_word;
}

int decodeGolomb(int m, int bits[20], int i, int b)
{
    int j = 0;
    int result[3] = {0,0,0};
    
    while (bits[j] != 1)    //получение числа целой части
    {
        result[0] ++;
        j++;
    }
    
    int k = 1;
    
    if (cur1 == cur2)
        b = log2(m);
    
    for (j ++ ; j < i; j++, k++)    //получения числа дробной части
    {
        result[2] += pow(2, (b-k))*bits[j];
        if (j < (i-1))
            result[1] += pow(2, (b-k-1))*bits[j];
    }
    
    result[2] = result[2] + m - pow(2,b);
    
    if (result[1] < (pow(2, b) - m))
    {
        result[0] = result[0]*m + result[1];
    }
    else
    {
        result[0] = result[0]*m + result[2];
        cur1 = cur2;
    }
    return result[0];
}

int getbits(int m, int bit)
{
    int bits[20];
    int i = 0;
    int b = (int)(log2(m) + 1);
    
    FILE *f = fopen(code,"rb");
    fseek(f, cur1, SEEK_SET);
    
    bits[i] = bit;
    i++;
    
    while (bits[i - 1] != 1)    //читаем целую часть деления
    {
        fscanf(f, "%d ",&bits[i]);
        i++;
    }
    
    if (test_m(m) == 0)
    {
        for (int j = 0; j < b; j++, i++)    //читаем дробную часть деления
        {
            fscanf(f, "%d ",&bits[i]);
            if (j == (b - 2))
                cur1 = ftell(f);    // если было потрачено b-1 бит
        }
        
        cur2 = ftell(f);            // если потрачено b бит
    }
    else
    {
        for (int j = 0; j < log2(m); j++, i++)    //читаем дробную часть деления
        {
            fscanf(f, "%d ",&bits[i]);
        }
        
        cur2 = ftell(f);
        cur1 = cur2;
    }
    int res =  decodeGolomb(m, bits, i, b);
    fclose(f);
    return res;
}

void test_geom_rasp(double p)
{
    double kol[60];
    double h = 0;
    int max = 30;
    int k = 0;
    int exp = 10000;
    for (int i = 0; i < max; i++)
        kol[i] = 0;
    for (int i = 0; i < exp; i++)
    {
        k = get_n(p);
        if (k < 30)
            kol[k]++;
    }
    
    for (int i = 0; i < max; i++)
    {
        kol[i] = (double)(kol[i]/exp);
        if (kol[i] != 0)
            h += (double)(kol[i]*log2(kol[i]));
    }
    
    h = -h;
    cout << "\nH_th = " << h << endl;
    
}


void compress(int kol)  //энтропия
{
    double b = 1;
    int i = 0;
    while (b < max_n)
    {
        i++;
        b = b*2;
    }
    
    i = kol*i;
    b = (double)sym/i;
    b = (double)(1 - b);
    b = (double)sym/kol;
    cout<< "H_pr = " << b << endl;
    
}

int main(int argc, const char * argv[])
{
    srand(time(0));
    
    int m = 6;
    int kk = 0;
    double m_op = 0;
    sym = 0;
    max_n = 0;
    double p = 0.2;
    k_file = 0;
    
    FILE *f = fopen("/Users/alesya/Desktop/Study/code/code_1.txt", "wb");
    fclose(f);
    
    m_op = (double)(-1)*log2(2 - p);
    m_op = (double)m_op/log2(1 - p);
    if (m_op < 1)
        m_op = 1;
    
    cout << "Input p = ";
    cin >> p;
    
    cout << "Do you use m-optimum? [1 - yes, another - no]:";
    cin >> kk;
    if (kk == 1)
        m = (int)m_op+1;
    
    // int kol = 10;
    char a;
    cur = 0;
    
    test_geom_rasp(p);
    
    // кодирование
    int kol = codeGolomb(m);
    
    compress(kol);
    cout << "\nout:"<< endl;
    
    //декодиование
    
    f = fopen(code,"rb");
    m = 0;
    cur = 0;
    int bit = 0;
    fscanf(f,"%d/n", &m);
    fscanf(f, "%c",&a);
    fscanf(f, "%d ",&bit);
    cur1 = ftell(f);
    fclose(f);
//    delete f;
    
    FILE *out = fopen(decode, "wb");
    int res_msg = 0;
    
    
    while (bit != EOF)
    {
        res_msg = getbits(m, bit);
        fprintf(out,"%d ", res_msg);
        
        FILE *f = fopen(code,"rb");
        fseek(f, cur1, SEEK_SET);
        fscanf(f, "%d",&bit);
        cur1 = ftell(f);
        fclose(f);
    }
    
    
    
    return 0;
}