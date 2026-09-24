#define ENCODEROUTPUT 540  // Pulses per rotation
#define PWM 9              // L298N PWM connected to pin 9
#define encoder0pinA 3     // Hall sensor A connected to pin 3 (external interrupt)

#include <mcp_can.h>

unsigned char buf[8];
const int SPI_CS_PIN = 10;
MCP_CAN CAN(SPI_CS_PIN);  // CS pin for CAN module

// Driving one-way motor encoder
int M_left = 4;  // Direction IN1 (L298N)
int E_left = 5;  // Direction IN2 (L298N)
float rpm = 0.0f;
volatile unsigned long encoderValue = 0;  // Pulse count
unsigned long lastmillis = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(M_left, OUTPUT);
  pinMode(E_left, OUTPUT);
  pinMode(encoder0pinA, INPUT_PULLUP);
  pinMode(PWM, OUTPUT);
  digitalWrite(M_left, LOW);
  digitalWrite(E_left, HIGH);
  attachInterrupt(digitalPinToInterrupt(encoder0pinA), updateEncoder, FALLING);

  while (CAN_OK != CAN.begin(CAN_500KBPS))
  {
    Serial.println("CAN BUS Shield init fail");
    Serial.println(" Init CAN BUS Shield again");
    delay(100);
  }
  Serial.println("CAN BUS Shield init ok!");
}

void loop()
{
  unsigned char len = 0;
  float v;
  float pi = 3.14159265;

  if (CAN_MSGAVAIL == CAN.checkReceive())  // If something received
  {
    CAN.readMsgBuf(&len, buf);
    unsigned long canId = CAN.getCanId();
    Serial.println("--------------------------------");
    int value = (int)buf[7];

    if (value >= 12 && value <= 25) {
      int val_output = map(value, 12, 25, 0, 255);

      // Start a fresh one-second RPM measurement at this PWM command.
      detachInterrupt(digitalPinToInterrupt(encoder0pinA));
      encoderValue = 0;
      attachInterrupt(digitalPinToInterrupt(encoder0pinA), updateEncoder, FALLING);
      analogWrite(PWM, val_output);
      lastmillis = millis();
      while (millis() - lastmillis < 1000UL) { }

      detachInterrupt(digitalPinToInterrupt(encoder0pinA));
      rpm = encoderValue * 60.0f / ENCODEROUTPUT;
      Serial.print("rpm =");
      Serial.println(rpm);
      Serial.print("Linear Velocity (V)=");
      Serial.print(v = ((2 * pi / 60) * rpm * 0.03));
      Serial.println("m/s");
      Serial.print("PWM =");
      Serial.println(val_output);
      Serial.print("dis =");
      Serial.println(value);
      encoderValue = 0;
      lastmillis = millis();
      attachInterrupt(digitalPinToInterrupt(encoder0pinA), updateEncoder, FALLING);
    }
    else Stop();
  }
}

void updateEncoder()
{
  encoderValue++;  // Count each pulse detected on Hall sensor A
}

void Stop()  // Motor stops
{
  digitalWrite(PWM, LOW);
}
