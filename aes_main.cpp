/*
aes操作对象主要是字节，getBytesfromcases从样例形式转化为字节形式的string
*/
#pragma once
#include "my_aes.h"
#define CASE_LEN 4
#ifndef string
#include<string>
#endif
bool isEqual(byte* str1, byte* str2) {
	for (int i = 0; i < 16; i++) {
		if (str1[i] != str2[i])
			return false;
	}
	return true;
}
void print_String(string str) {
	char ch, ch1;
	for (int i = 0; i < str.length(); i++) {
		ch = (str[i] >> 4 & 0xf);
		ch1 = str[i] & 0x0f;
		cout << "0x" << hex << int(ch) << hex << int(ch1) << ',';
	}
	cout << endl;
}
void pring_Bytes(byte* key, bool flag = 1) {
	for (int i = 0; i < 16; ++i)
	{
		cout << hex << key[i].to_ulong() << " ";
		if (flag) {
			if ((i + 1) % 4 == 0)
				cout << endl;
		}
	}
	cout << endl;
}
string  getBytesfromcases(string txt);
//从string里取出各个char，赋值给byte数据类型
void convert_type(string str, byte* mm, bool flag = 1);
static const struct aes_test_case {
	int num, mode; // mode 1 = encrypt
	string txt, key, out;//16
} cases[] = {
	{ 1, 1,    "0001, 0001, 01a1, 98af, da78, 1734, 8615, 3566",
			   "0001, 2001, 7101, 98ae, da79, 1714, 6015, 3594",
			   "6cdd, 596b, 8f56, 42cb, d23b, 4798, 1a65, 422a"
	},
	{ 2, 1,   "3243, f6a8, 885a, 308d, 3131, 98a2, e037, 0734",
			  "2b7e, 1516, 28ae, d2a6, abf7, 1588, 09cf, 4f3c",
			  "3925, 841d, 02dc, 09fb, dc11, 8597, 196a, 0b32"
	},

	{ 3, 0,     "6cdd, 596b, 8f56, 42cb, d23b, 4798, 1a65, 422a",
				"0001, 2001, 7101, 98ae, da79, 1714, 6015, 3594",
				"0001, 0001, 01a1, 98af, da78, 1734, 8615, 3566"
	},
	{ 4, 0,     "3925, 841d, 02dc, 09fb, dc11, 8597, 196a, 0b32",
				"2b7e, 1516, 28ae, d2a6, abf7, 1588, 09cf, 4f3c",
				"3243, f6a8, 885a, 308d, 3131, 98a2, e037, 0734"}
};
void test_grade() {
	int sum = 0;
	string txt, out, key0;

	for (int i = 0; i < CASE_LEN; i++)
	{
		byte plain[16], key[16], cipher[16];
		if (cases[i].mode == 1) {
			//兼容函数
			txt = getBytesfromcases(cases[i].txt);
			out = getBytesfromcases(cases[i].out);
			key0 = getBytesfromcases(cases[i].key);
			/*if (i == 0)
				print_String(key0);
			*/
			convert_type(txt, plain);
			convert_type(out, cipher);
			convert_type(key0, key, 0);

			word w[4 * (Nr + 1)];
			KeyExpansion(key, w);
			encrypt(plain, w);
			if (isEqual(plain, cipher)) {
				sum++;
			}
			else
				cout << "Error occured in Encrypt\t" << cases[i].num << "\t testing case" << endl;
		}
		else
		{

			//兼容函数
			txt = getBytesfromcases(cases[i].txt);
			out = getBytesfromcases(cases[i].out);
			key0 = getBytesfromcases(cases[i].key);
			convert_type(txt, cipher);
			convert_type(out, plain);
			convert_type(key0, key, 0);
			word w[4 * (Nr + 1)];
			KeyExpansion(key, w);
			decrypt(cipher, w);
			if (isEqual(cipher, plain))
				sum++;
			else
				cout << "Error occured in Decrypt\t" << cases[i].num << "\t testing case" << endl;
		}
	}
	if (sum != 4) {
		cout << "Testing grade is:" << sum << "/4" << endl;
	}
	else
		cout << "All the test passed!" << endl;
}
//key正常赋值，而plain和cipher要转置,此时flag==1
void convert_type(string str, byte* mm, bool flag) {
	if (flag) {
		for (int j = 0; j < 4; j++)
			for (int i = 0; i < 4; i++)
				mm[4 * j + i] = str[4 * i + j];//转置

	}
	else {
		for (int i = 0; i < 16; i++)
			mm[i] = str[i];
	}

};

//输入2byte的string，输出char
char getCharfromHex_0x(string str_0x) {
	//大写转小写，未实现
	int result = 0;
	for (int i = 0; i < 2; i++) {
		if (str_0x[i] <= '9' && str_0x[i] >= '0') {
			if (i == 0)
				result += (str_0x[i] - '0') * 16;
			else
				result += (str_0x[i] - '0');
		}
		else if (str_0x[i] <= 'f' && str_0x[i] >= 'a') {
			if (i == 0)
				result += ((str_0x[i] - 'a') + 10) * 16;
			else
				result += (str_0x[i] - 'a') + 10;
		}
	}
	return char(result);
}//checked

//输入形式是去除了0x的16进制，输出byte构成的string
string  getBytesfromcases(string txt) {
	//txt.erase(std::remove(txt.begin(), txt.end(), ','), txt.end());//去除,号，用' '代替
	string mid = "";
	string result = "";
	for (int i = 0; i < txt.length(); i++) {
		if ((txt[i] >= '0' && txt[i] <= '9') || (txt[i] <= 'z' && txt[i] >= 'a'))
			result += txt[i];
		else
			continue;
	}
	txt = result;
	result = "";
	for (int i = 0; i < txt.length(); i += 2) {
		mid += txt[i];
		mid += txt[i + 1];
		result += getCharfromHex_0x(mid);
		mid = "";
	}
	return result;
};
int main()

{
	test_grade();
	return 0;
}
