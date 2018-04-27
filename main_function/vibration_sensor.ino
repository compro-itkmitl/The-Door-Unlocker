int TP_init(){
    delay(10);
    return pulseIn(vib_pin, HIGH);
}

int selector(){
    int count = 0, id_num = 0;
    while (count != 5){
        if(id_num > 5){
            id_num = 5;
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

<<<<<<< Updated upstream
void reAdy_knock(String password) {
    int check = 0, afk = 0;
    int readyToBreak = 0;
=======
void reAdy_knock(string password[]){
  int check = 0, afk = 0;
  int id_use = selector();    
  while(bluetooth avaliable){
    delay(100);
    int messure = TP_init();
>>>>>>> Stashed changes

    while (1) {
        delay(100);
        int messure = TP_init();
        readyToBreak++;
        if (readyToBreak == 200) {
            break;
        }

        if (afk == 5) {
            digitalWrite(tone_pin, HIGH);
            delay(300);
            digitalWrite(tone_pin, LOW);
            check = 0;
            afk = 0;
            continue;
        }

        if (messure == 0) {
            afk++;
            continue;
        }

        Serial.print("Messure = "); Serial.println(messure);
        int arrayPassword[4]; // = spilt password
        for (int j=0; j<5; j++) {
            for (int i=0; i < password.length(); i++) {
                if (isDigit(password[i])) {
                    arrayPassword[j] = (int) password[i];
                    continue;
                }
            }
        }

        if (arrayPassword[check] + 2000 > messure > arrayPassword[check] + 2000)
            check++;
        else {
            check = 0;
            digitalWrite(tone_pin, HIGH);
            delay(300);
            digitalWrite(tone_pin, LOW);
        }

        if (check == 5) {
            digitalWrite(tone_pin, HIGH);
            delay(100);
            digitalWrite(tone_pin, LOW);
            digitalWrite(tone_pin, HIGH);
            delay(300);
            digitalWrite(tone_pin, LOW);

            mortorRoll();
        }
    }
}