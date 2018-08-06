#include "wavLoader.h"

wavFile * loadWav(char * filePath){
	//open file and get size
	FILE *fptr;
	long fSize;
	fptr = fopen(filePath,"rb");
	if (fptr == NULL){
		fprintf(stderr, "%s", "File could not be opened!\n");
		return NULL;
	}
	fseek ( fptr , 0 , SEEK_END );
	fSize = ftell(fptr);
	rewind(fptr);

	//create file buffer
	char * buffer = (char*) malloc (sizeof(char)*fSize);
	if (buffer == NULL){
		fprintf(stderr, "%s", "Memory allocation error will attempt to read from disk!\n");
	}

	//populate buffer in 1 byte increments
	if (fread(buffer,1,fSize,fptr) !=fSize){
		fprintf(stderr, "%s", "File could not be read!\n");
		return NULL;
	}


	unsigned int fmt;
	memcpy(&fmt, buffer, sizeof(unsigned int)); //this boy gonna be backwards cos lil endian
	std::cout << std::hex << fmt << std::endl;
	std::cout << std::hex << (int)buffer[8] << std::endl;

	/*if (!( (fmt == WAVE) && (audioFMT == 1) )){ //byte 8 is format, byte 18 is audioFormat
		fprintf(stderr, "%s", "File is not standard wav!\n");
		return null;
	}

	//file info

	wf.Channels = ;//2 bytes at 20
	wf.SampleRate = ;//4 bytes at 24
	wf.BitsPerSample = ;//2 bytes at 32

	//file data
	wf.AudioData = ;//memcpy from address 44 for address 40 bytes
	*/
	fclose(fptr);
	return NULL;
}