#include <SD.h>
#include <SPI.h>
#include <stdlib.h>

File myFile;
void setup() {
    Serial.begin(9600);
    while (!Serial) {
        ; // wait for serial port to connect. Needed for native USB port only
    }

    Serial.println("Initializing SD card...");

    if (!SD.begin(4)) {
        Serial.println("initialization failed!");
        while (1);
    }
    Serial.println("initialization done.");
    
    myFile = SD.open("example.txt");
    int slen = 0;
    if (myFile) {
        Serial.print("==================Start reading the file==================\n");
        Serial.println();

//        while (myFile.available()) {
//            Serial.print(myFile.readString());
//            //you can us Serial.write(myFile.read());
//        }
        slen = myFile.size();
        Serial.print("strlen = "); Serial.println(slen);
//        Serial.println(strmyFile);
//===========Make var for string in txt file==========
        String strmyFile = "";
        while (myFile.available()) {
            char ltr = myFile.read();
            strmyFile += ltr;      
        }
//====================================================
        int count_N = 0, nothing = 0;
        Serial.println("2nd: "); 
        Serial.println(strmyFile);
        Serial.println(strmyFile.length());
        Serial.println("===================Checking char part======================");
        for (int i=0; i < strmyFile.length(); i++) {
            if (strmyFile[i] == '\n') { // if you want to compare a char u need to use '' not ""
                count_N++;
            } else {
                nothing++;
            }
        } 
        Serial.print("NumofN = "); Serial.println(count_N);
        Serial.print("nothing = "); Serial.println(nothing);

        Serial.println("=================Spilt data part==========================");
        String Current_line = "";
        int line = 2;
        Current_line = printLineN(line);
        Serial.print("Current_line : "); Serial.println(line);
        Serial.println(Current_line);
        Serial.println("================Selecting data part========================");
        Serial.print("Selecting bluetooth of ID: "); Serial.println(line);
        String mac_address = selectBLTH(Current_line);
        Serial.println(mac_address);
        String knocking_list = selectKNCK(Current_line);
        Serial.print("knocking pattern = ");
        Serial.println(knocking_list);
        Serial.println("======================Test part============================");
        Serial.print("ID : ");
        String id_number = selectID(Current_line);
        Serial.println(id_number);
        // close the file:
        myFile.close();
    } else {
        // if the file didn't open, print an error:
        Serial.println("Error opening the file.");
    }
//    String test_text = "abcdefghij";
//    Serial.print(test_text[5]); //return f
  //======================= =======end of setup()===============================
}
//testing 1, 2, 3. strlen = 16 +2 <--- ??? where the +2 come from
void loop() {
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
    String text_line = "";
    while (true) {
        cr = myFile.read();
        text_line += cr;
        if (cr == '\n') {
            break;
        }
    }
    return text_line; //2,00:CD:FE:8B:70:01,[6:7:10:9:8]  if lineN == 2
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

String selector(String label, String Currentln) {
    //BLTH = bluetooth, ID = ID number or line, KNCK = knocking pattern
    //ex of Currentln: 2,00:CD:FE:8B:70:01,[6:7:10:9:8]
    if (label == "BLTH") {
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
    if (label == "ID") {
        String id_number = "";
        for (int i = 0; i < Currentln.length(); i++) {
            if (Currentln[i] == ',') {
                break;
            } else {
                id_number += Currentln[i];
            }
        }
        return id_number; // 2

    }
    if (label == "KNCK") {
        String knocking_list = "";
        bool start = false;
        for (int i = 0; i < Currentln.length(); i++) {
            if (Currentln[i] == '[' && start == false) {
                start = true;
                continue;
            }
            if (start) {
                if (Currentln[i] == ']') {
                    start = false;
                } else {
                    knocking_list += Currentln[i];
                }
            }
        }
        return knocking_list; // 6:7:10:9:8
    }
}

