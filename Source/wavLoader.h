#ifndef WAVLOADER_H
#define WAVLOADER_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#define BIG_ENDIAN_TAG 0
#define LITTLE_ENDIAN_TAG 1

/*
http://www-mmsp.ece.mcgill.ca/Documents/AudioFormats/WAVE/WAVE.html
http://soundfile.sapp.org/doc/WaveFormat/

wav format
wav is actually a wrapper for another format which is the riff format
riff files store everything in juicy lil chunks

every riff chunk has this layout:
4 byte ChunkID
4 byte ChunkSize (this excludes the size of chunk id and chunk size)
the rest of the chunks info

How are Wave files stored:
Chunk 1 ("Riff Chunk Descriptor"):
 Endian	|Pos|  Size  |Data Stored		| Descriptions/Example

big	 	| 0 |4 bytes |Chunk ID			| RIFF (0x52494646)
little 	| 4 |4 bytes |Chunk Size		| 4 + (8 + Chunk2Size) + (8 + Chunk3Size) (size of full file excluding the first 8 bytes)
big	 	| 8 |4 bytes |Format			| WAVE (0x57415645)

Chunk 2 ("Format chunk"):
big	 	| 12 |4 bytes |Chunk ID			| fmt (0x666D7420)
little	| 16 |4 bytes |Chunk Size		| 16 (0x10000000), 18 or 40
little	| 20 |2 bytes |AudioFormat		| 1(0x0001) == WAVE_FORMAT_PCM, 3(0x0003) == WAVE_FORMAT_IEEE_FLOAT, 6(0x0006) == WAVE_FORMAT_ALAW, 7(0x0007) == WAVE_FORMAT_MULAW, 1(0xFFFE) == WAVE_FORMAT_EXTENSIBLE
little	| 22 |2 bytes |NumChannels		| number of channels
little	| 24 |4 bytes |Sample Rate		| sample rate like 8000 or 44100
little	| 28 |4 bytes |Byte Rate		| SampleRate * NumChannels * BitsPerSample/8
little	| 32 |2 bytes |Block Align		| NumChannels * BitsPerSample/8 (num of bytes per one audio sample)
little	| 34 |2 bytes |Bits Per Sample 	| number of bits per sample
TODO: cbsize and shit might write into a seperate branch after finishing to have a light weight version for arduino platforms etc

Chunk 2 ("Data Chunk"):
big	 	| 36 |4 bytes |Chunk ID			| data (0x64617461)
little	| 40 |4 bytes |Chunk Size		| wav data size
little	| 44 |X bytes |WAV Data 		| The actual sound data

how is the wav data stored:
each sample has (Block Align)Bytes, the left channel is the first half of the sample and the right channel is the second half.

PCM format
1 to 8-bit samples are "unsigned", offset binary lol
9 and higher bit samples are 2s complement integers, from -32768 to 32767.
*/

//struct sample??

struct wavFile{
    public:
    	int Channels;
        int SampleRate;
        int BlockAlign;
        int BitsPerSample;
        int NumberOfSamples;
        unsigned char * Samples;
    private:
        
};

struct sample{
	public:
		long * Amplitude; //malloc array as channels
};

wavFile * loadWav(char * filePath);
sample * createSample(wavFile * wFile, int n);

long pullFromBuffer(unsigned char * buffer, int length, int startAddress, int dataType);
long pullLittleSigned(unsigned char * buffer, int length, int startAddress);

#endif /* WAVLOADER_H */