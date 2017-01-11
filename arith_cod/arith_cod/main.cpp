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
float fr1[2][10];

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



void code(const char *msg_f, const char *code, const char *fr_f, int l)
{
    int flag = 0;
    int count = 0;
    int count_word = 0;
    int msg = 0;
    float a = 0;
    int kol = 0;
    int max_word = 0;
    float h1 = 0, h2 = 0, l1 = 0, l2 = 0;
    
    FILE *f;
    //float* fr_r = new float [100000];
    float fr_r[1000];
    
    
    for (int i =0; i < 1000; i++)
        fr_r[i] = 0;
    
    cout << "Do you want read freq. from file?[1 - yes, 2 - equiprobably, 'another' - no]  ";
    cin >> flag;
    
    float h = 0;
    float aa = 0;
    
    // получение частоты слов
    
    if (flag == 1)
    {
        f = fopen(fr_f, "rb");
        fscanf(f, "%d ",&msg);
        
        while(msg != -1)
        {
            fr_r[kol] = (float)msg/1000000;
            aa = (float)log2(fr_r[kol]);
            aa = (float)aa * fr_r[kol];
            h += (float)aa;
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
                    fr_r[i] = (float)(fr_r[i - 1] + 1/max_word);
                else
                    fr_r[i] = (float)1/max_word;
                aa = (float)log2(fr_r[i]);
                aa = (float)aa * fr_r[i];
                h += (float)aa;
                msg = fr_r[i]*1000000;
                fprintf(f,"%d ", msg);
            }
        else
            for (int i = 0; i < max_word; i++)
            {
                fr_r[i] = (float)fr_r[i]/kol;
                aa = (float)log2(fr_r[i]);
                aa = (float)aa * fr_r[i];
                h += (float)aa;
                msg = fr_r[i]*1000000;
                fprintf(f,"%d ", msg);
            }
        msg = -1;
        fprintf(f,"%d ", msg);
        fclose (f);
    }
    
    h = (float)h * (-1);
    cout << "H_th = " << h << endl;
    
    
    /*float** fr = new float* [2];
    fr[0] = new float [max_word];
    fr[1] = new float [max_word];*/
    
    
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
                h1 = (float)fr_r[j];
                flag = j;
            }
        fr1[1][i] += (float)h1;
        fr_r[flag] = 0;
        fr1[0][i] = flag;
    }
    
   // free(fr_r);
    
    f = fopen(msg_f, "rb");
    FILE *out = fopen (code, "wb");
    fscanf(f, "%d ",&msg);
    kol = 0;
    fr1[max_word - 1][0] = 1;
    
    while(msg != -1)
    {
        count_word++;
        if (kol == 0)
        {
            for (int i = 0; i < max_word; i++)
                if (msg == fr1[0][i])
                {
                    h1 = (float)fr1[1][i];
                    l1 = (float)fr1[1][i - 1];
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
                    h2 = (float)fr1[1][i];
                    l2 = (float)fr1[1][i - 1];
                    if (i == 0)
                        l2 = 0;
                    i = max_word;
                }
            h1 =(float) (l1 + (h1 - l1)*h2);
            l1 =(float) (l1 + (h1 - l1)*l2);
        }
        
        kol++;
        
        if (kol == l)
        {
            flag = (float)h1*100;
            count ++;
            fprintf(out,"%d ", flag);
            to2(flag);
            kol = 0;
        }
        fscanf(f, "%d ",&msg);
    }
    
    cout << count_word;
    
    if (kol > 0)
    {
        flag = h1*100;
        count ++;
        fprintf(out,"%d ", flag);
    }
    
    h = 0;
    h = (float)count/count_word;
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
    
   // float* fr_r = new float [100000];
    
    float fr_r[1000];
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
            fr_r[kol] = (float)msg/1000000;
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
                    fr_r[i] = (float)(fr_r[i - 1] + 1/max_word);
                else
                    fr_r[i] = (float)1/max_word;
        else
            while(msg != -1)
            {
                fr_r[kol] = (float)msg/1000000;
                fscanf(f, "%d ",&msg);
                kol++;
            }
        fclose(f);
    }
    
   /* float** fr = new float* [2];
    fr[0] = new float [max_word];
    fr[1] = new float [max_word];*/
    
    
    float h1 = 0, l1 = 0, h2 = 0, l2 = 0;
    
    
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
                h1 = (float)fr_r[j];
                flag = j;
            }
        fr1[1][i] += (float)h1;
        fr_r[flag] = 0;
        fr1[0][i] = flag;
    }
    
    fr1[max_word - 1][0] = 1;
   // free(fr_r);
    
    fscanf(f_in, "%d ",&msg);
    kol = 0;
    
    while(msg != -1)
    {
        if (kol == l)
        {
            kol = -1;
            fscanf(f_in, "%d ",&msg);
        }
        else
        {
            if (kol == 0)
                l1 = (float)msg/100;
            
            for (int i = 0; i < max_word; i++)
                if (l1 < fr1[1][i])
                {
                    flag = fr1[0][i];
                    fprintf(f_out,"%d ", flag);
                    h2 = fr1[1][i];
                    l2 = fr1[1][i - 1];
                    i = max_word;
                }
            l1 = (float)(l1 - l2)/(h2 - l2);
        }
        kol++;
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
            //cout << "0";
            count ++;
        }
        else
           //  cout << "1";
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
    
    int l = 2;
    k_file = 0;

    
    code(msg, code_f,fr, l);
    
    decode (decode_f, code_f, fr, l);
    srav();
    
    return 0;
}
