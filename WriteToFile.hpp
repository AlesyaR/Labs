//
//  WriteToFile.hpp
//  GolombCoder
//
//  Created by MacBook on 29.11.16.
//  Copyright © 2016 kosa. All rights reserved.
//

#ifndef WriteToFile_hpp
#define WriteToFile_hpp

#include <stdio.h>
#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>

class CWriteToFile
{
    std::vector<int>                m_lStream;
    
    
public:
                                    CWriteToFile(std::string strFile, int* vData, int size_m)
    {
        m_lStream = std::vector <int> (vData, vData + size_m);
        writeToFile(strFile, toBitstream());
    }
    
public:
    void                            writeToFile(std::string, std::vector<char> );
    std::vector<char>               toBitstream();
    char                            getBitCode(int iFirstBit);
    
};

#endif /* WriteToFile_hpp */
