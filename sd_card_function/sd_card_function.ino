#include <SD.h>
#include <SPI.h>

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
            myFile.print(","); myFile.print(data[i], 2); 
        }
        myFile.println(); //new line for next data
        myFile.close();
    }
}

