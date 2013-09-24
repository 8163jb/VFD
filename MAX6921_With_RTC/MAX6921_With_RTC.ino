#include <RTCdue.h>
#include <Scheduler.h>

#define  dataPin  7
#define  clockPin  4
#define  loadPin  8

RTC_DS1307 rtc;

unsigned long nowMicros=0;
unsigned long ulRefreshcounter;

volatile byte _mask[9][3] = {
  {B00000000,B00001000,B00000000},
  {B00000000,B00010000,B00000000},
  {B00000000,B00100000,B00000000},
  {B00000000,B01000000,B00000000},
  {B00000000,B10000000,B00000000},
  {B00000001,B00000000,B00000000},
  {B00000010,B00000000,B00000000},
  {B00000100,B00000000,B00000000},
  {B00001000,B00000000,B00000000}
};

volatile byte _data[9][3] = {
  {B00000000,B00000000,B00000000},
  {B00000000,B00000000,B00000000},
  {B00000000,B00000000,B00000000},
  {B00000000,B00000000,B00000000},
  {B00000000,B00000000,B00000000},
  {B00000000,B00000000,B00000000},
  {B00000000,B00000000,B00000000},
  {B00000000,B00000000,B00000000},
  {B00000000,B00000000,B00000000}
};

byte _numbers[10][3] = {
  {B00000000,B00000111,B11100000},
  {B00000000,B00000000,B01100000},
  {B00000000,B00000110,B11010000},
  {B00000000,B00000111,B10010000},
  {B00000000,B00000011,B00110000},
  {B00000000,B00000101,B10110000},
  {B00000000,B00000101,B11110000},
  {B00000000,B00000111,B00000000},
  {B00000000,B00000111,B11110000},
  {B00000000,B00000111,B10110000},
};

byte _blank[3] = {B00000000,B00000000,B00000000};

byte _dash[3] = {B00000000,B00000000,B00010000};

void setup()
{
  rtc.begin();
  rtc.adjust(DateTime(__DATE__, __TIME__));
  pinMode(loadPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
}

void loop()
{
  nowMicros=micros();
  if ((nowMicros-ulRefreshcounter)>1000)
  {
    refresh(); 
    ulRefreshcounter=nowMicros;
  }
  DateTime now = rtc.now();
  int hr = now.hour();
  int mn = now.minute();
  int sc = now.second();
  for(int i = 0; i < 3; i++)
  {
    _data[1][i] = _numbers[hr / 10][i];
    _data[2][i] = _numbers[hr % 10][i];
    _data[3][i] = _dash[i];
    _data[4][i] = _numbers[mn / 10][i];
    _data[5][i] = _numbers[mn % 10][i];
    _data[6][i] = _dash[i];
    _data[7][i] = _numbers[sc / 10][i];
    _data[8][i] = _numbers[sc % 10][i];
  }
}

void refresh() 
{ 
  for(int a = 0; a < 9; a++)
  {
    long data = 0;
    long mask = 0;
    long out = 0;
    
    digitalWrite(clockPin, LOW); 
    digitalWrite(loadPin, LOW); 
    
    for(int b = 0; b < 3; b++)
    {  
      data = (data << 8) + _data[a][b];
    }
      
    for(int b = 0; b < 3; b++)
    {  
      mask = (mask << 8) + _mask[a][b];
    }
    
    out = mask | data;
    
    for(int i = 0; i < 20; i++)
    {
      digitalWrite(clockPin, 0);
      digitalWrite(dataPin, (out & (1 << i)));
      digitalWrite(clockPin, 1);
    }
    
    digitalWrite(loadPin, HIGH);
    digitalWrite(loadPin, LOW);
  }
}
  
   

