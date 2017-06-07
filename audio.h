#ifndef AUDIO_H
#define AUDIO_H

#include <vector>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <math.h>

namespace MRXADA002 {
    template<typename T, int N> class Audio {
    private:
        std::vector<T> data_vector;
        int channels;
        //int sampleRate;
        //int fileSizeInBytes;
    public:

        //NORMALIZE FUNCTOR
        class Normalize {
        public:
            Normalize(const double & rmsRate) : ptr(&rmsRate) {}

            T operator()(const T & vectorVal) {
                int out = (int)(*ptr * vectorVal);
                if (sizeof(T)==1) {
                    if (out > INT8_MAX) {    //clamp value for  8 bit
                        out = INT8_MAX;
                    }
                }
                if (sizeof(T)==2) {
                    if (out > INT16_MAX) {    //clamp value for  16 bit
                        out = INT16_MAX;
                    }
                }
                return out;
            }
            const double * ptr;
        };

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
        Audio<T, N> operator+(const Audio<T, N> & rhs) {
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

        //CONCATENATE OPERATOR
        Audio<T, N> operator|(const Audio<T,N> & rhs) {
            Audio<T, N> a(*this);                       //use copy constructor on lhs audio
            for (int i = 0; i < rhs.data_vector.size(); ++i) {
                a.data_vector.push_back(rhs.data_vector[i]);         //append rhs's values onto data_vector
            }
            return a;
        }

        //VOLUME FACTOR OPERATOR
        Audio<T, N> operator*( std::pair<float, float> rhs) {
            Audio<T, N> a(*this);               //use copy constructor
            for (int i = 0; i < a.data_vector.size(); ++i) {
                a.data_vector[i]*=rhs.first; //scale amplitude
            }
            return a;
        }

        //CUT RANGE OPERATOR
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

        //REVERSE FUNCTION
        Audio<T, N> & reverse() {
            std::reverse(this->data_vector.begin(), this->data_vector.end());
            return *this;
        }

        //RANGED ADD FUNCTION
        Audio<T, N> ranged_add(const Audio<T,N> & rhs, int rangeStart, int rangeEnd) {
            Audio<T, N> range1;
            range1.data_vector.resize(rangeEnd-rangeStart);
            Audio<T, N> range2;
            range2.data_vector.resize(rangeEnd-rangeStart);
            std::copy(this->data_vector.begin()+rangeStart, this->data_vector.begin()+rangeEnd, range1.data_vector.begin());
            std::copy(rhs.data_vector.begin()+rangeStart, rhs.data_vector.begin()+rangeEnd, range2.data_vector.begin());
            Audio<T, N> out = range1 + range2;
            return out;
        }

        //RMS FUNCTION
        double rms() {
            return sqrt(std::accumulate(this->data_vector.begin(), this->data_vector.end(), 0.0, [&](double a, double b){return a + pow(b, 2)/this->data_vector.size();}));
        }

        //NORMALIZE FUNCTION
        Audio<T, N> normalize(std::pair<double, double> rms) {
            Audio<T, N> a(*this);
            Normalize functor(rms.first);
            std::transform(this->data_vector.begin(), this->data_vector.end(), a.data_vector.begin(), functor);
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

        //NORMALIZE FUNCTOR SECILIALIZED FOR STEREO
        class Normalize {
        public:
            Normalize(const std::pair<double, double> & rmsRate) : ptr(&rmsRate) {}

            std::pair<T, T> operator()(std::pair<T, T> vectorVal) {
                std::pair<T, T> out;               //output pair
                int outLeft = (int)(vectorVal.first * ptr->first);
                int outRight = (int)(vectorVal.second * ptr->second);
                if (sizeof(T)==1) {
                    if (outLeft > INT8_MAX) {    //clamp value for  8 bit
                        outLeft = INT8_MAX;
                    }
                    if (outRight > INT8_MAX) {    //clamp value for  8 bit
                        outRight = INT8_MAX;
                    }
                }
                if (sizeof(T)==2) {
                    if (outLeft > INT16_MAX) {    //clamp value for  16 bit
                        outLeft = INT16_MAX;
                    }
                    if (outRight > INT16_MAX) {    //clamp value for  16 bit
                        outRight = INT16_MAX;
                    }
                }
                out.first = outLeft;
                out.second = outRight;
                return out;
            }
            const std::pair<double,double> * ptr;
            int channels;
        };

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
        Audio<T, 2> operator+(const Audio<T, 2> & rhs) {
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
        Audio<T, 2> operator|(const Audio<T,2> & rhs) {
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

        //REVERSE FUNCTION
        Audio<T, 2> & reverse() {
            std::reverse(this->data_vector.begin(), this->data_vector.end());
            return *this;
        }

        //RANGED ADD FUNCTION
        Audio<T, 2> ranged_add(const Audio<T,2> & rhs, int rangeStart, int rangeEnd) {
            Audio<T, 2> range1;
            range1.data_vector.resize(rangeEnd-rangeStart);
            Audio<T, 2> range2;
            range2.data_vector.resize(rangeEnd-rangeStart);
            std::copy(this->data_vector.begin()+rangeStart, this->data_vector.begin()+rangeEnd, range1.data_vector.begin());
            std::copy(rhs.data_vector.begin()+rangeStart, rhs.data_vector.begin()+rangeEnd, range2.data_vector.begin());
            Audio<T, 2> out = range1 + range2;
            return out;
        }

        //RMS FUNCTION
        std::pair<double, double> rms() {
            double left = sqrt(std::accumulate(this->data_vector.begin(), this->data_vector.end(), 0.0, [&](double a, std::pair<T,T> b){return a + pow(b.first, 2)/this->data_vector.size();}));
            double right = sqrt(std::accumulate(this->data_vector.begin(), this->data_vector.end(), 0.0, [&](double a, std::pair<T,T> b){return a + pow(b.second, 2)/this->data_vector.size();}));
            std::pair<double, double> ans (left, right);
            return ans;
        }

        //NORMALIZE FUNCTION
        Audio<T, 2> normalize(std::pair<double, double> rms) {
            Audio<T, 2> a(*this);
            Normalize functor(rms);
            std::transform(this->data_vector.begin(), this->data_vector.end(), a.data_vector.begin(), functor);
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
