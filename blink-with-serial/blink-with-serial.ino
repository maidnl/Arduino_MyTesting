/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://docs.arduino.cc/hardware/

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  https://docs.arduino.cc/built-in-examples/basics/Blink/
*/

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <soc.h> /* Pulls in the STM32 CMSIS headers */
#include <stm32h5xx_hal.h>

#include <stm32_ll_system.h>

//#include "Nicla_System.h"
// the setup function runs once when you press reset or power the board
#define LED_BUILTIN D6
LOG_MODULE_REGISTER(adc_debug);



void dump_adc2_registers(void) {
    LOG_INF("--- STM32H5 ADC2 & GPIO Debug ---");
    
    /* ADC2 Configuration and Status */
    LOG_INF("ADC2 CFGR:  0x%08X", ADC2->CFGR);
    LOG_INF("ADC2 CFGR2:  0x%08X", ADC2->CFGR2);
    LOG_INF("ADC2 DIFSEL: 0x%08X", ADC2->DIFSEL);
    
    /* ADC2 Offsets */
    LOG_INF("ADC2 OFR1:   0x%08X", &ADC2->OFR1);
    LOG_INF("ADC2 OFR2:   0x%08X", &ADC2->OFR2);
    LOG_INF("ADC3 OFR3:   0x%08X", &ADC2->OFR3);
    LOG_INF("ADC4 OFR4:   0x%08X", &ADC2->OFR4);
    
    /* GPIOF Mode (Check PF13 and PF14) */
    LOG_INF("GPIOF MODER: 0x%08X", GPIOF->MODER);

    LOG_INF("ADDRESS %p", &ADC2->CFGR);

    Serial.print("DR ");
    Serial.println(ADC2->DR);
}

void setup() {
  Serial.begin(115200);
  while (!Serial) {
  }
  Serial.println("*** setup ***");
  
  analogReadResolution(12);
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  
 

 
  __HAL_RCC_VREF_CLK_ENABLE();
  LL_VREFBUF_SetVoltageScaling(LL_VREFBUF_VOLTAGE_SCALE0); 

    /* 3. Disable High Impedance mode to connect the buffer to the VREF+ pin */
    LL_VREFBUF_DisableHIZ();

    /* 4. Enable the Voltage Reference Buffer */
    LL_VREFBUF_Enable();

    /* 5. Wait for the voltage to stabilize (VRR bit goes high) */
    while(LL_VREFBUF_IsVREFReady() == 0) {
        delay(1);
    }


/*
  HAL_SYSCFG_VREFBUF_VoltageScalingConfig(SYSCFG_VREFBUF_VOLTAGE_SCALE0);
  HAL_SYSCFG_EnableVREFBUF();
  HAL_SYSCFG_VREFBUF_HighImpedanceConfig(SYSCFG_VREFBUF_HIGH_IMPEDANCE_DISABLE);
 */
   
    k_msleep(1);

  Serial.println("CALIBRATED!");
  delay(2000);
}

// the loop function runs over and over again forever
void loop() {
  


  int a0 = analogRead(A0);
  int a1 = analogRead(A1);

  Serial.print("a0 = ");
  Serial.println(a0);
  Serial.print("a1 = ");
  Serial.println(a1);

  digitalWrite(LED_BUILTIN, HIGH);  // change state of the LED by setting the pin to the HIGH voltage level
  delay(1000);                      // wait for a second
  digitalWrite(LED_BUILTIN, LOW);   // change state of the LED by setting the pin to the LOW voltage level
  delay(1000);                      // wait for a second
  
}
