#ifndef TEMPERATURE_SENSOR_H
#define TEMPERATURE_SENSOR_H
class TemperatureSensor {
private:
	float temperature;
	int deviceid;
	const float MaxTem = 40.0f;
	const float MinTem = -40.0f;
public:
	TemperatureSensor(int id, float initTemp);
	bool setData(float temp);
	float getData() const;
	int getDeviceId() const;
	bool isTemp(float temp);
};
#endif