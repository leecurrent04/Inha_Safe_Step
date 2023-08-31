#define SERIAL_BAUDRATE 115200
#define DELAY_TIME      1000

const byte RF24_ADDRESS[6] = "00001";

int8_t state_helmet_on = 0;
uint8_t state_over_people = 0;  

uint8_t count_disconnect_rf24 = 0;

int count = 0;
const unsigned long interval = 1000;
unsigned long prevTime = 0;



/* Measure the voltage at 5V and resistance of your 3.3k resistor, and enter */
const float VCC = 4.98;      // Measured voltage of Ardunio 5V line
const float R_DIV = 3230.0;  // Measured resistance of 3.3k resistor


/* Sensor Pin Setting */
const int pin_length = 8;
const int pin_arr[pin_length]{ A0, A1, A2, A3, A4, A5, A6, A7};

/* Target Sensors */
//fst
const int fst_target_arr[pin_length / 2]{ 0, 1, 2 };
//snd
const int snd_target_arr[pin_length / 2]{ 3, 4, 5 };

/* Function Prototype */
void pin_setup(const int, const int&);  // Set up initial pin settings
void print_db(const float);            // Show information about pressure sensors
const int over_two(const float);       // Judge over two guys riding on kickboard

int8_t Recevice_rf24();

void blink();