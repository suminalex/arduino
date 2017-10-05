int Button = 8; // кнопка на цифре №8 (Работает по "-")
int ACC = 2; // Аксессуары на цифре №2 (Работает по "+")
int IGN1 = 3; // Зажигание 1 на цифре №3 (Работает по "+")
int IGN2 = 4; // Зажигание 2 на цифре №4 (Работает по "+")
int STARTER = 5; // Стартер на цифре №6 (Работает по "+")
int pedal = 6; // Педаль тормоза на цифре №6 (Работает по "-")
int gena = 7;  // Сигнал генератора на цифре №7 (Работает по "-")
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool     buttonState = false;
uint32_t millisButton = 0;
uint8_t  clickCount = 0;
uint8_t  clickCount2 = 0;             
int dvigstate = 0;
long previousMillis = 0;       
long interval = 4000;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                                                               
                                                                /////////Таймер (пока не используется)/////////
                                                                
unsigned long millis_old[1];//массив для хранения последних данных о времени срабатывания таймера.Количество элементов соответствует количеству таймеров
 
boolean my_timer(unsigned long time, byte nomer)//функция таймер, time-время задержки, nomer-номер таймера
{
  unsigned long current_millis;//текущее значение миллисекунды
  current_millis=millis();
  if (current_millis-millis_old[nomer]>time) //если время срабатывания таймера наступило
  {
    millis_old[nomer]=current_millis;//перезаписываем время последнего срабатывания таймера
    return true;//возвращаем true
  } else return false;//возвращаем false

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() 
{
   pinMode(Button, INPUT_PULLUP);
   pinMode(ACC, OUTPUT);
   pinMode(IGN1, OUTPUT);
   pinMode(IGN2, OUTPUT);
   pinMode(STARTER, OUTPUT);
   pinMode(pedal, INPUT_PULLUP);
   pinMode(gena, INPUT_PULLUP);
   Serial.begin(9600);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

                                                                 /////////////////////Прерывание///////////////////////
void readClickCount() {
  // fix press
    if (digitalRead(Button) == LOW && !buttonState && (millis() - millisButton) > 50 ){
    buttonState = true;
    millisButton = millis();
        clickCount2++;
    if (clickCount2 >= 1) clickCount2 = 0;
  }
  
  // fix release
  if (digitalRead(Button) == HIGH && buttonState && (millis() - millisButton) > 50  ){
    buttonState = false;
    millisButton = millis();
        clickCount++;
    if (clickCount >= 5) clickCount = 0;
  }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

                                                                      ////////////////////Функции//////////////////////

void OFF() // Всё выключено
{
      digitalWrite(ACC, LOW);
      digitalWrite(IGN1, LOW);
      digitalWrite(IGN2, LOW);
      digitalWrite(STARTER, LOW);
      
      
}

void ACCfunc() // Функция Аксессуары
{
      digitalWrite(ACC, HIGH);
      digitalWrite(IGN1, LOW);
      digitalWrite(IGN2, LOW);
      digitalWrite(STARTER, LOW);
      
}


void IGNfunc() //Функция Зажигание
{
      digitalWrite(ACC, HIGH);
      digitalWrite(IGN1, HIGH);
      digitalWrite(IGN2, HIGH);
      digitalWrite(STARTER, LOW);
      
}

void enginestartfunc() //Запуск двигателя (Включаем зажигание на 2сек, затем подключаем стартер на 8 сек)
{    
     unsigned long lastMillis2 = millis();  
       uint8_t  clickCount3 = clickCount;   
        while (!digitalRead(gena) == LOW && !(millis() - lastMillis2 > 2000) && (clickCount3 == clickCount))
              { 
               readClickCount();
               digitalWrite(ACC, LOW);
               digitalWrite(IGN1, HIGH);
               digitalWrite(IGN2, LOW);  
               digitalWrite(STARTER, LOW); 
              }
               unsigned long lastMillis3 = millis();
               while (!digitalRead(gena) == LOW && !(millis() - lastMillis3 > 8000) && (clickCount3 == clickCount)) 
                     {
                      readClickCount();
                      digitalWrite(ACC, LOW);
                      digitalWrite(IGN1, HIGH);
                      digitalWrite(IGN2, LOW);  
                      digitalWrite(STARTER, HIGH);
                     }                                                    
 
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop()
{
readClickCount(); //Считаем кол-во нажатий
 
switch (clickCount) {

     case 0: //Нулевое нажатие (по умолчанию при запуске контроллера)
     if (dvigstate == 2)
        {
         IGNfunc();
         clickCount = 2;
         }
     else if (dvigstate == 0)
         {
          clickCount = 0;
         }
      Serial.print("ACC: ");
      Serial.print(digitalRead(ACC));
      Serial.print(" ");
      Serial.print("IGN1: ");
      Serial.print(digitalRead(IGN1));
      Serial.print(" ");
      Serial.print("IGN2: ");
      Serial.print(digitalRead(IGN2));
      Serial.print(" ");
      Serial.print("STARTER: ");
      Serial.print(digitalRead(STARTER));
      Serial.print(" ");
      Serial.print("pedal: ");
      Serial.print(!digitalRead(pedal));
      Serial.print(" ");
      Serial.print("gena: ");
      Serial.print(!digitalRead(gena));
      Serial.print(" case: 0 ");
      Serial.print(" dvigstate: ");
      Serial.println(dvigstate);  
      break;
      
     case 1: //Первое нажатие
      if (!digitalRead(pedal) == LOW && dvigstate == 0) 
        {
         ACCfunc();
         dvigstate = 1;      
        }
      else if (!digitalRead(pedal) == HIGH && dvigstate == 0)
        { 
         unsigned long lastMillis = millis(); 
         uint8_t  clickCount5 = clickCount;                                                        
         while (!digitalRead(gena) == LOW && !(millis() - lastMillis > 10000) && (clickCount5 == clickCount)) //При смене значения времени - поменять значения в функции enginestartfunc. Для корректной работы времени задержки зажигания и прокрутки стартера значение  в этом блоке должно соответствовать сумме значений из функции enginestartfunc
           {
            enginestartfunc(); 
           }
              if (!digitalRead(gena) == HIGH)
                  {
                   IGNfunc();
                   dvigstate = 3;
                   tone(A1,2000,300);
                  }
              else if (!digitalRead(gena) == LOW)
                  {
                   IGNfunc();
                   dvigstate = 2;
                   tone(A1,2000,1500);
                   clickCount = 0;///////
                  }
        }
      Serial.print("ACC: ");
      Serial.print(digitalRead(ACC));
      Serial.print(" ");
      Serial.print("IGN1: ");
      Serial.print(digitalRead(IGN1));
      Serial.print(" ");
      Serial.print("IGN2: ");
      Serial.print(digitalRead(IGN2));
      Serial.print(" ");
      Serial.print("STARTER: ");
      Serial.print(digitalRead(STARTER));
      Serial.print(" ");
      Serial.print("pedal: ");
      Serial.print(!digitalRead(pedal));
      Serial.print(" ");
      Serial.print("gena: ");
      Serial.print(!digitalRead(gena));
      Serial.print(" case: 1 ");
      Serial.print(" dvigstate: ");
      Serial.println(dvigstate);
      break;
      
     case 2: //Второе нажатие
     if (!digitalRead(pedal) == LOW && dvigstate == 1 )
        {
         IGNfunc();
         dvigstate = 2;
        }
     else if (!digitalRead(pedal) == HIGH && dvigstate == 1)
        {
         uint8_t  clickCount5 = clickCount;
         unsigned long lastMillis = millis();                                                          
         while (!digitalRead(gena) == LOW && !(millis() - lastMillis > 10000) && (clickCount5 == clickCount)) //При смене значения времени - поменять значения в функции enginestartfunc. Для корректной работы времени задержки зажигания и прокрутки стартера значение  в этом блоке должно соответствовать сумме значений из функции enginestartfunc
         {
          enginestartfunc(); 
         } 
              if (!digitalRead(gena) == HIGH)
                {
                 IGNfunc();
                 dvigstate = 3;
                 tone(A1,2000,300);
                 clickCount = 1;
                }
              else if (!digitalRead(gena) == LOW)
                {
                 IGNfunc();
                 dvigstate = 2;
                 tone(A1,2000,1500);
                 clickCount = 0;
                }
        }
     else if (!digitalRead(pedal) == HIGH && dvigstate == 3)
        {
         OFF();
         dvigstate = 0;
         clickCount = 0; 
        }
     else if (!digitalRead(pedal) == LOW && dvigstate == 3)
        {
         ACCfunc();
         dvigstate = 1;
         clickCount = 1;
        }
      Serial.print("ACC: ");
      Serial.print(digitalRead(ACC));
      Serial.print(" ");
      Serial.print("IGN1: ");
      Serial.print(digitalRead(IGN1));
      Serial.print(" ");
      Serial.print("IGN2: ");
      Serial.print(digitalRead(IGN2));
      Serial.print(" ");
      Serial.print("STARTER: ");
      Serial.print(digitalRead(STARTER));
      Serial.print(" ");
      Serial.print("pedal: ");
      Serial.print(!digitalRead(pedal));
      Serial.print(" ");
      Serial.print("gena: ");
      Serial.print(!digitalRead(gena));
      Serial.print(" case: 2 ");
      Serial.print(" dvigstate: ");
      Serial.println(dvigstate);
      break;
      
     case 3: //Третье нажатие
     if (!digitalRead(pedal) == HIGH && dvigstate == 2)
       { 
         uint8_t  clickCount5 = clickCount;
         unsigned long lastMillis = millis();                                                          
         while (!digitalRead(gena) == LOW && !(millis() - lastMillis > 10000) && (clickCount5 == clickCount)) //При смене значения времени - поменять значения в функции enginestartfunc. Для корректной работы времени задержки зажигания и прокрутки стартера значение  в этом блоке должно соответствовать сумме значений из функции enginestartfunc
           {
            enginestartfunc(); 
           }
             if (!digitalRead(gena) == HIGH)
               {
                IGNfunc();
                dvigstate = 3;
                tone(A1,2000,300);
                clickCount = 1;
               }
             else if (!digitalRead(gena) == LOW)
               {
                IGNfunc();
                dvigstate = 2;
                tone(A1,2000,1500);
                clickCount = 0;
               }
       }
    else if (!digitalRead(pedal) == LOW && dvigstate == 2)
       {
        ACCfunc();
        dvigstate = 1;
       }
    else if (!digitalRead(pedal) == HIGH && dvigstate == 3)
       {
        OFF();
        dvigstate = 0;
        clickCount = 0; 
       }
    else if (!digitalRead(pedal) == LOW && dvigstate == 3)
       {
        ACCfunc();
        dvigstate = 1;
        clickCount = 1;    
       }
      Serial.print("ACC: ");
      Serial.print(digitalRead(ACC));
      Serial.print(" ");
      Serial.print("IGN1: ");
      Serial.print(digitalRead(IGN1));
      Serial.print(" ");
      Serial.print("IGN2: ");
      Serial.print(digitalRead(IGN2));
      Serial.print(" ");
      Serial.print("STARTER: ");
      Serial.print(digitalRead(STARTER));
      Serial.print(" ");
      Serial.print("pedal: ");
      Serial.print(!digitalRead(pedal));
      Serial.print(" ");
      Serial.print("gena: ");
      Serial.print(!digitalRead(gena));
      Serial.print(" case: 3 ");
      Serial.print(" dvigstate: ");
      Serial.println(dvigstate);
      break;

     case 4: //Четвертое нажатие
     if (!digitalRead(pedal) == HIGH && dvigstate == 1)
        {
         uint8_t  clickCount5 = clickCount;
         unsigned long lastMillis = millis();                                                          
         while (!digitalRead(gena) == LOW && !(millis() - lastMillis > 10000) && (clickCount5 == clickCount)) //При смене значения времени - поменять значения в функции enginestartfunc. Для корректной работы времени задержки зажигания и прокрутки стартера значение  в этом блоке должно соответствовать сумме значений из функции enginestartfunc
               {
                enginestartfunc(); 
               }
       if (!digitalRead(gena) == HIGH)
          {
           IGNfunc();
           dvigstate = 3;
           tone(A1,2000,300);
           clickCount = 1;
         }
       else if (!digitalRead(gena) == LOW)
          {
           IGNfunc();
           dvigstate = 2;
           tone(A1,2000,1500);
           clickCount = 0;
          }
        } 
     else if (!digitalRead(pedal) == LOW && dvigstate == 1)
         {
          OFF();
          dvigstate = 0;
          clickCount = 0; 
         }
      Serial.print("ACC: ");
      Serial.print(digitalRead(ACC));
      Serial.print(" ");
      Serial.print("IGN1: ");
      Serial.print(digitalRead(IGN1));
      Serial.print(" ");
      Serial.print("IGN2: ");
      Serial.print(digitalRead(IGN2));
      Serial.print(" ");
      Serial.print("STARTER: ");
      Serial.print(digitalRead(STARTER));
      Serial.print(" ");
      Serial.print("pedal: ");
      Serial.print(!digitalRead(pedal));
      Serial.print(" ");
      Serial.print("gena: ");
      Serial.print(!digitalRead(gena));
      Serial.print(" case: 3 ");
      Serial.print(" dvigstate: ");
      Serial.println(dvigstate);
      break;

}   
}

