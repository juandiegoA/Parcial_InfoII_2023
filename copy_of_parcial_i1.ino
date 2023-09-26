const int data = 2;
const int latch = 3;
const int clock = 4;

bool menu=false;//solo tiene dos estados
int arr[8];
char patron[200];
int **matriz;

int row[8] = {127, 191, 223, 239, 247, 251, 253, 254};
// columnH para limpiar todo
int columnH[8] = {0,0,0,0,0,0,0,0};

void setup()
{
  Serial.begin(9600);
  // 74HC595
  pinMode(data, OUTPUT); // data
  pinMode(latch, OUTPUT); // store
  pinMode(clock, OUTPUT); // shift
  
  pinMode(13, OUTPUT);
  Serial.println("Introduzca 'V' para verificacion, I para imagen y P para patrones.");
}



void verificacion(){
  for(int k = 0; k<100; k++)
  {
    for(int i=0; i<8; i++)
    {
      digitalWrite(latch, LOW);
      shiftOut(data, clock, LSBFIRST, 255);
      shiftOut(data, clock, LSBFIRST, row[i]);
      digitalWrite(latch, HIGH);
      delay(2);
    }
  }
}
void limpiar(){
  for(int k = 0; k<1; k++)
  {
    for(int i=0; i<8; i++)
    {
      digitalWrite(latch, LOW);
      shiftOut(data, clock, LSBFIRST, 0);
      shiftOut(data, clock, LSBFIRST, row[i]);
      digitalWrite(latch, HIGH);
      delay(2);
    }
  }
}
void Dibujar(){
   for(int k = 0; k<100; k++)
  {
    for(int i=0; i<8; i++)
    {
      digitalWrite(latch, LOW);
      shiftOut(data, clock, LSBFIRST, arr[i]);
      shiftOut(data, clock, LSBFIRST, row[i]);
      digitalWrite(latch, HIGH);
      delay(2);
    }
  }
}
void readBinaryString(char *s) {
  int result = 0;
  for(int j=0;j<8;j++){
    result=0;
  while(*s != ',' && *s!='\0') {
    result=result<<1; //desplaza los bits una posicion a la izquierda
    //por ejemplo 11100 queda 111000
    if(*s == '1'){
      result = result | 1;//convierte en 1 la posicion indicada
    }
    s++; //incrementa la direccion
  }
  s++;//avanza la direccion del apuntador para superar la coma y continuar
  arr[j]=result;//guarda el numero convertido
  }
}
void publicar(){

  bool salir=true;
  int NumeroPatrones=0;
  while(salir){
    if (Serial.available() > 0){
      NumeroPatrones = Serial.parseInt();
      Serial.println(NumeroPatrones);
      salir=false;
    }
  }
  salir=true;
  Serial.println("Ingrese tiempo");
  int Tiempo=0;
  while(salir){
    if (Serial.available() > 0){
      Tiempo = Serial.parseInt();
      salir=false;
    }
  }
  //creacion de la matriz dinamica de patrones a graficar
  matriz=new int*[8];
  for(int i=0;i<8;i++){
  matriz[i]=new int[NumeroPatrones];
  }
  int contadorPatrones=0;
  salir=true;
  Serial.println("Introduzca patron");
  while(salir){
    if (Serial.available() > 0){
      size_t count = Serial.readBytesUntil('\n', patron, 200);
 	  readBinaryString(patron);
      	for(int r=0;r<8;r++){
  		*(*(matriz+r)+contadorPatrones)=arr[r];//matriz [i][j]
  		}
      delay(1000);
      contadorPatrones++;
      Serial.println("Introduzca patron");
    }
    if(contadorPatrones>=NumeroPatrones){
      salir=false;
    }
  }
  //DIBUJANDO
   for(int p = 0; p<NumeroPatrones; p++)
  {
     int aux[8];
    for(int i=0; i<8; i++)
    {
      aux[i]=*(*(matriz+i)+p);
    }
     for(int d=0; d<120; d++){
       for(int d2=0; d2<8; d2++){
        digitalWrite(latch, LOW);
        shiftOut(data, clock, LSBFIRST, aux[d2]);
        shiftOut(data, clock, LSBFIRST, row[d2]);
        digitalWrite(latch, HIGH);
        delay(1);
     }
     }
     delay(Tiempo);
   }
}
void loop()
{
   if (Serial.available() > 0) {
    
     if(menu){
      size_t count = Serial.readBytesUntil('\n', patron, 200);
 
       delay(1000);
       //Serial.println(patron);
       readBinaryString(patron);//"11111111,11111111,11111111");
       menu=false;
       Dibujar();
       Serial.println("Introduzca 'V' para verificacion, I para imagen y P para patrones.");
     }
     else{

    int inByte = Serial.read();

    switch (inByte) {

      case 'V':
	   Serial.println("Verificando: ");
       verificacion();
       Serial.println("Verificado.");
       Serial.println("Introduzca 'V' para verificacion, I para imagen y P para patrones.");
		break;

      case 'I':
		Serial.println("Introduzca patron:");
        menu=true; //permite entrada menu
        break;
      case 'P':
		Serial.println("Introduzca numero de patrones:");
        publicar();
        Serial.println("Introduzca 'V' para verificacion, I para imagen y P para patrones.");
        break;

      default:

        // turn all the LEDs off:
      Serial.println("Introduzca una opcion valida");
      delay(1000);

    }
   }

  }
  limpiar();
  
}

//ejemplos=11110011,11110011,11110011,11110011,11110011,11110011,11110011,11110011

//11110011,11110011,11000011,11110011,11110011,11000011,11110011,11000011

//11111111,11111111,11000011,11000011,11110011,11000011,11110011,11000011