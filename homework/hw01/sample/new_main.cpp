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

struct AudioData
{
    uint32_t m_ID;
    char     m_FirstLang;
    char     m_SecondLang;
    uint32_t m_Payload;
};

struct AudioList
{
    uint32_t m_ID;
    uint32_t m_AudioCount;
    uint32_t m_Payload;
    uint32_t m_InPayload;
    vector <AudioData> m_AudioData;
};

struct VideoData
{
    uint32_t m_ID;
    uint32_t m_Payload;
};

struct Frame
{
    uint32_t m_ID;
    uint32_t m_Payload;
    uint32_t m_InPayload;
    VideoData m_Video;
    AudioList m_Audio;
};

struct Header
{
    uint32_t m_ID;
    uint32_t m_FrameCount;
    uint32_t m_Payload;
    uint32_t m_InPayload;
    vector <Frame> m_Frames;
};

void ChangeEndianness  ( uint32_t & Origin , const uint32_t EID )
{
    if (EID==BIG_ENDIAN_ID)
    {
    unsigned char * OriginStringed = (unsigned char *) &Origin;
    Origin =    (uint32_t)OriginStringed[3] | (uint32_t)OriginStringed[2]<<8 |
                (uint32_t)OriginStringed[1]<<16 | (uint32_t)OriginStringed[0]<<24;
    }
    return;
}



bool  filterFile          ( const char      * srcFileName,
                            const char      * dstFileName,
                            const char      * lang )
{
    ifstream input;
    ofstream output;
    Header header;
    header.m_Payload=0;

    uint32_t zero = 0;


    input.open(srcFileName,ios::binary | ios::in );
    if (!input)
        return false;
    
    output.open(dstFileName,ios::binary | ios::out );
    if (!output)
    {
        input.close();
        return false;
    }

    if ( !input.read((char*)&header.m_ID,4) ||
    !input.read((char*)&header.m_FrameCount,4) ||
    !input.read((char*)&header.m_InPayload,4) )
        return false;

    if (!((header.m_ID==BIG_ENDIAN_ID)||(header.m_ID==LITTLE_ENDIAN_ID)))
        return false;

    
    ChangeEndianness(header.m_FrameCount,header.m_ID);
    ChangeEndianness(header.m_InPayload,header.m_ID);


    for ( uint32_t i=0 ; i<header.m_FrameCount ; i++ )
    {
        Frame frame;
        frame.m_Payload=0;

        if (!input.read((char*)&frame.m_ID,4) ||
            !input.read((char*)&frame.m_InPayload,4))
            return false;
        
        ChangeEndianness(frame.m_InPayload,header.m_ID);

        header.m_Payload+=8;
        int FrameStart = input.tellg();

        if (frame.m_ID!=FRAME_ID)
            return false;


        header.m_Frames.push_back(frame);
        
        //VIDEO DATA READING STARTS HERE

        VideoData video_data;
        
        if (! input.read((char*)&video_data.m_ID,4) ||
            ! input.read((char*)&video_data.m_Payload,4) )
            return false;
        
        ChangeEndianness(video_data.m_Payload,header.m_ID);
        
        header.m_Payload+=8;
        header.m_Frames[i].m_Payload+=8;

        if (video_data.m_ID!=VIDEO_DATA_ID)
            return false;
        
        input.seekg(video_data.m_Payload+4,ios::cur); //+4 is for CRC
        header.m_Payload+=video_data.m_Payload+4;
        header.m_Frames[i].m_Payload+=video_data.m_Payload+4;

        header.m_Frames[i].m_Video=video_data;

        //AUDIO LIST READING STARTS HERE

        AudioList audio_list;
        audio_list.m_Payload=0;

        if (!input.read((char*)&audio_list.m_ID,4) ||
            !input.read((char*)&audio_list.m_AudioCount,4) ||
            !input.read((char*)&audio_list.m_InPayload,4) )
            return false;
        
        ChangeEndianness(audio_list.m_AudioCount,header.m_ID);
        ChangeEndianness(audio_list.m_InPayload,header.m_ID);
        
        int AudioStart = input.tellg();
        header.m_Payload+=12;
        header.m_Frames[i].m_Payload+=12;

        if (audio_list.m_ID!=AUDIO_LIST_ID)
            return false;

        //AUDIO DATA READING STARTS HERE

        for ( uint32_t j=0 ; j<audio_list.m_AudioCount ; j++ )
        {
            AudioData audio_data;
            if (!input.read((char*)&audio_data.m_ID,4) ||
                !input.read((char*)&audio_data.m_FirstLang,1) ||
                !input.read((char*)&audio_data.m_SecondLang,1) ||
                !input.read((char*)&audio_data.m_Payload,4) )
                return false;

        ChangeEndianness(audio_data.m_Payload,header.m_ID);

            if (audio_data.m_ID!=AUDIO_DATA_ID)
                return false;

            if ( (audio_data.m_FirstLang == lang[0]) && (audio_data.m_SecondLang == lang[1] ) )
            {
                audio_list.m_Payload+=audio_data.m_Payload+4+10;
                header.m_Payload+=audio_data.m_Payload+4+10;
                header.m_Frames[i].m_Payload+=audio_data.m_Payload+4+10;
                audio_list.m_AudioData.push_back(audio_data);
                header.m_Frames[i].m_Audio.m_Payload=audio_data.m_Payload+4+10;
                input.seekg((audio_data.m_Payload+4),ios::cur);
                continue;
            }
            else
            {
                input.seekg((audio_data.m_Payload+4),ios::cur);
                continue;
            }
        }

        header.m_Frames[i].m_Audio=audio_list;

        if (input.tellg()!= (AudioStart+audio_list.m_InPayload))
            return false;

        input.seekg(4,ios::cur); // AUDIO LIST CRC
        header.m_Payload+=4;
        header.m_Frames[i].m_Payload+=4;
        //CHECKING AND ADDING TO ACTUAL PAYLOAD HERE

        

        if (input.tellg()!= (FrameStart+frame.m_InPayload))
            return false;

        input.seekg(4,ios::cur); //frame CRC
        header.m_Payload+=4;
    }

    if (input.tellg()!= (header.m_InPayload+12))
        return false;
    
    int tmp;
    if(!input.read((char*)&tmp,4)) //frame CRC
        return false;
    
    if ( input.peek() != EOF )
        return false;



    //-------------------------------------------------------------------------------------

    ChangeEndianness(header.m_FrameCount,header.m_ID);
    ChangeEndianness(header.m_Payload,header.m_ID);

    input.seekg(0,ios::beg);
    if (!output.write((char*)&header.m_ID,4) ||
        !output.write((char*)&header.m_FrameCount,4) ||
        !output.write((char*)&header.m_Payload,4) )
        return false;

    input.seekg(12,ios::cur);
    ChangeEndianness(header.m_FrameCount,header.m_ID);

    for ( uint32_t i=0 ; i<header.m_FrameCount ; i++)
    {
        ChangeEndianness(header.m_Frames[i].m_Payload,header.m_ID);
        ChangeEndianness(header.m_Frames[i].m_Video.m_Payload,header.m_ID);

        if( !output.write((char*)&header.m_Frames[i].m_ID,4) ||
            !output.write((char*)&header.m_Frames[i].m_Payload,4) ||

            !output.write((char*)&header.m_Frames[i].m_Video.m_ID,4) ||
            !output.write((char*)&header.m_Frames[i].m_Video.m_Payload,4))
            return false;
        input.seekg(16,ios::cur);
        //int var = input.tellg();
        ChangeEndianness(header.m_Frames[i].m_Video.m_Payload,header.m_ID);

        for ( uint32_t j = 0 ; j<header.m_Frames[i].m_Video.m_Payload ; j++)
        {
            int32_t buffer;
            if( !input.read((char*)&buffer,1) ||
                !output.write((char*)&buffer,1) )
                return false;
        }

        if (!output.write((char*)&zero,4)) //video CRC
            return false;
        input.seekg(4,ios::cur);

        uint32_t audio_files;

        if ( header.m_Frames[i].m_Audio.m_AudioData.empty() )
            audio_files = 0;
        else
            audio_files = 1;

        ChangeEndianness(audio_files,header.m_ID);
        ChangeEndianness(header.m_Frames[i].m_Audio.m_Payload,header.m_ID);


        if(!output.write((char*)&header.m_Frames[i].m_Audio.m_ID,4)||
            !output.write((char*)&audio_files,4)||
            !output.write((char*)&header.m_Frames[i].m_Audio.m_Payload,4))
            return false;
        input.seekg(12,ios::cur);

        for( uint32_t j = 0 ; j<header.m_Frames[i].m_Audio.m_AudioCount ; j++ )
        {
            AudioData audio_data;
            if(!input.read((char*)&audio_data.m_ID,4) ||
                !input.read((char*)&audio_data.m_FirstLang,1) ||
                !input.read((char*)&audio_data.m_SecondLang,1) ||
                !input.read((char*)&audio_data.m_Payload,4))
                return false;

            if ( (audio_data.m_FirstLang == lang[0]) && (audio_data.m_SecondLang == lang[1] ) )
            {
                //ChangeEndianness(audio_data.m_Payload,header.m_ID);
                
                if(!output.write((char*)&audio_data.m_ID,4)||
                !output.write((char*)&audio_data.m_FirstLang,1)||
                !output.write((char*)&audio_data.m_SecondLang,1)||
                !output.write((char*)&audio_data.m_Payload,4))
                    return false;
                for ( uint32_t k = 0 ; k<header.m_Frames[i].m_Audio.m_AudioData[0].m_Payload ; k++)
                {
                    int32_t buffer;
                    if(!input.read((char*)&buffer,1)||
                    !output.write((char*)&buffer,1))
                    return false;
                }
                if(!output.write((char*)&zero,4)) // AUDIO DATA CRC
                    return false; 
                input.seekg(4,ios::cur);
                //var = input.tellg();
                continue;
            }
            else
            {
                ChangeEndianness(audio_data.m_Payload,header.m_ID);
                input.seekg((audio_data.m_Payload+4),ios::cur);
                //var = input.tellg();
                continue;
            }
        }
        //var = input.tellg();
        input.seekg(8,ios::cur);
        //var = input.tellg();
        if(!output.write((char*)&zero,4)|| // AUDIO LIST CRC
            !output.write((char*)&zero,4)) // FRAME LIST CRC
            return false;
    }
    if(!output.write((char*)&zero,4)) // HEADER LIST CRC
        return false;


    input.close();
    output.close();
    return true;
}

#ifndef __PROGTEST__
int main(void)
{
    assert (!filterFile("in_3996392.bin","out_mine.out","cs"));

    assert (filterFile("in_0000.in","out_0000_cs_mine.out","cs"));
        
    assert (filterFile("in_0001.in","out_0001_cs_mine.out","cs"));
    
    assert (filterFile("in_0002.in","out_0002_en_mine.out","en"));

    assert (filterFile("in_0003.in","out_0003_en_mine.out","en"));

    assert (filterFile("in_0004.in","out_0004_pl_mine.out","pl"));

    assert (filterFile("in_0005.in","out_0005_cs_mine.out","cs"));

    assert (!filterFile("in_0006.in","out_0006_cs_mine.out","cs"));

    assert (filterFile("big-endian/in_0000.in","big-endian/out_0000_cs_mine.out","cs"));

    assert (filterFile("big-endian/in_0001.in","big-endian/out_0001_cs_mine.out","cs"));

    assert (filterFile("big-endian/in_0002.in","big-endian/out_0002_en_mine.out","en"));

    assert (filterFile("big-endian/in_0003.in","big-endian/out_0003_en_mine.out","en"));

    assert (filterFile("big-endian/in_0004.in","big-endian/out_0004_pl_mine.out","pl"));

    assert (filterFile("big-endian/in_0005.in","big-endian/out_0005_cs_mine.out","cs"));

    assert (!filterFile("big-endian/in_0006.in","big-endian/out_0006_cs_mine.out","cs"));

}
#endif