//
//  ReadFromFile.cpp
//  GolombCoder
//
//  Created by MacBook on 29.11.16.
//  Copyright © 2016 kosa. All rights reserved.
//

#include "ReadFromFile.hpp"
#include <iostream>
#include <fstream>


void CReadFromFile::_readFromFile(std::string strFileName)
{
    std::ifstream inFile;
    inFile.open(strFileName);
    
    inFile.seekg(0, std::ios_base::end);
    std::streampos fileSize = inFile.tellg();
    m_vData.resize(fileSize);
    
    inFile.seekg(0, std::ios_base::beg);
    inFile.read(&m_vData[0], fileSize);
}

std::vector<int> CReadFromFile::getBytes()
{
    std::vector<int> vStreamData;
    
    std::vector<char>::iterator it;
    for(it = m_vData.begin(); it != m_vData.end(); ++it)
    {
        char cCurrData = *it;
        
        for(int i = 0; i != sizeof(char)*8; i++)
        {
            char tmp = cCurrData << i;
            tmp = tmp >> (sizeof(char)*8 - 1);
            tmp = tmp & 1;
            vStreamData.push_back(tmp);
        }
    }
    
    return vStreamData;
}