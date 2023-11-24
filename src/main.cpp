#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <DHT.h>
#include <ESP32PWM.h>
#include <ESP32Servo.h>

#define PIN_SERVO 18

DHT tempSensor(13, DHT11);
const int MQ2pin = 33;
const int MQ7pin = 36;

const int templedPin = 27;
const int humledPin = 25;
const int MQ2ledPin = 32;
const int MQ7ledPin = 3;

const int touchPin = 2;

float temperature = 0; 
float humidity = 0;
int MQ2data = 0;
int MQ7data = 0;

bool tempAlert = false;
bool humidityAlert = false;
bool MQ2Alert = false;
bool MQ7Alert = false;

bool MQ7History = false; 

Servo mg90s;

// put function definitions here:

// TEMP & HUMIDITY SENSOR
void task1(void *parameters){
  for(;;){
    temperature = tempSensor.readTemperature();
    humidity = tempSensor.readHumidity();

    if (temperature >= 23){
      tempAlert = true;
    }else if (temperature <= 23){
      tempAlert = false;
    }

    if (humidity >= 50){
      humidityAlert = true;
    }else if (humidity <= 50){
      humidityAlert = false;
    }

    Serial.print("Temperature:");
    Serial.print(temperature);
    Serial.print("C/Humidity: ");
    Serial.print(humidity);
    Serial.print("%\n");

    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}

//SERVO MOTOR
void task2(void *parameters){
  for(;;){
    if(MQ7Alert == true){
      mg90s.attach(PIN_SERVO);
      mg90s.write(180);
      mg90s.detach();
      Serial.println("SERVOIF");
    }
    //if (MQ7Alert == false && MQ7History == false){
      //mg90s.write(0);
    //}
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

// MQ2 GAS SENSOR
void task3(void *parameters){
  for(;;){
    // Reading ADC Values
    MQ2data = analogRead(MQ2pin);
    if (MQ2data >= 400){
      MQ2Alert = true;
    }
    if (MQ2data <= 400){
      MQ2Alert = false;
    }
    Serial.println("MQ2 Data: ");
    Serial.println(MQ2data);

    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}

// MQ7 GAS SENSOR
void task4(void *parameters){
  for(;;){
    // Reading ADC Values
    MQ7data = analogRead(MQ7pin);
    if (MQ7data >= 450){
      MQ7Alert = true;
    }
    else if (MQ7data <= 450){
      MQ7Alert = false;
    }

    Serial.println("MQ7 Data: ");
    Serial.println(MQ7data);

    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}

// MQ2 GAS SENSOR ALERT
void task5(void *parameters){
  for(;;){
    if (MQ2Alert == true){
      digitalWrite(MQ2ledPin, HIGH);
    }else if (MQ2Alert == false){
      digitalWrite(MQ2ledPin, LOW);
    }

    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

// MQ7 GAS SENSOR ALERT
void task6(void *parameters){
  for(;;){
    if (MQ7Alert == true){
      digitalWrite(MQ7ledPin, HIGH);
    }else if (MQ7Alert == false){
      digitalWrite(MQ7ledPin, LOW);
    }

    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

// TEMP SENSOR ALERT
void task7(void *parameters){
  for(;;){
    if (tempAlert == true){
      digitalWrite(templedPin, HIGH);
    }else if (tempAlert == false){
      digitalWrite(templedPin, LOW);
    }

    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

// HUMIDITY SENSOR ALERT
void task8(void *parameters){
  for(;;){
    if (humidityAlert == true){
      digitalWrite(humledPin, HIGH);
    }else if (humidityAlert == false){
      digitalWrite(humledPin, LOW);
    }

    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

// SYSTEM RESET
void task9(void *parameters){
  for(;;){
    if (touchRead(touchPin)<= 50){
      MQ7History = false;
    }

    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  tempSensor.begin();
  
  pinMode (templedPin, OUTPUT);
  pinMode (humledPin, OUTPUT);
  pinMode (MQ2ledPin, OUTPUT);
  pinMode (MQ7ledPin, OUTPUT);

  xTaskCreate(
    task1, //func name
    "Task 1", //task name
    5000, //stack size
    NULL, //task parameters
    1, //task priority
    NULL //task handle
  );

  xTaskCreate(
    task2, //func name
    "Task 2", //task name
    1000, //stack size
    NULL, //task parameters
    9, //task priority
    NULL //task handle
  );

  xTaskCreate(
    task3, //func name
    "Task 3", //task name
    1000, //stack size
    NULL, //task parameters
    7, //task priority
    NULL //task handle
  );

  xTaskCreate(
    task4, //func name
    "Task 4", //task name
    1000, //stack size
    NULL, //task parameters
    8, //task priority
    NULL //task handle
  );

  xTaskCreate(
    task5, //func name
    "Task 5", //task name
    1000, //stack size
    NULL, //task parameters
    6, //task priority
    NULL //task handle
  );

  xTaskCreate(
    task6, //func name
    "Task 6", //task name
    1000, //stack size
    NULL, //task parameters
    4, //task priority
    NULL //task handle
  );

  xTaskCreate(
    task7, //func name
    "Task 7", //task name
    1000, //stack size
    NULL, //task parameters
    3, //task priority
    NULL //task handle
  );

  xTaskCreate(
    task8, //func name
    "Task 8", //task name
    1000, //stack size
    NULL, //task parameters
    2, //task priority
    NULL //task handle
  );

  xTaskCreate(
    task9, //func name
    "Task 9", //task name
    1000, //stack size
    NULL, //task parameters
    5, //task priority
    NULL //task handle
  );
}

void loop() {
  // put your main code here, to run repeatedly:
}

