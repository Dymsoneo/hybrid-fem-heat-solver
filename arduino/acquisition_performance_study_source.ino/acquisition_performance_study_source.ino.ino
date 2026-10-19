
#include <max6675.h>
#include <math.h>

constexpr int THERMO_SCK = 6;
constexpr int THERMO_CS = 5;
constexpr int THERMO_SO = 4;

constexpr unsigned long SAMPLE_INTERVAL_MS = 500;

MAX6675 thermocouple(
  THERMO_SCK,
  THERMO_CS,
  THERMO_SO
);

unsigned long sampleId = 0;
unsigned long nextSampleTime = 0;

void setup()
{
  Serial.begin(9600);

  delay(500);
  nextSampleTime = millis();
}

void loop()
{
  unsigned long now = millis();

  if ((long)(now - nextSampleTime) >= 0)
  {
      
    unsigned long timestamp = millis();
    double temperature = thermocouple.readCelsius();

    Serial.print(sampleId);
    Serial.print(',');
    Serial.print(timestamp);
    Serial.print(',');
    Serial.println(temperature, 2);

    sampleId++;
    nextSampleTime += SAMPLE_INTERVAL_MS;
  }
}
