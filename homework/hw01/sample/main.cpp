#ifndef __PROGTEST__
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

const uint32_t BIG_ENDIAN_ID    = 0x4d6d6d4d;
const uint32_t LITTLE_ENDIAN_ID = 0x49696949;
const uint32_t FRAME_ID         = 0x46727246;
const uint32_t VIDEO_DATA_ID    = 0x56696956;
const uint32_t AUDIO_LIST_ID    = 0x416c6c41;
const uint32_t AUDIO_DATA_ID    = 0x41757541;
#endif /* __PROGTEST__ */

class Header
{
    public:
    uint32_t m_ID;
    uint32_t m_FrameCount;
    uint32_t m_Payload;
};

bool  filterFile          ( const char      * srcFileName,
                            const char      * dstFileName,
                            const char      * lang )
{
    ifstream input;
    ofstream output;
    Header header;
    input.open(srcFileName,ios::binary);
    if (!input)
        return false;
    
    output.open(dstFileName,ios::binary);
    if (!output)
    {
        input.close();
        return false;
    }
    
    input.read((char*)&header.m_ID,4);
    output.write((char*)&header.m_ID,4);
    if (!((header.m_ID==BIG_ENDIAN_ID)||(header.m_ID==LITTLE_ENDIAN_ID)))
    {
        input.close();
        output.close();
        return false;
    }

    input.seekg(4,input.beg);
    input.read((char*)&header.m_FrameCount,4);
    output.write((char*)&header.m_FrameCount,4);
    
    input.seekg(4,input.cur);
    input.read((char*)&header.m_Payload,4);
    output.write((char*)&header.m_Payload,4);
    

    for(int i=0 ; i<header.m_FrameCount ; i++ )
    {
        int32_t FrameID;
        input.seekg(4,input.cur);
        input.read((char*)FrameID,4);
        if (FrameID!=FRAME_ID)
        {
            input.close();
            output.close();
            return false;
        }
            
    }

    input.close();
    output.close();
    return true;
}

int main()
{
    filterFile("in_0000.in","out_0000_cs.out","cs");
    return 0;
}