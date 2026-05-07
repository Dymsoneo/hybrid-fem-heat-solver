#pragma once

#include <string>
#include <vector>

// Class to represent the Heat Transfer Coefficient (HTC) model. HTC can be constant or temperature-dependent based on a table. 
// Temperature-dependent HTC is evaluated using linear interpolation.

class HTCModel
{
public:
	HTCModel() = default;

	HTCModel(double constantHTC);

	HTCModel(const std::vector<double>& surfaceTemperatures, const std::vector<double>& htcValues);

	HTCModel(const std::string& filePath);


	double getHTC(double surfaceTemperature) const;

private:

	bool isConstant{ true };
	double constantHTC{ 0.0 };

	std::vector<double> surfaceTemperatures;
	std::vector<double> htcValues;

	double interpolate(double surfaceTemperature) const;

	void loadFromCSV(const std::string& filePath);

	void validateInput() const;
};