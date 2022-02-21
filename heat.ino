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

#define PIN_HEAT_CH_1 (4)
#define PIN_HEAT_CH_2 (5)
#define PIN_HEAT_CH_3 (6)
#define PIN_HEAT_CH_4 (7)

#define PIN_TEST (9)

void setup() {
  Serial.begin(9600);
  pinMode(PIN_HEAT_CH_1, OUTPUT);
  pinMode(PIN_HEAT_CH_2, OUTPUT);
  pinMode(PIN_HEAT_CH_3, OUTPUT);
  pinMode(PIN_HEAT_CH_4, OUTPUT);

  // pinMode(PIN_TEST, OUTPUT);
}

void loop() {
  static unsigned long minutes = 0;
  static int16_t head_cm = 0;

  static uint32_t vbat_filter = 0;
  static uint8_t pwmCounter = 0;

  static uint8_t tcnt0_old = 0;
  uint8_t tcnt0_new = TCNT0 & 0xC0;  // triggered at T0OVF x 4
  if(tcnt0_old != tcnt0_new){
    tcnt0_old = tcnt0_new;
    // timer0 change event
    pwmCounter++;

    // digitalWrite(PIN_HEAT_CH_4, HIGH);
    // digitalWrite(PIN_HEAT_CH_4, LOW);
    // analogWrite(PIN_TEST, 5);

    uint32_t vbat_raw = analogRead(PIN_VIN);
    if (vbat_filter == 0) {
      vbat_filter = vbat_raw << 8L;
    }

    vbat_filter -= vbat_filter >> 8L;
    vbat_filter += vbat_raw;

    uint16_t vSolar_dv = ((vbat_filter >> 8L) * 354L) >> 8L; // 350 uncal'd
    uint16_t vTarget_dv = 650;

    uint8_t demand = 0;
    if(vSolar_dv > vTarget_dv){
      uint16_t error = vSolar_dv - vTarget_dv;
      if (error > 63){
        error = 63;
      }
      demand = error << 2;
    }

    // demand = 0x88;

    uint8_t counter = pwmCounter << 4;
    if((counter == 0) || (counter > demand)){
      digitalWrite(PIN_HEAT_CH_1, LOW);
    } else{
      digitalWrite(PIN_HEAT_CH_1, HIGH);
    }

    if(demand >= 0x04) demand -= 0x04;

    counter += 0x40;
    if((counter == 0) || (counter > demand)){
      digitalWrite(PIN_HEAT_CH_2, LOW);
    } else{
      digitalWrite(PIN_HEAT_CH_2, HIGH);
    }

    if(demand >= 0x04) demand -= 0x04;

    counter += 0x40;
    if((counter == 0) || (counter > demand)){
      digitalWrite(PIN_HEAT_CH_3, LOW);
    } else{
      digitalWrite(PIN_HEAT_CH_3, HIGH);
    }

    if(demand >= 0x04) demand -= 0x04;

    counter += 0x40;
    if((counter == 0) || (counter > demand)){
      digitalWrite(PIN_HEAT_CH_4, LOW);
    } else{
      digitalWrite(PIN_HEAT_CH_4, HIGH);
    }

    if(pwmCounter == 0){
      Serial.print("vbat ");
      Serial.println(vSolar_dv);
    }

  }

}
