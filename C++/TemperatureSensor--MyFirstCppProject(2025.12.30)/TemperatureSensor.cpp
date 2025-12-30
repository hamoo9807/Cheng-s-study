#include "TemperatureSensor.h"
#include <iostream>
bool TemperatureSensor::isTemp(float temp)
{
	return temp >= MinTem && temp <= MaxTem;
}
TemperatureSensor::TemperatureSensor(int id, float initTemp)
{
	if (isTemp(initTemp))
	{
		temperature = initTemp;
	}
	else
	{
		temperature = 0.0f;
		std::cout << "警告！设备：" << deviceid << "温度超出合法范围，已修正为0度" << std::endl;
	}
}
bool TemperatureSensor::setData(float temp)
{
	if (isTemp(temp))
	{
		temperature = temp;
		return true;
	}
	else
	{
		std::cout << "错误！设备：" << deviceid << "温度;" << temp << "度已超出量程" << MinTem << "--" << MaxTem << "度" << std::endl;
		return false;
	}
}
float TemperatureSensor::getData() const
{
	return temperature;
}
int TemperatureSensor::getDeviceId() const
{
	return deviceid;
}