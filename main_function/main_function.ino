#include <SD.h>
#include <SPI.h>

//======== Define port=============
#define vib_pin A1 //vibration sensor
#define tone_pin 7 //buzzer
#define MOSI 11
#define MISO 12
#define CLK 13 //SCK
#define cs 4
#define stepPin  3
#define dirPin  5
#define poten A2

File myFile;

int id = 0, dataNum = 1;

int customDelay, customDelayMapped;

String data[20] = {""}, temp[20] = {""};

String fileName = "pass.txt";
String strmyFile = "";

//==========================================================================

void setup() {
    //setup pin
    pinMode(tone_pin, OUTPUT);
    pinMode(stepPin, OUTPUT);
    pinMode(dirPin, OUTPUT);

    pinMode(cs, INPUT);
    pinMode(vib_pin, INPUT);
    pinMode(MOSI, INPUT);
    pinMode(MISO, INPUT);
    pinMode(CLK, INPUT);

    Serial.begin(9600);
    while (!Serial) {
        ;
    }
    //checking sd card before enter the void loop.
    checkSD(cs); 
    //==============label part===================
    myFile = SD.open(fileName, FILE_WRITE);
    Serial.println(fileName);
    if (myFile) {
        Serial.println("myFile opening fine.");
        if (myFile.size() == 0) {
            Serial.println("Writing label...");
            myFile.println("ID,KNCK_PAT");
            Serial.println("Done writting label.");
        }
        myFile.close(); //close the file
    } else {
        Serial.println("Error Openning.");
    }
    //===========================================
}

bool settingDone = false;
int numPat = 1; //since 2nd pattern
void loop() {
    int label_length = -1; // the 1st char is 0
    int current_id = 0;
    if (chipSelectConnected(cs)) {
        myFile = SD.open(fileName);
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
        myFile.close();       
    //==================================debug==========================================
        // Serial.println("==================================================================");
        // myFile = SD.open(fileName);
        //        while (myFile.available()) {
        //            Serial.print(myFile.readString());
        //            Serial.write(myFile.read());
        //        } Serial.println();
        // myFile.close();
        // Serial.print("strmyFile :"); Serial.println(strmyFile);
        // Serial.print("label_length :"); Serial.println(label_length);
        // Serial.print("Ex-id"); Serial.println(id);
        // Serial.print("current_id :"); Serial.println(current_id);
        // Serial.print("updated-id"); Serial.println(id);
        // Serial.print("Label_length :"); Serial.println(label_length);
        // Serial.println("==================================================================");
        // ====================check and write zone==============
        if (numPat < 5 && settingDone == false) {
            if (strmyFile.length() == label_length) { //1st round of password setting
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
                Serial.println("1st round");
                numPat++;
                tonePin();
            } else { // not the 1st round
                if (data[0] == temp[0] && data[1] == temp[1]) {
                    Serial.println("Use the different pattern.");
                    Serial.print("gen_id : "); Serial.println(data[0]); Serial.print("ex_id : "); Serial.println(temp[0]);
                    Serial.print("New_KNCK_PAT : "); Serial.println(data[1]); Serial.print("ex-KNCK_PAT : "); Serial.println(temp[1]);
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
                    //============Temp part===========
                    for (int i=0; i < dataNum; i++) {
                        temp[i] = data[i];
                    }
                    numPat++;
                    tonePin();
                }
            }
        } else if (numPat == 5 && settingDone == false) { // ready for unlock system
            Serial.println("Done setting");
            for (int i=0; i<3; i++) {
                tonePin();
            }
            settingDone = true;
        } else { //w8ing for incoming pattern
            Serial.println("Now start to knock the door.");
            int line = selector();
            if (line == 6) {// edit password
                Serial.println("=================Enter Edit Mode=================");
                Serial.print("Pattern you want to change :");
                int pattern = selector();
                Serial.println(pattern);
                String pattern_temp[5];
                for (int i=1; i < 6; i++) {
                    pattern_temp[i] = selectKNCK(printLineN(i));
                }
                SD.remove(fileName);
                //crete new password
                Serial.println("Enter your new pattern: ");
                String N_password = "[";
                for (int i = 0; i < 5; i++) {
                    String messure = (String)TP_init();
                    if (messure == "0") {
                        i--;
                        continue;
                    }
                    Serial.println(messure);
                    N_password += messure;
                    N_password += ':';
                }
                N_password += ']';
                //edit step
                for (int i=0; i < 6;i++) {
                    if (i == 0) {
                        myFile = SD.open(fileName, FILE_WRITE);
                        if (myFile) {
                            myFile.print("ID,KNCK_PAT");
                            myFile.println();
                        }
                        myFile.close();
                    }
                    if (i == pattern) {
                        myFile = SD.open(fileName, FILE_WRITE);
                        if (myFile) {
                            myFile.print(i);
                            myFile.print(","); myFile.print(N_password);
                            myFile.println();
                        }
                        myFile.close();
                    } else { // non same pattern number and label
                        myFile = SD.open(fileName, FILE_WRITE);
                        if (myFile) {
                            myFile.print(i);
                            myFile.print(","); myFile.print(pattern_temp[i]);
                            myFile.println();
                        }
                        myFile.close();
                    }
                }
                Serial.println("Done editting.");
                tonePin();
            } else { // authen 1-5
                if (line != 0) {
                    Serial.print("Enter pattern number "); Serial.print(line);
                    Serial.println(" :");
                    String insidePattern = selectKNCK(printLineN(line));
                    reAdy_knock(insidePattern);
                }
            }
        }
    } else { //card disconnected.
        Serial.println("Card not found.");
        while (!chipSelectConnected(cs)) {
            ; // w8 untill chipSelectConnected
        }
    }
    delay(1000);
}
