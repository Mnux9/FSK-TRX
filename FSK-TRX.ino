#define TXLED 4
#define RXLED 5

void setup() {

  Serial.begin(9600);
  Serial.print("hi");
  pinMode(TXLED, OUTPUT);
  pinMode(RXLED, OUTPUT);

  digitalWrite(TXLED, LOW);
  digitalWrite(RXLED, HIGH);
  

}

static String buffer;
//Default frequency in Hz (doesnt really matter JWST-X will change it anyways)
static String freq = "00007000000";

void loop() {
    while (Serial.available() > 0) {
        char rx = Serial.read();
        buffer += rx;
        if (rx != ';') { // THIS IS IMPORTANT!!!
            continue;
        }
        buffer.trim();
        
        //This is where the arduino anwsers the requests sent from WSJT-X
        //Static anwsers that dont change in my usecase (see kenwood ts-480 cat datasheet for more info)
        //Radio ID (TS-480)
        if (buffer.equals("ID;")) {
            Serial.print("ID020;");
        }
        //Power status (Power ON)
        else if (buffer.startsWith("PS")) {
            Serial.print("PS1;");
        }
        //Automatic status reporting (Disabled)
        else if (buffer.equals("AI;")) {
            Serial.print("AI0;");
        }
        //VFO B frequency
        else if (buffer.equals("FB;")) {
            Serial.print("FB00007000000;");
        }
        //Operating mode (USB)
        else if (buffer.equals("MD;")) {
            Serial.print("MD2;");
        }
        //DSP filtering bandwith (0)
        else if (buffer.equals("FW;")) {
            Serial.print("FW0000;");
        }

        //Dynamic anwsers that are changed
        //Radio status
        else if (buffer.equals("IF;")) {
            Serial.print("IF00007000000     +00000000002000000 ;");
        }
        //VFO A frequency
        else if (buffer.equals("FA;")) {
            Serial.print("FA");
            Serial.print(freq);
            Serial.print(";");
        }

    
        //This is where the arduino acknowledges commands frm WSJT-X    
        //WSJT-X wants to change the VFO A freq
        else if (buffer.startsWith("FA0")) {
            freq = buffer.substring(2,13);
        }

        else if (buffer.startsWith("READ;")) {
            Serial.print(freq);
        }


        
        //Transmitt
        else if (buffer.equals("TX;")) {
            digitalWrite(TXLED, HIGH);
            digitalWrite(RXLED, LOW);
        }
        //Receive
        else if (buffer.equals("RX;")) {
            digitalWrite(TXLED, HIGH);
            digitalWrite(RXLED, LOW);
        }

        buffer = "";
    }
}
