//require int TP_init()

int selector(){
    int timeout = 0, set_password = 0;
    while (1){
        delay(100)
        if(timeout == 5) 
            return set_password;
        int messure = TP_init();
        if (messure == 0){
            timeout++;
        }
        else
            timeout = 0;
    }
}