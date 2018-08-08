#ifndef WAVLOADER_H
#define WAVLOADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BIG_ENDIAN_TAG 0
#define LITTLE_ENDIAN_TAG 1

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
 Endian	|Pos|  Size  |Data Stored		| Descriptions

big	 	| 0 |4 bytes |Chunk ID			| RIFF (0x52494646)
little 	| 4 |4 bytes |Chunk Size		| 4 + (8 + Chunk2Size) + (8 + Chunk3Size) (size of full file excluding the first 8 bytes)
big	 	| 8 |4 bytes |Format			| WAVE (0x57415645)

Chunk 2 ("Format chunk"):
big	 	| 12 |4 bytes |Chunk ID			| fmt (0x666D7420)
little	| 16 |4 bytes |Chunk Size		| 16 (0x10000000)
little	| 20 |2 bytes |AudioFormat		| If this is equal to 1 its PCM audio otherwise its got some compression or something
little	| 22 |2 bytes |NumChannels		| number of channels
little	| 24 |4 bytes |Sample Rate		| sample rate like 8000 or 44100
little	| 28 |4 bytes |Byte Rate		| SampleRate * NumChannels * BitsPerSample/8
little	| 32 |2 bytes |Block Align		| NumChannels * BitsPerSample/8 (num of bytes per one audio sample)
little	| 34 |2 bytes |Bits Per Sample 	| number of bits per sample

Chunk 2 ("Data Chunk"):
big	 	| 36 |4 bytes |Chunk ID			| data (0x64617461)
little	| 40 |4 bytes |Chunk Size		| wav data size
little	| 44 |X bytes |WAV Data 		| The actual sound data

how is the wav data stored:
each sample has (Block Align)Bytes, the left channel is the first half of the sample and the right channel is the second half.
*/

//struct sample??

struct wavFile{
	int Channels;
	int SampleRate;
	int BlockAlign;
	int BitsPerSample;
	unsigned char * AudioData;      
};

struct wavFile * loadWav(char * filePath);

unsigned long pullFromBuffer(unsigned char * buffer, int length, int startAddress, int dataType);

#endif /* WAVLOADER_H */