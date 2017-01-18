#include <VirtualWire.h>
//#include <EEPROM.h>


enum ComType {none, transmitter, reciever, transiever};
#define BPS 5000 //Bits per second

class comlib{

  private:
    int transmitter_pin=0;
    int reciever_pin=0;
    ComType comtype = none;
    int Address=0;
    uint8_t buf[VW_MAX_MESSAGE_LEN];
    uint8_t buflen = VW_MAX_MESSAGE_LEN;


    void setup_reciever(){ //if not standalone run setup_emitter() first
      vw_set_rx_pin(reciever_pin);
      vw_setup(BPS);// Bits per sec
      vw_rx_start();
    }

    void setup_transmitter(bool standalone=false){
      vw_set_tx_pin(transmitter_pin);
      if(standalone) vw_setup(BPS); // Bits per sec
    }

    bool isUDP(uint8_t buf){ // no reliable data connection aka no tcp
      if(buf&192==0)
        return true;
      return false;
    }

    uint8_t makeUDP(){
      return 0;
    }
    
    uint8_t Dadd(uint8_t buf){
      return buf&127;
    }
    
    uint8_t Sadd(uint8_t* buf){ // sender address
      uint8_t add=buf[0]&63;
      add<<=1;
      add|=(buf[1]>>7);
      return add;
    }
    
    void makeSadd(uint8_t* buf){ // sender address
      buf[0]|=(ADD>>1);
      buf[1]|=(ADD<<7);
      return;
    }


  public:

    comlib(ComType comtype,int pin, int address){


    }


    comlib(ComType _comtype, int transmitterPin, int recieverPin, int address){


    }


    bool hasMessage(){
      return vw_have_message(); //get and check if udp
    }


    char* getMessage(){



    }


    bool sendMessage(char* buff, uint8_t length, uint8_t address, bool wait){
      if(buflen-2<length){
        return false; //meassage too long
      }
      buf[0]=buf[1]=0;
      buf[0]=makeUDP();
      makeSadd(buf);
      buf[1]|=Dadd(address); // reciever address
      for(int i=0; i>length;i++){
        buf[i+2]=buff[i]
      }
    }


    bool makeConnection(int address){


    }

    bool hasMessageOnConnection(){


    }


    char* getMessageOnConnection(){



    }


    bool sendMessageOnConnection(char* buff, int length, int address){


    }

    bool connectionActive(){

    }


    bool endConnection(){



    }


};



void loop()
{
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;
  if(read_state==0){
    Serial.println("Please enter red intensity in a value 0-256");
    read_state++;
  }
  if(Serial.available()){
    if(read_state==1){
      red=Serial.parseInt();
      Serial.println("Please enter green intensity in a value 0-256");
      read_state++;
    }else if(read_state==2){
      green=Serial.parseInt();
      Serial.println("Please enter blue intensity in a value 0-256");
      read_state++;
    }else if(read_state==3){
      blue=Serial.parseInt();
      Serial.println("Please enter (de)activation value or store command (32/)128 or 64");
      read_state++;
    }else if(read_state==4){
      status=Serial.parseInt();
      read_state++;
    } 
  }
  if(read_state==5){
      Serial.println("Preparing and sending data burst.");

      buf[2]=status;
      buf[3]=red;
      buf[4]=green;
      buf[5]=blue;
      digitalWrite(led_pin, HIGH);
      vw_send((uint8_t *)buf, 6);
      vw_wait_tx(); 
      digitalWrite(led_pin, LOW);
      read_state=0;
    } 
  delay(100);
  //digitalWrite(send_pin,HIGH);
  //delay(100);
  //digitalWrite(send_pin,LOW);
}

#include <VirtualWire.h>
#include <EEPROM.h>

const int led_pin = 13;
const int receive_pin = 8;
#define ADD 1  //loacal address
#define STATUS 0
#define RED 1
#define GREEN 2
#define BLUE 3
#define RED_PIN 3
#define GREEN_PIN 5
#define BLUE_PIN 6



void setup(){

    pinMode(led_pin, OUTPUT);
    pinMode(RED_PIN, OUTPUT);
    pinMode(GREEN_PIN, OUTPUT);
    pinMode(BLUE_PIN, OUTPUT);
    digitalWrite(led_pin, LOW);
    if(EEPROM.read(STATUS)==1){
      digitalWrite(led_pin, HIGH);
      analogWrite(RED_PIN, EEPROM.read(RED));
      analogWrite(GREEN_PIN, EEPROM.read(GREEN));
      analogWrite(BLUE_PIN, EEPROM.read(BLUE));
    }
    //analogWrite(RED_PIN, 25);
    Serial.begin(9600);
    Serial.println("Start");
}


void loop()
{
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;
  if(vw_have_message()){
    digitalWrite(led_pin, HIGH);
    Serial.println("got here");
    if(vw_get_message(buf, &buflen)){
     if(buflen==6 && isUDP(buf[0]) && Dadd(buf[1])==ADD){
      for( int i; i<6;i++){
        Serial.println(buf[i]);
      }
      Serial.println("Continues");
      if((buf[2]&64)!=0){
        Serial.println("store");
        EEPROM.write(RED,buf[3]);
        EEPROM.write(GREEN,buf[4]);
        EEPROM.write(BLUE,buf[5]);
      }
      if((buf[2]&128)!=0){
        Serial.println("on");
        EEPROM.write(STATUS,1);
        digitalWrite(led_pin, HIGH);
        analogWrite(RED_PIN, EEPROM.read(RED));
        analogWrite(GREEN_PIN, EEPROM.read(GREEN));
        analogWrite(BLUE_PIN, EEPROM.read(BLUE));
      }
      if((buf[2]&32)!=0){
        Serial.println("off");
        EEPROM.write(STATUS,0);
        digitalWrite(led_pin, LOW);
        digitalWrite(RED_PIN, LOW);
        digitalWrite(GREEN_PIN, LOW);
        digitalWrite(BLUE_PIN, LOW);
      }
     }
    }
  }
}