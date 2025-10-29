
#include <Arduino.h>
#include "RTClib.h"


int Current_Page=1;

#define TIME_OUT 2000

NexText Page3_txt_getDate = NexText(3, 1, "setDate");
NexText Page3_txt_getTime = NexText(3, 15, "setTime");

NexRtc nex_rtc;

NexButton Page3_bt_setDate = NexButton(3, 20, "b14");
NexButton Page3_bt_setTime = NexButton(3, 21, "b15");

NexText Page1_displayDate = NexText(1, 15, "p1_dispDate");
NexText Page1_displayTime = NexText(1, 16, "p1_dispTime");

NexText Page2_displayDate = NexText(2, 14, "p2_dispDate");
NexText Page2_displayTime = NexText(2, 15, "p2_dispTime");

NexText Page3_displayDate = NexText(3, 23, "p3_dispDate");
NexText Page3_displayTime = NexText(3, 22, "p3_dispTime");

char buffer[100] = {0};
const char *formattedDate;
const char *formattedTime;

uint8_t isDayValid = 0, isMonthValid = 0, isYearValid = 0;
uint8_t isHourValid = 0, isMinuteValid = 0, isSecondValid = 0;

long timeControl = 0;

RTC_DS1307 rtc;


/*
 * Button component pop callback function. 
 * In this example,the button's text value will plus one every time when it is released. 
 */
void setDatePopCallback(void *ptr)
{
  char storeDateBuffer[25];
  Page3_txt_getDate.getText(storeDateBuffer, sizeof(storeDateBuffer));

  int8_t day = -1;
  int16_t month = -1;
  int16_t year = -1;

  char *extractDate;
  extractDate = strtok(storeDateBuffer, "/");

  while (extractDate != NULL)
  {
    uint16_t num = atoi(extractDate);
    if (day == -1)
    {
      if (num > 0 && num <= 31)
      {
        isDayValid = 1;
        day = num;
      }
      else
      {
        isDayValid = 0;
      }
    }

    else if (month == -1)
    {
      if (num >= 1 && num <= 12)
      {
        isMonthValid = 1;
        month = num;
      }
      else
      {
        isMonthValid = 0;
      }
    }

    else if (year == -1)
    {
      if (num >= 2000 && num <= 2099)
      {
        isYearValid = 1;
        year = num;
      }
      else
      {
        isYearValid = 0;
      }
    }
    extractDate = strtok(NULL, "/");
  }

  if ((isDayValid && isMonthValid && isYearValid) == 1)
  {
    DateTime time = rtc.now();

    rtc.adjust(DateTime((uint16_t)year, (uint16_t)month, (uint8_t)day, time.hour(), time.minute(), time.second()));

    time = rtc.now();

    String putDate = time.timestamp(DateTime::TIMESTAMP_DATE);
    const char *dispDate = putDate.c_str();
    Page1_displayDate.setText(dispDate);
    Page2_displayDate.setText(dispDate);

    Page3_txt_getDate.setText("DONE");

    isDayValid = 0;
    isMonthValid = 0;
    isYearValid = 0;
  }
  else
  {

    Page3_txt_getDate.setText("ERROR");

    Page1_displayDate.setText(formattedDate);
    Page2_displayDate.setText(formattedDate);
  }
}

void setTimePopCallback(void *ptr)
{

  char storeTimeBuffer[30];
  Page3_txt_getTime.getText(storeTimeBuffer, sizeof(storeTimeBuffer));

  int8_t hours = -1;
  int8_t minutes = -1;
  int8_t seconds = -1;

  char *extractTime;
  extractTime = strtok(storeTimeBuffer, ":");

  while (extractTime != NULL)
  {
    uint8_t num = atoi(extractTime);
    // Serial.println(num);
    if (hours == -1)
    {
      if (num > 0 && num <= 24)
      {
        isHourValid = 1;
        hours = num;
      }
      else
      {
        isHourValid = 0;
      }
    }

    else if (minutes == -1)
    {
      if (num >= 00 && num <= 59)
      {
        isMinuteValid = 1;
        minutes = num;
      }
      else
      {
        isMinuteValid = 0;
      }
    }

    else if (seconds == -1)
    {
      if (num >= 00 && num <= 59)
      {
        isSecondValid = 1;
        seconds = num;
      }
      else
      {
        isSecondValid = 0;
      }
    }
    extractTime = strtok(NULL, ":");
  }

  if ((isHourValid && isMinuteValid && isSecondValid) == 1)
  {
    DateTime time = rtc.now();

    rtc.adjust(DateTime(time.year(), time.month(), time.day(), (uint8_t)hours, (uint8_t)minutes, (uint8_t)seconds));

    time = rtc.now();

    String putTime = time.timestamp(DateTime::TIMESTAMP_TIME);
    const char *dispTime = putTime.c_str();
    Page1_displayTime.setText(dispTime);
    Page2_displayTime.setText(dispTime);

    Page3_txt_getTime.setText("DONE");

    isHourValid = 0;
    isMinuteValid = 0;
    isSecondValid = 0;
  }
  else
  {

    Page3_txt_getTime.setText("ERROR");

    Page1_displayTime.setText(formattedTime);
    Page2_displayTime.setText(formattedTime);
  }
}

void InitializeRTC()
{

  if (!rtc.begin())
  {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    abort();
  }

  if (!rtc.isrunning())
  {
    Serial.println("RTC is NOT running, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

  /* Register the pop event callback function of the current button component. */
  Page3_bt_setDate.attachPop(setDatePopCallback, &Page3_bt_setDate);
  Page3_bt_setTime.attachPop(setTimePopCallback, &Page3_bt_setTime);
}

#define RTC_TIMEOUT 1000
volatile uint16_t rct_counter=0;

void RTCHandler()
{
  if(rct_counter == RTC_TIMEOUT){
    DateTime time = rtc.now();
    char dt[16];
    sprintf(dt, "%02d/%02d/%02d", time.day(),time.month(),time.year());
    String timeStamp = time.timestamp(DateTime::TIMESTAMP_TIME);
    formattedTime = timeStamp.c_str();
    
    Page1_displayDate.setText(dt);
    Page1_displayTime.setText(formattedTime);
    Page2_displayDate.setText(dt);
    Page2_displayTime.setText(formattedTime);
    Page3_displayDate.setText(dt);
    Page3_displayTime.setText(formattedTime);

    rct_counter=0;
  }
  rct_counter++;
}