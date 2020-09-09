#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <map>
#include "../read_write_wav.h"

#define MAXTIME		2646000
#define TUNETIME	125
#define ML300		13230	// 44100*300/1000
#define ML500		22050	// 44100*500/1000
#define ML800		35280	// 44100*800/1000

using namespace std;

const float Fs = 44100; //sample rate (samples /second)
float audio_buffer[MAXTIME];
float* sample_digdin5;
float* sample_ha3;
float* sample_qdlc5;
float* sample_soltosom8;
float* sample_vai3;
int currentPosition = 0;

float freq2midi(float freq)
{
    return 12*log2( freq/440) + 69;
}

float midi2freq(int m)
{
    return 440*pow(2,(m-69.0f)/12.0f);
}

void makeNote(int midinote, float amp, int period){
// the period is in milliseconds, amplitude is less than 1.0
	int oldCP = currentPosition;
    int write_len = round(period*Fs/1000); // samples
    float freq = midi2freq(midinote);
  
    for (currentPosition; currentPosition < (write_len + oldCP); currentPosition++){
    	audio_buffer[currentPosition] = amp * sin(2*M_PI*freq*((currentPosition - oldCP)/Fs));
    	//cout << "valor [" + std::to_string(currentPosition) + "] = " + std::to_string(audio_buffer[currentPosition]) << endl;
    }
}

void makeSilence(int period){
	int oldCP = currentPosition;
    int write_len = round(period*Fs/1000); // samples

	for (currentPosition; currentPosition < (write_len + oldCP); currentPosition++){
    	audio_buffer[currentPosition] = 0;
    }
}

void makeTune(int octave, char note, int periodOff){
	int midinote = 0;

	switch(octave){
		case 1:
			midinote = 24;
		case 2:
			midinote = 36;
		case 3:
			midinote = 48;
			break;
		case 4:
			midinote = 60;
			break;
		case 5:
			midinote = 72;
			break;
		case 6:
			midinote = 84;
			break;
		case 7:
			midinote = 96;
			break;
		case 8:
			midinote = 108;
			break;
		default:
			break;
	}

	switch(note){
		case 'c':
			midinote += 0;
			break;
		case 'C':
			midinote += 1;
			break;
		case 'd':
			midinote += 2;
			break;
		case 'D':
			midinote += 3;
			break;
		case 'e':
			midinote += 4;
			break;
		case 'f':
			midinote += 5;
			break;
		case 'F':
			midinote += 6;
			break;
		case 'g':
			midinote += 7;
			break;
		case 'G':
			midinote += 8;
			break;
		case 'a':
			midinote += 9;
			break;
		case 'A':
			midinote += 10;
			break;
		case 'B':
			midinote += 11;
			break;
		default:
			break;
	}

	makeNote(midinote, 0.7f, 100);
	makeSilence(periodOff * TUNETIME);
}

void sampleInit(){
	int buffer_len3;
	int buffer_len5;
	int buffer_len8;
	int sample_rate = 44100;

	/* isso aqui da segmentation fault nao sei pq kkkkkkk
	for(long int n = 0; n < ML300; n++){
		sample_ha3[n] = 0;
		sample_vai3[n] = 0;
	}
	for(long int n = 0; n < ML500; n++){
		sample_qdlc5[n] = 0;
		sample_digdin5[n] = 0;
	}
	for(long int n = 0; n < ML800; n++){
		sample_soltosom8[n] = 0;
	}
	*/

	read_wave_file("digdin5.wav", sample_digdin5, &buffer_len5, &sample_rate);
	read_wave_file("ha3.wav", sample_ha3, &buffer_len3, &sample_rate);
	read_wave_file("qdlc5.wav", sample_qdlc5, &buffer_len5, &sample_rate);
	read_wave_file("soltosom8.wav", sample_soltosom8, &buffer_len8, &sample_rate);
	read_wave_file("vai3.wav", sample_vai3, &buffer_len3, &sample_rate);

	sample_digdin5[buffer_len5] = -1.0f;
	sample_ha3[buffer_len3] = -1.0f;
	sample_qdlc5[buffer_len5] = -1.0f;
	sample_soltosom8[buffer_len8] = -1.0f;
	sample_vai3[buffer_len3] = -1.0f;
}

void writeSample(float *sample){
	int pos = 0;

	while(sample[pos] != -1.0f){
    	audio_buffer[currentPosition] = sample[pos];
    	currentPosition++;
    	pos++;
    }
}

void music(){
	makeSilence(250);
	writeSample(sample_soltosom8);
	makeSilence(250);
	makeTune(4, 'F', 3);
	makeTune(4, 'a', 1);
	makeTune(5, 'C', 3);
	makeTune(4, 'a', 3);
	makeTune(4, 'F', 1);
	makeTune(4, 'd', 1);
	makeTune(4, 'd', 1);
	makeTune(4, 'd', 2);

	writeSample(sample_qdlc5);
	makeSilence(300);

	makeTune(4, 'C', 1);
	makeTune(4, 'D', 1);

	makeTune(4, 'F', 1);
	makeTune(4, 'a', 1);
	makeTune(5, 'C', 3);
	makeTune(4, 'a', 3);
	makeTune(4, 'F', 1);

	makeTune(5, 'e', 5);
	makeTune(5, 'D', 1);
	makeTune(5, 'd', 1);

	writeSample(sample_digdin5);
	makeSilence(200);

	makeTune(4, 'G', 3);
	makeTune(5, 'C', 1);
	makeTune(4, 'F', 3);
	makeTune(5, 'C', 3);
	makeTune(4, 'G', 3);
	makeTune(5, 'C', 3);

	makeTune(4, 'g', 1);
	makeTune(4, 'F', 3);
	makeTune(4, 'e', 3);
	makeTune(4, 'e', 1);
	makeTune(4, 'e', 1);
	makeTune(4, 'e', 7);
	makeTune(4, 'e', 1);
	makeTune(4, 'e', 1);
	makeTune(4, 'e', 1);

	writeSample(sample_qdlc5);
	makeSilence(200);

	makeTune(4, 'D', 3);
	makeTune(4, 'd', 3);
	makeTune(4, 'C', 3);
	makeTune(4, 'a', 1);
	makeTune(5, 'C', 3);
	makeTune(4, 'a', 3);

	makeTune(4, 'F', 1);
	makeTune(4, 'e', 1);
	makeTune(4, 'e', 1);
	makeTune(4, 'e', 3);
	makeTune(5, 'e', 1);
	makeTune(5, 'e', 1);
	makeTune(5, 'e', 1);

	writeSample(sample_vai3);
	makeSilence(200);

	makeTune(4, 'F', 1);
	makeTune(4, 'a', 1);
	makeTune(5, 'C', 3);
	makeTune(4, 'a', 3);
	makeTune(4, 'F', 1);
	makeTune(5, 'e', 7);
	makeTune(5, 'd', 2);

	writeSample(sample_ha3);
	makeSilence(300);

	makeTune(4, 'b', 1);
	makeTune(4, 'g', 1);
	makeTune(4, 'd', 1);
	makeTune(4, 'C', 3);
	makeTune(4, 'b', 1);
	makeTune(4, 'g', 1);
	makeTune(4, 'C', 1);

	makeTune(4, 'a', 1);
	makeTune(4, 'F', 1);
	makeTune(4, 'c', 1);
	makeTune(3, 'b', 3);
	makeTune(4, 'f', 1);
	makeTune(4, 'd', 1);
	makeTune(3, 'b', 1);

	makeTune(4, 'e', 1);
	makeTune(4, 'e', 1);
	makeTune(4, 'e', 3);

	writeSample(sample_vai3);
	makeSilence(400);

	makeTune(4, 'A', 1);
	makeTune(4, 'B', 1);
	makeTune(5, 'C', 1);
	makeTune(5, 'd', 1);
	makeTune(5, 'F', 1);
	makeTune(5, 'a', 1);

	makeSilence(500);
	writeSample(sample_ha3);
	makeSilence(250);

}

int main()
{
	for(int n = 0; n < MAXTIME; n++){
		audio_buffer[n] = 0;
	}
	sampleInit();

	music();

    //string wav_name = "oscillator_" + std::to_string(freq) + "Hz.wav";
	write_wave_file("output.wav", audio_buffer, currentPosition, Fs);
    cout << "done." << endl;
    return 0;
}

