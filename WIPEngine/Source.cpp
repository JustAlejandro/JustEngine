#include "IvMatrix33.h"
#include <iostream>
int main() {
	IvMatrix33 test,test2;
	for (int i = 0; i < 9; i++)
	{
		test.mV[i] = i;
		test2.mV[i] = i;
	}
	test = (test * test2);

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			std::cout << test(j, i) << " ";
		}
		std::cout << std::endl;
	}
	std::system("pause");
}