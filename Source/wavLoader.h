#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

/*
wav format
wav is actually a wrapper for another format which is the riff format
riff files store everything in juicy lil chunks

every riff chunk has this layout:
4 byte ChunkID
4 byte ChunkSize (this excludes the size of chunk id and chunk size)
the rest of the chunks info

How are Wave files stored:
Chunk 1 ("Riff Chunk Descriptor"):
0 |4 bytes |Chunk ID		| RIFF (0x52494646)
4 |4 bytes |Chunk Size		| 4 + (8 + Chunk2Size) + (8 + Chunk3Size) (size of full file excluding the first 8 bytes)
8 |4 bytes |Format			| WAVE (0x57415645)

Chunk 2 ("Format chunk"):
12|4 bytes |Chunk ID		| fmt (0x666D7420)
16|4 bytes |Chunk Size		| 16 (0x10000000)
18|2 bytes |AudioFormat		| If this is equal to 1 its PCM audio otherwise its got some compression or something
20|2 bytes |NumChannels		| number of channels
24|4 bytes |Sample Rate		| sample rate like 8000 or 44100
28|4 bytes |Byte Rate		| SampleRate * NumChannels * BitsPerSample/8
32|2 bytes |Block Align		| NumChannels * BitsPerSample/8 (num of bytes per one audio sample)
34|2 bytes |Bits Per Sample | number of bits per sample

Chunk 2 ("Data Chunk"):
36|4 bytes |Chunk ID		| data (0x64617461)
40|4 bytes |Chunk Size		| wav data size
44|X bytes |WAV Data 		| The actual sound data

how is the wav data stored:
each sample has (Block Align)Bytes, the left channel is the first half of the sample and the right channel is the second half.
*/

class wavFile{
    public:
    	int Channels;
        int SampleRate;
        int BitsPerSample;
        int * AudioData;
    private:
        
};

wavFile * loadWav(char * filePath);
