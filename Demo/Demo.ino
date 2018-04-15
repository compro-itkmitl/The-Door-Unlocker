#include <SD.h>
#include <SPI.h>

File myFile;

int id=0, dataNum=2;
int dat1=A0/*, dat2=A1*/;
bool  First_round = 1;
float data[20] = {0} ,temp[20] = {0};

String fileName = "test.txt";

void setup() {
    pinMode(dat1,INPUT);
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
        myFile.println("ID,Data1,Data2");
        myFile.close();
    }
    //===========================================
}

void loop() {
    if (SD.begin(4)) {
        //============Define port zone=============
        data[0] = analogRead(dat1);
        data[1] = 5;
        if (First_round == 1) {
            id++;
            //============Serial part==========
            Serial.print("id: ");
            Serial.print(id);
            Serial.print("       Value1 : ");
            Serial.print(data[0]);
            Serial.print("       Value2 : ");
            Serial.println(data[1]);
            //============File part===========
            writeSD(myFile, fileName, dataNum);
            //============Temp part===========
            for (int i = 0; i < dataNum; i++) {
                temp[i] = data[i];
            }
            First_round = 0;
        } else {

            if (data[0] == temp[0] && data[1] == temp[1]) {
                ;
            } else { //no repeat data
                id++;
                //============File part===========
                writeSD(myFile, fileName, dataNum);
                //============Serial part=========
                Serial.print("id: ");Serial.print(id);
                Serial.print("       Value1 : ");
                Serial.print(data[0]);
                Serial.print("       Value2 : ");
                Serial.println(data[1]);
            }
            
            //============Temp part===========
            for (int i=0; i < dataNum; i++) {
                temp[i] = data[i];
            } 
        }
    } else {
        Serial.println("Card not found.");
        while (!SD.begin(4)) {
            ;
        }
    }
    delay(2000);
}

void checkSD(int cs){
    Serial.println("Checking SD card...");
    delay(300);
    if (!SD.begin(cs)) {
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
            myFile.print(","); myFile.print(data[i], 2); 
        }
        myFile.println(); //new line for next data
        myFile.close();
    }
}
