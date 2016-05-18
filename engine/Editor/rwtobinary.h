#pragma once

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <sstream>

#define BLOCKS 64

struct Data
{
	int age;
	std::string name;
	float randomNumber;
};

class Student
{
public:
	Student();
	
	void saveToBinary(std::string fileName);
	void loadFromBinary(std::string fileName);
};