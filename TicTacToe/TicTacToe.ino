
int poz_curenta = 0;
int poz_prev = 0;

int jucator = 1;
boolean won = false;

int led_1 = 12;
int pin_senzor = A5;
int val_senzor = 0;
int led_jucator = 3;
int pin_buton = 2;
int val_buton = 0;

int m[3][3] = {
  {0,0,0},
  {0,0,0},
  {0,0,0},
};

unsigned long blinkled2 = 0; //jucatorul al doilea 
unsigned long blinkk = 0; //pt casuta selectata

int ciclu1 = LOW;
int ciclu2 = LOW;

void schimb() 
{
  if(jucator == 1) jucator = 2;
  else jucator = 1;
}

int getVal(int poz) 
{
  return m[poz / 3][poz % 3];
}

void setVal(int poz, int valoare) 
{
  m[poz / 3][poz % 3] = valoare;
}

void win(int led1, int led2, int led3) 
{
  schimb();
  for(int led = 0; led <= 8; led++) 
  {
    setVal(led, 0);
  }
  setVal(led1, jucator);
  setVal(led2, jucator);
  setVal(led3, jucator);
}

void setup() {
  
  for(int led = (int)(led_1 - 8); led <= led_1; led++) 
  {
    pinMode(led, OUTPUT);
  }
  pinMode(led_jucator, OUTPUT);
  
  pinMode(pin_buton, INPUT);
}

void loop() 
{
  // update
  if((unsigned long)(millis() - blinkk) >= 200) 
  {
      ciclu2 = !ciclu2;
      blinkk = millis();
    }
  if((unsigned long)(millis() - blinkled2) >= 350) 
  {
      ciclu1 = !ciclu1;
      blinkled2 = millis();
  }
  // update led jucator
  if(jucator == 1) 
  {
      digitalWrite(led_jucator, HIGH);
  } else {
      digitalWrite(led_jucator, ciclu1);
  }
  // update în matrice
  for(int led = 0; led <= 8; led++) 
  {
    if(getVal(led) == 0) 
    {
        digitalWrite(12 - led, LOW);
    } else if(getVal(led) == 1) 
    {
        digitalWrite(12 - led, HIGH);
    } else if(getVal(led) == 2) 
    {
        digitalWrite(12 - led, ciclu1);
    } else if(getVal(led) == 3) 
    {
        digitalWrite(12 - led, ciclu2);
    }
  }
  
  if(!won) 
  {
    val_senzor = analogRead(pin_senzor);
    val_buton = digitalRead(pin_buton);
  
    poz_prev = poz_curenta;

    for(int poz = 1; poz <= 9; poz++) 
    {
      if(val_senzor < (int)(((float)poz) / 9.0 * 1023)) 
      {
        poz_curenta = (int)poz - 1;
        break;
      }
    }

    if(val_buton == 1) 
    {
      if(getVal(poz_curenta) != 1 && getVal(poz_curenta) != 2) 
      {
        setVal(poz_curenta, jucator);
        schimb();
      }
    } else {
      if(getVal(poz_curenta) == 0) 
      {
        setVal(poz_curenta, 3);
      }
      if(poz_curenta!= poz_prev && getVal(poz_prev) == 3) 
      {
        setVal(poz_prev, 0);
      }
    }

    // conditiile de castig
    for(int i = 0; i <= 2; i++) 
    {
      // Orizontal
      if(getVal(3*i) == getVal(3*i+1) && getVal(3*i) == getVal(3*i+2) && getVal(3*i) != 0) 
      {
        won = true;
        win(3*i, 3*i+1, 3*i+2);
        break;
      }
      
      // Vertical 
      else if(getVal(i) == getVal(i+3) && getVal(i) == getVal(i+6) && getVal(i) != 0) 
      {
        won = true;
        win(i, i+3, i+6);
        break;
      }
    }

    // Diagonala
    if(!won) {
      // sus dreapta jos stanga 
      if(getVal(0) == getVal(4) && getVal(0) == getVal(8) && getVal(0) != 0) 
      {
        won = true;
        win(0, 4, 8);
      }
      // sus stanga jos dreapta
      else if(getVal(2) == getVal(4) && getVal(2) == getVal(6) && getVal(2) != 0) 
      {
        won = true;
        win(2, 4, 6);
      }
    }
  }
}
