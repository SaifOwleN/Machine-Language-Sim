//#include<bits/stdc++.h>
#include <bitset>
#include <cctype>
#include <deque>
#include <fstream>
#include <iostream>
#include <iterator>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>



std::string hexToBinary(const std::string &hexString) {
  // Convert hexadecimal string to integer
  int decimalValue = std::stoi(hexString, nullptr, 16);

  // Convert integer to binary string
  std::bitset<8> binaryValue(decimalValue);

  // Get the binary string
  return binaryValue.to_string();
}

std::string hexToBinary4(const std::string &hexString) {
  // Convert hexadecimal string to integer
  int decimalValue = std::stoi(hexString, nullptr, 16);

  // Convert integer to binary string
  std::bitset<4> binaryValue(decimalValue);

  // Get the binary string
  return binaryValue.to_string();
}

unsigned long binaryToInt(const std::string &binaryString) {
  std::bitset<8> bits(binaryString);
  return bits.to_ulong();
}

unsigned long binaryToInt4(const std::string &binaryString) {
  std::bitset<4> bits(binaryString);
  return bits.to_ulong();
}

std::string twosComplementAddition(std::string valueX, std::string valueY) {
  std::bitset<8> binaryX(valueX);
  std::bitset<8> binaryY(valueY);
  std::bitset<8> sum;
  bool carry = false;

  for (int i = 0; i < 8; ++i) {
    bool bitA = binaryX[i];
    bool bitB = binaryY[i];

    // XOR for the sum
    sum[i] = bitA ^ bitB ^ carry;

    // Calculate the carry
    carry = (bitA & bitB) | (carry & (bitA ^ bitB));
  }

  return sum.to_string();
}

std::string decimalToHex(const int &decimalInteger) {
  std::stringstream ss;
  ss << std::hex << decimalInteger;
  return ss.str();
}

class MainMemory {
private:
  int size;
  std::vector<std::string> memory;

public:
  MainMemory() {
    size = 256;
    memory.assign(size, "");
  }
  void store(int address, std::string value) { memory[address] = value; }
  std::string read(int address) { return memory[address]; }
};

class Registers {
private:
  int size;
  std::vector<std::string> memory;

public:
  Registers() {
    size = 16;
    memory.assign(size, "00000000");
  }
  void store(int address, std::string value) { memory[address] = value; }
  std::string read(int address) { return memory[address]; }
};

class Instructions {
public:
  void execute(std::string s, Registers &cpuRegister, MainMemory &ram,
               int &programC, std::vector<std::string> &screen) {
    std::string instructionBin = s.substr(0, 4);
    std::string registerBin = s.substr(4, 4);
    std::string memoryCellBin = s.substr(9);

    int instruction = binaryToInt4(instructionBin);

    int reg = binaryToInt4(registerBin);

    int memoryCell = binaryToInt(memoryCellBin);
    std::cout << instruction << "||";
    std::string value;

    switch (instruction) {
    case 1:
      std::cout << "case1";

      value = ram.read(memoryCell);
      cpuRegister.store(reg, value);
      break;

    case 2:
      std::cout << "case2 ";

      cpuRegister.store(reg, memoryCellBin);
      break;

    case 3:
      std::cout << "case3 ";

      value = cpuRegister.read(reg);
      if (memoryCell == 0) {
        screen.push_back(value);
        break;
      } else {
        ram.store(memoryCell, value);
        break;
      }
    case 4: {
      std::string x = memoryCellBin.substr(0, 4);
      std::string y = memoryCellBin.substr(4);
      int regX = binaryToInt4(x);
      int regY = binaryToInt4(y);
      value = cpuRegister.read(regX);
      std::cout << "message" << std::endl;
      cpuRegister.store(regY, value);
      break;
    }
    case 5: {
      std::cout << "case5: " << std::endl;
      int regX = binaryToInt4(memoryCellBin.substr(0, 4));
      int regY = binaryToInt4(memoryCellBin.substr(4));
      std::string valueX = cpuRegister.read(regX);
      std::string valueY = cpuRegister.read(regY);
      std::string result = twosComplementAddition(valueX, valueY);
      cpuRegister.store(reg, result);
      break;
    }

    case 11: {
      if (cpuRegister.read(reg) == cpuRegister.read(0)) {
        std::cout << programC << "xdd" << std::endl;
        programC = memoryCell - 2;
        std::cout << programC << std::endl;
        break;
      }
      value = cpuRegister.read(reg);
      break;
    }
    case 12: {
      programC = 1000;
      break;
    }
    }
  }
};

class CPU {
private:
  int programCounter;
  std::string instructionRegister;
  std::vector<std::string> screen;
  Instructions xdd;
  void fetch(MainMemory &r) {
    instructionRegister =
        r.read(programCounter) + " " + r.read(programCounter + 1);
  };

public:
  CPU() : programCounter(0), instructionRegister("") {}

  void runProgram(MainMemory &ram, Registers &cpuRegister) {
    for (int i = 0; programCounter < 1000; i++) {
      std::cout << std::endl
                << "programCounter: " << programCounter << "||" << std::endl;
      if (programCounter > 254) {
        break;
      }
      fetch(ram);
      xdd.execute(instructionRegister, cpuRegister, ram, programCounter,
                  screen);
      programCounter += 2;
    }
  }
  std::string getIR() { return instructionRegister; }
  int getPC() { return programCounter; }
  void getScreen() {
    for (std::string line : screen) {
      std::cout << line << std::endl;
    };
  }
};

class Machine : CPU {
private:
public:
  MainMemory ram;
  Registers cpuRegister;
  CPU main;
  std::vector<std::string> linesAfter;

  Machine() {}

  void loadFile() {
    std::string file1 = "machineCode";
    std::cout << "1- load a program" << std::endl;
    std::cout << " " << std::endl;
    std::ifstream file(file1 + ".txt");
    int i = 0;
    std::string line;
    std::vector<std::string> linesPre;
    while (std::getline(file, line)) {
      linesPre.push_back(line);
    };
    for (const std::string &str : linesPre) {
      std::string firstPartHex = str.substr(0, 3);
      std::string secondPartHex = str.substr(4, 6);
      std::string thirdPartHex = str.substr(8);
      std::string firstPartBin = hexToBinary4(firstPartHex);
      std::string secondPartBin = hexToBinary4(secondPartHex);

      std::string thirdPartBin = hexToBinary(thirdPartHex);
      std::string firstLine = firstPartBin + secondPartBin;
      linesAfter.push_back(firstLine);
      linesAfter.push_back(thirdPartBin);
    };
    while (linesAfter.size() < 256) {
      linesAfter.push_back("00000000");
    }
    for (int i = 0; i < linesAfter.size(); i++) {
      ram.store(i, linesAfter[i]);
    };
    main.runProgram(ram, cpuRegister);
  };
  void printData(std::string userInput) {
    if (userInput == "memory") {
      std::ofstream output("ram.txt");
      for (int i = 0; i < 256; i++) {
        output << ram.read(i) << std::endl;
      }
    } else if (userInput == "IR") {
      std::cout << main.getIR() << std::endl;
    } else if (userInput == "registers") {
      std::ofstream output("Registers.txt");
      for (int i = 0; i < 16; i++) {
        output << cpuRegister.read(i) << std::endl;
      }
    } else if (userInput == "PC") {
      std::cout << main.getPC() << std::endl;
    } else if (userInput == "screen") {
      main.getScreen();
    }
  };
};

int main() {
  Machine PC;
  PC.loadFile();

  while (true) {
    std::string userInput;
    std::cout << "what do u want to print or (e) for exit";
    std::cin >> userInput;
    if (userInput == "e") {
      break;
    }
    PC.printData(userInput);
  }
  return 0;
}
//
