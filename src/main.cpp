
#include "PinChangeInterrupt.h"

const int       ONOFF_INPUT = A0; //R9DS에서 6채널의 펄스를 받아드릴 핀 
unsigned long   ONOFF_PulseWidth = 0;  //펄스폭 저장
unsigned long   ONOFF_StartHigh = 0;   //처음 high가 들어온 시간 저장
bool            ONOFF_NewPulse = 0;   // PulseWidth값이 바뀐경우 1,아닌경우 0으로 저장


const int       BRIGHT_INPUT = A1; // 3채널
unsigned long   BRIGHT_PulseWidth = 0;
unsigned long   BRIGHT_StartHigh = 0;
bool            BRIGHT_NewPulse = 0;


const int       RGB_INPUT = A2; // 5채널
unsigned long   RGB_PulseWidth= 0;
unsigned long   RGB_StartHigh= 0;
bool            RGB_NewPulse = 0;


const int       LED_PIN[3] = {6,5,3}; // pwm기능을 지원하는 핀을 삼색led rgb에 연결
bool            LED_ON = false; // 6채널값에 따라 변경되는 on/off상태를 저장할 변수
float           LED_BRIGHT = 255.0; // 3채널 값에 따라 변경되는 밝기를 저장할 변수
bool            LED_STATE[3]; // 5채널 값에 따라 선택되는 r,g,b값을 저장할 변수


void Pulse_calculate(const int& CH,unsigned long& StartHigh,bool& NewPulse,unsigned long& PulseWidth){
  
  if (digitalRead(CH) == HIGH){ //펄스가 low에서 high로 바뀌는 순간에 동작합니다. 
    StartHigh = micros(); // 현재 시간을 저장합니다.
    return; // low->high 변경될 때 발생되는 인터럽트를 종료합니다.
  }   
  
  // 펄스가 high->low로 변결될때 동작합니다.
  if( NewPulse == false){ //메인 loop에서  NewPulse를 false로 바꿔준 경우에만 작동합니다.
      PulseWidth = (unsigned long)(micros() - StartHigh); // 이전 low->high변화가 발생할때 시간과 현재 시간의 차이를 통해서 펄스폭을 계산합니다.
      StartHigh = 0; // 이후 계산을 위해 계산에 사용되었던 low->high가 변경될때의 시간을 초기화 합니다.
      NewPulse = true; // PulseWidth가 갱신되었음을 표시합니다.
  }
  
}

//각 6,3,5채널의 인터럽트 발생시 호출되는 함수로  채널에 해당하는 파라미터로 펄스폭 계산함수를 호출합니다.

void ONOFF(){    Pulse_calculate(ONOFF_INPUT,ONOFF_StartHigh,ONOFF_NewPulse,ONOFF_PulseWidth);}
void BRIGHT(){   Pulse_calculate(BRIGHT_INPUT,BRIGHT_StartHigh,BRIGHT_NewPulse,BRIGHT_PulseWidth);}
void RGB(){      Pulse_calculate(RGB_INPUT,RGB_StartHigh,RGB_NewPulse,RGB_PulseWidth);}


void setup(){

  //R9DS에서 나오는 신호를 입력으로 받는 핀을 설정합니다.
    pinMode(ONOFF_INPUT,INPUT_PULLUP);
    pinMode(BRIGHT_INPUT,INPUT_PULLUP);
    pinMode(RGB_INPUT,INPUT_PULLUP);
  
  //CHANGE설정을 사용해 각 채널의 신호가 high->low,low->high로 변경될 때 인터럽트를 발생시켜 펄스폭을 계산하도록 합니다.
    attachPCINT(digitalPinToPCINT(ONOFF_INPUT), ONOFF, CHANGE);
    attachPCINT(digitalPinToPCINT(BRIGHT_INPUT), BRIGHT, CHANGE);
    attachPCINT(digitalPinToPCINT(RGB_INPUT), RGB, CHANGE);

}


void loop(){

  if( ONOFF_NewPulse  == true ){ // 6채널의 펄스가 새롭게 계산되었을때 실행

    if(ONOFF_PulseWidth > 1500){ //6채널에서 설정할 수 있는 두 개 펄스폭 중 1100,1900 중에 1900인 경우
      LED_ON = true; // LED를 ON합니다.
    }     
    else{ // 펄스폭이 1100인 경우
      LED_ON = false; // LED를 OFF합니다.
      // 모든 LED를 끄고 LED를 OFF상태로 만듭니다.
      analogWrite(LED_PIN[0],0);
      analogWrite(LED_PIN[1],0);
      analogWrite(LED_PIN[2],0);
    }                         
    
    ONOFF_NewPulse = false; // 변경 펄스 값을 사용했음을 표시합니다.
  }
  
  
  if(LED_ON){ //3채널에서 LED를 켰을 경우에만 실행됩니다.

    if( BRIGHT_NewPulse  == true){// 3채널의 펄스가 새롭게 계산되었을때 실행
      
      //3채널의 펄스폭은 최소 1300에서 최대 1950이라 그 값을 LED 값을 제어하는데 필요한 0 ~255에 값으로 매핑합니다. 
      LED_BRIGHT = ((float)BRIGHT_PulseWidth - 1300.0) /2.5;           
      
      //매핑하여 구한 값이 0~255를 벗어나는 범위라면 범위내의 값으로 재설정합니다.
      if(LED_BRIGHT < 0.0)  LED_BRIGHT = 0.0;
      else if (LED_BRIGHT>255.0) LED_BRIGHT =255.0;

      BRIGHT_NewPulse  = false;// 변경 펄스 값을 사용했음을 표시합니다.
    }
  
    if( RGB_NewPulse  == true){// 5채널의 펄스가 새롭게 계산되었을때 실행
      //우선 값들을 모두 off로 초기화 합니다.
      LED_STATE[0]=LED_STATE[1]=LED_STATE[2]=0; 

      //5채널에서 선택가능한 펄스폭이 1100,1500,1900이 있을때,
      if(RGB_PulseWidth<1250)      LED_STATE[0]=1; //1100인 경우 red만 on
      else if(RGB_PulseWidth<1750) LED_STATE[1]=1; //1500인 경우 green만 on
      else                         LED_STATE[2]=1; // 1900인 경우 blue만 on
      RGB_NewPulse  = false;// 변경 펄스 값을 사용했음을 표시합니다.
    }

    //3번 채널에서 설정한 밝기값과 5채널에서 설정한 r,g,b의 개별 on/off상태를 곱해서 3,5,6핀의 출력으로 내보냅니다.
    analogWrite(LED_PIN[0],(int)(LED_STATE[0]*LED_BRIGHT));
    analogWrite(LED_PIN[1],(int)(LED_STATE[1]*LED_BRIGHT));
    analogWrite(LED_PIN[2],(int)(LED_STATE[2]*LED_BRIGHT));
  }
  
}

