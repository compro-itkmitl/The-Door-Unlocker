#include <SD.h>
#include <SPI.h>


//======== Define port=============
#define vib_pin  A1//vibration sensor
#define tone_pin  7//buzzer
#define MOSI 11
#define MISO 12
#define CLK 13
#define data1 A0
#define cs 4

File myFile;

int id=0, dataNum=2;

bool  First_round = 1;
String data[20] = {""} ,temp[20] = {""};

String fileName = "test.txt";
String strmyFile = "";

void setup() {
    //setup pin
    pinMode(tone_pin, OUTPUT);

    pinMode(cs, INPUT);
    //  pinMode(dat1,INPUT);
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
    checkSD(cs); 
    //==============label part===================
    myFile = SD.open(fileName, FILE_WRITE);

    if (myFile) {
        while (myFile.available()) {
            char ltr = myFile.read();
            strmyFile += ltr;
        }

        if (strmyFile.length() == 0) //nothing in .txt
            myFile.println("ID,BLETH,KNCK_PAT");
            myFile.close();
    }
    //===========================================
}


void loop() {
    int label_length = -1; // the 1st char is 0
    int current_id = 0;
    if (chipSelectConnected(cs)) {
        //============Define port zone=============
        data[0] = analogRead(data1);  // Bluetooth mac address
        data[1] = "[7:8:9:8:6]"; // knockong pattern
        //===============checking text in the file================
        strmyFile = "";
        while (myFile.available()) {
            char ltr = myFile.read();
            strmyFile += ltr;
        }
        //===================label's length=======================
        for (int i=0; i < strmyFile.length(); i++) {
            if (strmyFile[i] == '\n') {
                break;
            } else {
                label_length++;
            }
        }
        //====================count current id====================
        for (int i=0; i < strmyFile.length(); i++) {
            if (strmyFile[i] == '\n') { // if you want to compare a char u need to use '' not ""
                current_id++;
            }
        }
        //====================update id==========================
        if (label_length < strmyFile.length()) { // there're data in the file
            id = current_id;
        } 
        //====================check and write zone==============
        if (strmyFile.length() == label_length) { //1st round of collecting
            id++;
            //============Serial part==========
            Serial.print("id: ");
            Serial.print(id);
            Serial.print("       Mac_address : ");
            Serial.print(data[0]);
            Serial.print("       KNCK_PAT : ");
            Serial.println(data[1]);
            //============File part===========
            writeSD(myFile, fileName, dataNum);
            //============Temp part===========
            for (int i = 0; i < dataNum; i++) {
                temp[i] = data[i];
            }

        } else { // not the 1st round
            if (data[0] == temp[0] && data[1] == temp[1]) {
                ; // skip it
            } else { //no repeat data
                id++;
                //============File part===========
                writeSD(myFile, fileName, dataNum);
                //============Serial part=========
                Serial.print("id: ");
                Serial.print(id);
                Serial.print("       Mac_address : ");
                Serial.print(data[0]);
                Serial.print("       KNCK_PAT : ");
                Serial.println(data[1]);
            }
            
            //============Temp part===========
            for (int i=0; i < dataNum; i++) {
                temp[i] = data[i];
            } 
        }
    } else {
        Serial.println("Card not found.");
        while (!chipSelectConnected(cs)) {
            ;
        }
    }
    delay(2000);
}
