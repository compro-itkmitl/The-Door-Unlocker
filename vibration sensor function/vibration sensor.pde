long TP_init(int vib_pin){
	delay(10);
	return pulseIn(vib_pin, HIGH);
}

void vibration_controller(int vib_pin, int is_new, int password[]){
	if(is_new){// 1 is new member(not register yet) go register_func
		Serial.println("setting register...");
		delay(100);
		for(int i = 0; i < 5; i++)
			//if you have buzzer
			digitalWrite(tone_pin, HIGH);
			delay(100);
			digitalWrite(tone_pin, LOW);

			Serial.print("Enter you password ");
			Serial.println(i);
			password[i] = TP_init(vib_pin);
	}
	else{
		digitalWrite(tone_pin, HIGH);
		delay(100);
		digitalWrite(tone_pin, LOW);
		Serial.println("Enter you password ");

		int pass = 0;
		for(int i = 0; i < 5; i++){
			long mesure = TP_init(vib_pin);
			if(password[i]+300 > mesure > password[i]+300){
				pas++;
			}
			else break;
		}

		if(pass == 5){
			Serial.println("Unlock the door");
		}
	}
}
