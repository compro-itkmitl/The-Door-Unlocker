#include <SD.h>
#include <SPI.h>


//======== Define port=============
#define vib_pin  //vibration sensor
#define tone_pin  //buzzer
#define MOSI 11
#define MISO 12
#define CLK 13
#define data1 A0
#define cs //

File myFile;

int id=0, dataNum=2;
//int dat1=A0/*, dat2=A1*/;
bool  First_round = 1;
String data[20] = {""} ,temp[20] = {""};

String fileName = "test.txt";
String strmyFile = "";


void setup(){
	//setup pin

	pinMode(tone_pin, OUTPUT);

	pinMode(cs, INPUT);
	pinMode(dat1,INPUT);
	pinMode(vib_pin, INPUT);
	pinMode(MOSI, INPUT);
	pinMode(MISO, INPUT);
	pinMode(CLK, INPUT);
	pinMode(data1, INPUT);
//    pinMode(dat2,INPUT);

    Serial.begin(9600);
    while (!Serial) {
        ;
    }
    //checking sd card before enter the void loop.
    checkSD(); 
    //==============label part===================
    myFile = SD.open(fileName, FILE_WRITE);
    if (myFile) {
        myFile.println("ID,BLETH,KBNCK_PAT");
        myFile.close();
    }
    //====check id====
    //==============convert text in .txt in be string==========
    while (myFile.available()) {
        char ltr = myFile.read();
        strmyFile += ltr;
    }
    //=====================================
}


void loop()
{
    if(is_insystem()){//bluetooth func available

        for (int i = 0; i < count; ++i)
        {
            if(ID == selectBLTH(printLineN(i))){
                reAdy_knock(password);
            }
        }

    }

}

void is_insystem(){
    
}

void register_bt(){

}