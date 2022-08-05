#pragma once
#include <iostream>
#include <vector>

struct MyVector {
	char** data;
	int size;
};

bool findAllInternal(const char* pattern, char* anyStr, MyVector**& result, int& count);
void releaseMatch(MyVector** result, int count);
std::vector<std::vector<std::string>> inline findAll(std::string pattern, std::string anyStr) {
	std::vector <std::vector<std::string>> out;
	
	MyVector** result;
	int count;
	if (!findAllInternal(pattern.c_str(), (char*)anyStr.c_str(), result, count)) {
		return out;
	}
	for (int i = 0; i < count; i++) {
		std::vector<std::string> sub;
		for (int j = 0; j < result[i]->size; j++) {
			sub.push_back(result[i]->data[j]);
		}
		out.push_back(sub);
	}
	releaseMatch(result, count);

	return out;
}





