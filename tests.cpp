#define CATCH_CONFIG_MAIN
#include "audio.h"
#include "catch.hpp"
#include <cmath>

using namespace MRXADA002;

std::vector<int8_t> v1{1,2,3,4,5};

double rms = 3.31662;

std::vector<int8_t> v1cut{1,5};  //result of v1 cut by [1,3]

std::vector<int8_t> v1v1{1,2,3,4,5,1,2,3,4,5}; //result of v1 | v1

std::vector<int8_t> v1x2{2,4,6,8,10}; //result of v1+v1

std::vector<int8_t> v1rev{5,4,3,2,1}; //reverse of v1

std::vector<int8_t> vradd{6,9,12}; //result of ranged add of v1 and v1x2 on [1,4)

TEST_CASE("MOVE SEMANTICS") {
    std::cout << "Testing Audio move assignment operator." << '\n';
    Audio<int8_t, 1> sound1(v1);
    Audio<int8_t, 1> sound2(v1);
    Audio<int8_t, 1> sound3 = std::move(sound2);
    REQUIRE(sound1==sound3);
    std::cout << "Testing Audio move constructor." << '\n';
    Audio<int8_t, 1> sound4(std::move(sound3));
    REQUIRE(sound1==sound4);
}

TEST_CASE("COPY SEMANTICS") {
    std::cout << "Testing Audio copy assignment operator." << '\n';
    Audio<int8_t, 1> sound1(v1);
    Audio<int8_t, 1> sound2 = sound1;
    REQUIRE(sound1==sound2);
    std::cout << "Testing Audio copy constructor." << '\n';
    Audio<int8_t, 1> sound3(sound1);
    REQUIRE(sound1==sound3);
}

TEST_CASE("+ OPERATOR") {
    std::cout << "Testing Audio + operator." << '\n';
    Audio<int8_t, 1> sound1(v1);
    Audio<int8_t, 1> sound2(v1);
    Audio<int8_t, 1> sound3 = sound1 + sound2;
    Audio<int8_t, 1> sound4(v1x2);
    REQUIRE(sound3==sound4);
}

TEST_CASE("| OPERATOR") {
    std::cout << "Testing Audio | operator." << '\n';
    Audio<int8_t, 1> sound1(v1);
    Audio<int8_t, 1> sound2(v1);
    Audio<int8_t, 1> sound3 = sound1 | sound2;
    Audio<int8_t, 1> sound4(v1v1);
    REQUIRE(sound3==sound4);
}

TEST_CASE("* OPERATOR") {
    std::cout << "Testing Audio * operator." << '\n';
    Audio<int8_t, 1> sound1(v1x2);
    std::pair<float, float> factor (0.5f, 0.5f);
    Audio<int8_t, 1> sound2 = sound1 * factor;
    Audio<int8_t, 1> sound3(v1);
    REQUIRE(sound2==sound3);
}

TEST_CASE("^ OPERATOR") {
    std::cout << "Testing Audio ^ operator." << '\n';
    Audio<int8_t, 1> sound1(v1);
    std::pair<int, int> range (1,3);
    Audio<int8_t, 1> sound2 = sound1^range;
    Audio<int8_t, 1> sound3(v1cut);
    REQUIRE(sound2==sound3);
}
TEST_CASE("REVERSE") {
    std::cout << "Testing Audio reverse method." << '\n';
    Audio<int8_t, 1> sound1(v1);
    Audio<int8_t, 1> sound2 = sound1.reverse();
    Audio<int8_t, 1> sound3(v1rev);
    REQUIRE(sound2==sound3);
}
TEST_CASE("RANGED ADD") {
    std::cout << "Testing Audio ranged add method." << '\n';
    Audio<int8_t, 1> sound1(v1);
    Audio<int8_t, 1> sound2(v1x2);
    Audio<int8_t, 1> sound3 = sound1.ranged_add(sound2, 1, 4);
    Audio<int8_t, 1> sound4(vradd);
    REQUIRE(sound3==sound4);
}
TEST_CASE("RMS") {
    std::cout << "Testing Audio rms method." << '\n';
    Audio<int8_t, 1> sound1(v1);
    double rmsFunc = sound1.rms();
    REQUIRE(std::abs(rms-rmsFunc)<0.00001);
}
TEST_CASE("NORMALIZE") {
    std::cout << "Testing Audio normalize method." << '\n';
    Audio<int8_t, 1> sound1(v1);
    std::pair<double, double> rmsRatio(2.0, 2.0);
    Audio<int8_t, 1> sound2 = sound1.normalize(rmsRatio);
    Audio<int8_t, 1> sound3(v1x2);
    REQUIRE(sound2==sound3);
}
