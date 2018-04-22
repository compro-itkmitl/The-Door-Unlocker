int TP_init(){
  delay(10);
  return pulseIn(vib_pin, HIGH);
}

void reAdy_knock(string password[]){
  int check = 0, afk = 0;
  while(bluetooth avaliable){
    delay(100);
    int messure = TP_init();

    if (afk == 5) 
      {
        digitalWrite(tone_pin, HIGH);
        delay(300);
        digitalWrite(tone_pin, LOW);
        check = 0;
        afk = 0;
        continue;
       }

    if (messure == 0) 
      {
        afk++;
        continue;
      }

    Serial.print("Messure = ");
    Serial.println(messure);

    if(password[check]+2000 > messure > password[check]+2000) check++;
    else{
      check = 0;
      digitalWrite(tone_pin, HIGH);
      delay(300);
      digitalWrite(tone_pin, LOW);
    }

    if (check == 5){
      digitalWrite(tone_pin, HIGH);
      delay(100);
      digitalWrite(tone_pin, LOW);
      digitalWrite(tone_pin, HIGH);
      delay(300);
      digitalWrite(tone_pin, LOW);

      digitalWrite(Mortor, HIGH); // not a read code
    }
  }
}