#include "audio.h"
#include <sstream>

using namespace MRXADA002;
int main(int argc, char** argv) {

    std::string outFilename;

    int sampleRate = atoi(argv[2]);
    int bitCount = atoi(argv[4]);
    int noChannels = atoi(argv[6]);

    int argIndex = 7;

    if (std::string(argv[argIndex])=="-o") {
        ++argIndex;
        outFilename = std::string(argv[argIndex]);
        ++argIndex;
    } else {
        outFilename = "out";
    }

    //ADD op
    if (std::string(argv[argIndex])=="-add") {
        ++argIndex;
        std::string inFilename1 = std::string(argv[argIndex]);
        ++argIndex;
        std::string inFilename2 = std::string(argv[argIndex]);
        if (noChannels==1) {
            if (bitCount==8) {
                Audio<int8_t, 1> sound1;
                Audio<int8_t, 1> sound2;
                sound1.load(inFilename1);
                sound2.load(inFilename2);
                Audio<int8_t, 1> sound3 = sound1 + sound2;
                sound3.save(outFilename);
            }
            if (bitCount==16) {
                Audio<int16_t, 1> sound1;
                Audio<int16_t, 1> sound2;
                sound1.load(inFilename1);
                sound2.load(inFilename2);
                Audio<int16_t, 1> sound3 = sound1 + sound2;
                sound3.save(outFilename);
            }
        }
        if (noChannels==2) {
            if (bitCount==8) {
                Audio<int8_t, 2> sound1;
                Audio<int8_t, 2> sound2;
                sound1.load(inFilename1);
                sound2.load(inFilename2);
                Audio<int8_t, 2> sound3 = sound1 + sound2;
                sound3.save(outFilename);
            }
            if (bitCount==16) {
                Audio<int16_t, 2> sound1;
                Audio<int16_t, 2> sound2;
                sound1.load(inFilename1);
                sound2.load(inFilename2);
                Audio<int16_t, 2> sound3 = sound1 + sound2;
                sound3.save(outFilename);
            }
        }
    }
    //CONCATENATE op
    else if (std::string(argv[argIndex])=="-cat") {
        ++argIndex;
        std::string inFilename1 = std::string(argv[argIndex]);
        ++argIndex;
        std::string inFilename2 = std::string(argv[argIndex]);
        if (noChannels==1) {
            if (bitCount==8) {
                Audio<int8_t, 1> sound1;
                Audio<int8_t, 1> sound2;
                sound1.load(inFilename1);
                sound2.load(inFilename2);
                Audio<int8_t, 1> sound3 = sound1 | sound2;
                sound3.save(outFilename);
            }
            if (bitCount==16) {
                Audio<int16_t, 1> sound1;
                Audio<int16_t, 1> sound2;
                sound1.load(inFilename1);
                sound2.load(inFilename2);
                Audio<int16_t, 1> sound3 = sound1 | sound2;
                sound3.save(outFilename);
            }
        }
        if (noChannels==2) {
            if (bitCount==8) {
                Audio<int8_t, 2> sound1;
                Audio<int8_t, 2> sound2;
                sound1.load(inFilename1);
                sound2.load(inFilename2);
                Audio<int8_t, 2> sound3 = sound1 | sound2;
                sound3.save(outFilename);
            }
            if (bitCount==16) {
                Audio<int16_t, 2> sound1;
                Audio<int16_t, 2> sound2;
                sound1.load(inFilename1);
                sound2.load(inFilename2);
                Audio<int16_t, 2> sound3 = sound1 | sound2;
                sound3.save(outFilename);
            }
        }
    }
    //VOLUME FACTOR
    else if (std::string(argv[argIndex])=="-v") {
        ++argIndex;
        float r1 = atof(argv[argIndex]);
        ++argIndex;
        float r2 = atof(argv[argIndex]);
        ++argIndex;
        if (r1 < 0.0f)  //clamp values
            r1 = 0.0f;
        if (r1 > 1.0f)
            r1 = 1.0f;
        if (r2 < 0.0f)
            r2 = 0.0f;
        if (r2 > 1.0f)
            r2 = 1.0f;
        std::pair<float, float> pairFactor (r1, r2);
        std::string inFilename1 = std::string(argv[argIndex]);
        if (noChannels==1) {
            if (bitCount==8) {
                Audio<int8_t, 1> sound1;
                sound1.load(inFilename1);
                Audio<int8_t, 1> sound3 = sound1*pairFactor;
                sound3.save(outFilename);
            }
            if (bitCount==16) {
                Audio<int16_t, 1> sound1;
                sound1.load(inFilename1);
                Audio<int16_t, 1> sound3 = sound1*pairFactor;
                sound3.save(outFilename);
            }
        }
        if (noChannels==2) {
            if (bitCount==8) {
                Audio<int8_t, 2> sound1;
                sound1.load(inFilename1);
                Audio<int8_t, 2> sound3 = sound1*pairFactor;
                sound3.save(outFilename);
            }
            if (bitCount==16) {
                Audio<int16_t, 2> sound1;
                sound1.load(inFilename1);
                Audio<int16_t, 2> sound3 = sound1*pairFactor;
                sound3.save(outFilename);
            }
        }
    }
    //CUT RANGE OP
    else if (std::string(argv[argIndex])=="-cut") {
        ++argIndex;
        int r1 = atoi(argv[argIndex]);
        ++argIndex;
        int r2 = atoi(argv[argIndex]);
        ++argIndex;
        std::pair<int, int> range (r1, r2);
        std::string inFilename1 = std::string(argv[argIndex]);
        if (noChannels==1) {
            if (bitCount==8) {
                Audio<int8_t, 1> sound1;
                sound1.load(inFilename1);
                Audio<int8_t, 1> sound3 = sound1^range;
                sound3.save(outFilename);
            }
            if (bitCount==16) {
                Audio<int16_t, 1> sound1;
                sound1.load(inFilename1);
                Audio<int16_t, 1> sound3 = sound1^range;
                sound3.save(outFilename);
            }
        }
        if (noChannels==2) {
            if (bitCount==8) {
                Audio<int8_t, 2> sound1;
                sound1.load(inFilename1);
                Audio<int8_t, 2> sound3 = sound1^range;
                sound3.save(outFilename);
            }
            if (bitCount==16) {
                Audio<int16_t, 2> sound1;
                sound1.load(inFilename1);
                Audio<int16_t, 2> sound3 = sound1^range;
                sound3.save(outFilename);
            }
        }
    }
    //REVERSE
    else if (std::string(argv[argIndex])=="-rev") {
        ++argIndex;
        std::string inFilename1 = std::string(argv[argIndex]);
        if (noChannels==1) {
            if (bitCount==8) {
                Audio<int8_t, 1> sound1;
                sound1.load(inFilename1);
                sound1.reverse();
                sound1.save(outFilename);
            }
            if (bitCount==16) {
                Audio<int16_t, 1> sound1;
                sound1.load(inFilename1);
                sound1.reverse();
                sound1.save(outFilename);
            }
        }
        if (noChannels==2) {
            if (bitCount==8) {
                Audio<int8_t, 2> sound1;
                sound1.load(inFilename1);
                sound1.reverse();
                sound1.save(outFilename);
            }
            if (bitCount==16) {
                Audio<int16_t, 2> sound1;
                sound1.load(inFilename1);
                sound1.reverse();
                sound1.save(outFilename);
            }
        }
    }
    //RANGED ADD OP
    else if (std::string(argv[argIndex])=="-radd") {
        ++argIndex;
        int r1Sec = atoi(argv[argIndex]);
        ++argIndex;
        int r2Sec = atoi(argv[argIndex]);
        int r1Sample = r1Sec*sampleRate;
        int r2Sample = r2Sec*sampleRate;
        ++argIndex;
        std::string inFilename1 = std::string(argv[argIndex]);
        ++argIndex;
        std::string inFilename2 = std::string(argv[argIndex]);
        if (noChannels==1) {
            if (bitCount==8) {
                Audio<int8_t, 1> sound1;
                Audio<int8_t, 1> sound2;
                sound1.load(inFilename1);
                sound2.load(inFilename2);
                Audio<int8_t, 1> sound3 = sound1.ranged_add(sound2, r1Sample, r2Sample);
                sound3.save(outFilename);
            }
            if (bitCount==16) {
                Audio<int16_t, 1> sound1;
                Audio<int16_t, 1> sound2;
                sound1.load(inFilename1);
                sound2.load(inFilename2);
                Audio<int16_t, 1> sound3 = sound1.ranged_add(sound2, r1Sample, r2Sample);
                sound3.save(outFilename);
            }
        }
        if (noChannels==2) {
            if (bitCount==8) {
                Audio<int8_t, 2> sound1;
                Audio<int8_t, 2> sound2;
                sound1.load(inFilename1);
                sound2.load(inFilename2);
                Audio<int8_t, 2> sound3 = sound1.ranged_add(sound2, r1Sample, r2Sample);
                sound3.save(outFilename);
            }
            if (bitCount==16) {
                Audio<int16_t, 2> sound1;
                Audio<int16_t, 2> sound2;
                sound1.load(inFilename1);
                sound2.load(inFilename2);
                Audio<int16_t, 2> sound3 = sound1.ranged_add(sound2, r1Sample, r2Sample);
                sound3.save(outFilename);
            }
        }
    }
    else if (std::string(argv[argIndex])=="-rms") {
        ++argIndex;
        std::string inFilename1 = std::string(argv[argIndex]);
        if (noChannels==1) {
            if (bitCount==8) {
                Audio<int8_t, 1> sound1;
                sound1.load(inFilename1);
                std::cout << "RMS: " << sound1.rms() << '\n';
                sound1.save(outFilename);
            }
            if (bitCount==16) {
                Audio<int16_t, 1> sound1;
                sound1.load(inFilename1);
                std::cout << "RMS: " << sound1.rms() << '\n';
                sound1.save(outFilename);
            }
        }
        if (noChannels==2) {
            if (bitCount==8) {
                Audio<int8_t, 2> sound1;
                sound1.load(inFilename1);
                std::cout << "Left RMS: " << sound1.rms().first << '\n';
                std::cout << "Right RMS: " << sound1.rms().second << '\n';
                sound1.save(outFilename);
            }
            if (bitCount==16) {
                Audio<int16_t, 2> sound1;
                sound1.load(inFilename1);
                std::cout << "Left RMS: " << sound1.rms().first << '\n';
                std::cout << "Right RMS: " << sound1.rms().second << '\n';
                sound1.save(outFilename);
            }
        }
    }
    else if (std::string(argv[argIndex])=="-norm") {
        ++argIndex;
        double rmsleft = atof(argv[argIndex]);  //desired left rms
        ++argIndex;
        double rmsright = atof(argv[argIndex]); //desired right rms
        ++argIndex;
        std::string inFilename1 = std::string(argv[argIndex]);
        if (noChannels==1) {
            if (bitCount==8) {
                Audio<int8_t, 1> sound1;
                sound1.load(inFilename1);
                double rmsCurrent = sound1.rms();
                double rmsRatioL = rmsleft / rmsCurrent; //ratio = desired rms/current rms
                double rmsRatioR = rmsleft / rmsCurrent;
                std::pair<double, double> rmsVals (rmsRatioL, rmsRatioR); //put ratios into pair of left/right
                Audio<int8_t, 1> sound2 = sound1.normalize(rmsVals);
                sound2.save(outFilename);
            }
            if (bitCount==16) {
                Audio<int16_t, 1> sound1;
                sound1.load(inFilename1);
                double rmsCurrent = sound1.rms();
                double rmsRatioL = rmsleft / rmsCurrent;
                double rmsRatioR = rmsleft / rmsCurrent;
                std::pair<double, double> rmsVals (rmsRatioL, rmsRatioR);
                Audio<int16_t, 1> sound2 = sound1.normalize(rmsVals);
                sound2.save(outFilename);
            }
        }
        if (noChannels==2) {
            if (bitCount==8) {
                Audio<int8_t, 2> sound1;
                sound1.load(inFilename1);
                std::pair<double, double> rmsCurrent = sound1.rms();
                double rmsRatioL = rmsleft / rmsCurrent.first;
                double rmsRatioR = rmsleft / rmsCurrent.second;
                std::pair<double, double> rmsVals (rmsRatioL, rmsRatioR);
                Audio<int8_t, 2> sound2 = sound1.normalize(rmsVals);
                sound2.save(outFilename);
            }
            if (bitCount==16) {
                Audio<int16_t, 2> sound1;
                sound1.load(inFilename1);
                std::pair<double, double> rmsCurrent = sound1.rms();
                double rmsRatioL = rmsleft / rmsCurrent.first;
                double rmsRatioR = rmsleft / rmsCurrent.second;
                std::pair<double, double> rmsVals (rmsRatioL, rmsRatioR);
                Audio<int16_t, 2> sound2 = sound1.normalize(rmsVals);
                sound2.save(outFilename);
            }
        }
    }

    //DEFAULT - just load and save 1 audio file
    else {
        std::string inFilename1 = std::string(argv[argIndex]);
        if (bitCount==8) {
            if (noChannels==1) {
              Audio<int8_t, 1> sound1;
              sound1.load(inFilename1);
              sound1.save(outFilename);
            }
            if (noChannels==2) {
                Audio<int8_t, 2> sound1;
                sound1.load(inFilename1);
                sound1.save(outFilename);
            }
        }
        if (bitCount==16) {
            if (noChannels==1) {
              Audio<int16_t, 1> sound1;
              sound1.load(inFilename1);
              sound1.save(outFilename);
            }
            if (noChannels==2) {
                Audio<int16_t, 2> sound1;
                sound1.load(inFilename1);
                sound1.save(outFilename);
            }
        }
    }
    return 0;

}
