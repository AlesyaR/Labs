//
//  main.cpp
//  Haffman
//
//  Created by Alesya on 16.10.16.
//  Copyright © 2016 Alesya. All rights reserved.
//

#include <iostream>
#include <time.h>
#include "/Users/alesya/Desktop/Study/code/WriteToFile.cpp"
#include "/Users/alesya/Desktop/Study/code/WriteToFile.hpp"
#include "/Users/alesya/Desktop/Study/code/ReadFromFile.cpp"
#include "/Users/alesya/Desktop/Study/code/ReadFromFile.hpp"

using namespace std;

int n = 30;

void decode (const char *in, const char *out)
{
    FILE *f_in = fopen (in, "rb");
    FILE *f_out = fopen (out, "wb");
    FILE *f_tree = fopen ("/Users/alesya/Desktop/Study/code/tree.txt", "rb");
    
    int* word = new int[n];
    int cur = 0;
    int out_msg = -1;
    int msg = 0;
    int flag = 0;
    int kol_word = 0;
    
    for (int i = 0; i < n; i++)
        word[i] = -1;
    
    fscanf(f_tree, "%d ", &kol_word);
    
    int** tree = new int* [kol_word];
    for (int j = 0; j < kol_word; j++)
        tree[j] = new int [n];
    
    fscanf(f_tree, "%d ", &msg) ;
    while (msg != -2)
    {
        if (msg == -1)
        {
            tree[flag][cur] = -1;
            flag++;
            cur = -1;
        }
        else
            tree[flag][cur] = msg;
        cur++;
        fscanf(f_tree, "%d ", &msg) ;
    }
    fclose(f_tree);

    
    
    cur = 0;
    flag = 0;
    fscanf(f_in, "%d ", &msg);
    
    while(msg != -1)
    {
        word[cur] = msg;
        out_msg = -1;
        flag = 0;
        for (int i = 0; (i < kol_word) && (flag != 2); i++)
        {
            for (int j = 1; (tree[i][j] != -1) && (flag == 0); j++)
                if (word[j - 1] != tree[i][j])
                    flag = 1;
            if ((flag == 0) && (tree[i][0] != -1))
            {
                flag = 2;
                out_msg = tree[i][0];
            }
            else
                flag = 0;
        }
        if (out_msg != -1)
        {
            fprintf(f_out,"%d ", out_msg);
            cur = 0;
            for (int i =0; i < n; i++)
                word[i] = -1;
        }
        else
            cur++;
        fscanf(f_in, "%d ", &msg);
    }
    fclose(f_in);
    fclose(f_out);
}



void code(const char *in, const char *out)
{
    FILE *f_in = fopen(in, "rb");
    FILE *f_out = fopen(out, "wb");
    FILE *f_tree = fopen ("/Users/alesya/Desktop/Study/code/tree.txt", "rb");
    
    int msg = 0;
    int i = 0;
    int k = 0;
    int kol_word = 0;
    int* in_file = new int[8];
    int k_file = 0;
    int sym_code = 0;
    int count = 0;

    
    fscanf(f_tree, "%d ", &kol_word);
    
    int** tree = new int* [kol_word];
    for (int j = 0; j < kol_word; j++)
        tree[j] = new int [n];
    
    fscanf(f_tree, "%d ", &msg) ;
    while (msg != -2)
    {
        if (msg == -1)
        {
            tree[i][k] = -1;
            i++;
            k = -1;
        }
        else
            tree[i][k] = msg;
        k++;
        fscanf(f_tree, "%d ", &msg) ;
    }
    fclose(f_tree);
    
    for (int i =0; i < 8; i++)
        in_file[i] = -1;
    
    fscanf(f_in, "%d ", &msg);
    while(msg != EOF)
    {
        count ++;
        k = -1;
        for (int j = 0; (j < kol_word); j++)
            if (tree[j][0] == msg)
            {
                for (i = 1; tree[j][i] != -1; i++)
                {
                    fprintf(f_out,"%d ", tree[j][i]);
                    sym_code ++;
                    in_file[k_file] = tree[j][i] - '0';
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
                j = kol_word;
            }
        fscanf(f_in, "%d ", &msg) ;
    }
    
    double h =(double)sym_code/count;
    cout << "H_pr = " << h<< endl;
    
    i = -1;
    fprintf(f_out,"%d ", i);
    fclose(f_in);
    fclose(f_out);
}


void build_tree(const char *in, int kol_msg, const char *out)
{
    FILE *f_in = fopen(in, "rb");
    
    
    int kol_v = n + 2;
    int msg = 0, lvl = 1, flag = 0;
    int kol_pr = 60;
    int kol_word = 0;
    int kol_word_msg = 0;
    double h = 0;
    double a = 0;
    
    double* fr = new double [n];
    
    double** pr_ch = new double* [kol_pr];  //таблица для построения дереваю (0 столбец) - сумма по веткам, (1) - уровень ветки, (>1) - вершины в ветке
    for (int i = 0; i < kol_pr; i++)
        pr_ch[i] = new double [kol_v];
     
    for (int i = 0; i < n; i++)
    {
        fr[i] = (double)0;
    }
    
    fscanf(f_in, "%d ", &msg);
    while(msg != EOF)               //считывание сообщения и подсчет частот символов
    {
        fr[msg]++;
        if (kol_word < msg)
            kol_word = msg;
        fscanf(f_in, "%d ", &msg) ;
        kol_word_msg ++;
    }
    fclose(f_in);
    
    for (int i = 0; i < kol_pr; i++)
    {
        if (i < n)
        {
            pr_ch[i][0] = (double)fr[i]/kol_word_msg;
            if (i <= kol_word)
            {
                a = (double)log2(pr_ch[i][0]);
                a = (double)a*pr_ch[i][0];
                h +=(double) a;
            }
            pr_ch[i][1] = 0;
            pr_ch[i][2]= i;
            lvl = 3;
        }
        else
            lvl = 0;
        for (int j = lvl; j < kol_v; j++)
            pr_ch[i][j] = -1;
    }
   
    h =(double)h*(-1);
    cout << "H_th = " << h<< endl;
    
    flag = 0;
    double min1 = 1, min2 = 1;
    int ind1 = -1, ind2 = -1, cur = n;
    lvl = 1;
    
    kol_word++;
    int** tree = new int* [kol_word];
    for (int i = 0; i < kol_word; i++)
        tree[i] = new int [n];
    
    for (int i = 0; i < kol_word; i++)
        for (int j = 0; j < n; j++)
            tree[i][j] = -1;
    
    //построение дерева     { если вышли за пределы можно пойти по второму кругу}
    
    
    while(flag == 0)
    {
        min1 = min2 = 1;
        for (int i = 0; (pr_ch[i][0] != -1); i++)   // нашли два минимальных значения
            if ((pr_ch[i][0] < min1) && (pr_ch[i][0] > 0))
            {
                min1 = (double)pr_ch[i][0];
                ind1 = i;
            }
        for (int i = 0; (pr_ch[i][0] != -1); i++)
            if ((pr_ch[i][0] < min2) && (pr_ch[i][0] > 0) && (i != ind1))
            {
                min2 = (double)pr_ch[i][0];
                ind2 = i;
            }
        
        pr_ch[cur][0] = min1 + min2;
    
        if ((ind1 == -1) && (ind2 == -1))
            break;
        
        if ((ind1 != -1) && (ind2 != -1))
        {   if ((pr_ch[ind1][1] < pr_ch[ind2][1]))
        {
            int k = ind2;
            ind2 = ind1;
            ind1 = k;
        }
        }
        else
        {
            ind2 = 0;
        }
        
        pr_ch[cur][1] = pr_ch[ind1][1] + 1;
        
        lvl = 2;
        int a = 0, b = 9;
        for (int  j = 2; pr_ch[ind1][j] != -1; j++, lvl ++)
        {
            pr_ch[cur][lvl] = (double)pr_ch[ind1][j];
            
            a = pr_ch[ind1][j];
            b = n - 1;
            while(tree[a][b] != -1)
                b--;
            tree[a][b] = 1;
            
        }
        for (int  j = 2; pr_ch[ind2][j] != -1; j++, lvl ++)
        {
            pr_ch[cur][lvl] = (double)pr_ch[ind2][j];
            
            a = pr_ch[ind2][j];
            b = n - 1;
            while(tree[a][b] != -1)
                b--;
            tree[a][b] = 0;
            
        }
        
        pr_ch[ind1][0] = -2;
        pr_ch[ind2][0] = -2;
        if (pr_ch[cur][0] >= 0.9999999)
            flag = 1;
        cur++;
    }
    
    for (int i = 0; i < kol_word; i++)
    {
        ind1 = 0;
        flag = 0;
        while(tree[i][ind1] == -1)
            ind1++;
        for (ind2 = 0; ind1 < n; ind1++, ind2++)
            pr_ch[0][ind2] = tree[i][ind1];
        pr_ch[0][ind2] = -1;
        
        for (ind1 = 0; ind1 < n; ind1++)
        {
            if (pr_ch[0][ind1] == -1)
                flag = 1;
            if (flag == 0)
                tree[i][ind1] = pr_ch[0][ind1];
            else
                tree[i][ind1] = -1;
        }
    }
    
    ind2 = 0;
    for (int i = 0; i < kol_word; i++)
        if (tree[i][0] != -1)
            ind2++;
        
    
    ind1 = -1;
    
    FILE *f_tree = fopen ("/Users/alesya/Desktop/Study/code/tree.txt", "wb");
    
    fprintf(f_tree,"%d ",ind2);
    
    for (int i = 0; i < kol_word; i++)
    {
        if (tree[i][0] != -1)
        {
            fprintf(f_tree,"%d ", i);
            for (int j = 0; tree[i][j] != -1; j++)
                fprintf(f_tree,"%d ", tree[i][j]);
            fprintf(f_tree,"%d ",ind1);
        }
    }
    ind1 = -2;
    fprintf(f_tree,"%d ",ind1);
    fclose(f_tree);
    //free(pr_ch);
    free(tree);
    free(fr);

    //кодирование
    code(in, out);
    
}

void srav()
{
    FILE *f_in = fopen ("/Users/alesya/Desktop/Study/code/msg.txt", "rb");
    FILE *f_out = fopen ("/Users/alesya/Desktop/Study/code/decode.txt", "rb");
    
    int msg = -1, dec = -1;
    
    fscanf(f_in, "%d ", &msg);
    fscanf(f_out, "%d ", &dec);
    
    while(msg != -1)
    {
        if (msg != dec)
            cout << "NOOOOOOOOO!!!!!!";
        fscanf(f_in, "%d ", &msg);
        fscanf(f_out, "%d ", &dec);
        
    }
    cout << "\nfinish\n";

}


int main(int argc, const char * argv[])
{
    srand(time(NULL));
    const char *in = "/Users/alesya/Desktop/Study/code/msg.txt";
    const char *out = "/Users/alesya/Desktop/Study/code/code.txt";
    const char *out_2 = "/Users/alesya/Desktop/Study/code/decode.txt";
    FILE *f = fopen ("/Users/alesya/Desktop/Study/code/code_1.txt", "w");
    fclose(f);

    int kol = 10;
    
    build_tree(in,kol,out);
    decode(out, out_2);
    srav();
    
    
    
    return 0;
}
