int TP_init() {
    delay(10);
    return pulseIn(vib_pin, HIGH);
}

int selector() {
    int count = 0, id_num = 0;
    while (count != 7){
        if(id_num > 6){
            id_num = 6;
            break;
        }
        int messure = TP_init();
        if (messure == 0){
            count++;
            continue;
        } else {
            id_num++;
            count = 0;
        }
    }

  return id_num;
}

void reAdy_knock(String insidePass) {
    int check = 0, afk = 0;
    int readyToBreak = 0;
    //====================spilt insidePass============================
    //ex of insidePass 6:7:10:9:8
    int insidePassArray[4];
    int arrayIndex = 0;
    String pass_temp = "";
    for (int i=0; i < insidePass.length(); i++) {
        if (isDigit(insidePass[i])) {
            pass_temp += insidePass[i];
        } else {//if (insidePass[i] == ':'){
        insidePassArray[arrayIndex] = pass_temp.toInt();
            pass_temp = "";
            arrayIndex++;
        }
        if (i == (insidePass.length()-1)) { // last index
            insidePassArray[arrayIndex] = pass_temp.toInt();
            pass_temp = "";
        }    
    }

    while (1) {
        delay(100);
        int messure = TP_init();
        readyToBreak++;
        if (readyToBreak == 200) {
            break;
        }

        if (afk == 5) {
            tonePin();
            Serial.println("Timed out");
            check = 0;
            afk = 0;
            continue;
        }

        if (messure == 0) {
            afk++;
            continue;
        }

        Serial.print("Messure = "); Serial.println(messure);

        if (insidePassArray[check] + 2000 > messure > insidePassArray[check] + 2000)
            check++;
        else {
            check = 0;
            tonePin();
        }

        if (check == 5) {
            tonePin();
            Serial.println("Unlocking");
            mortorRoll();
            tonePin();
        }
    }
}

void tonePin() {
    digitalWrite(tone_pin, HIGH);
    delay(300);
    digitalWrite(tone_pin, LOW);
}
