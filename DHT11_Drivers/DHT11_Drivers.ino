#define DHT11_PIN 2

uint8_t *Read_Sensor();

void setup()
{
  uint8_t* sensor_data;
  uint8_t Temperature,Humidity;
  char buff[200];

  // -- Initialize the pin of the sensor -- //
  pinMode(DHT11_PIN,INPUT);     // Need to configure the pin as INPUT_PULLUP if there is no external Pull-Up Resistor
  digitalWrite(DHT11_PIN,LOW);
  // -- Initialize the pin of the sensor -- //

  Serial.begin(9600);

  // -- main program -- //
  sensor_data = Read_Sensor();
  
  sprintf(
    buff,
    "x[0] = %d: (RH Integral)\nx[1] = %d: (RH Decimal)\nx[2] = %d: (T Integral)\nx[3] = %d: (T Decimal)\nx[4] = %d: (Checksum)",
    sensor_data[0],
    sensor_data[1],
    sensor_data[2],
    sensor_data[3],
    sensor_data[4]);
  Serial.println(buff);
  Temperature = sensor_data[2];
  Humidity = sensor_data[0];
  sprintf(buff,"Temperature: %d[C]\nHumidity: %d[precent]",Temperature,Humidity);
  Serial.println(buff);
  // -- main program -- //
}

void loop()
{
  while(1);
}

uint8_t *Read_Sensor()
{
  uint8_t *data_out = (uint8_t*)malloc(5*sizeof(uint8_t));

  for(int i=0;i<5;i++)
  {
    data_out[i] = 0;
  }
  
  // Send Start bit //
  pinMode(DHT11_PIN,OUTPUT);
  delay(18);  // 18 msec pulse LOW state
  // Start reading data from the sensor
  pinMode(DHT11_PIN,INPUT); // Need to configure the pin as INPUT_PULLUP if there is no external Pull-Up Resistor

  
  while(digitalRead(DHT11_PIN) == HIGH);  // wait for response from sensor.
  while(digitalRead(DHT11_PIN) == LOW);   // Start condition LOW
  while(digitalRead(DHT11_PIN) == HIGH); // Start Condition HIGH
  

  // Reading all 5 Bytes:
  for(int byte_counter = 0; byte_counter < 5; byte_counter++)
  {
    for(int bit_counter = 7; bit_counter >= 0; bit_counter--)
    {
      data_out[byte_counter] |= (pulseIn(DHT11_PIN,HIGH) < 50) ? (0 << bit_counter) : (1 << bit_counter);
    }
  }
  return(data_out);
}
