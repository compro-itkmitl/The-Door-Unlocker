long TP_init(){
  delay(10);
  return pulseIn(vib_pin, HIGH);
}

void reAdy_knock(string password[]){
  int check = 0, afk = 0;
  while(bluetooth avaliable){
    delay(100);
    long messure = TP_init();

    if (afk == 5) 
      {
        digitalWrite(tone_pin, HIGH);
        delay(200);
        digitalWrite(tone_pin, LOW);
        check = 0;
       }
    if (messure == 0) afk++;

    if(messure == password[check]) check++;

    if (check == 4){
      digitalWrite(tone_pin, HIGH);
      delay(100);
      digitalWrite(tone_pin, LOW);
      digitalWrite(tone_pin, HIGH);
      delay(300);
      digitalWrite(tone_pin, LOW);

      digitalWrite(Mortor, HIGH);
    }
  }
}