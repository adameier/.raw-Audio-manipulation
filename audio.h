#ifndef AUDIO_H
#define AUDIO_H

#include <vector>
#include <cstdint>
#include <fstream>
#include <iostream>

namespace MRXADA002 {
    template<typename T, int N> class Audio {
    private:
        std::vector<T> data_vector;
        int channels;
        int sampleRate;
        int fileSizeInBytes;
    public:
        Audio() : channels(N) {} //default constructor

        void load(std::string filename) {

            const char * cfilename = filename.c_str();
            std::ifstream file(cfilename, std::ios::binary);

            file.seekg(0, file.end);
            fileSizeInBytes = file.tellg();
            file.seekg(0, file.beg);

            int NumberOfSamples = fileSizeInBytes / (sizeof(T) * channels);
            data_vector.resize(NumberOfSamples);

            file.read((char*)&data_vector[0], fileSizeInBytes);
            std::cout << "File loaded." << '\n';
            file.close();
        }

        void save(std::string filename) {
            filename = filename + ".raw";
            const char * cfilename = filename.c_str();
            std::ofstream file(cfilename, std::ios::binary);

            file.write((char*)&data_vector[0], fileSizeInBytes);
            std::cout << "File saved." << '\n';
            file.close();

        }
    };

    template<typename T> class Audio<T, 2> {
    private:
        std::vector<std::pair<T,T>> data_vector;
        int channels;
        int sampleRate;
        int fileSizeInBytes;
    public:
        Audio() : channels(2){}

        void load(std::string filename) {

            const char * cfilename = filename.c_str();
            std::ifstream file(cfilename, std::ios::binary);

            file.seekg(0, file.end);
            fileSizeInBytes = file.tellg();
            file.seekg(0, file.beg);

            int NumberOfSamples = fileSizeInBytes / (sizeof(T) * channels);
            data_vector.resize(NumberOfSamples);

            file.read((char*)&data_vector[0], fileSizeInBytes);
            std::cout << "Stereo file loaded." << '\n';
            file.close();
        }

        void save(std::string filename) {
            filename = filename + ".raw";
            const char * cfilename = filename.c_str();
            std::ofstream file(cfilename, std::ios::binary);

            file.write((char*)&data_vector[0], fileSizeInBytes);
            std::cout << "Stereo file saved." << '\n';
            file.close();

        }
    };
}

#endif
