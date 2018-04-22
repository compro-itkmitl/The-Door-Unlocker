#include <SD.h>
#include <SPI.h>


//======== Define port=============
#define vib_pin  A1//vibration sensor
#define tone_pin  7//buzzer
#define MOSI 11
#define MISO 12
#define CLK 13
#define data1 A0
//#define cs 4

File myFile;

int id=0, dataNum=2;
String data[20] = {""} ,temp[20] = {""};

String fileName = "unrepeat.txt";
String strmyFile = "";
//==========================================================================
void checkSD(int CS){
    Serial.println("Checking SD card...");
    delay(300);
    if (!SD.begin(CS)) {
        Serial.println("Card failed.");
        return;
    }
    Serial.println("SD card initialization done.");
    delay(2000);
}

void writeSD(File myFile, String fileName, int dataNum){
    //you have to define ur header by your own in data[]
    myFile = SD.open(fileName, FILE_WRITE);
    if (myFile) {
        myFile.print(id);
        for (int i=0; i < dataNum; i++) {
            myFile.print(","); myFile.print(data[i]); 
        }
        myFile.println(); //new line for next data
        myFile.close();
    }
}

String printLineN(int lineN) {
    /*ex text in example.txt
    ID,BLETH,KNCK_PAT
    1,00:CD:FE:8B:70:00,[5:1:2:3:4]
    2,00:CD:FE:8B:70:01,[6:7:10:9:8]
    3,00:A0:C9:14:C8:29,[1:2:3:4:5]
    4,00:CD:FE:14:70:00,[7:8:9:8:6]
    5,00:A0:C9:14:C8:30,[1:1:1:1] */

    myFile.seek(0);
    char cr;

    for (int i = 0; i < (lineN);) {
        cr = myFile.read();
         if (cr == '\n') {
            i++;
        }
    }
    //Now we are at the right line
    String Currentln = "";
    while (true) {
        cr = myFile.read();
        Currentln += cr;
        if (cr == '\n') {
            break;
        }
    }
    return Currentln; //2,00:CD:FE:8B:70:01,[6:7:10:9:8]  if lineN == 2
}

String selectBLTH(String Currentln) {
    //ex of Currentln: 2,00:CD:FE:8B:70:01,[6:7:10:9:8]
    String mac_address = "";
    bool start = false;
    for (int i=0; i < Currentln.length(); i++) {
        if (Currentln[i] == ',' && start == false) { // 1st ,
            start = true;
            continue;
        }
        if (start) {
            if (Currentln[i] == ',') {
                break;
            } else {
                mac_address += Currentln[i];
            }
        }
    }    
    return mac_address; //00:CD:FE:8B:70:01
}

String selectKNCK(String Currentln) {
    //ex of Currentln: 2,00:CD:FE:8B:70:01,[6:7:10:9:8]
    String knocking_list = "";
    bool start = false;
    for (int i=0; i < Currentln.length(); i++) {
        if (Currentln[i] == '[' && start == false) { // 1st ,
            start = true;
            continue;
        }
        if (start) {
            if (Currentln[i] == ']') {
                break;
            } else {
                knocking_list += Currentln[i];
            }
        }
    }
    return knocking_list; //6:7:10:9:8
}

String selectID(String Currentln) {
    //ex of Currentln: 2,00:CD:FE:8B:70:01,[6:7:10:9:8]
    String id_number = "";
    for (int i = 0; i < Currentln.length(); i++) {
        if (Currentln[i] == ',') {
            break;
        } else {
            id_number += Currentln[i];
        }
    }
    return id_number; // "2"
}

int chipSelectConnected(int CS) {
    if (SD.begin(CS)) {
        return 1;
    } else {
        return 0;
    }
}

//==========================================================================
void setup(){
  //setup pin
    pinMode(tone_pin, OUTPUT);

    pinMode(4, INPUT);
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
    checkSD(4); 
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
    if (chipSelectConnected(4)) {
        //============Define port zone=============
        data[0] = analogRead(data1);  // Bluetooth mac address
        data[1] = "[7:8:9:8:6]";
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
        while (!chipSelectConnected(4)) {
            ;
        }
    }
    delay(2000);
}
