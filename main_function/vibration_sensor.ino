int TP_init(){
  delay(10);
  return pulseIn(vib_pin, HIGH);
}

int selector(){
  int count = 0, id = 0;
  while (count == 5){
    if(id > 5){
      id = 5;
      break;
    }
    int messure = TP_init();
    if (messure == 0){
      count++;
      continue;
    }
    else{
      id++;
      count = 0;
    }
  }

  return id;
}

void reAdy_knock(string password[]){
  int check = 0, afk = 0;
  int id_use = selector();
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