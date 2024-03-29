#include <ModbusMaster.h>

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

const int defaultBaudRate = 115200;
int timerTask1, timerTask2, timerTask3;
float battChargeCurrent, battDischargeCurrent, battOverallCurrent, battChargePower;
float bvoltage, ctemp, btemp, bremaining, lpower, lcurrent, pvvoltage, pvcurrent, pvpower;
float stats_today_pv_volt_min, stats_today_pv_volt_max;
uint8_t result;
bool rs485DataReceived = true;
bool loadPoweredOn = true;


ModbusMaster node;

void preTransmission() {
}

void postTransmission() {
}

// A list of the regisities to query in order
typedef void (*RegistryList[])();

RegistryList Registries = {
  AddressRegistry_3100,
  AddressRegistry_3106,
  AddressRegistry_310D,
  AddressRegistry_311A,
  AddressRegistry_331B,
};

// keep log of where we are
uint8_t currentRegistryNumber = 0;

// function to switch to next registry
void nextRegistryNumber() {
  // better not use modulo, because after overlow it will start reading in incorrect order
  currentRegistryNumber++;
  if (currentRegistryNumber >= ARRAY_SIZE(Registries)) {
    currentRegistryNumber = 0;
  }
}

// exec a function of registry read (cycles between different addresses)
void executeCurrentRegistryFunction() {
  Registries[currentRegistryNumber]();
}

void AddressRegistry_3100() {
  result = node.readInputRegisters(0x3100, 6);

  if (result == node.ku8MBSuccess) {

    pvvoltage = node.getResponseBuffer(0x00) / 100.0f;
    Serial.print("PV Voltage: ");
    Serial.println(pvvoltage);

    pvcurrent = node.getResponseBuffer(0x01) / 100.0f;
    Serial.print("PV Current: ");
    Serial.println(pvcurrent);

    pvpower = (node.getResponseBuffer(0x02) | node.getResponseBuffer(0x03) << 16) / 100.0f;
    Serial.print("PV Power: ");
    Serial.println(pvpower);

    bvoltage = node.getResponseBuffer(0x04) / 100.0f;
    Serial.print("Battery Voltage: ");
    Serial.println(bvoltage);

    battChargeCurrent = node.getResponseBuffer(0x05) / 100.0f;
    Serial.print("Battery Charge Current: ");
    Serial.println(battChargeCurrent);
  }
}

void AddressRegistry_3106() {
  result = node.readInputRegisters(0x3106, 2);

  if (result == node.ku8MBSuccess) {
    battChargePower = (node.getResponseBuffer(0x00) | node.getResponseBuffer(0x01) << 16) / 100.0f;
    Serial.print("Battery Charge Power: ");
    Serial.println(battChargePower);
  }
}

void AddressRegistry_310D() {
  result = node.readInputRegisters(0x310D, 3);

  if (result == node.ku8MBSuccess) {
    lcurrent = node.getResponseBuffer(0x00) / 100.0f;
    Serial.print("Load Current: ");
    Serial.println(lcurrent);

    lpower = (node.getResponseBuffer(0x01) | node.getResponseBuffer(0x02) << 16) / 100.0f;
    Serial.print("Load Power: ");
    Serial.println(lpower);
  } else {
    rs485DataReceived = false;
    Serial.println("Read register 0x310D failed!");
  }
}

void AddressRegistry_311A() {
  result = node.readInputRegisters(0x311A, 2);

  if (result == node.ku8MBSuccess) {
    bremaining = node.getResponseBuffer(0x00) / 1.0f;
    Serial.print("Battery Remaining %: ");
    Serial.println(bremaining);

    btemp = node.getResponseBuffer(0x01) / 100.0f;
    Serial.print("Battery Temperature: ");
    Serial.println(btemp);
  } else {
    rs485DataReceived = false;
    Serial.println("Read register 0x311A failed!");
  }
}

void AddressRegistry_331B() {
  result = node.readInputRegisters(0x331B, 2);

  if (result == node.ku8MBSuccess) {
    battOverallCurrent = (node.getResponseBuffer(0x00) | node.getResponseBuffer(0x01) << 16) / 100.0f;
    Serial.print("Battery Discharge Current: ");
    Serial.println(battOverallCurrent);
  } else {
    rs485DataReceived = false;
    Serial.println("Read register 0x331B failed!");
  }
}


#define p_ledtick LED_BUILTIN
int ledState = LOW;
unsigned long previousMillis = 0;

void setup() {
  Serial.begin(defaultBaudRate);
  Serial1.begin(defaultBaudRate, SERIAL_8N1, 4, 5);  //rx 4, tx 5

  // Modbus slave ID 1
  node.begin(1, Serial1);

  // callbacks to toggle DE + RE on MAX485
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);

  pinMode(p_ledtick, OUTPUT);

  delay(2000);
  Serial.println("Setup OK!");
  Serial.println("----------------------------");
  Serial.println();
}

void loop() {
  unsigned long currentMillis = millis();
  const long interval = 5000;
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }

    digitalWrite(p_ledtick, ledState);

    executeCurrentRegistryFunction();
    nextRegistryNumber();
  }
}
