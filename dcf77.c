#include <stdio.h>

unsigned char bit DCF77 @ 0x95;
unsigned char telegram[59];

unsigned int get_pause(void){

    unsigned int ms=0;
    
    while(DCF77==1);
    while(DCF77==0){
        _wait_ms(1);
        ms++;
    }
    printf("Pause: %d ms\n",ms);
    return ms;
}

unsigned char get_pulse(void){
    unsigned int ms=0;

    while(DCF77==0);
    while(DCF77==1){
        _wait_ms(1);
        ms++;
    }
    printf("Pulse: %u ms",ms);
    if(ms<150){
        printf("-> 0\n");
        return 0;
    }
    else{
        printf("-> 1\n");
        return 1;
    }
    ms=0;
}

void show_data(void){

    unsigned char minute;
    unsigned char hour;
    unsigned char day;
    unsigned char month;
    unsigned char year;
    unsigned char weekday;
    unsigned char timezone;

    int i;

    while(get_pause() < 1000);
    for(i=0;i<59;i++){
        telegram[i]=get_pulse();
        printf("Second: %u\n",i);
    }

    minute = telegram[21] + telegram[22]*2 + telegram[23]*4 + telegram[24]*8 + telegram[25]*10 + telegram[26]*20 + telegram[27]*40;
    hour = telegram[29] + telegram[30]*2 + telegram[31]*4 + telegram[32]*8 + telegram[33]*10 + telegram[34]*20;
    day = telegram[36]*1 + telegram[37]*2+telegram[38]*4+telegram[39]*8+telegram[40]*10+telegram[41]*20;
    month = telegram[45] + telegram[46]*2 + telegram[47]*4 + telegram[48]*8 + telegram[49]*10;
    year = telegram[50] + telegram[51]*2 + telegram[52]*4 + telegram[53]*8 + telegram[54]*10 + telegram[55]*20 + telegram[56]*40 + telegram[57]*80;
    weekday = telegram[42] + telegram[43]*2 + telegram[44]*4;
    timezone = telegram[17];

    switch(weekday){
        case 1: printf("Montag"); break;
        case 2: printf("Dienstag"); break;
        case 3: printf("Mittwoch"); break;
        case 4: printf("Donnerstag"); break;
        case 5: printf("Freitag"); break;
        case 6: printf("Samstag"); break;
        case 7: printf("Sonntag"); break;
    }
    printf(", %u. %u. %u, %u:%u Uhr - ",day,month,year,hour,minute);
    if(timezone==1) printf("MESZ");
    else printf("MEZ");
}


void main(void){
	//first init serial interface
    while(1) {
        show_data();
    }
}
