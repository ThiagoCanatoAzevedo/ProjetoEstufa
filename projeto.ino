#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 10, 5, 4, 3, 2);

int backLight       = 13;
const int ledSaida1 = 6;
const int ledSaida2 = 7;
const int pinoSensorUmidade = A0;

const int botaoEnter     = 0;
const int botaoSelecione = 8;
const int botaoAdicao    = 9;

int adicionaEnter       = 0;
int adicionaSelecionar  = 1;
int adicionaBotaoAdicao = 0;

int estadoSelecione      = 1;
int estadoEnter          = 1;
int estadoBotaoAdiciona1 = 0;
int estadoBotaoAdiciona2 = 0;

int teste = 0;

int travarTelaConfigsQuandoForParaOpcoes = 0;
int tempoEspera = 0;
unsigned long valorEmMilisegundos;
unsigned long valorEmMilisegundos2;

bool if1_executado = false;
bool if2_executado = false;

String mensagem;
String tempoAgua;
String intervalo;
String parteTempoAgua1, parteTempoAgua2;
String parteIntervalo1, parteIntervalo2;

void setup() {
  pinMode(backLight, OUTPUT);
  digitalWrite(backLight, HIGH);
  lcd.begin(16, 2);
  lcd.print("     ESTUFA   ");
  lcd.setCursor(0, 1);
  lcd.print("       ET     ");
  pinMode(botaoSelecione, INPUT_PULLUP);
  pinMode(botaoEnter, INPUT_PULLUP);
  pinMode(botaoAdicao, INPUT_PULLUP);
  pinMode(pinoSensorUmidade, INPUT);
  // pinMode(ledSaida1, OUTPUT);
  // pinMode(ledSaida2, OUTPUT);
  delay(1000);
  lcd.setCursor(0, 0);
  lcd.print(" DEFINA OPCOES ");
  lcd.setCursor(0, 1);
  lcd.print("CONF.1     SELEC");
  Serial.begin(9600);
}

void loop() {
  //ENTER
  if (!digitalRead(botaoEnter)) {
    estadoEnter                          = 0;
    adicionaEnter                        = adicionaEnter + 1;
    travarTelaConfigsQuandoForParaOpcoes = 1;

    if (adicionaEnter > 2) {
      adicionaEnter = 1;
    }

    delay(400); 
  }

  //SELECIONE
  if (!digitalRead(botaoSelecione) && travarTelaConfigsQuandoForParaOpcoes == 0) {
    adicionaSelecionar = adicionaSelecionar + 1;
    estadoSelecione    = 0;
    
    if (adicionaSelecionar > 4) {
      adicionaSelecionar = 1;
    }
    
    delay(400); 
  }

  //ADICIONAR
  if (!digitalRead(botaoAdicao)) {
    Serial.println(adicionaSelecionar);
    // estadoBotaoAdiciona1, estadoBotaoAdiciona2 = 0;

    if (adicionaSelecionar == 1) {
      estadoBotaoAdiciona1 = estadoBotaoAdiciona1 + 1;
      if (estadoBotaoAdiciona1 > 6) {
        estadoBotaoAdiciona1 =0;
      }
    }
    if (adicionaSelecionar == 2) {
      estadoBotaoAdiciona2 = estadoBotaoAdiciona2 + 1;
      if (estadoBotaoAdiciona2 > 17) {
        estadoBotaoAdiciona2 = 0;
      }

    }
    delay(400);
  }

  //SWITCH ENTER
  if (estadoEnter == 0) {
    travarTelaConfigsQuandoForParaOpcoes = 1;
    String valoresOpcaoConfig1[] = { "05 SEG", "10 SEG", "15 SEG", "20 SEG", "40 SEG", "01 MIN", "02 MIN" };
    String valoresOpcaoConfig2[] = { "01 MIN", "02 MIN", "05 MIN", "10 MIN", "20 MIN", "30 MIN", "01  HR", "02 HRS", "04 HRS", "06 HRS", "08 HRS", 
    "12 HRS", "16 HRS", "1  DIA","2 DIAS","3 DIAS","5 DIAS", "7 DIAS" };

    switch (adicionaEnter) {
      case 1:
        if (adicionaSelecionar == 1) {
          lcd.setCursor(0, 0);
          lcd.print("   TEMPO AGUA   ");
          lcd.setCursor(0, 1);
          lcd.print(valoresOpcaoConfig1[estadoBotaoAdiciona1] + "     SELEC");
        }
        if (adicionaSelecionar == 2) {
          lcd.setCursor(0, 0);
          lcd.print("   INTERVALO   ");
          lcd.setCursor(0, 1);
          lcd.print(valoresOpcaoConfig2[estadoBotaoAdiciona2] + "     SELEC");
        }

        if (adicionaSelecionar == 3) {
          tempoEspera = 0;

          lcd.setCursor(0, 0);
          lcd.print("INICIALIZADO   ");
          lcd.setCursor(0, 1);
          lcd.print("APERTE PARA SAIR           ");

          tempoAgua = valoresOpcaoConfig1[estadoBotaoAdiciona1];
          intervalo = valoresOpcaoConfig2[estadoBotaoAdiciona2];
           
          int pontoDeDivisaoTempoAgua = tempoAgua.length() / 2;
          int pontoDeDivisaoIntervalo = intervalo.length() / 2;

          parteTempoAgua1 = tempoAgua.substring(0, pontoDeDivisaoTempoAgua);
          parteTempoAgua2 = tempoAgua.substring(pontoDeDivisaoTempoAgua);
          if(parteTempoAgua2 == "SEG"){
            valorEmMilisegundos = parteTempoAgua1.toInt() * 1000;
            // Serial.println(valorEmMilisegundos);
          }
          if(parteTempoAgua2 == "MIN"){
            valorEmMilisegundos = parteTempoAgua1.toInt() * 60000;
            // Serial.println(valorEmMilisegundos);
          }

          parteIntervalo1 = intervalo.substring(0, pontoDeDivisaoIntervalo);
          parteIntervalo2 = intervalo.substring(pontoDeDivisaoIntervalo);
          if(parteIntervalo2 == "SEG"){
            valorEmMilisegundos2 = parteIntervalo1.toInt() * 1000;
          }
          else if(parteIntervalo2 == "MIN"){
            valorEmMilisegundos2 = parteIntervalo1.toInt() * 60000;
          }
          else{
            valorEmMilisegundos2 = parteIntervalo1.toInt() * 3600000;
          }
          digitalWrite(ledSaida1, HIGH);

          delay(4000);

          String texto = "   "+String(analogRead(pinoSensorUmidade))+"     ";
          lcd.setCursor(0, 0);
          lcd.print("   UMIDADE     ");
          lcd.setCursor(0, 1);
          lcd.print(texto+ "    ");
          delay(6000);
        }

        if (adicionaSelecionar == 4) {
          lcd.setCursor(0, 0);
          lcd.print("PARANDO...      ");
          lcd.setCursor(0, 1);
          lcd.print("                ");

          delay(1);

        }


       
        break;

      case 2:
        travarTelaConfigsQuandoForParaOpcoes = 0;
        if(adicionaSelecionar == 3){
          mensagem = "CONF.COMECAR" ;
        }
        else if(adicionaSelecionar == 4){
          mensagem = "CONF.PARAR";
        }
        else{
          mensagem = "CONF." + String(adicionaSelecionar);
        }

        lcd.setCursor(0, 0);
        lcd.print(" DEFINA OPCOES ");
        lcd.setCursor(0, 1);
        lcd.print(mensagem);
        break;
    }

    delay(1);
  }

  //SWITCH SELECIONE
  if (travarTelaConfigsQuandoForParaOpcoes == 0) {
    
    switch (adicionaSelecionar) {
      case 1:
        lcd.setCursor(0, 0);
        lcd.print(" DEFINA OPCOES ");
        lcd.setCursor(0, 1);
        lcd.print("CONF.1     SELEC");
        break;
      case 2:
        lcd.setCursor(0, 0);
        lcd.print(" DEFINA OPCOES ");
        lcd.setCursor(0, 1);
        lcd.print("CONF.2     SELEC");
        break;
      case 3:
        lcd.setCursor(0, 0);
        lcd.print(" DEFINA OPCOES ");
        lcd.setCursor(0, 1);
        lcd.print("CONF.COMECAR    ");
        break;
      case 4:
        lcd.setCursor(0, 0);
        lcd.print(" DEFINA OPCOES ");
        lcd.setCursor(0, 1);
        lcd.print("CONF.PARAR      ");
        break;
    }

    delay(1);
  }

}
