// ATMEL ATMEGA8 & 168 / ARDUINO
//
//                  +-\/-+
//            PC6  1|    |28  PC5 (AI 5)
//      (D 0) PD0  2|    |27  PC4 (AI 4)
//      (D 1) PD1  3|    |26  PC3 (AI 3)
//      (D 2) PD2  4|    |25  PC2 (AI 2)
// PWM+ (D 3) PD3  5|    |24  PC1 (AI 1)
//      (D 4) PD4  6|    |23  PC0 (AI 0)
//            VCC  7|    |22  GND
//            GND  8|    |21  AREF
//            PB6  9|    |20  AVCC
//            PB7 10|    |19  PB5 (D 13)
// PWM+ (D 5) PD5 11|    |18  PB4 (D 12)
// PWM+ (D 6) PD6 12|    |17  PB3 (D 11) PWM
//      (D 7) PD7 13|    |16  PB2 (D 10) PWM
//      (D 8) PB0 14|    |15  PB1 (D 9) PWM
//                  +----+


#define PIN_VIN (A3)

#define PIN_HEAT_4 (7)

void setup() {
  Serial.begin(9600);
  pinMode(PIN_HEAT_4, OUTPUT);
}

void loop() {
  static unsigned long minutes = 0;
  static int16_t head_cm = 0;

  static uint32_t vbat_filter = 0;

  delay(1000);
  // uint16_t vbat_mv = ((uint16_t)analogRead(PIN_VBAT) * 111) >> 2;

  uint32_t vbat_raw = analogRead(PIN_VIN);
  if (vbat_filter == 0) {
    vbat_filter = vbat_raw << 8L;
  }

  vbat_filter -= vbat_filter >> 8L;
  vbat_filter += vbat_raw;

  uint32_t vbat_mv = ((vbat_filter >> 8L) * 7104L) >> 8;

  Serial.print("vbat ");
  Serial.println(vbat_mv);

  delay(1000);
  unsigned long newMinutes = (millis() >> 16); // dt = 65.536 secs
  Serial.print("time ");
  Serial.println(newMinutes);


}
