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
        //int fileSizeInBytes;
    public:
        //default constructor
        Audio() : channels(N) {}

        //copy constructor
        Audio(const Audio<T, N> & rhs) :
            data_vector(rhs.data_vector),
            channels(rhs.channels) {}

        //move constructor
        Audio(Audio<T, N> && rhs) :
            data_vector(std::move(rhs.data_vector)),
            channels(rhs.channels) {}

        //copy assignment operator
        Audio<T, N> & operator=(const Audio<T, N> & rhs) {
            data_vector = rhs.data_vector;
            channels = rhs.channels;
            return *this;
        }

        //move assignment operator
        Audio<T, N> & operator=(Audio<T, N> && rhs) {
            data_vector = std::move(rhs.data_vector);
            channels = rhs.channels;
            return *this;
        }

        //+ OPERATOR
        Audio<T, N> operator+(const Audio<T, N> rhs) {
            Audio<T, N> a(*this);
            int m = std::min(a.data_vector.size(), rhs.data_vector.size());
            for (int i = 0; i < m; ++i) {
                int newVal = a.data_vector[i] + rhs.data_vector[i]; //add amplitudes
                if (sizeof(T)==1) {
                    if (newVal > INT8_MAX) {    //clamp value for  8 bit
                        newVal = INT8_MAX;
                    }
                    a.data_vector[i] = newVal;
                }
                if (sizeof(T)==2) {             //clamp value for 16 bit
                    if (newVal > INT16_MAX) {
                        newVal = INT16_MAX;
                    }
                    a.data_vector[i] = newVal;
                }
            }
            return a;
        }

        //CONCATENATE
        Audio<T, N> operator|(const Audio<T,N> rhs) {
            Audio<T, N> a(*this);                       //use copy constructor on lhs audio
            for (int i = 0; i < rhs.data_vector.size(); ++i) {
                a.data_vector.push_back(rhs.data_vector[i]);         //append rhs's values onto data_vector
            }
            return a;
        }

        //VOLUME FACTOR
        Audio<T, N> operator*( std::pair<float, float> rhs) {
            Audio<T, N> a(*this);               //use copy constructor
            for (int i = 0; i < a.data_vector.size(); ++i) {
                a.data_vector[i]*=rhs.first; //scale amplitude
            }
            return a;
        }

        //CUT RANGE
        Audio<T, N> operator^(std::pair<int, int> rhs ) {
            Audio<T, N> a;
            for (int i = 0; i < rhs.first; ++i) {
                a.data_vector.push_back(this->data_vector[i]);
            }
            for (int i = rhs.second + 1; i < this->data_vector.size(); ++i) {
                a.data_vector.push_back(this->data_vector[i]);
            }
            return a;
        }

        void load(std::string filename) {

            const char * cfilename = filename.c_str();
            std::ifstream file(cfilename, std::ios::binary);

            file.seekg(0, file.end);
            int fileSizeInBytes = file.tellg();
            file.seekg(0, file.beg);

            int numberOfSamples = fileSizeInBytes / (sizeof(T) * channels);
            std::cout << sizeof(T) << '\n';
            data_vector.resize(numberOfSamples);

            file.read((char*)&data_vector[0], fileSizeInBytes);
            std::cout << "File loaded." << '\n';
            file.close();
        }

        void save(std::string filename) {
            filename = filename + ".raw";
            const char * cfilename = filename.c_str();
            std::ofstream file(cfilename, std::ios::binary);

            int fileSizeInBytes = data_vector.size() * sizeof(T) * channels; //calculate bytes to be written
            file.write((char*)&data_vector[0], fileSizeInBytes);
            std::cout << "File saved." << '\n';
            file.close();

        }
    };

    //STEREO TEMPLATE SPECIALIZATION
    template<typename T> class Audio<T, 2> {
    private:
        std::vector<std::pair<T,T>> data_vector;
        int channels;
        //int sampleRate;
        //int fileSizeInBytes;
    public:
        //default constructor
        Audio() : channels(2){}

        //copy constructor
        Audio(const Audio<T, 2> & rhs) :
            data_vector(rhs.data_vector),
            channels(rhs.channels){}

        //move constructor
        Audio(Audio<T, 2> && rhs) :
            data_vector(std::move(rhs.data_vector)),
            channels(rhs.channels){}

        //copy assignment operator
        Audio<T, 2> & operator=(const Audio<T, 2> & rhs) {
            data_vector = rhs.data_vector;
            channels = rhs.channels;
            return *this;
        }

        //move assignment operator
        Audio<T, 2> & operator=(Audio<T, 2> && rhs) {
            data_vector = std::move(rhs.data_vector);
            channels = rhs.channels;
            return *this;
        }

        //+ OPERATOR
        Audio<T, 2> operator+(const Audio<T, 2> rhs) {
            Audio<T, 2> a(*this);
            int m = std::min(a.data_vector.size(), rhs.data_vector.size());
            for (int i = 0; i < m; ++i) {
                int newValLEFT = a.data_vector[i].first + rhs.data_vector[i].first; //add amplitudes of lefts
                int newValRIGHT = a.data_vector[i].second + rhs.data_vector[i].second; //add amplitudes of rights
                if (sizeof(T)==1) {
                    if (newValLEFT > INT8_MAX) {    //clamp value for  8 bit
                        newValLEFT = INT8_MAX;
                    }
                    if (newValRIGHT > INT8_MAX) {    //clamp value for  8 bit
                        newValRIGHT = INT8_MAX;
                    }
                    a.data_vector[i].first = newValLEFT;
                    a.data_vector[i].second = newValRIGHT;
                }
                if (sizeof(T)==2) {
                    if (newValLEFT > INT16_MAX) {    //clamp left value for 16 bit
                        newValLEFT = INT16_MAX;
                    }
                    if (newValRIGHT > INT16_MAX) {    //clamp right value for 16 bit
                        newValRIGHT = INT16_MAX;
                    }
                    a.data_vector[i].first = newValLEFT;
                    a.data_vector[i].second = newValRIGHT;
                }
            }
            return a;
        }

        //CONCATENATE
        Audio<T, 2> operator|(const Audio<T,2> rhs) {
            Audio<T, 2> a(*this);                       //use copy constructor on lhs audio
            for (int i = 0; i < rhs.data_vector.size(); ++i) {
                a.data_vector.push_back(rhs.data_vector[i]);         //append rhs's values onto data_vector
            }
            return a;
        }

        //VOLUME FACTOR
        Audio<T, 2> operator*( std::pair<float, float> rhs) {
            Audio<T, 2> a(*this);                       //use copy constructor
            for (int i = 0; i < a.data_vector.size(); ++i) {
                a.data_vector[i].first*=rhs.first;  //scale amplitude
                a.data_vector[i].second*=rhs.second;
            }
            return a;
        }

        //CUT RANGE
        Audio<T, 2> operator^(std::pair<int, int> rhs ) {
            Audio<T, 2> a;
            for (int i = 0; i < rhs.first; ++i) {
                a.data_vector.push_back(this->data_vector[i]);
            }
            for (int i = rhs.second + 1; i < this->data_vector.size(); ++i) {
                a.data_vector.push_back(this->data_vector[i]);
            }
            return a;
        }

        void load(std::string filename) {

            const char * cfilename = filename.c_str();
            std::ifstream file(cfilename, std::ios::binary);

            file.seekg(0, file.end);
            int fileSizeInBytes = file.tellg();
            file.seekg(0, file.beg);

            int numberOfSamples = fileSizeInBytes / (sizeof(T) * channels);
            data_vector.resize(numberOfSamples);

            file.read((char*)&data_vector[0], fileSizeInBytes);
            std::cout << "Stereo file loaded." << '\n';
            file.close();
        }

        void save(std::string filename) {
            filename = filename + ".raw";
            const char * cfilename = filename.c_str();
            std::ofstream file(cfilename, std::ios::binary);

            int fileSizeInBytes = data_vector.size() * sizeof(T) * channels; //calculate bytes to be written
            file.write((char*)&data_vector[0], fileSizeInBytes);
            std::cout << "Stereo file saved." << '\n';
            file.close();

        }
    };
}

#endif
