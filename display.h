#define MAX_BUTTON  16
#define GP_MAX_BUTTON  4

__attribute__((packed)) struct sPinInfo{
  NexNumber  btnnumber;
  int pin; 
  int state;
  } MegaPinMap1[MAX_BUTTON]={
    { NexNumber(2,4,"bt_2_ei0_p1_00"), 60 , 0},
    { NexNumber(2,5,"bt_2_ei1_p1_01"), 61 , 0},
    { NexNumber(2,6,"bt_2_ei2_p1_02"), 62 , 0},
    { NexNumber(2,7,"bt_2_ei3_p1_03"), 63 , 0},
    { NexNumber(2,8,"bt_2_ei4_p1_04"), 64 , 0},
    { NexNumber(2,9,"bt_2_ei5_p1_05"), 65 , 0},
    { NexNumber(2,10,"bt_2_ei6_p1_06"), 66 , 0},
    { NexNumber(2,11,"bt_2_ei7_p1_07"), 67 , 0},
    { NexNumber(2,12,"bt_2_ei8_p1_08"), 68 , 0},
    { NexNumber(2,13,"bt_2_ei9_p1_09"), 69 , 0},
    { NexNumber(2,14,"bt_2_eo0_p1_10"), 30 , 0},
    { NexNumber(2,15,"bt_2_eo1_p1_11"), 31 , 0},
    { NexNumber(2,16,"bt_2_eo2_p1_12"), 32 , 0},
    { NexNumber(2,17,"bt_2_eo3_p1_13"), 33 , 0},
    { NexNumber(2,18,"bt_2_eo4_p1_14"), 34 , 0},
    { NexNumber(2,19,"bt_2_eo5_p1_15"), 35 , 0}
  },
  MegaPinMap2[MAX_BUTTON]={
    { NexNumber(2,20,"bt_2_eo6_p2_00"), 36 , 0},
    { NexNumber(2,21,"bt_2_eo7_p2_01"), 37 , 0},
    { NexNumber(2,22,"bt_2_eo8_p2_02"), 40 , 0},
    { NexNumber(2,23,"bt_2_eo9_p2_03"), 41 , 0},
    { NexNumber(1,2,"bt_1_st0_p2_04"), 54 , 0},
    { NexNumber(1,3,"bt_1_sp1_p2_05"), 55 , 0},
    { NexNumber(1,4,"bt_1_di2_p2_06"), 56 , 0},
    { NexNumber(1,5,"bt_1_di3_p2_07"), 57 , 0},
    { NexNumber(1,6,"bt_1_di4_p2_08"), 58 , 0},
    { NexNumber(1,7,"bt_1_di5_p2_09"), 59 , 0},
    { NexNumber(1,14,"bt_1_mp0_p2_10"), 22 , 0},
    { NexNumber(1,8,"bt_1_do1_p2_11"), 23 , 0},
    { NexNumber(1,9,"bt_1_do2_p2_12"), 24 , 0},
    { NexNumber(1,10,"bt_1_do3_p2_13"), 25 , 0},
    { NexNumber(1,11,"bt_1_do4_p2_14"), 26 , 0},
    { NexNumber(1,12,"bt_1_do5_p2_15"), 27 , 0}
  },
  GPMap[GP_MAX_BUTTON]={
    { NexNumber(1,17,"bt_1_bi0_uc_34"), 48 , 0},
    { NexNumber(1,18,"bt_1_bi1_uc_35"), 47 , 0},
    { NexNumber(1,19,"bt_1_bi2_uc_36"), 46 , 0},  
    { NexNumber(1,22,"bt_1_emg_uc_32"), 49 , 0}
  };
 
NexText tx_1_ls_out =  NexText(1,21,"tx_1_ls_out");
NexButton b_1_rst_uc_25= NexButton(1,23,"b_1_rst_uc_25");
NexNumber bt_1_rdy_uc_26 = NexNumber(1,1,"bt_1_rdy_uc_26");
NexNumber bt_1_las_ok = NexNumber(1,13,"bt_1_las_ok");

NexButton b_pg1_nxt = NexButton(1,21,"b_pg1_nxt");
NexButton b_pg2_back = NexButton(2,3,"b_pg2_back");
NexButton b_pg2_nxt = NexButton(2,2,"b_pg2_nxt");
NexButton b_pg3_back = NexButton(3,3,"b_pg3_back");
NexButton b_pg3_home = NexButton(3,2,"b_pg3_home");

NexText p1_tx =  NexText(1,25,"p1_txt");
NexText p2_tx =  NexText(2,27,"p2_txt");
NexText p3_tx =  NexText(3,25,"p3_txt");

NexPage page1    = NexPage(1, 0, "homepage"); 
NexPage page2    = NexPage(2, 0, "Expage"); 
NexPage page3    = NexPage(3, 0, "setting"); 

#define RESET_PIN 45
#define READY_PIN 44


enum STATUS_MAP{
  NORMAL_OPERATION = 1,
  TEMP_ALARM = 0,
  POWER_ALARM = 4,
  EMI_NOT_READY = 5,
  BACK_REFLECTION = 2,
  SYSTEM_ALARAM = 3
}eStatusMap;


  struct DigitalInput {
    uint8_t p0;
    uint8_t p1;
    uint8_t p2;
    uint8_t p3;
    uint8_t p4;
    uint8_t p5;
    uint8_t p6;
    uint8_t p7;
    uint8_t p8;
    uint8_t p9;
    uint8_t p10;
    uint8_t p11;
    uint8_t p12;
    uint8_t p13;
    uint8_t p14;
    uint8_t p15;
  } ;


struct DigitalInput digitalReadAll(struct sPinInfo pinmap[]){    
    struct DigitalInput input;
    input.p0= digitalRead(pinmap[0].pin);
    input.p1= digitalRead(pinmap[1].pin);
    input.p2= digitalRead(pinmap[2].pin);
    input.p3= digitalRead(pinmap[3].pin);
    input.p4= digitalRead(pinmap[4].pin);
    input.p5= digitalRead(pinmap[5].pin);
    input.p6= digitalRead(pinmap[6].pin);
    input.p7= digitalRead(pinmap[7].pin);
    input.p8= digitalRead(pinmap[8].pin);
    input.p9= digitalRead(pinmap[9].pin);
    input.p10= digitalRead(pinmap[10].pin);
    input.p11= digitalRead(pinmap[11].pin);
    input.p12= digitalRead(pinmap[12].pin);
    input.p13= digitalRead(pinmap[13].pin);
    input.p14= digitalRead(pinmap[14].pin);
    input.p15= digitalRead(pinmap[15].pin);
    return input;
}