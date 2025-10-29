#include <Arduino.h>
#include "Nextion.h"
#include "display.h"
#include "rtc.h"

volatile bool Invalidate_Page =false;
volatile bool Invalidate_Page_b_pg1_nxt=false;
volatile bool Invalidate_Page_b_pg2_back =false;
volatile bool Invalidate_Page_b_pg3_back = false;
volatile bool Invalidate_Page_b_pg3_home=false;


void SetInitialPinStatusGPIO();
void SetStatusText();
void SetInitialPinStatus( struct DigitalInput input, struct sPinInfo pinmap[], bool invert_pin_10_12);
void SetInitialPinStatusPage1();
void SetInitialPinStatusPage2();


/*
 * Register a button object to the touch event list.  
 */
NexTouch *nex_listen_list[] =
    {
        &Page3_bt_setDate, &Page3_bt_setTime, &b_1_rst_uc_25,
        &b_pg1_nxt, &b_pg2_back, &b_pg2_nxt, &b_pg3_back, &b_pg3_home,
        &page1, &page2, &page3,NULL
    };


void b_1_rst_uc_25_Callback(void *ptr)
{
    digitalWrite(RESET_PIN,1);
    delay(3000);
    digitalWrite(RESET_PIN,0);
}


void page1_invalidate_Callback(void *ptr)
{
    //p1_tx.setText("P1 Start");
    page1.show();
    Current_Page=1;
    SetInitialPinStatusPage1();
    //p1_tx.setText("P1 End");
}

void page2_invalidate_Callback(void *ptr)
{
    //p2_tx.setText("P2 Start");
    page2.show();
    Current_Page=2;
    SetInitialPinStatusPage2();
    //p2_tx.setText("P2 End");
}

void page3_invalidate_Callback(void *ptr)
{
    //p3_tx.setText("P3 Start");
    page3.show();
    Current_Page=3;
    //p3_tx.setText("P3 End");
}


void page_invalidate_Callback_b_pg1_nxt(void *ptr)
{
    //p2_tx.setText("P2 Start");
    page2.show();
    Current_Page=2;
    SetInitialPinStatusPage2();
    //p2_tx.setText("P2 End");

}

void page_invalidate_Callback_b_pg2_back(void *ptr)
{
    //p1_tx.setText("P1 Start");
    page1.show();
    Current_Page=1;
    SetInitialPinStatusPage1();
    //p1_tx.setText("P1 End");
}

void page_invalidate_Callback_b_pg3_back(void *ptr)
{
    //p2_tx.setText("P2 Start");
    page2.show();
    Current_Page=2;    
    SetInitialPinStatusPage2();
    //p2_tx.setText("P2 End");
}

void page_invalidate_Callback_b_pg3_home(void *ptr)
{
    //p1_tx.setText("P1 Start");
    page1.show();
    Current_Page=1;
    SetInitialPinStatusPage1();
    //p1_tx.setText("P1 End");
}

void page_invalidate_Callback_b_pg2_nxt(void *ptr){
    //p3_tx.setText("P3 Start");
    page3.show();
    Current_Page=3;
    //p3_tx.setText("P3 End");

}

#define MAXPIN 16

void InitializePins(){

     for(int pin_index=0; pin_index<MAXPIN; pin_index++){
        pinMode(MegaPinMap1[pin_index].pin, INPUT);
        pinMode(MegaPinMap2[pin_index].pin, INPUT);
     }

    pinMode(RESET_PIN, OUTPUT);
    pinMode(READY_PIN, OUTPUT);
    SetInitialPinStatus(digitalReadAll(MegaPinMap1), MegaPinMap1, false);
    SetInitialPinStatus(digitalReadAll(MegaPinMap2), MegaPinMap2, true);
    SetInitialPinStatusGPIO();
    
    b_1_rst_uc_25.attachPop(b_1_rst_uc_25_Callback, &b_1_rst_uc_25);
    
    b_pg1_nxt.attachPush(page_invalidate_Callback_b_pg1_nxt, &b_pg1_nxt);
    b_pg2_back.attachPush(page_invalidate_Callback_b_pg2_back, &b_pg2_back);
    b_pg2_nxt.attachPush(page_invalidate_Callback_b_pg2_nxt, &b_pg2_nxt);
    b_pg3_back.attachPush(page_invalidate_Callback_b_pg3_back, &b_pg3_back);
    b_pg3_home.attachPush(page_invalidate_Callback_b_pg3_home, &b_pg3_home);
    
    page1.attachPop(page1_invalidate_Callback);
    page2.attachPop(page2_invalidate_Callback);
    page3.attachPop(page3_invalidate_Callback);
    
}


void SetInitialPinStatusGPIO(){
    for(int pin_index=0;pin_index<GP_MAX_BUTTON;pin_index++){
      int buttonState =digitalRead(GPMap[pin_index].pin);
      GPMap[pin_index].btnnumber.setValue(buttonState);
      GPMap[pin_index].state=buttonState;
    }     

    SetStatusText();
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

void SetInitialPinStatus( struct DigitalInput input, struct sPinInfo pinmap[], bool invert_pin_10_12){
    pinmap[0].btnnumber.setValue(input.p0);   pinmap[0].state=input.p0;
    pinmap[1].btnnumber.setValue(input.p1);   pinmap[1].state=input.p1;
    pinmap[2].btnnumber.setValue(input.p2);   pinmap[2].state=input.p2;
    pinmap[3].btnnumber.setValue(input.p3);   pinmap[3].state=input.p3;
    pinmap[4].btnnumber.setValue(input.p4);   pinmap[4].state=input.p4;
    pinmap[5].btnnumber.setValue(input.p5);   pinmap[5].state=input.p5;
    pinmap[6].btnnumber.setValue(input.p6);   pinmap[6].state=input.p6;
    pinmap[7].btnnumber.setValue(input.p7);   pinmap[7].state=input.p7;
    pinmap[8].btnnumber.setValue(input.p8);   pinmap[8].state=input.p8;
    pinmap[9].btnnumber.setValue(input.p9);   pinmap[9].state=input.p9;
    pinmap[11].btnnumber.setValue(input.p11);   pinmap[11].state=input.p11;
    pinmap[13].btnnumber.setValue(input.p13);   pinmap[13].state=input.p13;
    pinmap[14].btnnumber.setValue(input.p14);   pinmap[14].state=input.p14;
    pinmap[15].btnnumber.setValue(input.p15);   pinmap[15].state=input.p15;

    if(invert_pin_10_12){
        uint8_t inverted= input.p10 ? 0 : 1;
        pinmap[10].btnnumber.setValue(inverted);   pinmap[10].state=inverted; 
        
        inverted= input.p12 ? 0 : 1;
        pinmap[12].btnnumber.setValue(inverted);   pinmap[12].state=inverted;
    }
    else{    
        pinmap[10].btnnumber.setValue(input.p10);   pinmap[10].state=input.p10;
        pinmap[12].btnnumber.setValue(input.p12);   pinmap[12].state=input.p12;
    }  
}


void SetInitialPinStatusPage1(){

    struct DigitalInput input = digitalReadAll(MegaPinMap2);
    bool invert_pin_10_12=true;

    MegaPinMap2[4].btnnumber.setValue(input.p4);   MegaPinMap2[4].state=input.p4;
    MegaPinMap2[5].btnnumber.setValue(input.p5);   MegaPinMap2[5].state=input.p5;
    MegaPinMap2[6].btnnumber.setValue(input.p6);   MegaPinMap2[6].state=input.p6;
    MegaPinMap2[7].btnnumber.setValue(input.p7);   MegaPinMap2[7].state=input.p7;
    MegaPinMap2[8].btnnumber.setValue(input.p8);   MegaPinMap2[8].state=input.p8;
    MegaPinMap2[9].btnnumber.setValue(input.p9);   MegaPinMap2[9].state=input.p9;
    MegaPinMap2[11].btnnumber.setValue(input.p11);   MegaPinMap2[11].state=input.p11;
    MegaPinMap2[13].btnnumber.setValue(input.p13);   MegaPinMap2[13].state=input.p13;
    MegaPinMap2[14].btnnumber.setValue(input.p14);   MegaPinMap2[14].state=input.p14;
    MegaPinMap2[15].btnnumber.setValue(input.p15);   MegaPinMap2[15].state=input.p15;

    if(invert_pin_10_12){
        uint8_t inverted= input.p10 ? 0 : 1;
        MegaPinMap2[10].btnnumber.setValue(inverted);   MegaPinMap2[10].state=inverted; 
        
        inverted= input.p12 ? 0 : 1;
        MegaPinMap2[12].btnnumber.setValue(inverted);   MegaPinMap2[12].state=inverted;
    }
    else{    
        MegaPinMap2[10].btnnumber.setValue(input.p10);   MegaPinMap2[10].state=input.p10;
        MegaPinMap2[12].btnnumber.setValue(input.p12);   MegaPinMap2[12].state=input.p12;
    }  

    SetInitialPinStatusGPIO();    
}

void SetInitialPinStatusPage2(){

    struct DigitalInput input = digitalReadAll(MegaPinMap1);
    bool invert_pin_10_12=false;

    MegaPinMap1[0].btnnumber.setValue(input.p0);   MegaPinMap1[0].state=input.p0;
    MegaPinMap1[1].btnnumber.setValue(input.p1);   MegaPinMap1[1].state=input.p1;
    MegaPinMap1[2].btnnumber.setValue(input.p2);   MegaPinMap1[2].state=input.p2;
    MegaPinMap1[3].btnnumber.setValue(input.p3);   MegaPinMap1[3].state=input.p3;
    MegaPinMap1[4].btnnumber.setValue(input.p4);   MegaPinMap1[4].state=input.p4;
    MegaPinMap1[5].btnnumber.setValue(input.p5);   MegaPinMap1[5].state=input.p5;
    MegaPinMap1[6].btnnumber.setValue(input.p6);   MegaPinMap1[6].state=input.p6;
    MegaPinMap1[7].btnnumber.setValue(input.p7);   MegaPinMap1[7].state=input.p7;
    MegaPinMap1[8].btnnumber.setValue(input.p8);   MegaPinMap1[8].state=input.p8;
    MegaPinMap1[9].btnnumber.setValue(input.p9);   MegaPinMap1[9].state=input.p9;
    MegaPinMap1[11].btnnumber.setValue(input.p11);   MegaPinMap1[11].state=input.p11;
    MegaPinMap1[13].btnnumber.setValue(input.p13);   MegaPinMap1[13].state=input.p13;
    MegaPinMap1[14].btnnumber.setValue(input.p14);   MegaPinMap1[14].state=input.p14;
    MegaPinMap1[15].btnnumber.setValue(input.p15);   MegaPinMap1[15].state=input.p15;

    if(invert_pin_10_12){
        uint8_t inverted= input.p10 ? 0 : 1;
        MegaPinMap1[10].btnnumber.setValue(inverted);   MegaPinMap1[10].state=inverted; 
        
        inverted= input.p12 ? 0 : 1;
        MegaPinMap1[12].btnnumber.setValue(inverted);   MegaPinMap1[12].state=inverted;
    }
    else{    
        MegaPinMap1[10].btnnumber.setValue(input.p10);   MegaPinMap1[10].state=input.p10;
        MegaPinMap1[12].btnnumber.setValue(input.p12);   MegaPinMap1[12].state=input.p12;
    }  

    input = digitalReadAll(MegaPinMap2);
    MegaPinMap2[0].btnnumber.setValue(input.p0);   MegaPinMap2[0].state=input.p0;
    MegaPinMap2[1].btnnumber.setValue(input.p1);   MegaPinMap2[1].state=input.p1;
    MegaPinMap2[2].btnnumber.setValue(input.p2);   MegaPinMap2[2].state=input.p2;
    MegaPinMap2[3].btnnumber.setValue(input.p3);   MegaPinMap2[3].state=input.p3;

}

void ActOnPinStatusChange(struct DigitalInput input, struct sPinInfo pinmap[], bool invert_pin_10_12){
    
    if(input.p0 != pinmap[0].state){
        Serial.println("");
        Serial.print("Update for Pin:");
        Serial.print(0);
        Serial.print("  old State:");
        Serial.print(pinmap[0].state);        
        Serial.print("  Current State:");
        Serial.print(input.p0);   
        pinmap[0].btnnumber.setValue(input.p0);   pinmap[0].state=input.p0; }
    if(input.p1 != pinmap[1].state){  
        Serial.println("");
        Serial.print("Update for Pin:");
        Serial.print(1);
        Serial.print("  old State:");
        Serial.print(pinmap[1].state);        
        Serial.print("  Current State:");
        Serial.print(input.p1); 
        pinmap[1].btnnumber.setValue(input.p1);   pinmap[1].state=input.p1; }
    if(input.p2 != pinmap[2].state){  
        Serial.println("");
        Serial.print("Update for Pin:");
        Serial.print(2);
        Serial.print("  old State:");
        Serial.print(pinmap[2].state);        
        Serial.print("  Current State:");
        Serial.print(input.p2); 
        pinmap[2].btnnumber.setValue(input.p2);   pinmap[2].state=input.p2; }
    if(input.p3 != pinmap[3].state){  
        Serial.println("");
        Serial.print("Update for Pin:");
        Serial.print(3);
        Serial.print("  old State:");
        Serial.print(pinmap[3].state);        
        Serial.print("  Current State:");
        Serial.print(input.p3); 
        pinmap[3].btnnumber.setValue(input.p3);   pinmap[3].state=input.p3; }
    if(input.p4 != pinmap[4].state){  
        Serial.println("");
        Serial.print("Update for Pin:");
        Serial.print(4);
        Serial.print("  old State:");
        Serial.print(pinmap[4].state);        
        Serial.print("  Current State:");
        Serial.print(input.p4); 
        pinmap[4].btnnumber.setValue(input.p4);   pinmap[4].state=input.p4; }
    if(input.p5 != pinmap[5].state){  
        Serial.println("");
        Serial.print("Update for Pin:");
        Serial.print(5);
        Serial.print("  old State:");
        Serial.print(pinmap[5].state);        
        Serial.print("  Current State:");
        Serial.print(input.p5); 
        pinmap[5].btnnumber.setValue(input.p5);   pinmap[5].state=input.p5; }
    if(input.p6 != pinmap[6].state){  
        Serial.println("");
        Serial.print("Update for Pin:");
        Serial.print(6);
        Serial.print("  old State:");
        Serial.print(pinmap[6].state);        
        Serial.print("  Current State:");
        Serial.print(input.p6); 
        pinmap[6].btnnumber.setValue(input.p6);   pinmap[6].state=input.p6; }
    if(input.p7 != pinmap[7].state){          Serial.println("");
        Serial.print("Update for Pin:");
        Serial.print(7);
        Serial.print("  old State:");
        Serial.print(pinmap[7].state);        
        Serial.print("  Current State:");
        Serial.print(input.p7); 
        pinmap[7].btnnumber.setValue(input.p7);   pinmap[7].state=input.p7; }
    if(input.p8 != pinmap[8].state){          
        Serial.println("");
        Serial.print("Update for Pin:");
        Serial.print(8);
        Serial.print("  old State:");
        Serial.print(pinmap[8].state);        
        Serial.print("  Current State:");
        Serial.print(input.p8); 
        pinmap[8].btnnumber.setValue(input.p8);   pinmap[8].state=input.p8; }
    if(input.p9 != pinmap[9].state){  
        Serial.println("");
        Serial.print("Update for Pin:");
        Serial.print(9);
        Serial.print("  old State:");
        Serial.print(pinmap[9].state);        
        Serial.print("  Current State:");
        Serial.print(input.p9); 
        pinmap[9].btnnumber.setValue(input.p9);   pinmap[9].state=input.p9; }
    if(input.p11 != pinmap[11].state){          
        Serial.println("");
        Serial.print("Update for Pin:");
        Serial.print(11);
        Serial.print("  old State:");
        Serial.print(pinmap[11].state);        
        Serial.print("  Current State:");
        Serial.print(input.p11); 
        pinmap[11].btnnumber.setValue(input.p11);   pinmap[11].state=input.p11; }
    if(input.p13 != pinmap[13].state){  
        Serial.println("");
        Serial.print("Update for Pin:");
        Serial.print(13);
        Serial.print("  old State:");
        Serial.print(pinmap[13].state);        
        Serial.print("  Current State:");
        Serial.print(input.p13); 
        pinmap[13].btnnumber.setValue(input.p13);   pinmap[13].state=input.p13; }
    if(input.p14 != pinmap[14].state){  
        Serial.println("");
        Serial.print("Update for Pin:");
        Serial.print(14);
        Serial.print("  old State:");
        Serial.print(pinmap[14].state);        
        Serial.print("  Current State:");
        Serial.print(input.p14); 
        pinmap[14].btnnumber.setValue(input.p14);   pinmap[14].state=input.p14; }
    if(input.p15 != pinmap[15].state){  
        Serial.println("");
        Serial.print("Update for Pin:");
        Serial.print(15);
        Serial.print("  old State:");
        Serial.print(pinmap[15].state);        
        Serial.print("  Current State:");
        Serial.print(input.p15); 
        pinmap[15].btnnumber.setValue(input.p15);   pinmap[15].state=input.p15; }
    
    if(invert_pin_10_12){
        uint8_t inverted= input.p10 ? 0 : 1;
        if(inverted != pinmap[10].state){  pinmap[10].btnnumber.setValue(inverted);   pinmap[10].state=inverted; }
        
        inverted= input.p12 ? 0 : 1;
        if(inverted != pinmap[12].state){  pinmap[12].btnnumber.setValue(inverted);   pinmap[12].state=inverted; }
    }
    else{    
        if(input.p10 != pinmap[10].state){  pinmap[10].btnnumber.setValue(input.p10);   pinmap[10].state=input.p10; } 
        if(input.p12 != pinmap[12].state){  pinmap[12].btnnumber.setValue(input.p12);   pinmap[12].state=input.p12; }
    }
}

void ProcessPins()
{
    struct DigitalInput inputs=  digitalReadAll(MegaPinMap1);
    ActOnPinStatusChange(inputs, MegaPinMap1, false);
    inputs=  digitalReadAll(MegaPinMap2);
    ActOnPinStatusChange(inputs, MegaPinMap2, true);

    for(int pin_index=0;pin_index<GP_MAX_BUTTON;pin_index++){
      bool statechaged=false;
      int buttonState =digitalRead(GPMap[pin_index].pin);
      if(buttonState != GPMap[pin_index].state){
        Serial.println("");
        Serial.print("Update for Pin:");
        Serial.print(pin_index);
        Serial.print("  old State:");
        Serial.print(GPMap[pin_index].state);        
        Serial.print("  Current State:");
        Serial.print(buttonState);        
        GPMap[pin_index].btnnumber.setValue(buttonState);
        GPMap[pin_index].state=buttonState;
        statechaged=true;  
      }
      if(statechaged)
        SetStatusText();
    }  
}

static unsigned long EStopDelay=0;
#define ESTOP_DELAY 600
static int estop_state =0;
#define ESTOP_PIN 3

void EStop_Effect(){
   int buttonState = digitalRead(GPMap[ESTOP_PIN].pin);
   if(buttonState==0){
        if(EStopDelay==0){
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


NexNumber  btnnumber0p1 = NexNumber(2,4,"bt_2_ei0_p1_00");
NexNumber  btnnumber1p1 =NexNumber(2,5,"bt_2_ei1_p1_01");
NexNumber  btnnumber2p1 = NexNumber(2,6,"bt_2_ei2_p1_02");
NexNumber  btnnumber3p1 = NexNumber(2,7,"bt_2_ei3_p1_03");
NexNumber  btnnumber4p1 = NexNumber(2,8,"bt_2_ei4_p1_04");
NexNumber  btnnumber5p1 = NexNumber(2,9,"bt_2_ei5_p1_05");
NexNumber  btnnumber6p1 = NexNumber(2,10,"bt_2_ei6_p1_06");
NexNumber  btnnumber7p1 = NexNumber(2,11,"bt_2_ei7_p1_07");
NexNumber  btnnumber8p1 = NexNumber(2,12,"bt_2_ei8_p1_08");
NexNumber  btnnumber9p1 = NexNumber(2,13,"bt_2_ei9_p1_09");
NexNumber  btnnumber10p1 = NexNumber(2,14,"bt_2_eo0_p1_10");
NexNumber  btnnumber11p1= NexNumber(2,15,"bt_2_eo1_p1_11");
NexNumber  btnnumber12p1 = NexNumber(2,16,"bt_2_eo2_p1_12");
NexNumber  btnnumber13p1 = NexNumber(2,17,"bt_2_eo3_p1_13");
NexNumber  btnnumber14p1 = NexNumber(2,18,"bt_2_eo4_p1_14");
NexNumber  btnnumber15p1 = NexNumber(2,19,"bt_2_eo5_p1_15");

NexNumber  btnnumber0p2 = NexNumber(2,20,"bt_2_eo6_p2_00");
NexNumber  btnnumber1p2 =NexNumber(2,21,"bt_2_eo7_p2_01");
NexNumber  btnnumber2p2 =NexNumber(2,22,"bt_2_eo8_p2_02");
NexNumber  btnnumber3p2 =NexNumber(2,23,"bt_2_eo9_p2_03");
NexNumber  btnnumber4p2 =NexNumber(1,2,"bt_1_st0_p2_04");
NexNumber  btnnumber5p2 =NexNumber(1,3,"bt_1_sp1_p2_05");
NexNumber  btnnumber6p2 =NexNumber(1,4,"bt_1_di2_p2_06");
NexNumber  btnnumber7p2 =NexNumber(1,5,"bt_1_di3_p2_07");
NexNumber  btnnumber8p2 =NexNumber(1,6,"bt_1_di4_p2_08");
NexNumber  btnnumber9p2 =NexNumber(1,7,"bt_1_di5_p2_09");
NexNumber  btnnumber10p2 =NexNumber(1,14,"bt_1_mp0_p2_10");
NexNumber  btnnumber11p2 =NexNumber(1,8,"bt_1_do1_p2_11");
NexNumber  btnnumber12p2 =NexNumber(1,9,"bt_1_do2_p2_12");
NexNumber  btnnumber13p2 =NexNumber(1,10,"bt_1_do3_p2_13");
NexNumber  btnnumber14p2 =NexNumber(1,11,"bt_1_do4_p2_14");
NexNumber  btnnumber15p2 =NexNumber(1,12,"bt_1_do5_p2_15");

NexNumber  btnnumber0gp =NexNumber(1,17,"bt_1_bi0_uc_34");
NexNumber  btnnumber1gp =NexNumber(1,18,"bt_1_bi1_uc_35");
NexNumber  btnnumber2gp =NexNumber(1,19,"bt_1_bi2_uc_36");
NexNumber  btnnumber3gp =NexNumber(1,22,"bt_1_emg_uc_32");

void PinToDisplay(){
    struct DigitalInput inputs=  digitalReadAll(MegaPinMap1);
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

    inputs=  digitalReadAll(MegaPinMap2);
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

    btnnumber0gp.setValue(digitalRead(GPMap[0].pin));
    btnnumber1gp.setValue(digitalRead(GPMap[1].pin));
    btnnumber2gp.setValue(digitalRead(GPMap[2].pin));
    btnnumber3gp.setValue(digitalRead(GPMap[3].pin));

    SetStatusText();
}

void TestPins(){

NexNumber  btnnumber8 =NexNumber(2,12,"bt_2_ei8_p1_08");
NexNumber  btnnumber9 =NexNumber(2,12,"bt_2_ei9_p1_09");
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
