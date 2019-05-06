// Robert Phu 1091357
// Simulation of a multi-cycle un-pipelined MIPS processor
// compiles on linux with g++ -std=c++11 main.cpp


#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
using namespace std;

void stringsplit(string input, string& output1, string& output2);

int main()
{
	
	ifstream infile;
	string infilename;
	string temp;
	int flag = 0;
	string temp1;
	string temp2;
	//Allocate array for $R0-$R31 and memory from 0-996
	int REGISTERS[32]  = { 0 };
	int MEMORY[250] = { 0 };
	int clock_cycle = 0;
	int instruction_count = 1;
	string CODE_STRING[250];
	int index_code = 0;
	int program_counter = 0;
	string current_instruction;
	int instructiondecode;
	int instdecode_num;
	int instruction_type = 0;
	int rd, rs, rt, funct,imm,base;
	int temporary_storage;
	bool stringmatch;


	//Ask user for input file
	cout << "Please enter input file name.\n";
	getline(cin, infilename);
	//infilename = "./input";
	infile.open(infilename);
	ofstream outfile;
	outfile.open("output");

	//code to parse input file
	if (infile.is_open())
	{
		while (getline(infile, temp))
		{
			//Code to parse Registers/Memory/Code from file.
			cout << temp << endl;
			//Register Values
			if ((flag == 1) && (temp.compare("MEMORY") != 1)) {
				//cout << "REGISTER FOUND ";
				//Split line into two seperate strings
				stringsplit(temp, temp1, temp2);			
				//Delete R from string to use as index
				temp1.erase(0, 1);				
				int register_index = stoi(temp1, nullptr, 0);				
				int register_value = stoi(temp2, nullptr, 0);
				REGISTERS[register_index] = register_value;
				//cout << "REGISTER R" << register_index << " has " << REGISTERS[register_index] << endl;
			}
			// Memory Values
			else if ((flag == 2) && (temp.compare("CODE") != 1)) {
				//Split line into two seperate strings
				stringsplit(temp, temp1, temp2);
				int mem_location = stoi(temp1, nullptr, 0) / 4;
				int mem_value = stoi(temp2, nullptr, 0);
				MEMORY[mem_location] = mem_value;
				//cout << "MEMORY " << mem_location << " has " << MEMORY[mem_location] << endl;
			}

			else if ((flag == 3)) {
				//cout << "CODE FOUND ";
				cout << temp << '\t';
				//Load all code into array of strings
				CODE_STRING[index_code] = temp;
				index_code++;
				cout << endl;
			}
			else if (temp.compare("REGISTERS") == 1) flag = 1;
			else if (temp.compare("MEMORY") == 1) flag = 2;
			else if (temp.compare("CODE") == 1) flag = 3;
			
		}
		infile.close();
	}
	else cout << "Error. Input file named " << infilename << "not found.";

	//run until no more code found
	while (CODE_STRING[program_counter] != "") {

		//INSTRUCTION FETCH ////////////////////////////////////////////////////////////////////////////////

		clock_cycle++;
		//load first line of code using program counter
		current_instruction = CODE_STRING[program_counter];
		//increment program counter
		program_counter++;
		//checks if current line is an instruction

		outfile << "C#" << clock_cycle << " I" << instruction_count << "-IF" << endl;
		//INSTRUCTION DECODE + REGISTER READ//////////////////////////////////////////////////////////////////

		//check opcode for instruction type
		instructiondecode = stoi(current_instruction.substr(0, 6), nullptr, 2);
		switch (instructiondecode) {

			//R-Type ADD/SUB/SLT
		case 0:
			instruction_type = 1;
			//store registers to be used
			rs = stoi(current_instruction.substr(6, 5), nullptr, 2);
			rt = stoi(current_instruction.substr(11, 5), nullptr, 2);
			rd = stoi(current_instruction.substr(16, 5), nullptr, 2);
			funct = stoi(current_instruction.substr(26, 6));
			clock_cycle++;
			outfile << "C#" << clock_cycle << " I" << instruction_count << "-ID" << endl;
			break;

			//I-Type ADDI
		case 8:
			instruction_type = 2;
			rs = stoi(current_instruction.substr(6, 5), nullptr, 2);
			rt = stoi(current_instruction.substr(11, 5), nullptr, 2);
			imm = stoi(current_instruction.substr(16, 16), nullptr, 2);
			cout << current_instruction.substr(16, 16);
			clock_cycle++;
			outfile << "C#" << clock_cycle << " I" << instruction_count << "-ID" << endl;
			break;

			//I-Type BEQ
		case 4:
			instruction_type = 3;
			rs = stoi(current_instruction.substr(6, 5), nullptr, 2);
			rt = stoi(current_instruction.substr(11, 5), nullptr, 2);
			imm = stoi(current_instruction.substr(16, 16), nullptr, 2);
			clock_cycle++;
			outfile << "C#" << clock_cycle << " I" << instruction_count << "-ID" << endl;
			break;

			//I-Type BNE
		case 5:
			instruction_type = 4;
			rs = stoi(current_instruction.substr(6, 5), nullptr, 2);
			rt = stoi(current_instruction.substr(11, 5), nullptr, 2);
			imm = stoi(current_instruction.substr(16, 16), nullptr, 2);
			clock_cycle++;
			outfile << "C#" << clock_cycle << " I" << instruction_count << "-ID" << endl;
			break;

			//I-Type LW
		case 35:
			instruction_type = 5;
			base = stoi(current_instruction.substr(6, 5), nullptr, 2);
			rt = stoi(current_instruction.substr(11, 5), nullptr, 2);
			imm = stoi(current_instruction.substr(16, 16), nullptr, 2); //offset
			clock_cycle++;
			outfile << "C#" << clock_cycle << " I" << instruction_count << "-ID" << endl;
			break;

			//I-Type SW
		case 53:
			instruction_type = 6;
			base = stoi(current_instruction.substr(6, 5), nullptr, 2);
			rt = stoi(current_instruction.substr(11, 5), nullptr, 2);
			imm = stoi(current_instruction.substr(16, 16), nullptr, 2); //offset
			clock_cycle++;
			outfile << "C#" << clock_cycle << " I" << instruction_count << "-ID" << endl;
		}


		//EXECUTION OR CALCULATE ADDRESS////////////////////////////////////////////////////
		switch (instruction_type) {

			//R-Type ADD/SUB
		case 1:
			//IF ADD
			if (funct == 100000) {
				temporary_storage = REGISTERS[rs] + REGISTERS[rt];
			}
			//IF SUB
			else if (funct == 100010) {
				temporary_storage = REGISTERS[rs] - REGISTERS[rt];
			}
			//IF SLT
			else if (funct == 101010) {
				if (REGISTERS[rs] < REGISTERS[rt]) {
					temporary_storage = 1;
				}
				else temporary_storage = 0;
			}
			clock_cycle++;
			outfile << "C#" << clock_cycle << " I" << instruction_count << "-EX" << endl;
			break;

			//I-Type ADDI
		case 2:
			temporary_storage = REGISTERS[rs] + imm;
			clock_cycle++;
			outfile << "C#" << clock_cycle << " I" << instruction_count << "-EX" << endl;
			break;

			//I-Type BEQ
		case 3:
			if (REGISTERS[rs] == REGISTERS[rt]) {
				program_counter += imm;
			}
			clock_cycle++;
			outfile << "C#" << clock_cycle << " I" << instruction_count << "-EX" << endl;
			instruction_count++;
			break;

			//I-Type BNE
		case 4:
			if (REGISTERS[rs] != REGISTERS[rt]) {
				program_counter += imm;
			}
			clock_cycle++;
			outfile << "C#" << clock_cycle << " I" << instruction_count << "-EX" << endl;
			instruction_count++;
			break;
		
			//I-Type LW
		case 5:
			temporary_storage = (imm / 4) + (base / 4);
			clock_cycle++;
			outfile << "C#" << clock_cycle << " I" << instruction_count << "-EX" << endl;
			break;

			//I-Type SW
		case 6:
			temporary_storage = (imm / 4) + (base / 4);
			clock_cycle++;
			outfile << "C#" << clock_cycle << " I" << instruction_count << "-EX" << endl;
			break;
		}

		//MEMORY LOAD////////////////////////////////////////////////////////////////////////
		switch (instruction_type) {

		default:
			break;

			//I-Type LW
		case 5:
			temporary_storage = MEMORY[temporary_storage];
			clock_cycle++;
			outfile << "C#" << clock_cycle << " I" << instruction_count << "-MEM" << endl;
			break;

			//I-Type SW
		case 6:
			MEMORY[temporary_storage] = REGISTERS[rt];
			clock_cycle++;
			outfile << "C#" << clock_cycle << " I" << instruction_count << "-MEM" << endl;

			break;
		}
	

		//WRITE BACK TO REGISTER////////////////////////////////////////////////////////////
		switch (instruction_type) {

		default:
			break;

			//R-Type ADD/SUB/SLT
		case 1:
			//IF ADD
			REGISTERS[rd] = temporary_storage;
			clock_cycle++;
			outfile << "C#" << clock_cycle << " I" << instruction_count << "-WB" << endl;
			instruction_count++;
			break;

		case 2:
			//ADDI
			REGISTERS[rt] = temporary_storage;
			clock_cycle++;
			outfile << "C#" << clock_cycle << " I" << instruction_count << "-WB" << endl;
			instruction_count++;
			break;
		
		case 5: 
			//I-Type LW
			REGISTERS[rt] = temporary_storage;
			clock_cycle++;
			outfile << "C#" << clock_cycle << " I" << instruction_count << "-WB" << endl;
			instruction_count++;
			break;
		}

	}

	outfile << endl << endl << "REGISTERS" << endl;

	//output non empty registers and memory

	for (int x = 0; x < 32; x++) {
		if (REGISTERS[x] != 0) {
			outfile << 'R' << x << ' ' << REGISTERS[x] << endl;
		}
	}


	outfile << endl << endl << "MEMORY" << endl;
	for (int x = 0; x < 250; x++) {
		if (MEMORY[x] != 0) {
			outfile << x*4 << ' ' << REGISTERS[x] << endl;
		}
	}
	
	outfile.close();


	//string test;
	//getline(cin, test);

    //std::cout << test; 	
}

void stringsplit(string input, string& output1, string& output2){
	//simple function to split string in half with space delimiter
	int position = 0;
	string delimiter = " ";
	string temp;

	while ((position = input.find(delimiter)) != string::npos) {
		output1 = input.substr(0, position);
		input.erase(0, position + delimiter.length());    
		output2 = input;
	}
}
