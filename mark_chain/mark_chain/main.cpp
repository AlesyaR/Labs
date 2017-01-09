//
//  main.cpp
//  mark_chain
//
//  Created by Alesya on 31.10.16.
//  Copyright © 2016 Alesya. All rights reserved.
//

#include <iostream>
#include <time.h>

using namespace std;

void message(const char *s, int kol_msg, const char *matrix)
{
    int n = 6;
    int pos = -1;
    double sum = 0;
    FILE *f = fopen(s, "wb");
    FILE *f_m = fopen(matrix, "wb");
    
    cout <<"Input n {2..6} = ";
    cin >> n;
    
    double **P = new double* [n];
    for (int i = 0; i < n; i++)
        P[i] = new double [n];
    
    double **P_sum = new double* [n];
    for (int i = 0; i < n; i++)
        P_sum[i] = new double [n];

    
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            P[i][j] = 0;

    
    cout << "Do you want to input P{}? [1 - yes, 'another' - no]:  ";
    cin>>pos;
    
    if (pos == 1)
    {
        for (int i = 0; i < n; i++)
        {
            sum = 0;
            for (int j = 0; j < n; j++)
            {
                cout << "\nP["<<i<<"]["<<j<<"] = ";
                cin >> P[i][j];
                sum += P[i][j];
            }
            if (sum > 1)
            {
                cout << "Sum > 1! Retype:\n";
                i--;
            }
            if (sum < 1)
            {
                cout << "Sum < 1! Retype:\n";
                i--;
            }
            
        }
        sum = 0;
    }
    else
    {
        for (int i = 0; i < n; i++)         // генерация матрицы переходов
        {
            for (int j = 0; j < n-1; j++)
            {
                if (j == -1)
                    j = 0;
                
                sum = 0;
                for (int k = 0; k < j; k++)
                    sum += (double)P[i][k];
                P[i][j] = (double)rand()/RAND_MAX;
                
                while((sum + P[i][j]) > 1)
                    P[i][j] = (double)rand()/RAND_MAX;
            }
            P[i][n-1] = (double)(1 - sum - P[i][n-2]);
        }
    }
    
    pos = 0;
    double min = 100;
    int fl = 0;
    int ch = 0;
    double r = 0;
    
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (j == 0)
                P_sum[i][j] = P[i][j];
            else
                P_sum[i][j] = P_sum[i][j - 1] + P[i][j];
    
    for (int m = 0; m < kol_msg; m++)   //Генерация состояни марковской цепи и запись состояния в файл.
    {
        r = (double)rand()/RAND_MAX;
        ch = 0;
        
        /*  while (ch == 0)
        {
            sum = 0;
            min = 100;
            for (int i = 0; i < n; i++)
                if ((P[pos][i] < min) && (P[pos][i] != 0))
                {
                    min = (double)P[pos][i];
                    fl = i;
                }
            sum += (double)min;
            if (r < sum)
            {
                pos = fl;
                ch = 1;
                for (int i = 0; i < n; i++)
                    p_start[i][1] = 0;
                
            }
            else
                p_start[fl][1] = 1;
            
        }*/
        
        for(int i = 0; i < n; i++)
            if ((i == 0) && (P_sum[pos][i] > r))
            {
                fprintf(f,"%d ", i);
                pos = i;
            }
            else
                if ((P_sum[pos][i] <= r) && (P_sum[pos][i + 1] > r))
                {
                    pos = i + 1;
                    fprintf(f,"%d ", pos);
                }
        
        
    }
    pos = -1;
    min = -1;
    fprintf(f,"%d ", pos);
    
    fprintf(f_m,"%d ", n);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            pos = P[i][j] * 1000000000;
            fprintf(f_m,"%d ", pos);
        }
        //fprintf(f_m,"%f ", min);
    }
    
    fclose(f);
    fclose(f_m);
}


int main(int argc, const char * argv[])
{
    
    srand(time(NULL));
    const char *in = "/Users/alesya/Desktop/Study/code/msg.txt";
    const char *in_m = "/Users/alesya/Desktop/Study/code/matrix.txt";
    int kol = 10000;
    message(in, kol, in_m);
    
    return 0;
}
