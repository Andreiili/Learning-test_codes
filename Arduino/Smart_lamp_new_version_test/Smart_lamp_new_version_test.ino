////////////////////ESTE FACUT INCEPUT LUMINITE SI INCEPUT PAGINA, MAI TREBUIE ADAUGATE
/////////////MODURILE DE LUMINARE SI LA SETARI DE ADAUGAT CATE UNA DOUA FUNCTIONALITATI
///////////// LA CULOARE TREBUIE LUCRAT CA SA IA ATRIBUTELE CUM TREBUIE
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>

ESP8266WebServer server(80);




//LEDS/////////////////////////////////LEDS////////////////////////////////LEDS
#include <FastLED.h>


#define LED_PIN     3
#define NUM_LEDS    10
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define TRAIL    220

CRGB leds[NUM_LEDS];

String BRIGHTNESS = "100"; // cat de luminos este
long UPDATES_PER_SECOND = 100; // cate schimbari face pe secunda





CRGBPalette16 currentPalette;
TBlendType    currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;




String  red, green, blue;//date pentru stocarea din pagina web , reg,green,blue ia culorile pentru astea
byte mod_aprindere;//aici se va stoca ce culoare se va arpinde cand se va activa , 1 == white , 2== Color , 3 ==  Fade In Fade Out iar 0 == off

byte moduri_led; //modificare led-uri, la moduri, 0-negru, 1- alb, 2- rainbow, 3-culoare,


bool negru = 1;
bool  alb, rainbow, culoare, settings;//imi pune in pagina efectiv iar setting imi modifica starea
byte ora;
bool adunare_punctaj = true ; // asta este pentru atunci cand imi adauga sau scade pixelul , este facut fara for , deoarece astreapta sa mi-se termine for-ul ca sa se modifice

bool culoare_buton; //atunci cand am trimis codul sa imi afiseze butonul de off
bool christmas, petrecere, nor, randome, colors; // moduri de culoare
bool black;//de la colors mod negru


boolean paleta; //pentru palete asta
//////////////////////BUTON/////////////////////////////////////////////////////////


/*

  #define CLK 13 // ALB-7
  #define DT 12 //PORTOCALIU-6
  #define buton 14 //MARO-5

  int counter = 0;
  bool currentStateCLK;
  bool lastStateCLK;
  unsigned long lastButtonPress = 0;

*/

//TIMP_ON_OFF//////////////////////TIMP_ON_OFF////////////////TIMP_ON_OFF/////

#include <WiFiUdp.h>
#include <NTPClient.h>


const long utcOffsetInSeconds = 7200;// timp setat ceasul

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);



byte ora_off, ora_on = 0;//aici imi stocheaza informatia la ce ora mi-se incepe ciclu si se inchide
byte minut_off, minut_on = 0;

byte ora_local; //pentru a ajusta ora pentr a fii dupa orarul local, no matter unde ai fii
byte timp_local_off, timp_local_on; //pentru ora definitiva
bool  alb_ora, negru_ora, rainbow_ora, culoare_ora; //pentru ora activata  se stocheaza informatia


bool  off_timp, on_timp; //atunci cand se activeaza sa pot schimba in timpul acela in care inca se indeplineste conditia
///////////////////////////////////////////////////////////////////////////////


////SITE_INTRARE/////////////////////////////////////SITE_INTRARE/////////////

String nume ;// numele care va aparea la antet
bool informatii_introduce = 0; // daca s-au introdus informatiile atunci se va a fisa meniul de control

///////////////////////////////////////////////////////////////////////////////


////SITE_OBISNUIT////////////////////////////////////SITE_OBISNUIT/////////////

bool negru = 1;
bool  alb, rainbow, culoare, settings;//imi pune in pagina efectiv iar setting imi modifica starea
byte ora;
bool adunare_punctaj = true ; // asta este pentru atunci cand imi adauga sau scade pixelul , este facut fara for , deoarece astreapta sa mi-se termine for-ul ca sa se modifice

bool culoare_buton; //atunci cand am trimis codul sa imi afiseze butonul de off
bool christmas, petrecere, nor, randome, colors; // moduri de culoare
bool black;//de la colors mod negru


///////////////////////////////////////////////////////////////////////////////





String Log_in(bool intrebare) {
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<title>Modify</title>\n";
  ptr += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr += "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr += "p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<h1>Hello</h1>\n";
  ptr += "<p>Please introduce the name should you refer to me </p>\n";
  ptr += "<FORM METHOD='POST'action='/data'>\n";// trebuie sa am o pagina ca sa se execute cand intra pe pagina /color_send
  ptr += "<input type='text' name='nume'  style=' -moz-appearance: textfield' value=" + String(nume) + " >\n";
  ptr += "<p>Put the local time</p><input type='number' name='ora_local'  style='width: 2em ; -moz-appearance: textfield' value=" + String(ora_local) + " min='0' max='12'>\n";
  ptr += "<br><input type='submit' value='Send the info'>\n";
  ptr += "</form>\n";
  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}



String Pagina_Web(bool alb , bool negru , bool rainbow , bool culoare , byte ora, bool settings, bool christmas, bool petrecere, bool nor, bool randome, bool colors) {
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<script src=\"https://cdnjs.cloudflare.com/ajax/libs/jscolor/2.0.4/jscolor.min.js\"></script>\n";
  ptr += "<title>" + nume + "</title>\n";
  ptr += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr += "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr += ".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr += ".buttonn {display: block;width: 138px;background-color: #1abc9c;border: none;color: white;padding: 13px 1px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr += ".button-on {background-color: #1abc9c;}\n";
  ptr += ".button-on:active {background-color: #16a085;}\n";
  ptr += ".button-off {background-color: #77878A;}\n";
  ptr += ".button-off:active {background-color: #2c3e50;}\n";
  ptr += "p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<h1>" + nume + "</h1>\n";
  if (negru == 0)
    ptr += "<p>Lamp - State On</p><a class=\"button button-off\" href=\"/disable\">OFF</a>\n";
  else
    ptr += "<p>Lamp - State Off</p>\n";
  if (alb == 1)
    ptr += "<p>White light - State On</p><a class=\"button button-off\" href=\"/disable\">OFF</a>\n";
  else
    ptr += "<p>White light - State Off</p><a class=\"button button-on\" href=\"/White_light_on\">ON</a>\n";
  if (culoare_buton == 1 )
    ptr += "<p>Color - State On</p><a class=\"button button-off\" href=\"/Color_on\">MODIFY</a>\n";
  else if (culoare == 1)
  { ptr += "<p>Color - State On</p>\n";
    ptr += "<FORM METHOD='POST'action='/Color_off'>\n";// trebuie sa am o pagina ca sa se execute cand intra pe pagina /color_send
    ptr += "<p>Select the color</p>\n";
    ptr += "<input class=\"jscolor {onFineChange:'update(this)'}\" id=\"rgb\"></div>\n";
    ptr += "<input type='submit' value='Change Color'>\n";
    ptr += "<script>function update(picker) {document.getElementById('rgb').innerHTML = Math.round(picker.rgb[0]) + ', ' +  Math.round(picker.rgb[1]) + ', ' + Math.round(picker.rgb[2]);\n";
    ptr += "document.getElementById(\"change_color\").href=\"?r\" + Math.round(picker.rgb[0]) + \"g\" +  Math.round(picker.rgb[1]) + \"b\" + Math.round(picker.rgb[2]) + \"&\";}\n";
    ptr += "</script></form>\n";
  }
  else
    ptr += "<p>Color - State Off</p><a class=\"button button-on\" href=\"/Color_on\">ON</a>\n";
  if (rainbow == 1)
    ptr += "<p>Rainbow - State On</p><a class=\"button button-off\" href=\"/rainbow_off\">OFF</a>\n";
  else
    ptr += "<p>Rainbow - State Off</p><a class=\"button button-on\" href=\"/rainbow_on\">ON</a>\n";
  if (christmas == 1)
    ptr += "<p>Christmas mode- State On</p><a class=\"button button-off\" href=\"/christmas_off\">OFF</a>\n";
  else
    ptr += "<p>Christmas mode- State Off</p><a class=\"button button-on\" href=\"/christmas_on\">ON</a>\n";
  if (petrecere == 1)
    ptr += "<p>Party mode- State On</p><a class=\"button button-off\" href=\"/party_off\">OFF</a>\n";
  else
    ptr += "<p>Party mode- State Off</p><a class=\"button button-on\" href=\"/party_on\">ON</a>\n";
  if (nor == 1)
    ptr += "<p>Cloud mode- State On</p><a class=\"button button-off\" href=\"/cloud_off\">OFF</a>\n";
  else
    ptr += "<p>Cloud mode- State Off</p><a class=\"button button-on\" href=\"/cloud_on\">ON</a>\n";
  if (randome == 1)
    ptr += "<p>Random mode- State On</p><a class=\"button button-off\" href=\"/random_off\">OFF</a>\n";
  else
    ptr += "<p>Random mode- State Off</p><a class=\"button button-on\" href=\"/random_on\">ON</a>\n";
  if (colors == 1)
    ptr += "<p>Colors mode- State On</p><a class=\"button button-off\" href=\"/colors_off\">OFF</a>\n";
  else
    ptr += "<p>Colors mode- State Off</p><a class=\"button button-on\" href=\"/colors_on\">ON</a>\n";
  if (( (ora_on >= 1) || (minut_on >= 1) || (minut_off >= 1) || (ora_off >= 1)) && (ora == 1))
    ptr += "<p> Time Led- State On</p><a class=\"button button-off\" href=\"/ora_off\">OFF</a>\n";
  else if (ora == 2)
  {
    ptr += "<FORM METHOD='POST'action='/ora_off'>\n";
    ptr += "<p>The time when the led's will turn on or off (00:00-24:00)</p>\n";
    ptr += "<p>Lights On</p><input type='number' name='ora_on' style='width: 2em ; -moz-appearance: textfield' value=" + String(ora_on) + " min='0' max='24'>:<input type='number' name='minut_on' style='width: 2em ; -moz-appearance: textfield' value=" + String(minut_on) + " min='0' max='60'>\n";
    ptr += "<p>Lights Off</p><input type='number' name='ora_off' style='width: 2em ; -moz-appearance: textfield' value=" + String(ora_off) + " min='0' max='24'>:<input type='number' name='minut_off' style='width: 2em ; -moz-appearance: textfield' value=" + String(minut_off) + " min='0' max='60'>\n";
    ptr += "<p>Select the light mode to start</p>\n";
    ptr += "<p>White Light "   "<input type='radio'  name='alb' value='1' >\n";
    ptr += "<p>Color "   "<input type='radio'  name='culoare' value='1' >\n";
    ptr += "<p>Rainbow"   "<input type='radio'  name='rainbow' value='1'  ></p>\n";
    ptr += "<input type='submit' value='Send The Time Range' >\n";
    ptr += "</form>\n";
    ptr += "<br>\n";
    ptr += "<br>\n";
  }
  else
    ptr += "<p> Time Led- State Off</p><a class=\"button button-on\" href=\"/ora_on\">ON</a>\n";
  if (settings == 1) {
    ptr += "<FORM METHOD='POST'action='/set'>\n";
    ptr += "<p>Percentage of the brightness:</p>\n";
    ptr += "<input type='range' id='BRIGHTNESS' name='BRIGHTNESS' min='0' max='255'>\n";
    ptr += "<p>Updates per second</p>\n";
    ptr += "<p>Time</p><input type='number' name='UPDATES_PER_SECOND' style='width: 2em ; -moz-appearance: textfield' value=" + String(UPDATES_PER_SECOND) + " min='0' max='255'></p>\n";
    ptr += "<input type='submit' value='Send The Values'>\n";
  } else
    ptr += "<a class=\"buttonn button-on\" href=\"/settings\">Settings</a>\n";

  //  ptr += "\n";
  //  ptr += "\n";
  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}

void setup() {

  Serial.begin(115200);

  delay(3000);


  ////////////////////////////////LED///////////////////////////////////////////////////////////
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS.toInt() );


  for ( int i = 0; i < NUM_LEDS; i++)
    leds[i] = CRGB::Black;


  ///////////////////////////////BUTON/////////////////////////////////////////
  /*
    pinMode(CLK, INPUT);
    pinMode(DT, INPUT);
    pinMode(buton, INPUT_PULLUP);


    lastStateCLK = digitalRead(CLK);

    attachInterrupt(digitalPinToInterrupt(2), updateEncoder, CHANGE);
    attachInterrupt(digitalPinToInterrupt(3), updateEncoder, CHANGE);
  */
  ////////////////////////////WIFI////////////////////////////////////////////////////////////

  WiFi.begin("Travis", "alex5349!");
  //WiFi.begin("da", "neinnein");
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();



  timeClient.begin();


  server.on("/data", []() {

    nume = server.arg("nume");

    ora_local = server.arg("ora_local").toInt();

    ora_local = ora_local - timeClient.getHours();

    informatii_introduce = 1;
    server.send(200, "text/html", Pagina_Web(alb, negru, rainbow, culoare, ora, settings, christmas, petrecere, nor, randome, colors));
  });


  server.on("/", []() {
    if (informatii_introduce) {
      server.send(200, "text/html", Pagina_Web(alb, negru, rainbow, culoare, ora, settings, christmas, petrecere, nor, randome, colors));
    } else
      server.send(200, "text/html", Log_in(0));

  });



  server.on("/set", []() {
    if (informatii_introduce) {
      settings = 0;

      BRIGHTNESS = server.arg("BRIGHTNESS");
      UPDATES_PER_SECOND = server.arg("UPDATES_PER_SECOND").toInt();

      FastLED.setBrightness(  BRIGHTNESS.toInt() );

      server.send(200, "text/html", Pagina_Web(alb, negru, rainbow, culoare, ora, settings, christmas, petrecere, nor, randome, colors));
    } else
      server.send(200, "text/html", Log_in(0));

  });

  server.on("/settings", []() {


    if (informatii_introduce) {

      settings = 1;

      server.send(200, "text/html", Pagina_Web(alb, negru, rainbow, culoare, ora, settings, christmas, petrecere, nor, randome, colors));
    } else
      server.send(200, "text/html", Log_in(0));

  });



  server.on("/Color_off", []() {



    if (informatii_introduce) {

      rainbow = 0;
      alb = 0;
      negru = 0;
      culoare = 1;
      culoare_buton = 1;
      christmas = 0;
      petrecere = 0;
      nor = 0 ;
      randome = 0;
      colors = 0;
      //  red = server.arg("rgb");

      server.send(200, "text/html", Pagina_Web(alb, negru, rainbow, culoare, ora, settings, christmas, petrecere, nor, randome, colors));
    } else
      server.send(200, "text/html", Log_in(0));
  });


  server.on("/Color_on", []() {


    if (informatii_introduce) {

      rainbow = 0;
      alb = 0;
      negru = 0;
      culoare = 1;
      culoare_buton = 0;
      christmas = 0;
      petrecere = 0;
      nor = 0 ;
      randome = 0;
      colors = 0;

      server.send(200, "text/html", Pagina_Web(alb, negru, rainbow, culoare, ora, settings, christmas, petrecere, nor, randome, colors));
    } else
      server.send(200, "text/html", Log_in(0));
  });

  server.on("/rainbow_off", []() {


    if (informatii_introduce) {

      rainbow = 0;
      alb = 0;
      negru = 0;
      culoare = 0;
      culoare_buton = 0;
      christmas = 0;
      petrecere = 0;
      nor = 0 ;
      randome = 0;
      colors = 0;

      server.send(200, "text/html", Pagina_Web(alb, negru, rainbow, culoare, ora, settings, christmas, petrecere, nor, randome, colors));
    } else
      server.send(200, "text/html", Log_in(0));
  });

  server.on("/disable", []() {


    if (informatii_introduce) {

      rainbow = 0;
      alb = 0;
      negru = 1;
      culoare = 0;
      culoare_buton = 0;
      christmas = 0;
      petrecere = 0;
      nor = 0 ;
      randome = 0;
      colors = 0;

      server.send(200, "text/html", Pagina_Web(alb, negru, rainbow, culoare, ora, settings, christmas, petrecere, nor, randome, colors));
    } else
      server.send(200, "text/html", Log_in(0));
  });

  server.on("/rainbow_on", []() {


    if (informatii_introduce) {

      alb = 0;
      negru = 0;
      culoare = 0;
      rainbow = 1;
      culoare_buton = 0;
      christmas = 0;
      petrecere = 0;
      nor = 0 ;
      randome = 0;
      colors = 0;

      server.send(200, "text/html", Pagina_Web(alb, negru, rainbow, culoare, ora, settings, christmas, petrecere, nor, randome, colors));
    } else
      server.send(200, "text/html", Log_in(0));
  });

  server.on("/White_light_on", []() {


    if (informatii_introduce) {

      rainbow = 0;
      negru = 0;
      culoare = 0;
      alb = 1;
      culoare_buton = 0;
      christmas = 0;
      petrecere = 0;
      nor = 0 ;
      randome = 0;
      colors = 0;

      server.send(200, "text/html", Pagina_Web(alb, negru, rainbow, culoare, ora, settings, christmas, petrecere, nor, randome, colors));
    } else
      server.send(200, "text/html", Log_in(0));
  });


  server.on("/ora_on", []() {
    ora = 2;


    if (informatii_introduce) {

      server.send(200, "text/html", Pagina_Web(alb, negru, rainbow, culoare, ora, settings, christmas, petrecere, nor, randome, colors));
    } else
      server.send(200, "text/html", Log_in(0));
  });


  server.on("/ora_off", []() {
    ora = 1;

    if (informatii_introduce) {




      ora_on = server.arg("ora_on").toInt();
      minut_on = server.arg("minut_on").toInt();

      ora_off = server.arg("ora_off").toInt();
      minut_off = server.arg("minut_off").toInt();

      rainbow_ora = server.arg("rainbow").toInt();
      culoare_ora = server.arg("culoare").toInt();
      alb_ora = server.arg("alb").toInt();



      if ( (ora_on >= 1) || (minut_on >= 1))
        on_timp = 1;
      else
        on_timp = 0;

      if  ((minut_off >= 1) || (ora_off >= 1))
        off_timp = 1;
      else
        off_timp = 0;



      server.send(200, "text/html", Pagina_Web(alb, negru, rainbow, culoare, ora, settings, christmas, petrecere, nor, randome, colors));
    } else
      server.send(200, "text/html", Log_in(0));
  });


  server.on("/ora_dezactivare", []() {
    ora = 0;

    if (informatii_introduce)
      server.send(200, "text/html", Pagina_Web(alb, negru, rainbow, culoare, ora, settings, christmas, petrecere, nor, randome, colors));
    else
      server.send(200, "text/html", Log_in(0));
  });



  server.on("/christmas_off", []() {

    if (informatii_introduce) {

      rainbow = 0;
      alb = 0;
      negru = 0;
      culoare = 0;
      culoare_buton = 0;
      christmas = 0;
      petrecere = 0;
      nor = 0 ;
      randome = 0;
      colors = 0;

      server.send(200, "text/html", Pagina_Web(alb, negru, rainbow, culoare, ora, settings, christmas, petrecere, nor, randome, colors));
    } else
      server.send(200, "text/html", Log_in(0));
  });


  server.on("/christmas_on", []() {

    if (informatii_introduce) {

      rainbow = 0;
      alb = 0;
      negru = 0;
      culoare = 0;
      culoare_buton = 0;
      christmas = 1;
      petrecere = 0;
      nor = 0 ;
      randome = 0;
      colors = 0;

      server.send(200, "text/html", Pagina_Web(alb, negru, rainbow, culoare, ora, settings, christmas, petrecere, nor, randome, colors));
    } else
      server.send(200, "text/html", Log_in(0));
  });








  server.on("/party_off", []() {

    if (informatii_introduce) {

      rainbow = 0;
      alb = 0;
      negru = 0;
      culoare = 0;
      culoare_buton = 0;
      christmas = 0;
      petrecere = 0;
      nor = 0 ;
      randome = 0;
      colors = 0;

      server.send(200, "text/html", Pagina_Web(alb, negru, rainbow, culoare, ora, settings, christmas, petrecere, nor, randome, colors));
    } else
      server.send(200, "text/html", Log_in(0));
  });


  server.on("/party_on", []() {

    if (informatii_introduce) {

      rainbow = 0;
      alb = 0;
      negru = 0;
      culoare = 0;
      culoare_buton = 0;
      christmas = 0;
      petrecere = 1;
      nor = 0 ;
      randome = 0;
      colors = 0;

      server.send(200, "text/html", Pagina_Web(alb, negru, rainbow, culoare, ora, settings, christmas, petrecere, nor, randome, colors));
    } else
      server.send(200, "text/html", Log_in(0));
  });





  server.on("/cloud_off", []() {

    if (informatii_introduce) {

      rainbow = 0;
      alb = 0;
      negru = 0;
      culoare = 0;
      culoare_buton = 0;
      christmas = 0;
      petrecere = 0;
      nor = 0 ;
      randome = 0;
      colors = 0;

      server.send(200, "text/html", Pagina_Web(alb, negru, rainbow, culoare, ora, settings, christmas, petrecere, nor, randome, colors));
    } else
      server.send(200, "text/html", Log_in(0));
  });


  server.on("/cloud_on", []() {

    if (informatii_introduce) {

      rainbow = 0;
      alb = 0;
      negru = 0;
      culoare = 0;
      culoare_buton = 0;
      christmas = 0;
      petrecere = 0;
      nor = 1;
      randome = 0;
      colors = 0;

      server.send(200, "text/html", Pagina_Web(alb, negru, rainbow, culoare, ora, settings, christmas, petrecere, nor, randome, colors));
    } else
      server.send(200, "text/html", Log_in(0));
  });







  server.on("/random_off", []() {

    if (informatii_introduce) {

      rainbow = 0;
      alb = 0;
      negru = 0;
      culoare = 0;
      culoare_buton = 0;
      christmas = 0;
      petrecere = 0;
      nor = 0 ;
      randome = 0;
      colors = 0;

      server.send(200, "text/html", Pagina_Web(alb, negru, rainbow, culoare, ora, settings, christmas, petrecere, nor, randome, colors));
    } else
      server.send(200, "text/html", Log_in(0));
  });


  server.on("/random_on", []() {

    if (informatii_introduce) {

      rainbow = 0;
      alb = 0;
      negru = 0;
      culoare = 0;
      culoare_buton = 0;
      christmas = 0;
      petrecere = 0;
      nor = 0;
      randome = 1;
      colors = 0;

      server.send(200, "text/html", Pagina_Web(alb, negru, rainbow, culoare, ora, settings, christmas, petrecere, nor, randome, colors));
    } else
      server.send(200, "text/html", Log_in(0));
  });





  server.on("/colors_off", []() {

    if (informatii_introduce) {

      rainbow = 0;
      alb = 0;
      negru = 0;
      culoare = 0;
      culoare_buton = 0;
      christmas = 0;
      petrecere = 0;
      nor = 0 ;
      randome = 0;
      colors = 0;

      server.send(200, "text/html", Pagina_Web(alb, negru, rainbow, culoare, ora, settings, christmas, petrecere, nor, randome, colors));
    } else
      server.send(200, "text/html", Log_in(0));
  });


  server.on("/colors_on", []() {

    if (informatii_introduce) {

      rainbow = 0;
      alb = 0;
      negru = 0;
      culoare = 0;
      culoare_buton = 0;
      christmas = 0;
      petrecere = 0;
      nor = 0;
      randome = 0;
      colors = 1;

      server.send(200, "text/html", Pagina_Web(alb, negru, rainbow, culoare, ora, settings, christmas, petrecere, nor, randome, colors));
    } else
      server.send(200, "text/html", Log_in(0));
  });



  
  server.begin();



}
const CRGB lightcolor(8, 7, 1);

void loop(void) {
  server.handleClient();
  timeClient.update();

  // Serial.println(red);
  /*
    Serial.println("Real");
    Serial.print(timeClient.getHours());
    Serial.print(":");
    Serial.print(timeClient.getMinutes());

    Serial.println();
    Serial.println();

    Serial.println("ESP8266");
    Serial.print(ora_on);
    Serial.print(":");
    Serial.print(minut_on);

    Serial.println();
    Serial.println();
    Serial.println();
  */

  Serial.println("ora_local_on");
  Serial.println(timp_local_on);
  Serial.println("ora_local_off");
  Serial.println(timp_local_off);
  //  Serial.println("UPDATES_PER_SECOND");
  //  Serial.println(UPDATES_PER_SECOND);





  if (on_timp) {

    if ((ora_on - ora_local) < 0)
      timp_local_on = 24 - ora_on + ora_local;
    else
      timp_local_on = ora_on - ora_local;

    if ((timeClient.getHours()) == timp_local_on)
      if (timeClient.getMinutes() == minut_on) {
        alb = alb_ora;
        culoare = culoare_ora;
        rainbow = rainbow_ora;
        negru = 0;

        ora = 0;
        on_timp = 0;
      }
  }

  if (off_timp) {

    if ((ora_off - ora_local) < 0)
      timp_local_off = 24 - ora_off + ora_local;
    else
      timp_local_off = ora_off - ora_local;


    if (timeClient.getHours() == timp_local_off)
      if (timeClient.getMinutes() == minut_off) {
        alb = 0;
        culoare = 0;
        rainbow = 0;
        negru = 1;

        ora = 0;
        off_timp = 0;
      }

  }

  if (alb)
  {

    for ( int i = 0; i < NUM_LEDS; i++)
      leds[i] = CRGB(0xE1A024) ;

    paleta = 0;
  }

  /*
      if (culoare == 1)
      {

        for (uint8_t pixel = 0; pixel <= PixelCount; pixel++) {
          RgbColor color = RgbColor(red, green, blue);
          strip.SetPixelColor(pixel, color);

        }
      }
  */

  if (rainbow)
  {
    currentPalette = RainbowColors_p;         currentBlending = LINEARBLEND;
    //currentPalette = RainbowStripeColors_p;   currentBlending = NOBLEND;
    //currentPalette = RainbowStripeColors_p;   currentBlending = LINEARBLEND;

    paleta = 1;
  }


  if (negru) {

    for ( int i = 0; i < NUM_LEDS; i++)
      leds[i] = CRGB::Black;

    paleta = 0;
  }



  if (colors) {

    if (black) {
      currentBlending = LINEARBLEND;
      fill_solid( currentPalette, 16, CRGB::Black);
      currentPalette[0] = CRGB::White;
      currentPalette[4] = CRGB::White;
      currentPalette[8] = CRGB::White;
      currentPalette[12] = CRGB::White;
    } else {
      currentBlending = LINEARBLEND;

      CRGB purple = CHSV( HUE_PURPLE, 255, 255);
      CRGB green  = CHSV( HUE_GREEN, 255, 255);
      CRGB black  = CRGB::Black;

      currentPalette = CRGBPalette16(
                         green,  green,  black,  black,
                         purple, purple, black,  black,
                         green,  green,  black,  black,
                         purple, purple, black,  black );


    }
    paleta = 1;

  }

  //////////////////////fluid treuie facut/////////////////////////////////////////////////////////

  if (randome) {
    FastLED.delay(250 );
    
    for ( int i = 0; i <= NUM_LEDS; i++) {
      currentPalette[i] = 
      FastLED.show();
      delay(50);
    }
    
    currentBlending = LINEARBLEND;
    paleta = 1;

  }
  

  if (nor) {
    currentPalette = CloudColors_p;           currentBlending = LINEARBLEND;
    paleta = 1;

  }

  if (petrecere) {
    currentPalette = PartyColors_p;           currentBlending = LINEARBLEND;
    paleta = 1;

  }



  static uint8_t startIndex = 0;
  startIndex = startIndex + 1; /* motion speed */

  if (paleta)
    FillLEDsFromPaletteColors( startIndex);

  FastLED.show();
  if(!randome)
  FastLED.delay(1000 / UPDATES_PER_SECOND); // trebui facut ca prin site sa modifici timpu la care se da  update la led-uri


  //////////////////////christmas treuie facut/////////////////////////////////////////////////////////

  if (christmas) {
    for ( int i = 0; i <= NUM_LEDS; i++) {
      //Red, Saturation, Brightness
      leds[i] = CRGB( 0,     255,          255);
      FastLED.show();
      delay(50);
      fadeall();
    }
    for ( int i = NUM_LEDS; 0 <= i; i--) {
      //Green, Saturation, Brightness
      leds[i] = CRGB( 85,       255,       255);
      FastLED.show();
      delay(50);
      fadeall();
    }

  }

}




void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
  uint8_t brightness = 255;

  for ( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
    colorIndex += 3;
  }
}


void fadeall() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].nscale8(TRAIL);
  }
}

/*

  void updateEncoder() {
  currentStateCLK = digitalRead(CLK);

  if (currentStateCLK != lastStateCLK  && currentStateCLK == 1) {

    if (digitalRead(DT) != currentStateCLK) {
      counter --;
    } else {
      counter ++;
    }


    Serial.print("Counter: ");
    Serial.println(counter);
  }

  lastStateCLK = currentStateCLK;
  }


*/
