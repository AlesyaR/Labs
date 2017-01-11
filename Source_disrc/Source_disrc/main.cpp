//
//  main.cpp
//  Source_disrc
//
//  Created by Alesya on 11.12.16.
//  Copyright © 2016 Alesya. All rights reserved.
//

#include <iostream>
#include <time.h>
using namespace std;

int main(int argc, const char * argv[])
{
    int max = 0;
    int count_word = 100000;
    double r = 0;
    int kol = 0;
    int ind = 0;
    double sum = 0;
    const char *in = "/Users/alesya/Desktop/Study/code/msg.txt";
    const char *in_m = "/Users/alesya/Desktop/Study/code/fr.txt";
    FILE *f = fopen(in_m, "wb");
    
    
    srand(time(NULL));
    
    cout << "input max word of msg:  ";
    cin >> max;
    
    cout << "input count of word in msg:  ";
    cin >> count_word;
    
    
    double *fr = new double [max + 1];
    
    for (int i = 0; i <= max; i++)
        fr[i] = 0;
    
    for (int i = 0; i <= max; i++)
    {
        sum = 0;
        for (int j =0; j <= max; j++)
            sum += (double) fr[j];
        
        r = (double)rand()/RAND_MAX;
        ind = rand()%max + 1;
        while (fr[ind] != 0)
            ind = rand()%max + 1;
        if (i == 4)
            ind = 0;
        
        while ((sum + r) > 1)
            r = (double)rand()/RAND_MAX;
        if (i == max)
            r = (double)(1 - sum);
        fr[ind] = (double) r;
    }
   
    for (int i = 0; i <= max; i++)
    {
        kol = fr[i] * 1000000;
        fprintf(f,"%d ",kol);
    }
    
    kol = -1;
    fprintf(f,"%d ",kol);
    fclose(f);
    f = fopen(in, "wb");
    
    for (int i = 0; i < count_word; i++)
    {
        sum = (double)fr[0];
        r = (double)rand()/RAND_MAX;
        kol = 0;
        for (int j = 0; (j <= max) && (kol == 0); j++)
            if (r < sum)
            {
                fprintf(f,"%d ",j);
                kol = 1;
            }
            else
                sum += (double) fr[j+1];
    }
    kol = -1;
    
    fprintf(f,"%d ",kol);
    
    return 0;
}
