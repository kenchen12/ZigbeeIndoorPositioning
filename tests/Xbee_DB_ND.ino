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



#define N_NODES 3
#define P_23 2
#define P_21 1
#define P_17 0 
typedef struct {
  // MY SH SL DB NI
  String param[5];
  int db;
} NODE;
float constante[N_NODES] = {6.35, 4.67};
NODE n[N_NODES];
const float A = -23 ; //DB(1)
int n_nodes;
void setup()
{
  // Set up both ports at 9600 baud. This value is most important
  // for the XBee. Make sure the baud rate matches the config
  // setting of your XBee.
  Serial1.begin(9600);  //XBee/UART1/pins 0 and 1
  Serial.begin(9600);   //USB
}
void read_node() {
  String s, aux;
  int start = 0;
  n_nodes = 0;
  while (!Serial1.available()) {}
  s = Serial1.readString();
  //Serial.print("String : ");
  //Serial.println(s);
  //Serial.print("Length : ");
  //Serial.println(s.length());
  while (start < s.length()) {

    for (int i = 0; i < 5; i++) {

      aux = tokenize(s, &start);
      while (aux.equals("OK"))
        aux = tokenize(s, &start);
      n[n_nodes].param[i] = aux;
      //Serial.print("Token : ");
      //Serial.println(aux);
    }
    n_nodes++;
    //Serial.print("start : ");
    //Serial.println(start);
  }

}
void calc_distance(int _n){
  float c = 0;
  for(int i = 0; i < n_nodes; i++){
    if(n[i].param[4].equals("17")) c = constante[P_17];
    if(n[i].param[4].equals("21")) c = constante[P_21];
    if(n[i].param[4].equals("23")) c = constante[P_23];
  }
    
  double d = pow(10, (n[_n].db - A)/(-10*c));
  Serial.print("Distancia : ");
  Serial.println(d);
}
void node_discovery() {
  Serial1.print("+++");
  delay(1015);
  Serial1.println("ATND");
  //delay(1015);
  delay(3000);
  read_node();
  for (int i = 0; i < n_nodes; i++)
    n[i].db = -hextoi(n[i].param[3]);
  for(int i = 0; i < n_nodes; i++){
    Serial.print(i);
    Serial.print("'s dbm : ");
    Serial.println(n[i].db);
    float _n = (A - n[i].db)/(10* log10(1.5));
    Serial.print("n : ");
    Serial.println(_n);
    calc_distance(i);
    for(int j = 0; j < 5; j++){
      Serial.print(j);
      Serial.print(" : ");
      Serial.println(n[i].param[j]);
    }
  }
  //Serial1.println("ATCN");
  //delay(2100);
}
void loop()
{
  delay(1015);
  Serial.println("Running ND");
  node_discovery();
  Serial.println("Done running ND");
}

int hextoi(String s) {
  int i = (s.charAt(0) - (s.charAt(0) > 57 ? 55 : '0')) * 16;
  i += s.charAt(1) - (s.charAt(1) > 57 ? 55 : '0');
  return i;
}

String tokenize(String s, int *start) {
  while (s[(*start)] == 13) {
    (*start)++;
  }
  if (*start > s.length())
    return "";
  int to = *start;
  while (s[to] != 13) {
    to++;
  }
  if (to > s.length())
    return "";
  String tok = s.substring(*start, to);
  while (s[to] == 13 && to < s.length()) {
    to++;
  }
  *start = to;
  return tok;
}




