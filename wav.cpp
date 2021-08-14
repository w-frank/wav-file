/*
 * wav.cpp
 */

#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

// Riff Chunk
const string chunk_id = "RIFF";   //
const string chunk_size = "----"; // 4 bytes
const string format = "WAVE";     //

// FMT sub-chunk
const string subchunk1_id = "fmt "; // 4 bytes
const int subchunk1_size = 16;      // PCM
const int audio_format = 1;         //
const int num_channels = 2;         //
const int sample_rate = 44100;      //
const int byte_rate = sample_rate * num_channels * (subchunk1_size / 8);
const int block_align = num_channels * (subchunk1_size / 8);
const int bits_per_sample = 16;

// Data sub-chunk
const string subchunk2_id = "data";
const string subchunk2_size = "----";

// Audio constants
const int duration_sec = 2;
const int max_amplitude = 32760;
const double frequency = 250.0; // Hz
const double pi = 3.14;

void write_as_bytes(ofstream &file, int value, int byte_size)
{
    file.write(reinterpret_cast<const char*>(&value), byte_size);
}


int main()
{
    ofstream wav_file;
    wav_file.open("test.wav", ios::binary);

    if (wav_file.is_open())
    {
        // Write WAV header
        wav_file << chunk_id;
        wav_file << chunk_size;
        wav_file << format;

        wav_file << subchunk1_id;
        write_as_bytes(wav_file, subchunk1_size, 4);
        write_as_bytes(wav_file, audio_format, 2);
        write_as_bytes(wav_file, num_channels, 2);
        write_as_bytes(wav_file, sample_rate, 4);
        write_as_bytes(wav_file, byte_rate, 4);
        write_as_bytes(wav_file, block_align, 2);
        write_as_bytes(wav_file, bits_per_sample, 2);

        wav_file << subchunk2_id;
        wav_file << subchunk2_size;

        int start_audio = wav_file.tellp();

        for (int i = 0; i < (sample_rate * duration_sec); i++)
        {
            double amplitude = ((double)i / sample_rate) * max_amplitude;
            double value = sin((2 * pi * i * frequency) / sample_rate);

            double channel1 = (amplitude * value) / 2;
            double channel2 = (max_amplitude - amplitude) * value;

            write_as_bytes(wav_file, channel1, 2);
            write_as_bytes(wav_file, channel2, 2); 
        }

        int end_audio = wav_file.tellp();

        // Write subchunk2 size
        wav_file.seekp(start_audio - 4);
        write_as_bytes(wav_file, end_audio - start_audio, 4);

        wav_file.seekp(4, ios::beg);
        write_as_bytes(wav_file, end_audio - 8, 4);

    }

    wav_file.close();

    return 0;
}