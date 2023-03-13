#include <string>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <tuple>

bool checkA(int a);

// calculation of the multiplicative inverse
//based on the pseudocode from Nechvatal, J.: PUBLIC-KEY CRYPTOGRAPHY
int multInv(int a);

bool checkText(std::string text);

std::pair<int, int> calculateKeys(int max1, int max2, int max3);

std::string encrypt(std::string text, int a, int b);

std::string decrypt(std::string text, int a, int b);

std::string decryptWithoutKey(std::string text);