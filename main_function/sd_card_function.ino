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
    1,[5:1:2:3:4]
    2,[6:7:10:9:8]
    3,[1:2:3:4:5]
    4,[7:8:9:8:6]
    5,[1:1:1:1] */

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
    return Currentln; //2,[6:7:10:9:8]  if lineN == 2
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