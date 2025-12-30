#include <iostream>
#include "TemperatureSensor.h"
int main()
{
	TemperatureSensor newSensor(1,45.0f);
	std::cout << "Device ID: " << newSensor.getDeviceId() << std::endl;
    std::cout<<"Temperature"<<newSensor.getData()<<std::endl;
	newSensor.setData(25.0f);
	std::cout<<"Updated Temperature"<<newSensor.getData()<<std::endl;
	return 0;
}