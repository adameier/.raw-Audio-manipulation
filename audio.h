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
        //int sampleRate;
        int fileSizeInBytes;
    public:
        //default constructor
        Audio() : channels(N) {}

        //copy constructor
        Audio(const Audio<T, N> & rhs) :
            data_vector(rhs.data_vector),
            channels(rhs.channels),
            fileSizeInBytes(rhs.fileSizeInBytes) {}

        //move constructor
        Audio(Audio<T, N> && rhs) :
            data_vector(std::move(rhs.data_vector)),
            channels(rhs.channels),
            fileSizeInBytes(rhs.fileSizeInBytes) {}

        //copy assignment operator
        Audio<T, N> & operator=(const Audio<T, N> & rhs) {
            data_vector = rhs.data_vector;
            channels = rhs.channels;
            fileSizeInBytes = rhs.fileSizeInBytes;
            return *this;
        }

        //move assignment operator
        Audio<T, N> & operator=(Audio<T, N> && rhs) {
            data_vector = std::move(rhs.data_vector);
            channels = rhs.channels;
            fileSizeInBytes = rhs.fileSizeInBytes;
            return *this;
        }


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
        //int sampleRate;
        int fileSizeInBytes;
    public:
        //default constructor
        Audio() : channels(2){}

        //copy constructor
        Audio(const Audio<T, 2> & rhs) :
            data_vector(rhs.data_vector),
            channels(rhs.channels),
            fileSizeInBytes(rhs.fileSizeInBytes) {}

        //move constructor
        Audio(Audio<T, 2> && rhs) :
            data_vector(std::move(rhs.data_vector)),
            channels(rhs.channels),
            fileSizeInBytes(rhs.fileSizeInBytes) {}

        //copy assignment operator
        Audio<T, 2> & operator=(const Audio<T, 2> & rhs) {
            data_vector = rhs.data_vector;
            channels = rhs.channels;
            fileSizeInBytes = rhs.fileSizeInBytes;
            return *this;
        }

        //move assignment operator
        Audio<T, 2> & operator=(Audio<T, 2> && rhs) {
            data_vector = std::move(rhs.data_vector);
            channels = rhs.channels;
            fileSizeInBytes = rhs.fileSizeInBytes;
            return *this;
        }


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
