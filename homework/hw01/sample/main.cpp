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

struct Header
{
    uint32_t m_ID;
    uint32_t m_FrameCount;
    uint32_t m_Payload;
};

struct Frame
{
    uint32_t m_ID;
    uint32_t m_Payload;
};

struct VideoData
{
    uint32_t m_ID;
    uint32_t m_Payload;
};

struct AudioList
{
    uint32_t m_ID;
    uint32_t m_AudioCount;
    uint32_t m_Payload;
};

struct AudioData
{
    uint32_t m_ID;
    string   m_Language;
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

    input.read((char*)&header.m_FrameCount,4);
    output.write((char*)&header.m_FrameCount,4);
    
    input.read((char*)&header.m_Payload,4);
    output.write((char*)&header.m_Payload,4);
    

    for(uint32_t i=0 ; i<header.m_FrameCount ; i++ )
    {
        Frame frame;
        VideoData video_data;
        AudioList audio_list;

        input.read((char*)&frame.m_ID,4);
        output.write((char*)&frame.m_ID,4);
        if (frame.m_ID!=FRAME_ID)
        {
            input.close();
            output.close();
            return false;
        }

        input.read((char*)&frame.m_Payload,4);
        output.write((char*)&frame.m_Payload,4);

        input.read((char*)&video_data.m_ID,4);
        output.write((char*)&video_data.m_ID,4);
        if (video_data.m_ID!=VIDEO_DATA_ID)
        {
            input.close();
            output.close();
            return false;
        }

        input.read((char*)&video_data.m_Payload,4);
        output.write((char*)&video_data.m_Payload,4);

        for ( uint32_t j=0 ; j<video_data.m_Payload ; j++)
        {
            int32_t buffer;
            input.read((char*)&buffer,1);
            output.write((char*)&buffer,1);
        }

        input.read((char*)&audio_list.m_ID,4);
        output.write((char*)&audio_list.m_ID,4);
        if (audio_list.m_ID!=AUDIO_LIST_ID)
        {
            input.close();
            output.close();
            return false;
        }

        input.read((char*)&audio_list.m_AudioCount,4);
        output.write((char*)&audio_list.m_AudioCount,4);

        input.read((char*)&audio_list.m_Payload,4);
        output.write((char*)&audio_list.m_Payload,4);

        for ( uint32_t j=0 ; j<audio_list.m_AudioCount ; j++ )
        {
            AudioData audio_data;
            input.read((char*)&audio_data.m_ID,4);
            input.read((char*)&audio_data.m_Language,2);
            input.read((char*)&audio_data.m_Payload,4);
            cout << audio_data.m_Language;
        }
    }

    input.close();
    output.close();
    return true;
}

int main()
{
    if (filterFile("in_0000.in","out_0000_cs_mine.out","cs"))
        cout<<"OK!"<<endl;
    else 
        cout<<"ERROR!"<<endl;
    return 0;
}