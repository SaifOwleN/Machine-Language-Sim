#include <bitset>
#include <cctype>
#include <deque>
#include <fstream>
#include <iostream>
#include <iterator>
#include <ostream>
#include <string>
#include <vector>

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
    memory.assign(size, "");
  }
  void store(int address, std::string value) { memory[address] = value; }
  std::string read(int address) { return memory[address]; }
};

class Instructions {
public:
  void execute(std::string s, Registers &cpuRegister, MainMemory &ram,
               int &programC) {

    char instruction = s[2];
    int reg = s[6] - '0';
    std::string memoryCellHex = s.substr(10);
    int memoryCell = std::stoi(memoryCellHex, nullptr, 16);
    std::string value;

    switch (instruction) {
    case '1':
      value = ram.read(memoryCell);
      cpuRegister.store(reg, value);
      std::cout << "case1";
      break;

    case '2':
      cpuRegister.store(reg, std::to_string(memoryCell));
      std::cout << "case2";
      break;

    case '3':
      std::cout << "case3";

      value = cpuRegister.read(reg);
      if (memoryCellHex == "00") {
        std::cout << "value" << value;
        break;
      } else {
        std::cout << "xdddddMOTS";
        ram.store(memoryCell, value);
        break;
      }
    case '4': {
      std::cout << "case4";

      int regX = std::stoi(std::string(1, memoryCellHex[0]), nullptr, 16);
      int regY = std::stoi(std::string(1, memoryCellHex[1]), nullptr, 16);
      value = cpuRegister.read(regX);
      cpuRegister.store(regY, value);
      break;
    }

    case '5': {
      int regX = std::stoi(std::string(1, memoryCellHex[0]), nullptr, 16);
      int regY = std::stoi(std::string(1, memoryCellHex[1]), nullptr, 16);
      int valueInRegR = cpuRegister.read(regX) + cpuRegister.read(regY);
      int regR = reg;
      cpuRegister.store(regR, valueInRegR) break;
    }
    case 'B': {
      std::cout << "PC: " << typeid(programC).name() << " "
                << "memoryCell: " << typeid(memoryCell).name();
      if (cpuRegister.read(reg) == cpuRegister.read(0)) {
        programC = memoryCell;
      }
      value = cpuRegister.read(reg);
      break;
    }
    case 'C': {
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
  Instructions xdd;
  void fetch(MainMemory &r) {
    instructionRegister =
        r.read(programCounter) + " " + r.read(programCounter + 1);
  };

public:
  CPU() : programCounter(0), instructionRegister("") {}

  void runProgram(MainMemory &ram, Registers &cpuRegister) {
    for (int i = 0; programCounter < 1000; i++) {
      std::cout << std::endl << programCounter << "||" << std::endl;
      fetch(ram);
      xdd.execute(instructionRegister, cpuRegister, ram, programCounter);
      programCounter += 2;
    }
  }
  std::string getIR() { return instructionRegister; }
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
      std::string firstPart = str.substr(0, 7);
      std::string secondPart = str.substr(8);
      linesAfter.push_back(firstPart);
      linesAfter.push_back(secondPart);
    };
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
    } else {
      std::ofstream output("Registers.txt");
      for (int i = 0; i < 16; i++) {
        output << cpuRegister.read(i) << std::endl;
      }
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
