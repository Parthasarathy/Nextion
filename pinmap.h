#include <Arduino.h>
#include "Nextion.h"
#include "display.h"
#include "rtc.h"

/*
 * Register a button object to the touch event list.  
 */
NexTouch *nex_listen_list[] =
    {
        &Page3_bt_setDate, &Page3_bt_setTime, &b_1_rst_uc_25,
        NULL 
    };


void b_1_rst_uc_25_Callback(void *ptr)
{
    digitalWrite(RESET_PIN,1);
    delay(3000);
    digitalWrite(RESET_PIN,0);
    Serial.println("Reset Button: Callback");
}



#define MAXPIN 16

void InitializePins(){

     for(int pin_index=0; pin_index<MAXPIN; pin_index++){
        pinMode(MegaPinMap1[pin_index].pin, INPUT);
        pinMode(MegaPinMap2[pin_index].pin, INPUT);
     }

    for(int pin_index=0; pin_index<=GP_MAX_BUTTON; pin_index++){
        pinMode(GPMap[pin_index].pin, INPUT);
     }

    pinMode(RESET_PIN, OUTPUT);
    pinMode(READY_PIN, OUTPUT);
  
    b_1_rst_uc_25.attachPop(b_1_rst_uc_25_Callback, &b_1_rst_uc_25);
}


void SetStatusText(){
    uint8_t status;
    status=(digitalRead(GPMap[0].pin)<<2)|(digitalRead(GPMap[1].pin)<<1)|digitalRead(GPMap[2].pin);
    
    switch(status){
      case NORMAL_OPERATION:
            digitalWrite(READY_PIN, 1);
            bt_1_las_ok.setValue(1);
            if(digitalRead(MegaPinMap2[11].pin)!=0){                          
                bt_1_rdy_uc_26.setValue(1); 
            }               
            tx_1_ls_out.setText("NORMAL OPERATION");
            Serial.println("Status: NORMAL OPERATION");            
            break;
      case TEMP_ALARM:
            digitalWrite(READY_PIN, 0);
            bt_1_rdy_uc_26.setValue(0);
            bt_1_las_ok.setValue(0);
            tx_1_ls_out.setText("TEMP ALARM");
      break;
      case POWER_ALARM:

            digitalWrite(READY_PIN, 0);
            bt_1_rdy_uc_26.setValue(0);
            bt_1_las_ok.setValue(0);
            tx_1_ls_out.setText("POWER ALARM");
      break;
      case EMI_NOT_READY:
            digitalWrite(READY_PIN, 0);
            bt_1_rdy_uc_26.setValue(0);
            bt_1_las_ok.setValue(0);
            tx_1_ls_out.setText("EMI NOT READY");
      break;
      case BACK_REFLECTION:

            digitalWrite(READY_PIN, 0);
            bt_1_rdy_uc_26.setValue(0);
            bt_1_las_ok.setValue(0);
            tx_1_ls_out.setText("BACK REFLECTION");
      break;
      case SYSTEM_ALARAM:
            digitalWrite(READY_PIN, 0);
            bt_1_rdy_uc_26.setValue(0);
            bt_1_las_ok.setValue(0);
            tx_1_ls_out.setText("SYSTEM ALARAM");
      break;
      default:
            digitalWrite(READY_PIN, 0);
            bt_1_rdy_uc_26.setValue(0);
            bt_1_las_ok.setValue(0);
            tx_1_ls_out.setText(" ");
            Serial.println("Status: set to empty");
    }
}

static unsigned long EStopDelay=0;
#define ESTOP_DELAY 5
static int estop_state =0;
#define ESTOP_PIN 3

void EStop_Effect(){
   int buttonState = digitalRead(GPMap[ESTOP_PIN].pin);
   if(buttonState==0){
    Serial.println("bt_pressed");
        if(EStopDelay==0){
        Serial.print("Disp_update");
        Serial.println(estop_state);
          GPMap[ESTOP_PIN].btnnumber.setValue(estop_state);
          estop_state = !estop_state;
          GPMap[ESTOP_PIN].state=0;
        }

        EStopDelay++;
                
        if(EStopDelay >= ESTOP_DELAY){
           EStopDelay=0;           
        }        
   }
}

void PinToDisplay(){
    nexLoop(nex_listen_list);
    //Serial.println("Reading Digital 1:");
    struct DigitalInput inputs=  digitalReadAll(MegaPinMap1);
    //Serial.println("Completed Digital 1:");
    nexLoop(nex_listen_list);
    //Serial.println("Start Nextion Write 1:");
    btnnumber0p1.setValue(inputs.p0);
    btnnumber1p1.setValue(inputs.p1);
    btnnumber2p1.setValue(inputs.p2);
    btnnumber3p1.setValue(inputs.p3);
    btnnumber4p1.setValue(inputs.p4);
    btnnumber5p1.setValue(inputs.p5);
    btnnumber6p1.setValue(inputs.p6);
    btnnumber7p1.setValue(inputs.p7);
    btnnumber8p1.setValue(inputs.p8);
    btnnumber9p1.setValue(inputs.p9);
    btnnumber10p1.setValue(inputs.p10);
    btnnumber11p1.setValue(inputs.p11);
    btnnumber12p1.setValue(inputs.p12);
    btnnumber13p1.setValue(inputs.p13);
    btnnumber14p1.setValue(inputs.p14);
    btnnumber15p1.setValue(inputs.p15);
    //Serial.println("Stopped Nextion Write 1:");
    nexLoop(nex_listen_list);
    //Serial.println("Reading Digital 2:");
    inputs=  digitalReadAll(MegaPinMap2);
    //Serial.println("Completed Digital 2:");
    nexLoop(nex_listen_list);
    //Serial.println("Start Nextion Write 2:");    
    btnnumber0p2.setValue(inputs.p0);
    btnnumber1p2.setValue(inputs.p1);
    btnnumber2p2.setValue(inputs.p2);
    btnnumber3p2.setValue(inputs.p3);
    btnnumber4p2.setValue(inputs.p4);
    btnnumber5p2.setValue(inputs.p5);
    btnnumber6p2.setValue(inputs.p6);
    btnnumber7p2.setValue(inputs.p7);
    btnnumber8p2.setValue(inputs.p8);
    btnnumber9p2.setValue(inputs.p9);
    btnnumber10p2.setValue(inputs.p10);
    btnnumber11p2.setValue(inputs.p11);
    btnnumber12p2.setValue(inputs.p12);
    btnnumber13p2.setValue(inputs.p13);
    btnnumber14p2.setValue(inputs.p14);
    btnnumber15p2.setValue(inputs.p15);
    //Serial.println("Stopped Nextion Write 2:");
    nexLoop(nex_listen_list);
    SetStatusText();
}

void TestPins(){

    struct DigitalInput inputs=  digitalReadAll(MegaPinMap1);
    Serial.println("********************************");
    Serial.println("PCF 1 :");
    Serial.print("0 : ");   Serial.println(inputs.p0); MegaPinMap1[0].btnnumber.setValue(inputs.p0);
    Serial.print("1 : ");   Serial.println(inputs.p1); MegaPinMap1[1].btnnumber.setValue(inputs.p1);
    Serial.print("2 : ");   Serial.println(inputs.p2); MegaPinMap1[2].btnnumber.setValue(inputs.p2);
    Serial.print("3 : ");   Serial.println(inputs.p3); MegaPinMap1[3].btnnumber.setValue(inputs.p3);
    Serial.print("4 : ");   Serial.println(inputs.p4); MegaPinMap1[4].btnnumber.setValue(inputs.p4);
    Serial.print("5 : ");   Serial.println(inputs.p5); MegaPinMap1[5].btnnumber.setValue(inputs.p5);
    Serial.print("6 : ");   Serial.println(inputs.p6); MegaPinMap1[6].btnnumber.setValue(inputs.p6);
    Serial.print("7 : ");   Serial.println(inputs.p7); MegaPinMap1[7].btnnumber.setValue(inputs.p7);
    Serial.print("8 : ");   Serial.println(inputs.p8); MegaPinMap1[8].btnnumber.setValue(inputs.p8);
    Serial.print("9 : ");   Serial.println(inputs.p9); MegaPinMap1[9].btnnumber.setValue(inputs.p9);    
    Serial.print("10 : ");   Serial.println(inputs.p10); MegaPinMap1[10].btnnumber.setValue(inputs.p10);
    Serial.print("11 : ");   Serial.println(inputs.p11); MegaPinMap1[11].btnnumber.setValue(inputs.p11);
    Serial.print("12 : ");   Serial.println(inputs.p12); MegaPinMap1[12].btnnumber.setValue(inputs.p12);
    Serial.print("13 : ");   Serial.println(inputs.p13); MegaPinMap1[13].btnnumber.setValue(inputs.p13);
    Serial.print("14 : ");   Serial.println(inputs.p14); MegaPinMap1[14].btnnumber.setValue(inputs.p14);
    Serial.print("15 : ");   Serial.println(inputs.p15); MegaPinMap1[15].btnnumber.setValue(inputs.p15);
    //delay(1000);
    Serial.println("-------------------------------");
    inputs=  digitalReadAll(MegaPinMap2);
    Serial.println("PCF 2 :");
    Serial.print("0 : ");   Serial.println(inputs.p0); MegaPinMap2[0].btnnumber.setValue(inputs.p0);
    Serial.print("1 : ");   Serial.println(inputs.p1); MegaPinMap2[1].btnnumber.setValue(inputs.p1);
    Serial.print("2 : ");   Serial.println(inputs.p2); MegaPinMap2[2].btnnumber.setValue(inputs.p2);
    Serial.print("3 : ");   Serial.println(inputs.p3); MegaPinMap2[3].btnnumber.setValue(inputs.p3);
    Serial.print("4 : ");   Serial.println(inputs.p4); MegaPinMap2[4].btnnumber.setValue(inputs.p4);
    Serial.print("5 : ");   Serial.println(inputs.p5); MegaPinMap2[5].btnnumber.setValue(inputs.p5);
    Serial.print("6 : ");   Serial.println(inputs.p6); MegaPinMap2[6].btnnumber.setValue(inputs.p6);
    Serial.print("7 : ");   Serial.println(inputs.p7); MegaPinMap2[7].btnnumber.setValue(inputs.p7);
    Serial.print("8 : ");   Serial.println(inputs.p8); MegaPinMap2[8].btnnumber.setValue(inputs.p8);
    Serial.print("9 : ");   Serial.println(inputs.p9); MegaPinMap2[9].btnnumber.setValue(inputs.p9);
    Serial.print("10 : ");   Serial.println(inputs.p10); MegaPinMap2[10].btnnumber.setValue(inputs.p10);
    Serial.print("11 : ");   Serial.println(inputs.p11); MegaPinMap2[11].btnnumber.setValue(inputs.p11);
    Serial.print("12 : ");   Serial.println(inputs.p12); MegaPinMap2[12].btnnumber.setValue(inputs.p12);
    Serial.print("13 : ");   Serial.println(inputs.p13); MegaPinMap2[13].btnnumber.setValue(inputs.p13);
    Serial.print("14 : ");   Serial.println(inputs.p14); MegaPinMap2[14].btnnumber.setValue(inputs.p14);
    Serial.print("15 : ");   Serial.println(inputs.p15); MegaPinMap2[15].btnnumber.setValue(inputs.p15);
    //delay(1000);
    Serial.println("-------------------------------");
    Serial.println("GPIO :");
    Serial.print("BT12 : ");   Serial.println(digitalRead(GPMap[0].pin)); GPMap[0].btnnumber.setValue(digitalRead(GPMap[0].pin));
    Serial.print("BT13 : ");   Serial.println(digitalRead(GPMap[1].pin)); GPMap[1].btnnumber.setValue(digitalRead(GPMap[1].pin));
    Serial.print("BT14 : ");   Serial.println(digitalRead(GPMap[2].pin)); GPMap[2].btnnumber.setValue(digitalRead(GPMap[2].pin));
    Serial.print("BT15 : ");   Serial.println(digitalRead(GPMap[3].pin)); GPMap[3].btnnumber.setValue(digitalRead(GPMap[3].pin)); 
    SetStatusText();
    //delay(1000);
}
