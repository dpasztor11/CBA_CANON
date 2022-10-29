#include <string>
#include <vector>
#include <fstream>
#include <iostream>

long long binaryToNumber(std::string binary)
{
    long long sol = 0;
    for (int i = 0; i < binary.length(); i++)
    {
        sol = (sol << 1) + (binary[i] == '1');
    }
    return sol;
}

std::string numberToBinary(long long num, int vectorSize)
{
    std::string sol = "";
    long long toCheck = num;
    for (int j = 0; j < vectorSize; j++)
    {
        sol = (char)('0' + toCheck % 2) + sol;
        toCheck /= 2;
    }
    return sol;
}

long getCskEqCount(int len)
{
    switch (len)
    {
    case 6:
        return 843;
    case 5:
        return 7;
    case 4:
        return 4;
    default:
        throw;
    }
}

long getCsEqCount(int len)
{
    switch (len)
    {
    case 6:
        return 70985;
    case 5:
        return 16;
    case 4:
        return 6;
    default:
        throw;
    }
}

long getCEqCount(int len)
{
    switch (len)
    {
    case 6:
        return 44601292;
    case 5:
        return 314;
    case 4:
        return 12;
    default:
        throw;
    }
}

// TO DO: make one method for getting CBAs out of file?
std::vector<long long> getCsEq(int len, bool withZeroVector)
{
    std::ifstream csEqFile;
    csEqFile.open("txt/csEquivalence/csEq" + std::to_string(len) + ".txt");

    std::vector<long long> csEq;

    long count = getCsEqCount(len);
    std::string temp;

    // no first line or zero vector
    csEqFile >> temp;
    if (!withZeroVector)
        csEqFile >> temp;
    for (long long i = 0; i < count - 1; i++)
    {
        csEqFile >> temp;
        csEq.push_back(binaryToNumber(temp));
    }

    csEqFile.close();
    return csEq;
}

std::vector<long long> getCEq(int len, bool withZeroVector)
{
    std::ifstream cEqFile;
    cEqFile.open("txt/cEquivalence/cEq" + std::to_string(len) + ".txt");

    std::vector<long long> cEq;

    long count = getCEqCount(len);
    std::string temp;

    // no first line or zero vector
    cEqFile >> temp;
    if (!withZeroVector)
        cEqFile >> temp;
    for (long long i = 0; i < count - 1; i++)
    {
        cEqFile >> temp;
        cEq.push_back(binaryToNumber(temp));
    }

    cEqFile.close();
    return cEq;
}

void storeCBAsToFile(std::string fileName, std::vector<long long> cbas, int vectorSize)
{
    std::ofstream file;
    file.open(fileName);
    for (int i = 0; i < cbas.size(); i++)
    {
        file << numberToBinary(cbas[i], vectorSize) << std::endl;
    }
    file.close();
}