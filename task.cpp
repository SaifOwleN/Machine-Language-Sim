// #include<bits/stdc++.h>
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
  int decimalValue = std::stoi(hexString, nullptr, 16);

  std::bitset<8> binaryValue(decimalValue);

  return binaryValue.to_string();
}

std::string hexToBinary4(const std::string &hexString) {
  int decimalValue = std::stoi(hexString, nullptr, 16);

  std::bitset<4> binaryValue(decimalValue);

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

    sum[i] = bitA ^ bitB ^ carry;

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
  void store(int address, std::string value)  {
    memory[address] = value;
  }
  std::string read(int address)  { return memory[address]; }
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
  void store(int address, std::string value)  {
    memory[address] = value;
  }
  std::string read(int address)  { return memory[address]; }
};

class Instructions {
public:
  void execute(std::string s, Registers &cpuRegister, MainMemory &ram,
               int &programC, std::vector<std::string> &screen,char &stop) {
    std::string instructionBin = s.substr(0, 4);
    std::string registerBin = s.substr(4, 4);
    std::string memoryCellBin = s.substr(9);

    int instruction = binaryToInt4(instructionBin);

    int reg = binaryToInt4(registerBin);

    int memoryCell = binaryToInt(memoryCellBin);
    std::string value;

    switch (instruction) {
    case 1:

      value = ram.read(memoryCell);
      cpuRegister.store(reg, value);
      break;

    case 2:

      cpuRegister.store(reg, memoryCellBin);
      break;

    case 3:

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
      cpuRegister.store(regY, value);
      break;
    }
    case 5: {
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
        programC = memoryCell - 2;
        break;
      }
      value = cpuRegister.read(reg);
      break;
    }
    case 12: {
      stop ='y';
      break;
    }
    }
  }
};

class CPU {
private:
  int programCounter;
  std::string instructionRegister;
  char stop;

  std::vector<std::string> screen;
  Instructions xdd;
  void fetch(MainMemory &r) {
    instructionRegister =
        r.read(programCounter) + " " + r.read(programCounter + 1);
  };


public:
  CPU() : programCounter(0), instructionRegister(""),stop('n') {}

  void runProgram(MainMemory &ram, Registers &cpuRegister) {
    for (int i = 0; programCounter < 1000; i++) {
      if (programCounter > 254) {
        break;
      }
      fetch(ram);
      xdd.execute(instructionRegister, cpuRegister, ram, programCounter,
                  screen,stop);
      if (stop=='y')
      {
        break;
      }
      
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
    std::string file1;
    std::cout << "1- load a program" << std::endl;
    std::cout << " " << std::endl;
    std::cin >> file1;
    std::ifstream file(file1 + ".txt");
    int i = 0;
    std::string line;
    std::vector<std::string> linesPre;
    if (!file.is_open()) {
        std::cerr << "Error opening the file!" << std::endl;
        exit(EXIT_FAILURE);
    }
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
    char b;
    std::cout << "Run the Program (y,n)? ";
    std::cin >> b;
    if (b=='y')
    {
      main.runProgram(ram,cpuRegister);
    }else{
      exit(EXIT_SUCCESS);
    }
    
  };


  void printData(std::string userInput) {
    if (userInput == "1") {
      std::ofstream output("ram.txt");
      for (int i = 0; i < 256; i++) {
        output << ram.read(i) << std::endl;
      }
    }else if (userInput == "2") {
      std::ofstream output("Registers.txt");
      for (int i = 0; i < 16; i++) {
        output << cpuRegister.read(i) << std::endl;
      }
    } else if (userInput == "3") {
      std::cout << "Program Counter Stopped at: " << main.getPC() << std::endl;
    }else if (userInput == "4") {
      std::cout <<"Instruction Register's last value was: " << main.getIR() << std::endl;
    }  else if (userInput == "5") {
      main.getScreen();
    }
  };
};

int main() {
  Machine PC;
  PC.loadFile();

  while (true) {
    std::string userInput;
    std::cout << "What do you want to do: " << std::endl << "1-Memory " << std::endl << "2-Registers" << std::endl << "3-PC" <<std::endl << "4-IR" << std::endl << "5-screen" << std::endl << "e-Exit" << std::endl;
    std::cin >> userInput;
    if (userInput == "e") {
      exit(EXIT_SUCCESS);
    }
    PC.printData(userInput);
  }
  return 0;
}
//
