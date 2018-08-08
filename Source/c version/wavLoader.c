#include "wavLoader.h"

unsigned long pullFromBuffer(unsigned char * buffer, int length, int startAddress, int dataType){
	unsigned long byteSet;
	if (dataType){ //little endian
		byteSet = buffer[startAddress+(length-1)];
		for (int i = length-2; i >= 0; i--){ // -2 offset as need -1 cos array start at 0 and -1 as we already done one above
			//std::cout << std::hex << byteSet << " " << buffer[startAddress+i] << std::endl;
			byteSet = byteSet << 8 | buffer[startAddress+i];
		}
	}else{
		byteSet = buffer[startAddress];
		for (int i = 1; i < length; i++){
			byteSet = byteSet << 8 | buffer[startAddress+i];
		}
	}
	return byteSet;
}

struct wavFile * loadWav(char * filePath){
	//open file and get size
	FILE *fptr;
	unsigned long fSize;
	fptr = fopen(filePath,"rb");
	if (fptr == NULL){
		fprintf(stderr, "%s", "File could not be opened!\n");
		return NULL;
	}
	fseek ( fptr , 0 , SEEK_END );
	fSize = ftell(fptr);
	rewind(fptr);

	//create file buffer
	unsigned char * buffer = (unsigned char*) malloc (sizeof(char)*fSize);
	if (buffer == NULL){
		fprintf(stderr, "%s", "Memory allocation error will attempt to read from disk!\n");
		//TODO: disk streaming
	}

	//populate buffer in 1 byte increments
	if (fread(buffer,1,fSize,fptr) != fSize){
		fprintf(stderr, "%s", "File could not be read!\n");
		return NULL;
	}else{
		fclose(fptr);
	}

	if (!( (pullFromBuffer(buffer, 4, 8, BIG_ENDIAN_TAG) == 0x57415645) && (pullFromBuffer(buffer, 2, 20, LITTLE_ENDIAN_TAG) == 1) )){ //byte 8 is format, byte 20 is audioFormat
		fprintf(stderr, "%s", "File is not standard wav!\n");
		return NULL;
	}

	struct wavFile *wf;
	//file info
	wf->Channels = pullFromBuffer(buffer, 2, 22, LITTLE_ENDIAN_TAG);//2 bytes at 20
	wf->SampleRate = pullFromBuffer(buffer, 2, 24, LITTLE_ENDIAN_TAG);//4 bytes at 24
	wf->BlockAlign = pullFromBuffer(buffer, 2, 32, LITTLE_ENDIAN_TAG);//2 bytes at 32
	wf->BitsPerSample = pullFromBuffer(buffer, 2, 34, LITTLE_ENDIAN_TAG);//2 bytes at 34

	//file data
	unsigned long audioDataSize = pullFromBuffer(buffer, 4, 40, LITTLE_ENDIAN_TAG);
	wf->AudioData = (unsigned char*) malloc (sizeof(char)*audioDataSize); //4 bytes at 40

	//move the data array
	memmove(wf->AudioData, buffer+44, audioDataSize);
	free(buffer);
	
	printf ("Channels: %d\n", wf->Channels);
	printf ("Sample Rate: %d\n", wf->SampleRate);
	printf ("Block Align: %d\n", wf->BlockAlign);
	printf ("Bits Per Sample: %d\n", wf->BitsPerSample);
	printf ("Audio Data Size: %lu\n\n", audioDataSize);

	printf ("Audio Data: %x %x %x %x\n", wf->AudioData[0], wf->AudioData[1], wf->AudioData[2], wf->AudioData[3]);


	return NULL;
}