  
int fstones[] = {
  185, 207, 233, 247, 277, 311, 349, 370
};

//設定各接腳
int latchPin = 13;
int clockPin = 12;
int dataPin = 11;
int tonePin = 2;
int modePin = 17;
int below = 18;
int above = 19;

byte colData[8] = {
   B11111110,
   B11111101,
   B11111011,
   B11110111,
   B11101111,
   B11011111,
   B10111111,
   B01111111
}; //掃描

byte rowData[8] = {0};
       

byte presentState[]={1,1,1,1,1,1,1,1};
byte previousState[8];

byte presentStateUtil[]={1,1,1};
byte previousStateUtil[3];

int oct=0;
void Sound(){
  for(int i=0;i<8;i++){
    if( presentState[i]==LOW){
      if(oct>=0)tone(tonePin,fstones[i]<<oct);
      else tone(tonePin,fstones[i]>>-oct);
      return;
    }
  }
  noTone(tonePin);
}

int count=0;
void LED0(){
  for(int i=0;i<8;i++){
    if( presentState[i]==LOW&&previousState[i]==HIGH){
      if(count==8){
        ClearBoard();
      }
      rowData[i]+=1<<(7-count);
      count++;
    }
  }
}
void LED1(){
  for(int i=0;i<8;i++){
    if( presentState[i]==LOW&&previousState[i]==HIGH){
      for(int i=0;i<8;i++) rowData[i]=rowData[i]>>1;
      rowData[i]+=128;
    }
  }
}
void ClearBoard(){
  for(int i=0;i<8;i++) rowData[i]=0;
  count=0;
}

void setup() {
  pinMode(latchPin,OUTPUT);
  pinMode(clockPin,OUTPUT);
  pinMode(dataPin,OUTPUT);
  pinMode(modePin,INPUT_PULLUP);
  pinMode(above,INPUT_PULLUP);
  pinMode(below,INPUT_PULLUP);

  for(byte i=3;i<11;i++){
  	pinMode(i,INPUT_PULLUP);
  }

  delay(10);
}
int mode=0;
void loop() {
  for(int i=0;i<8;i++){
    previousState[i]=presentState[i];
    presentState[i]=digitalRead(i+3);
  }
  for(int i=0;i<3;i++){
    previousStateUtil[i]=presentStateUtil[i];
    presentStateUtil[i]=digitalRead(17+i);
  }
  if(presentStateUtil[0]==LOW&&previousStateUtil[0]==HIGH){
    mode++;
    ClearBoard();
    if(mode>1)mode=0;
  }
  if(presentStateUtil[1]==LOW&&previousStateUtil[1]==HIGH) oct--;
  if(presentStateUtil[2]==LOW&&previousStateUtil[2]==HIGH) oct++;
  Sound();
  
  if(mode==0)LED0();
  else LED1();
  
  for(int i = 0; i < 8 ; i++){
    digitalWrite(latchPin,LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, colData[i]);
    shiftOut(dataPin, clockPin, LSBFIRST, rowData[i]);
    digitalWrite(latchPin,HIGH);
    delay(7);
  }
}
