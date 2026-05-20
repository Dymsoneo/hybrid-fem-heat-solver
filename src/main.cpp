#include <iostream>

#include "io/SerialReader.h"


int main()
{
	SerialReader reader;

	reader.open("COM7", 9600);

	while (true) 
	{
		double temperature = reader.readTemperature();

		std::cout << "Furnace Temperature: " << temperature << "°C" << std::endl;
	}

    return 0;
}