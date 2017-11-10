/*****************************************************************
Leonardo_XBee_Serial_Passthrough.ino

Set up a serial port to pass data between an XBee Shield
and the serial monitor.

Hardware Hookup:
  The XBee Shield makes all of the connections you'll need
  between Arduino and XBee. If you have the shield make
  sure the SWITCH IS IN THE "UART" POSITION. That will connect
  the XBee's DOUT and DIN pins to Arduino pins 0 and 1.
*****************************************************************/

// Leonardo Serial  is the USB port
// Leonardo Serial1 is pins 0 and 1


 
#define N_NODES 2
typedef struct {
  // MY SH SL DB NI
  String param[5];
  int db;
}NODE;

NODE n[N_NODES];
int n_nodes;
void setup()
{
  // Set up both ports at 9600 baud. This value is most important
  // for the XBee. Make sure the baud rate matches the config
  // setting of your XBee.
  Serial1.begin(9600);  //XBee/UART1/pins 0 and 1
  Serial.begin(9600);   //USB  
}

void loop()
{
  delay(7000);
  Serial.println("Running ND");
  node_discovery();
  //Serial.println(n[0].db);
/*  if (Serial.available()) //USB
  { // If data comes in from serial monitor, send it out to XBee
  Serial1.write(Serial.read()); //XBee/UART1/pins 0 and 1

  }
  if (Serial1.available())   //XBee/UART1/pins 0 and 1
  { // If data comes in from XBee, send it out to serial monitor
    Serial.write(Serial1.read());  //Serial port
    
    //read_intensity();
      //delay(1000);
  }*/
}
void read_node(){
  int count = 0;
  String s;
  //Serial.println(count);
  //while(!Serial1.available()){}
  if (Serial1.available())   // Ok (?) received
   { 
    s = Serial1.readString();  //Print OK (?)
   }
 
  int from = 0, to = 0;
  //s = Serial1.readString();
  Serial.println(s);
  if(s.length() < 5)
    return;
  while(to < s.length()){
    if(s[from] == 13) {
      from++;
      to = from;
      continue;
    }
    while(s[to++] != 13){}
    if(s.substring(from, to-1).equals("OK")){
      from = to;
      to = from;
      while(s[to++] != 13){}
    }
    n[count].param[0] = s.substring(from, to-1);
    Serial.print("my = ");
    Serial.println(n[count].param[0]);
    from = to;
    to = from;
    while(s[to++] != 13){}
    n[count].param[1] = s.substring(from, to-1);
    Serial.print("sh = ");
    Serial.println(n[count].param[1]);
    from = to;
    to = from;
    while(s[to++] != 13){}
    n[count].param[2] = s.substring(from, to-1);
    Serial.print("sl = ");
    Serial.println(n[count].param[2]);
    from = to;
    to = from;
    while(s[to++] != 13){}
    n[count].param[3] = s.substring(from, to-1);
    //Serial.print("db = ");
    //Serial.println(n[count].param[3]);
    n[count].db = (n[count].param[3].charAt(0) - (n[count].param[3].charAt(0) > 57 ? 55 : '0')) * 16;
    n[count].db += n[count].param[3].charAt(1) - (n[count].param[3].charAt(1) > 57 ? 55 : '0');
    Serial.print("db int = ");
    Serial.println(n[count].db);   
    from = to;
    to = from;
    while(s[to++] != 13){}
    n[count].param[4] = s.substring(from, to-1);
    Serial.print("ni = ");
    Serial.println(n[count].param[4]);
    //Serial.println(to);
    count++;
    Serial.print("last = ");
    Serial.println(s.charAt(to+1));
    to++;
    from = to;
  }
  
  
  
  Serial.println("Sai");
  n_nodes = count;
  return;
}
void read_intensity(){
  delay(1000);  
  Serial1.print("+++"); //activate command mode
  delay(1000); //wait guard time
   if (Serial1.available())   // Ok (?) received
   { 
    Serial.println(Serial1.readString());  //Print OK (?)
   }
  delay(1200); // Wait guard time
  Serial1.println("ATDB"); //request RSSI
  delay(1200); //Wait for response
  if (Serial1.available())   //response received (?)
   { 
    Serial.println(Serial1.readString());  //Print hexadecimal response 
   }
}

void node_discovery(){
  Serial1.print("+++");
  delay(1010);
  Serial1.println("ATND");
  delay(1010);
  read_node();
  Serial1.println("ATCN");
  delay(2100);
  if (Serial1.available())   //response received (?)
   { 
    Serial.println(Serial1.readString());  //Print hexadecimal response 
   }
  //Serial.println(n[0].param[4].toInt(), DEC);
  
}


