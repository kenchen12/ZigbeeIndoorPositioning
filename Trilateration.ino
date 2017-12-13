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

//#define P_23 3
#define P_21 2
#define P_17 1 
#define P_12 0
typedef struct {
  // MY SH SL DB NI
  //String param[5];
  int db;
  int pos_x, pos_y;
  double distance;
} NODE;
//float constante[N_NODES] = {6.35, 4.67};
NODE n[N_NODES];
//const float A = -36 ; //DB(1)
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
  String s, aux, token[5];
  int start = 0, node;
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
      token[i] = aux;
    }
    if(token[4] == "12") node = P_12;
    if(token[4] == "17") node = P_17;
    if(token[4] == "21") node = P_21;
    //if(token[4] == "23") node = P_23;
    Serial.print("Placa : ");
    Serial.println(token[4]);
    n[node].db = hextoi(token[3]);
    Serial.print("Potência : ");
    Serial.println(n[node].db);
    Serial.println("-----------------------------------------------------------------");
    n_nodes++;
  }

}
void trilateration(){
  //n[0] = a
  //n[1] = b
  //n[2] = c
  double s = (pow(n[2].pos_x, 2) - pow(n[1].pos_x, 2)  + pow(n[2].pos_y, 2) - pow(n[1].pos_y, 2) - pow(n[1].distance, 2) + pow(n[2].distance, 2))/2;
  double t = (pow(n[0].pos_x, 2) - pow(n[1].pos_x, 2)  + pow(n[0].pos_y, 2) - pow(n[1].pos_y, 2) - pow(n[1].distance, 2) + pow(n[0].distance, 2))/2;
//var y = ((T * (xb - xc)) - (S * (xb - xa))) / (((ya - yb) * (xb - xc)) - ((yc - yb) * (xb - xa)));
  double y = ((t * (n[1].pos_x - n[2].pos_x)) - (s * (n[1].pos_x - n[0].pos_x))) / (((n[0].pos_y - n[1].pos_y) * (n[1].pos_x - n[2].pos_x)) - ((n[2].pos_y - n[1].pos_y) * (n[1].pos_x - n[0].pos_x)));
//var x = ((y * (ya - yb)) - T) / (xb - xa);
  double x = ((y * (n[0].pos_y - n[1].pos_y)) - t) / (n[1].pos_x - n[0].pos_x);
  Serial.print("X = ");
  Serial. println( x );
  Serial.print("Y = ");
  Serial. println( y );
}
void calc_distances(){
  const double _n[N_NODES] = {6.31, 5.96, 4.65}
  const double a[N_NODES~] = {-24, -34, -24}
//  double d = pow(10, (n[_n].db - A)/(-10*c));
  for( int i = 0; i < N_NODES; i++){
    n[i].distance = pow(10, (n[i].db - a[i])/(-10*_n[i]));
  }
//  Serial.print("Distancia : ");
//  Serial.println(d);
}
void node_discovery() {
  Serial1.print("+++");
  delay(1015);
  Serial1.println("ATND");
  //delay(1015);
  delay(3000);
  read_node();
}
void loop()
{
  delay(1015);
  Serial.println("Running ND");
  node_discovery();
  Serial.println("Done running ND");
  if(n_nodes == 3){
    calc_distances();
    trilateration();
  }
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




