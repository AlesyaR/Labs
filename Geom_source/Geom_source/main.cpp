//
//  main.cpp
//  Geom_source
//
//  Created by Alesya on 31.10.16.
//  Copyright © 2016 Alesya. All rights reserved.
//

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <math.h>
using namespace std;

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

void message(double p, int kol, const char *msg)
{
    FILE *f = fopen (msg, "wb");
    int n = 0;
    
    for (int i = 0; i < kol; i++)
    {
        n = get_n(p);
        fprintf(f,"%d ", n);
    }
    n = -1;
    fprintf(f,"%d ", n);
    fclose(f);
}

void test_geom_rasp(double p)
{
    FILE *out1 = fopen("pr.txt", "wb");
    FILE *out2 = fopen("th.txt", "wb");
    
    double kol[60];
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
        fprintf(out1,"%f\n",kol[i]);
        
        kol[i] = (double)(pow((1 - p),i)*p);
        fprintf(out2,"%f\n",kol[i]);
    }
    
}

int main(int argc, const char * argv[]) {
    
    srand(time(NULL));
    
    double p = 2;
    while (p > 1)
    {
        cout << "Input p : ";
        cin >> p;
    }
    
    int kol = 0;
    while (kol < 1)
    {
        cout << "Input length of message : ";
        cin >> kol;
    }
    
    const char *in = "/Users/alesya/Desktop/Study/code/msg.txt";
    
    test_geom_rasp(p);
    message(p, kol, in);

    return 0;
}
