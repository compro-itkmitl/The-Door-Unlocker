#include <SD.h>
#include <SPI.h>

//======== Define port=============
#define vib_pin A1 //vibration sensor
#define tone_pin 7 //buzzer
#define MOSI 11
#define MISO 12
#define CLK 13
#define data1 A0
#define cs 4
#define stepPin  3
#define dirPin  5
#define poten A2

File myFile;

int id = 0, dataNum = 1;

int customDelay, customDelayMapped;

String data[20] = {""}, temp[20] = {""};

String fileName = "test.txt";
String strmyFile = "";

//==========================================================================

void setup() {
    //setup pin
    pinMode(tone_pin, OUTPUT);
    pinMode(stepPin, OUTPUT);
    pinMode(dirPin, OUTPUT);

    pinMode(cs, INPUT);
     // pinMode(dat1,INPUT);
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
            myFile.println("ID,KNCK_PAT");
            myFile.close();
    }
    //===========================================
}

void loop() {
    int label_length = -1; // the 1st char is 0
    int current_id = 0;
    int numPat = 1; //since 2nd pattern
    if (chipSelectConnected(cs)) {
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
            if (strmyFile[i] == '\n') {
                current_id++;
            }
        }
        //====================update id==========================
        if (label_length < strmyFile.length()) { // there're data in the file
            id = current_id;
        } 
        //====================check and write zone==============
        if (numPat < 5) {
            if (strmyFile.length() == label_length) { //1st round of collecting
                id++;
                //============Serial part==========
                Serial.print("Entern your 5 pattern to unlock the door in the future:");
                Serial.println("Pattern 1: ");
                // =======enter pattern======
                String password = "[";
                for(int i = 0; i < 5; i++){
                    String messure = (String) TP_init();
                    if(messure == "0"){
                        i--;
                        continue;
                    }
                    Serial.println(messure);
                    password += messure;
                    password += ':';
                }
                password += ']';
                data[0] = password;
                //============File part===========
                writeSD(myFile, fileName, dataNum);
                //============Temp part===========
                for (int i = 0; i < dataNum; i++) {
                    temp[i] = data[i];
                }
            } else { // not the 1st round
                if (data[0] == temp[0] && data[1] == temp[1]) {
                    Serial.println("Use the different pattern.");
                } else { //no repeat data
                    id++;
                    //============Serial part=========
                    Serial.print("Pattern "); Serial.print(numPat); Serial.println(":");
                    // =======enter pattern======
                    String password = "[";
                    for(int i = 0; i < 5; i++){
                        String messure = (String) TP_init();
                        if(messure == "0"){
                            i--;
                            continue;
                        }
                        Serial.println(messure);
                        password += messure;
                        password += ':';
                    }
                    password += ']';
                    data[0] = password;
                    //============File part===========
                    writeSD(myFile, fileName, dataNum);
                }
                //============Temp part===========
                for (int i=0; i < dataNum; i++) {
                    temp[i] = data[i];
                }
                numPat++;
            }
        } else if (numPat == 5){ // ready for unlock system
            Serial.println("Done setting");
        } else { //w8ing for incoming pattern
            int line = selector();
            Serial.print("Enter pattern number "); Serial.print(line); Serial.println(" :");
            String input_knocking_pattern = "[";
            for (int i=0; i<5; i++) {
                String messure = (String) TP_init();
                if (messure == "0") {
                    i--;
                    continue;
                }
                Serial.println(messure);
                input_knocking_pattern += messure;
                input_knocking_pattern += ':';
            }
            input_knocking_pattern += ']';
            String insidePattern = selectKNCK(printLineN(line));
            reAdy_knock(input_knocking_pattern);
        }
    } else {
        Serial.println("Card not found.");
        while (!chipSelectConnected(cs)) {
            ; // w8 untill chipSelectConnected
        }
    }
    delay(1000);
}
