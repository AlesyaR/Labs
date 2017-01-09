//
//  ReadFromFile.hpp
//  GolombCoder
//
//  Created by MacBook on 29.11.16.
//  Copyright © 2016 kosa. All rights reserved.
//

#ifndef ReadFromFile_hpp
#define ReadFromFile_hpp

#include <stdio.h>
#include <string>
#include <vector>

class CReadFromFile
{
    std::vector<char>           m_vData;
    
public:
    CReadFromFile(std::string strFileName)
    {
        _readFromFile(strFileName);
    }
    
private:
    void                _readFromFile(std::string);
    
public:
    std::vector<int>    getBytes();
    
    
};

#endif /* ReadFromFile_hpp */
