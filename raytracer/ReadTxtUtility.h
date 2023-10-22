#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "World.h"

class ReadTxtUtility 
{
	public:
		ReadTxtUtility();
		~ReadTxtUtility();
		void ReadAndBuild(const std::string& file, World& world) const;
};