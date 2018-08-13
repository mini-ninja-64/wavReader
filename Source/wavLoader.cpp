#include "wavLoader.h"

long pullFromBuffer(unsigned char * buffer, int length, int startAddress, int dataType){
	long byteSet;
	if (dataType){ //little endian
		byteSet = buffer[startAddress+(length-1)];
		for (int i = length-2; i >= 0; i--){ // -2 offset as need -1 cos array start at 0 and -1 as we already done one above
			//std::cout << std::hex << byteSet << " " << (int) buffer[startAddress+i] << std::endl;
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

long pullLittleSigned(unsigned char * buffer, int length, int startAddress){
		/*  how to twos compliment lol
		im not rusty pshhh
		if the MSB is 1 then ya know this boy gon be negative
		
		conversion to signed:
		invert with invert operator ~
		then add 1
		if MSB is 1 then u have a negative
		else positive
	*/

	//TODO: clean and optimise
	long byteSet;
	if ((buffer[startAddress+(length-1)]>>7) == 1){
		byteSet = buffer[startAddress+(length-1)]^0xff;//invert but only want length of 1 byte so xor with 1 byte
		for (int i = length-2; i >= 0; i--){ // -2 offset as need -1 cos array start at 0 and -1 as we already done one above
			byteSet = byteSet << 8 | (buffer[startAddress+i]^0xff);
		}
		byteSet++;//inverted so now just+1
		byteSet*=-1;
	}else{
		byteSet = pullFromBuffer(buffer,length,startAddress,LITTLE_ENDIAN_TAG);
	}
	return byteSet;
}

sample * createSample(wavFile * wFile, int n){
	//TODO: optimise and neaten up this whole thing
	sample *samp;
	//printf ("Channels: %d\n", wFile->Channels);
	samp->Amplitude = (long*) malloc (sizeof(long)*wFile->Channels);//(long*) malloc (sizeof(long)*wFile->Channels);

	for (int c = 0; c < wFile->Channels; c++){
			samp->Amplitude[c] = pullFromBuffer(wFile->Samples, wFile->BitsPerSample/8, (n * wFile->Channels * wFile->BitsPerSample/8) + (c * wFile->BitsPerSample/8), LITTLE_ENDIAN_TAG);
			// samp->Amplitude[c] = wFile->Samples[(n * wFile->Channels * wFile->BitsPerSample/8)+(wFile->BitsPerSample/8)-1];
			// for (int o = (wFile->BitsPerSample/8)-2; o >= 0 ; o--){
			// 	samp->Amplitude[c] = (samp->Amplitude[c] << 8) | (wFile->Samples[ (n * wFile->Channels * wFile->BitsPerSample/8)-o]); // little endian amplitude gotten from the audio buffer
			// }
		}
	if(wFile->BitsPerSample <= 8){
		for (int c = 0; c < wFile->Channels; c++){
			samp->Amplitude[c] -= 1<<(wFile->BitsPerSample-1); //2^bps-1 = (2^bps)/2
		}	
	}else{
		for (int c = 0; c < wFile->Channels; c++){
			// long twosC = (~samp->Amplitude[c])+1;
			// if((samp->Amplitude[c]>>wFile->BitsPerSample)==1){
			// 	//twosC *= -1;
			// }
			// samp->Amplitude[c] = twosC;
			samp->Amplitude[c] = pullLittleSigned(wFile->Samples, wFile->BitsPerSample/8, (n * wFile->Channels * wFile->BitsPerSample/8) + (c * wFile->BitsPerSample/8));
		}
	}
	printf ("Amplitude: %ld\n", samp->Amplitude[0]);
	return samp;
}

wavFile * loadWav(char * filePath){
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

	wavFile *wf;
	//file info
	wf->Channels = pullFromBuffer(buffer, 2, 22, LITTLE_ENDIAN_TAG);//2 bytes at 20
	wf->SampleRate = pullFromBuffer(buffer, 2, 24, LITTLE_ENDIAN_TAG);//4 bytes at 24
	wf->BlockAlign = pullFromBuffer(buffer, 2, 32, LITTLE_ENDIAN_TAG);//2 bytes at 32
	wf->BitsPerSample = pullFromBuffer(buffer, 2, 34, LITTLE_ENDIAN_TAG);//2 bytes at 34
	unsigned long audioDataSize = pullFromBuffer(buffer, 4, 40, LITTLE_ENDIAN_TAG);
	wf->NumberOfSamples = (audioDataSize/wf->Channels) * (8/wf->BitsPerSample);//audio data size = channels * number of samples * bitsPerSample/8, number of samples = (audio data size * 8)/(channels*bitsper sample)

	//create sample array
	wf->Samples = (unsigned char*) malloc (sizeof(char)*audioDataSize); //4 bytes at 40

	//move the data array
	//right so currently mem moving to Audio data
	//so to get audio sample at specific point, we do (n*channel*bitspersample/8)
	memmove(wf->Samples, buffer+44, audioDataSize);
	free(buffer);
	
	printf ("Channels: %d\n", wf->Channels);
	printf ("Sample Rate: %d\n", wf->SampleRate);
	printf ("Block Align: %d\n", wf->BlockAlign);
	printf ("Bits Per Sample: %d\n", wf->BitsPerSample);
	printf ("Audio Data Size: %lu\n", audioDataSize);
	printf ("Track Length: %lu\n\n", (audioDataSize/wf->Channels/(wf->BitsPerSample/8)/wf->SampleRate));

	printf ("Audio Data: %x %x %x %x\n", wf->Samples[0], wf->Samples[1], wf->Samples[2], wf->Samples[3]);


	return wf;
}