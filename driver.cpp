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

    //TO CHANGE
    std::string inFilename1 = std::string(argv[argIndex]);
    if (bitCount = 8) {
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

}
