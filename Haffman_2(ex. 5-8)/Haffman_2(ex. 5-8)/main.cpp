//
//  main.cpp
//  Haffman
//
//  Created by Alesya on 16.10.16.
//  Copyright © 2016 Alesya. All rights reserved.
//

#include <iostream>
#include <time.h>
#include "math.h"
#include "/Users/alesya/Desktop/Study/code/WriteToFile.cpp"
#include "/Users/alesya/Desktop/Study/code/WriteToFile.hpp"
#include "/Users/alesya/Desktop/Study/code/ReadFromFile.cpp"
#include "/Users/alesya/Desktop/Study/code/ReadFromFile.hpp"


using namespace std;

int n = 30;
double mass[30][30];

struct tree_full
{
    int tree_i[40][40];
};

void decode (const char *in, const char *out)
{
    FILE *f_in = fopen (in, "rb");
    FILE *f_out = fopen (out, "wb");
    FILE *f_tree = fopen ("/Users/alesya/Desktop/Study/code/tree.txt", "rb");
    FILE *f_msg = fopen ("/Users/alesya/Desktop/Study/code/msg.txt", "rb");
    
    int* word = new int[40];
    //int word [40];
    int cur = 0;
    int out_msg = -1;
    int msg = 0;
    int lvl = 0;
    int flag = 0;
    int kol_word = 0;
    int kol_word_msg = 0;
    int qqqqqq = 0;
    
    for (int i = 0; i < 40; i++)
        word[i] = -1;
    
    fscanf(f_tree, "%d ", &kol_word);
    fscanf(f_tree, "%d ", &msg) ;
    
    //tree_full *tree = new tree_full[kol_word];
    
    tree_full tree[20];
    
    while (msg != -2)
    {
        if ((msg == -1) || (msg == -3))
        {
            tree[lvl].tree_i[flag][cur] = -1;
            cur = -1;
            if (msg == -3)
            {
                flag = 0;
                lvl++;
            }
            else
                flag++;
            
        }
        else
            tree[lvl].tree_i[flag][cur] = msg;
        cur++;
        fscanf(f_tree, "%d ", &msg) ;
    }
    fclose(f_tree);
    
    lvl = 0;
    cur = 0;
    flag = 0;
    fscanf(f_in, "%d ", &msg);
    
    while(msg != -1)
    {
        word[cur] = msg;
        out_msg = -1;
        flag = 0;
        
        for (int i = 0; (i <= kol_word) && (flag != 2); i++)
        {
            for (int j = 1; (tree[lvl].tree_i[i][j] != -1) && (flag == 0); j++)
                if (word[j - 1] != tree[lvl].tree_i[i][j])
                    flag = 1;
            if ((flag == 0) && (tree[lvl].tree_i[i][0] != -1))
            {
                if (qqqqqq == 9)
                    cout << "!!!";
                flag = 2;
                out_msg = tree[lvl].tree_i[i][0];
            }
            else
                flag = 0;
        }
        if (out_msg != -1)
        {
            fprintf(f_out,"%d ", out_msg);
            
            cur = 0;
            for (int i =0; i < 40; i++)
                word[i] = -1;
            lvl = out_msg;
            kol_word_msg ++;
            fscanf(f_msg, "%d ", &msg);
            if (msg != lvl)
                cout << "!!:";
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
    int lvl = 0;
    int kol_word = 0;
    int sym_code = 0;
    int max_word_msg = 0;
    int kol_word_msg = 0;
    int qqq = 0;
    
    
    int* in_file = new int[8];
    int k_file = 0;
    
    fscanf(f_tree, "%d ", &kol_word);
    //tree_full *tree = new tree_full[kol_word];
    
    tree_full tree[20];
    
    fscanf(f_tree, "%d ", &msg) ;
    
    while (msg != -2)
    {
        if ((msg == -1) || (msg == -3))
        {
            tree[lvl].tree_i[i][k] = -1;
            k = -1;
            if (msg == -3)
            {
                i = 0;
                lvl++;
            }
            else
                i++;
                
        }
        else
            tree[lvl].tree_i[i][k] = msg;
        k++;
        fscanf(f_tree, "%d ", &msg) ;
    }
    fclose(f_tree);
    
    fscanf(f_in, "%d ", &msg);
    if (msg > max_word_msg)
        max_word_msg = msg;
   
    kol_word_msg++;
    
    for (int y = 0; y < 8; y++)
        in_file[y] = -1;
    
    lvl = 0;
    while(msg != EOF)
    {
        k = -1;
    
        for (int j = 0; (j <= kol_word); j++)
            if (tree[lvl].tree_i[j][0] == msg)
            {
                for (i = 1; tree[lvl].tree_i[j][i] != -1; i++)
                {   fprintf(f_out,"%d ", tree[lvl].tree_i[j][i]);
                    sym_code ++;
                    
                    in_file[k_file] = tree[lvl].tree_i[j][i] - '0';
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
        lvl = msg;
        fscanf(f_in, "%d ", &msg) ;
        if (msg > max_word_msg)
            max_word_msg = msg;
        kol_word_msg++;

    }
    i = -1;
    fprintf(f_out,"%d ", i);
    

    double d = (double)sym_code/(kol_word_msg - 1);   //кол-во бит на слово
    cout << "\nH_pr = " << d << "\n";
  
    fclose(f_in);
    fclose(f_out);
   // free(tree);
}

void OBR_MATR(int cnt_str, double H[7])//, double mass[30][30])
{
    int i,j,k;
    double sum;
    
   /* double LU[n][n];
    double M_obr[n][n];
    
    
    
    double* H = new double [cnt_str];
    
    double H[6];
    
    for (i = 0; i < cnt_str;i++)
    {
        H[i] = 0;
        for (j = 0; j < cnt_str; j++)
        {
            if (mass[i][j] != 0)
            {
                sum = (double)log2(mass[i][j]);
                sum = (double)sum*mass[i][j];
                H[i] += (double)sum;
            }
        }
        if (H[i] != 0)
            H[i] *= (double)-1;
    }*/
    
    double LU[30][30];
    double M_obr[30][30];
    
    for(i=0;i<cnt_str;i++)
    {
        for(j=0;j<cnt_str;j++)
        {
            sum=0;
            if(i<=j)
            {
                for(k=0;k<i;k++)
                    sum+=LU[i][k]*LU[k][j];
                LU[i][j]=mass[i][j]-sum;//вычисляем элементы верхней треугольной матрицы
            }
            else
            {
                for(k=0;k<j;k++)
                    sum+=LU[i][k]*LU[k][j];
                if(LU[j][j]==0)
                    cout << "FAIL!!!\n";
                LU[i][j]=(mass[i][j]-sum)/LU[j][j];//вычисляем элементы нижней треугольной матрицы
            }
        }
        
    }
    int p;
    for(i=cnt_str-1;i>=0;i--)//нахождение обратной матрицы
    {
        for(j=cnt_str-1;j>=0;j--)
        {
            sum=0;
            if(i==j)
            {
                for(p=j+1;p<cnt_str;p++)
                    sum+=LU[j][p]*M_obr[p][j];
                M_obr[j][j]=(1-sum)/LU[j][j];
            }
            else if(i<j)
            {
                for(p=i+1;p<cnt_str;p++)
                    sum+=LU[i][p]*M_obr[p][j];
                M_obr[i][j]=-sum/LU[i][i];
            }
            else
            {
                for(p=j+1;p<cnt_str;p++)
                    sum+=M_obr[i][p]*LU[p][j];
                M_obr[i][j]=-sum;
            }
            
        }
    }
    
    double H_th = 0;
    for (int i = 0; i < cnt_str; i++)
    {
        H_th = (double)(H_th + H[i]*abs(M_obr[i][0]));
    }
    cout <<"H_th = " << H_th <<endl;
    
}



void build_tree(const char *in, int kol_msg, const char *out)
{
    
    FILE  *f_in;
    
    int kol_v = n + 2;
    int msg = 0, lvl = 1;
    int kol_pr = 40;
    int kol_word = 0;
    int kol_word_msg = 0;
    int flag = -1;
    
   /* double** fr = new double* [n];
    for (int i = 0; i < n; i++)
        fr[i] = new double [n];*/
    
    double fr[30][30];
    
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            fr[i][j] = (double)0;
    
    cout << "Do you want read matrix freq.? [1 - yes,'another' - no]:  ";
    cin >> flag;
    
    if (flag == 1)
    {
        f_in = fopen("/Users/alesya/Desktop/Study/code/matrix.txt", "rb");
        fscanf(f_in, "%d ", &n);
        
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
            {
                fscanf(f_in, "%d ", &flag);
                fr[i][j] = (double)flag /1000000000;
            }
        
        kol_word = n;
       // n = 30;
        
        fclose(f_in);
        
        f_in = fopen ("/Users/alesya/Desktop/Study/code/tree.txt", "wb");
        fprintf(f_in,"%d ",kol_word);
        fclose(f_in);
        
    }
    else
    {
        f_in = fopen(in, "rb");
        fscanf(f_in, "%d ", &msg);
        int* count_msg = new int [n];
        lvl = 0;
        kol_msg = 0;
        while(msg != EOF)               //считывание сообщения и подсчет частот символов
        {
            fr[lvl][msg]++;
            count_msg[lvl]++;
            if (kol_word < msg)
                kol_word = msg;
            kol_msg ++;
            lvl = msg;
            fscanf(f_in, "%d ", &msg) ;

        }
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                fr[i][j] = (double)fr[i][j]/count_msg[i];
         fclose(f_in);
        kol_word++;
        n = kol_word;
        f_in = fopen ("/Users/alesya/Desktop/Study/code/tree.txt", "wb");
        fprintf(f_in,"%d ",kol_word);
        fclose(f_in);
        //kol_word--;
    }
    
    
    lvl = 1;
    
    double min1 = 1, min2 = 1;
    int ind1 = -1, ind2 = -1, cur = n;
    
    for (int i = 0; i < 30; i++)
        for (int j = 0; j < 30; j++)
            if (i != j)
                mass[i][j] = fr[i][j];
            else
                mass[i][j] = (double)(fr[i][j] - 1);
   
    for (int i = 0; i < 30;i++)
        mass[0][i] = 1;
    
    double H[7];
    for (int i = 0; i < kol_word;i++)
    {
        H[i] = 0;
        for (int  j = 0; j < kol_word; j++)
        {
            if (fr[i][j] != 0)
            {
                H[6] = (double)log2(fr[i][j]);
                H[6] = (double)H[6]*fr[i][j];
                H[i] += (double)H[6];
            }
        }
        if (H[i] != 0)
            H[i] *= (double)-1;
    }

    OBR_MATR(kol_word, H);//, fr);
    
    flag = 0;
    kol_word++;
    
    
    
    for (int step = 0; step <= kol_word; step++)
    {
        
        double** pr_ch = new double* [kol_pr];  //таблица для построения дерева (0 столбец) - сумма по веткам, (1) - уровень ветки, (>1) - вершины в ветке
        for (int i = 0; i < kol_pr; i++)
            pr_ch[i] = new double [kol_v];
    
        for (int i = 0; i < kol_pr; i++)
        {
            if (i < n)
            {
                pr_ch[i][0] = (double)fr[step][i];
                pr_ch[i][1] = 0;
                pr_ch[i][2]= i;
                lvl = 3;
            }
            else
                lvl = 0;
            for (int j = lvl; j < kol_v; j++)
                pr_ch[i][j] = -1;
        }
    
        flag = 0;
        min1 = 1; min2 = 1;
        ind1 = -1; ind2 = -1; cur = n;
        lvl = 1;
    
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
    
        ind1 = -1;
    
        FILE *f_tree = fopen ("/Users/alesya/Desktop/Study/code/tree.txt", "rb+");
        fseek(f_tree, 0, SEEK_END);
    
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
        ind1 = -3;
        fprintf(f_tree,"%d ",ind1);
        fclose(f_tree);
        free(pr_ch);
        free(tree);
    }
    FILE *f_tree = fopen ("/Users/alesya/Desktop/Study/code/tree.txt", "rb+");
    fseek(f_tree, 0, SEEK_END);
    lvl = -2;
    fprintf(f_tree,"%d ",lvl);
    fclose(f_tree);
    
   // free(fr);
    
    //кодирование
    code(in, out);
    
}

void srav()
{
    FILE *f_in = fopen ("/Users/alesya/Desktop/Study/code/msg.txt", "rb");
    FILE *f_out = fopen ("/Users/alesya/Desktop/Study/code/decode.txt", "rb");
    
    int msg = -1, dec = -1;
    int kol = 1;
    
    fscanf(f_in, "%d ", &msg);
    fscanf(f_out, "%d ", &dec);
    
    while(msg != -1)
    {
        if (msg != dec)
            cout << "NOOOOOOOOO!!!!!!";
        fscanf(f_in, "%d ", &msg);
        fscanf(f_out, "%d ", &dec);
        kol++;
        
    }
    cout << "\nfinish";
    
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
