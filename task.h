#ifndef MACHINE_SIMULATOR_HEADER_H
#define MACHINE_SIMULATOR_HEADER_H

//libraries included in the code
#include <bitset>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

//Classes used in the code
class MainMemory {
private:
  int size;
  std::vector<std::string> memory;

public:
  MainMemory();
  void store(int address, std::string value);
  std::string read(int address);
};

class Registers {
private:
  int size;
  std::vector<std::string> memory;

public:
  Registers();
  void store(int address, std::string value);
  std::string read(int address);
};

class Instructions {
public:
  void execute(std::string s, Registers &cpuRegister, MainMemory &ram,
               int &programC, std::vector<std::string> &screen);
};

class CPU {
private:
  int programCounter;
  std::string instructionRegister;
  std::vector<std::string> screen;
  Instructions xdd;

  void fetch(MainMemory &r);

public:
  CPU();
  void runProgram(MainMemory &ram, Registers &cpuRegister);
  std::string getIR();
  int getPC();
  void getScreen();
};

class Machine : CPU {
public:
  MainMemory ram;
  Registers cpuRegister;
  CPU main;
  std::vector<std::string> linesAfter;

  Machine();
  void loadFile();
  void printData(std::string userInput);
};

// Prototype for helping functions
std::string hexToBinary(const std::string &hexString);
std::string hexToBinary4(const std::string &hexString);
unsigned long binaryToInt(const std::string &binaryString);
unsigned long binaryToInt4(const std::string &binaryString);
std::string twosComplementAddition(std::string valueX, std::string valueY);
std::string decimalToHex(const int &decimalInteger);

#endif // MACHINE_SIMULATOR_HEADER_H
