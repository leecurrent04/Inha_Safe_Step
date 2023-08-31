#include "./main.h"
#include "./PIN_DEFINITION.h"

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 nrf24(PIN_RF24_CE, PIN_RF24_CSN);

void setup()
{

    Serial.begin(SERIAL_BAUDRATE);  // open serial by 9600

    nrf24.begin();
    nrf24.openReadingPipe(0, RF24_ADDRESS);
    nrf24.setPALevel(RF24_PA_MIN);
    nrf24.startListening();

    pinMode(PIN_HALL_EFFECT_SENSOR, INPUT_PULLUP);
    pinMode(PIN_MOTOR, OUTPUT);
    pinMode(PIN_BUZZER, OUTPUT);
    pin_setup(pin_arr, pin_length);  // pin setup about two-people riding

    attachInterrupt(digitalPinToInterrupt(PIN_INTERRUPT), blink, CHANGE);
}


void loop()
{
    char debug_message[40];

    uint64_t currentTime = millis();
    if (currentTime - prevTime >= interval)
    {
        prevTime = currentTime;

        //Serial.print(String(count) + ", ");
        //count = 0;

        state_helmet_on = Recevice_rf24();
        if (state_helmet_on == -1)
        {
            // write any err
            count_disconnect_rf24++;
            if (count_disconnect_rf24 >= 3) 
            {
                Serial.println("Error");
            }
        }
        else
        {
            count_disconnect_rf24 = 0;
            Serial.print(String(state_helmet_on) + ", ");


            uint16_t ADC_arr[pin_length];
            uint16_t sum = 0;
            for (uint8_t i = 0; i < pin_length; i++)
            {
                ADC_arr[i] = analogRead(pin_arr[i]);
                Serial.print(String(ADC_arr[i]) + ",");
                sum += ADC_arr[i];
            }
            Serial.println("");
        }
    }
}

// check state of helmet wearing (nRF24)
int8_t Recevice_rf24()
{
    if (nrf24.available())
    {
        uint8_t temp_get_rf24[2];

        nrf24.read(&temp_get_rf24, sizeof(temp_get_rf24));
        return atoi(&temp_get_rf24[0]);
    }
    return -1;
}

/*
Turn on every sensors'pin
analog pin: 6, digital pin: 2
*/
void pin_setup(const int pin_arr[], const int pin_length)
{
    // Turn on pressure sensors
    for (int i = 0; i < pin_length; ++i)
        pinMode(pin_arr[i], INPUT);
}

/* Print all pressure sensors data */
void print_db(const float resistance_arr[])
{
    for (int i = 0; i < pin_length; ++i)
    {
        // Serial.println("Resistance" + String(i) + ": " + String(resistance_arr[i]) + " ohms");
        Serial.print(String(resistance_arr[i]) + ",");
    }
    //delay(DELAY_TIME);
}


/*
Judge how many guys ride kickboard 
If over two --> true
not --> false
*/
const int over_two(const float resistance_arr[], const int fst_target_arr[], const int snd_target_arr[])
{
    // Serial.println("===========================");

    int fst_cnt{ 1 };
    int snd_cnt{ 1 };
    int fst_or{ 0 };
    int snd_or{ 0 };
    for (int i = 0; i < pin_length / 2; ++i)
    {
        // User should step on foot-shaped stickers
        fst_cnt &= activation_func(resistance_arr[fst_target_arr[i]]);
        snd_cnt &= activation_func(resistance_arr[snd_target_arr[i]]);

        fst_or |= activation_func(resistance_arr[fst_target_arr[i]]);
        snd_or |= activation_func(resistance_arr[snd_target_arr[i]]);
    }

    delay(DELAY_TIME);
    Serial.print(String(fst_cnt) + String(snd_cnt) + String(fst_or) + String(snd_or) + ", ");

    if (fst_cnt && !snd_cnt)
    {
        if (snd_or)
            return 1;
        else
            return 0;
    }
    else if (!fst_cnt && snd_cnt)
    {
        if (fst_or)
            return 1;
        else
            return 0;
    }
    else
        return 1;
}


/* activation fuction */
inline const int activation_func(const float& target)
{
    if (target >= 3000.0)
        return 1;
    else
        return 0;
}


void blink()
{
    ++count;
}
