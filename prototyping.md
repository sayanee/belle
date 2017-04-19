# Prototyping

> Photos of various prototypes

## 1. nodeMCU

- nodeMCU is used to test the initial circuit
- the circuit has a buzzer and a push button to mimc the bell

    ![](images/prototype-1.JPG)

- Super simple interrupt from sleep by pressing the bell

    ```c
    void setup() {
      Serial.begin(9600);
      pinMode(BUILTIN_LED, OUTPUT);

      Serial.println('Start');
      Serial.println('Wake up!');
      digitalWrite(BUILTIN_LED, HIGH);
      delay(1000);
      digitalWrite(BUILTIN_LED, LOW);
      delay(1000);
      digitalWrite(BUILTIN_LED, HIGH);
      delay(1000);
      digitalWrite(BUILTIN_LED, LOW);
      delay(1000);
    }

    void loop() {
      Serial.println("Sleeping in.... 3");
      delay(100);
      Serial.println("Sleeping in.... 2");
      delay(100);
      Serial.println("Sleeping in.... 1");
      delay(100);
      ESP.deepSleep(5e6, WAKE_RF_DEFAULT);
    }
    ```

## 2. ESP12

- Refactored to use ESP12 modules for ESP8266
- Also AP2112 is used for the voltage conversion from `5V` to `3.3V`

    ![](images/prototype-2.jpg)
