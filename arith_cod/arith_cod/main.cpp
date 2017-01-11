//
//  main.cpp
//  arith_cod
//
//  Created by Alesya on 11.12.16.
//  Copyright © 2016 Alesya. All rights reserved.
//

#include <iostream>
#include <math.h>
#include "/Users/alesya/Desktop/Study/code/WriteToFile.cpp"
#include "/Users/alesya/Desktop/Study/code/WriteToFile.hpp"
#include "/Users/alesya/Desktop/Study/code/ReadFromFile.cpp"
#include "/Users/alesya/Desktop/Study/code/ReadFromFile.hpp"
using namespace std;

int in_file[8];
int k_file;
double fr1[2][10];
int del = 100;

void to2 (int n)
{
    int bits[32];
    int kol = 31;
    
    for (int i =0; i < 32; i++)
        bits[i] = -1;
    
    for (kol = 31; (n > 1); kol--)
    {
        bits[kol] = n%2;
        n = n/2;
    }
    bits[kol] = 1;
    
    for (int i = kol; i < 32; i++)
    {
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
    }
}

int kol_t;

void decode_fun(double l1, int l, int max_word)
{
    FILE *f_out = fopen("/Users/alesya/Desktop/Study/code/decode.txt", "rb+");
    fseek(f_out, 0, SEEK_END);
    
    int kol = 0, flag = 0, k = 0;
    double l2 = 0, h2 = 0, h1 = 0;
    l2 = 0;
    
    kol_t++;
    h2 = 1;
    h1 = (double)(l1 - l2)/(h2 - l2);
    while (kol != l)
    {
        if(kol_t== 24)
            int ii =0;
    
        if (((0 <= h1) && ( h1 < fr1[1][0])) || (abs(fr1[1][0] - h1) < 0.0000000001))
        {
            if (abs(fr1[1][0] - h1) < 0.00000000001)
            {
                h1 = (double)(h2 - l2);
                h2 = (double)(l2 + h1*fr1[1][1]);
                l2 = (double)(l2 + h1*fr1[1][0]);
                flag = fr1[0][1];
 
            }
            else
            {
                h1 = (double)(h2 - l2);
                h2 = (double)(l2 + h1*fr1[1][0]);
                l2 = (double)(l2 + h1*0);
                flag = fr1[0][0];
            }
            fprintf(f_out,"%d ", flag);
        }
        else
            for (int i = 1; i < max_word; i++)
                if (((fr1[1][i - 1] <= h1) && ( h1 < fr1[1][i])) || (abs(fr1[1][i] - h1) < 0.000000001))
                {
                    if (abs(fr1[1][i] - h1) < 0.000000001)
                        i++;
                    h1 = (double)(h2 - l2);
                    h2 = (double)(l2 + h1*fr1[1][i]);
                    l2 = (double)(l2 + h1*fr1[1][i-1]);
                    flag = fr1[0][i];
                    fprintf(f_out,"%d ", flag);
                    break;
                    
                }
        kol++;
        h1 = (double)(l1 - l2)/(h2 - l2);
    }
    
    fclose(f_out);
}

void code(const char *msg_f, const char *code, const char *fr_f, int l)
{
    int flag = 0;
    int count = 0;
    int count_word = 0;
    int msg = 0;
    double a = 0;
    int kol = 0;
    int max_word = 0;
    double h1 = 0, h2 = 0, l1 = 0, l2 = 0;
    
    FILE *f;
    //double* fr_r = new double [100000];
    double fr_r[1000];
    
    
    for (int i =0; i < 1000; i++)
        fr_r[i] = 0;
    
    cout << "Do you want read freq. from file?[1 - yes, 2 - equiprobably, 'another' - no]  ";
    cin >> flag;
    
    double h = 0;
    double aa = 0;
    
    // получение частоты слов
    
    if (flag == 1)
    {
        f = fopen(fr_f, "rb");
        fscanf(f, "%d ",&msg);
        
        while(msg != -1)
        {
            fr_r[kol] = (double)msg/1000000;
            aa = (double)log2(fr_r[kol]);
            aa = (double)aa * fr_r[kol];
            h += (double)aa;
            fscanf(f, "%d ",&msg);
            kol++;
        }
        max_word = kol;
        fclose(f);
    }
    else
    {
        f = fopen(msg_f, "rb");
        fscanf(f, "%d ",&msg);
        kol = 0;
        
         while(msg != -1)
         {
             fr_r[msg] ++;
             kol++;
             if (msg > max_word)
                 max_word = msg;
             fscanf(f, "%d ",&msg);
         }
        max_word++;
        fclose (f);
        
        f = fopen("/Users/alesya/Desktop/Study/code/fr_prac.txt", "wb");
        fprintf(f,"%d ", max_word);
        if (flag == 2)
            for (int i = 0; i <= max_word; i++)
            {   if (i > 0)
                    fr_r[i] = (double)(fr_r[i - 1] + 1/max_word);
                else
                    fr_r[i] = (double)1/max_word;
                aa = (double)log2(fr_r[i]);
                aa = (double)aa * fr_r[i];
                h += (double)aa;
                msg = fr_r[i]*1000000;
                fprintf(f,"%d ", msg);
            }
        else
            for (int i = 0; i < max_word; i++)
            {
                fr_r[i] = (double)fr_r[i]/kol;
                aa = (double)log2(fr_r[i]);
                aa = (double)aa * fr_r[i];
                h += (double)aa;
                msg = fr_r[i]*1000000;
                fprintf(f,"%d ", msg);
            }
        msg = -1;
        fprintf(f,"%d ", msg);
        fclose (f);
    }
    
    h = (double)h * (-1);
    cout << "H_th = " << h << endl;
    
    
    /*double** fr = new double* [2];
    fr[0] = new double [max_word];
    fr[1] = new double [max_word];*/
    
    
    for (int i = 0; i < max_word; i++)
    {
        flag = 0;
        h1 = 0;
        fr1[1][i] = 0;
        if (i > 0)
            fr1[1][i] = fr1[1][i - 1];
    
        for(int j = 0; j < max_word; j++)
            if (h1 < fr_r[j])
            {
                h1 = (double)fr_r[j];
                flag = j;
            }
        fr1[1][i] += (double)h1;
        fr_r[flag] = 0;
        fr1[0][i] = flag;
    }
    
   // free(fr_r);
    std::vector<int> iiVect;
    
    f = fopen(msg_f, "rb");
    FILE *out = fopen (code, "wb");
    fscanf(f, "%d ",&msg);
    iiVect.push_back(msg);
    kol = 0;
    fr1[max_word - 1][0] = 1;
    double xx = 0;

    while(msg != -1)
    {
        count_word++;
        if(kol_t== 23)
            int ii =0;
        
        if (kol == 0)
        {
            for (int i = 0; i < max_word; i++)
                if (msg == fr1[0][i])
                {
                    h1 = (double)fr1[1][i];
                    l1 = (double)fr1[1][i - 1];
                    if (i == 0)
                        l1 = 0;
                    break;
                }
        }
        else
        {
            for (int i = 0; i < max_word; i++)
                if (msg == fr1[0][i])
                {
                    h2 = (double)fr1[1][i];
                    l2 = (double)fr1[1][i - 1];
                    if (i == 0)
                        l2 = 0;
                    i = max_word;
                }
            xx = (double) (h1 - l1);
            h1 =(double) (l1 + xx*h2);
            l1 =(double) (l1 + xx*l2);
        }
        
        kol++;
        
        if (kol == l)
        {
            flag = (double)l1*del;
            count ++;
            fprintf(out,"%d ", flag);
            decode_fun(l1, l, max_word);
            to2(flag);
            kol = 0;
        }
        fscanf(f, "%d ",&msg);
        
        if(iiVect.size() < 100)
        {iiVect.push_back(msg);}
    }
    
    cout << count_word;
    
    if (kol > 0)
    {
        flag = l1*del;
        count ++;
        fprintf(out,"%d ", flag);
        to2(flag);
        decode_fun(l1, l, max_word);
    }
    
    h = 0;
    h = (double)count/count_word;
   // cout << "H_pr = " << h << endl;
    
    flag = -1;
    fprintf(out,"%d ", flag);
    fclose(f);
    fclose(out);
   // free(fr);
}

void decode(const char *decode_f,const char *code_f, const char *fr_f, int l)
{
    FILE *f_out = fopen(decode_f, "wb");
    FILE *f_in = fopen(code_f, "rb");
    int msg = 0;
    int flag = 0;
    int kol = 0;
    int max_word = 0;
    
   // double* fr_r = new double [100000];
    
    double fr_r[1000];
    for (int i =0; i < 1000; i++)
        fr_r[i] = 0;
    
    cout << "Do you want read freq. from file (first file)?[1 - yes, 2 - equiprobably, 'another' - no]  ";
    cin >> flag;
    
    // получение частоты слов
    
    if (flag == 1)
    {
        FILE *f = fopen(fr_f, "rb");
        fscanf(f, "%d ",&msg);
        
        while(msg != -1)
        {
            fr_r[kol] = (double)msg/1000000;
            fscanf(f, "%d ",&msg);
            kol++;
        }
        max_word = kol;
        fclose(f);
    }
    else
    {
        FILE *f = fopen("/Users/alesya/Desktop/Study/code/fr_prac.txt", "rb");
        fscanf(f, "%d ",&max_word);
        
        fscanf(f, "%d ",&msg);
        
        if (flag == 2)
            for (int i = 0; i <= max_word; i++)
                if (i > 0)
                    fr_r[i] = (double)(fr_r[i - 1] + 1/max_word);
                else
                    fr_r[i] = (double)1/max_word;
        else
            while(msg != -1)
            {
                fr_r[kol] = (double)msg/1000000;
                fscanf(f, "%d ",&msg);
                kol++;
            }
        fclose(f);
    }
    
   /* double** fr = new double* [2];
    fr[0] = new double [max_word];
    fr[1] = new double [max_word];*/
    
    
    double h1 = 0, l1 = 0, h2 = 0, l2 = 0;
    
    
    for (int i = 0; i < max_word; i++)
    {
        flag = 0;
        h1 = 0;
        fr1[1][i] = 0;
        if (i > 0)
            fr1[1][i] = fr1[1][i - 1];
        
        for(int j = 0; j < max_word; j++)
            if (h1 < fr_r[j])
            {
                h1 = (double)fr_r[j];
                flag = j;
            }
        fr1[1][i] += (double)h1;
        fr_r[flag] = 0;
        fr1[0][i] = flag;
    }
    
    fr1[max_word - 1][0] = 1;
   // free(fr_r);
    kol = 0;
    fscanf(f_in, "%d ",&msg);
    l1 = (double)msg/del;
    
    while(msg != -1)
    {
        
        if (kol == l)
        {
            kol = 0;
            fscanf(f_in, "%d ",&msg);
            l1 = (double)msg/del;
        }
        l2 = 0;
        h2 = 1;
        h1 = (double)(l1 - l2)/(h2 - l2);
        while (kol != l)
        {
            if ((0 <= h1) && ( h1 < fr1[1][0]))
            {
                h2 = l2 + (h2 - l2)*fr1[1][0];
                l2 = l2 + (h2 - l2)*0;
                flag = fr1[0][0];
                fprintf(f_out,"%d ", flag);
            }
            else
                for (int i = 1; i < max_word; i++)
                    if ((fr1[1][i - 1] <= h1) && ( h1 < fr1[1][i]))
                    {
                        h1 = (double)(h2 - l2);
                        h2 = (double)(l2 + h1*fr1[1][i]);
                        l2 = (double)(l2 + h1*fr1[1][i-1]);
                        flag = fr1[0][i];
                        //fprintf(f_out,"%d ", flag);
                    }
            kol++;
            h1 = (double)(l1 - l2)/(h2 - l2);
        }
    }

    fclose(f_in);
    fclose(f_out);
   // free(fr);
}

void srav()
{
    FILE *f_in = fopen ("/Users/alesya/Desktop/Study/code/msg.txt", "rb");
    FILE *f_out = fopen ("/Users/alesya/Desktop/Study/code/decode.txt", "rb");
    
    int msg = -1, dec = -1;
    int count = 0;
    
    fscanf(f_in, "%d ", &msg);
    fscanf(f_out, "%d ", &dec);
    
    while(msg != -1)
    {
        if (msg != dec)
        {
           // cout << "0";
            count ++;
        }
        else
          //   cout << "1";
        fscanf(f_in, "%d ", &msg);
        fscanf(f_out, "%d ", &dec);
        
    }
    cout << "\nfinish\n";
    
}


int main(int argc, const char * argv[])
{
    
    const char *msg = "/Users/alesya/Desktop/Study/code/msg.txt";
    const char *code_f = "/Users/alesya/Desktop/Study/code/code.txt";
    const char *decode_f = "/Users/alesya/Desktop/Study/code/decode.txt";
    const char *fr = "/Users/alesya/Desktop/Study/code/fr.txt";
    
    FILE *f = fopen("/Users/alesya/Desktop/Study/code/code_1.txt", "w");
    fclose(f);
    f = fopen("/Users/alesya/Desktop/Study/code/decode.txt", "w");
    fclose(f);
    
    int l = 4;
    k_file = 0;
    kol_t = 0;

    
    code(msg, code_f,fr, l);
    
    //decode (decode_f, code_f, fr, l);
    srav();
    
    return 0;
}
