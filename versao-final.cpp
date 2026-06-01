#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

Servo foguete;

const int ledVerde = 2;
const int ledAmarelo = 3;
const int ledVermelho = 4;
const int ledAzul = 5;
const int buzzer = 6;
const int servoPin = 7;
const int trigPin = 8;
const int echoPin = 9;
const int btnAzul = 10;
const int btnVermelho = 11;
const int botaoModo = 12;

bool modoAereo = true;
bool aguardandoBtnAzul = false;
bool emergenciaAtiva = false;
bool msgEmergencia = false;

unsigned long ultimo = 0;
unsigned long tempoInicio = 0;

const unsigned long pausa = 3000;
const unsigned long tempoMax = 7000;

String objeto = "";
String direcaoNova = "";

int atualServo = 90;
int novoServo = 90;

long distanciaAtual = 200;

bool pausaAtiva = false;
unsigned long inicioPausa = 0;

const unsigned long tempoPausa = 5000; 

void setup() {

  Serial.begin(9600);

  pinMode(ledVerde, OUTPUT);
  pinMode(ledAmarelo, OUTPUT);
  pinMode(ledVermelho, OUTPUT);
  pinMode(ledAzul, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(btnAzul, INPUT_PULLUP);
  pinMode(btnVermelho, INPUT_PULLUP);
  pinMode(botaoModo, INPUT_PULLUP);

  foguete.attach(servoPin);
  foguete.write(90);

  lcd.init();
  lcd.backlight();

  randomSeed(analogRead(A0));

  statusInicial();
}

void loop() {

  trocarModo();

if (pausaAtiva) {

  if (millis() - inicioPausa >= tempoPausa) {

    pausaAtiva = false;

    Serial.println();
    Serial.println("[SISTEMA]");
    Serial.println("Retornando ao monitoramento.");
    Serial.println();
  }

  return;
}

  if (emergenciaAtiva) {

    estadoEmergencia();
    return;
  }

  if (aguardandoBtnAzul) {

    verificarConfirmacao();

    if (millis() - tempoInicio >= tempoMax) {

      aguardandoBtnAzul = false;
      emergenciaAtiva = true;
    }

    return;
  }

  monitoramento();
}

void monitoramento() {

  digitalWrite(ledVerde, HIGH);
  digitalWrite(ledAmarelo, LOW);
  digitalWrite(ledVermelho, LOW);
  digitalWrite(ledAzul, LOW);

  noTone(buzzer);

  if (millis() - ultimo >= pausa) {

    ultimo = millis();

    distanciaAtual = medirDistancia();

    Serial.println("================================");
    Serial.println("[SISTEMA]");
    Serial.println("Monitorando trafego...");
    Serial.println("Analise preventiva ativa.");

    Serial.print("Distancia detectada: ");
    Serial.print(distanciaAtual);
    Serial.println(" cm");

    if (modoAereo) {
      Serial.println("Modo atual: AEREO");
    } else {
      Serial.println("Modo atual: ORBITAL");
    }

    if (distanciaAtual >= 80 && distanciaAtual <= 150) {

      AlertaPreventivo();
    }

    if (distanciaAtual < 80) {

      emergenciaAtiva = true;
    }
  }
}

void AlertaPreventivo() {

  aguardandoBtnAzul = true;

  gerarObjeto();
  gerarNovaRota();

  digitalWrite(ledVerde, LOW);
  digitalWrite(ledAmarelo, HIGH);
  digitalWrite(ledAzul, HIGH);

  tone(buzzer, 1000, 250);

  Serial.println();
  Serial.println("================================");
  Serial.println("[ALERTA PREVENTIVO]");

  Serial.println(objeto + " DETECTADO");

  Serial.print("Distancia: ");
  Serial.print(distanciaAtual);
  Serial.println(" cm");

Serial.println();
Serial.println("Calculando rota segura...");
delay(1000);

Serial.println("Recalculando trajetoria...");
delay(1000);

Serial.println();

Serial.println("Nova rota preventiva encontrada.");

Serial.print("Nova rota sugerida: ");
Serial.println(direcaoNova);

Serial.println();
Serial.println("Aguardando confirmacao...");

  Serial.println();

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print(objeto);

lcd.setCursor(0, 1);

if (direcaoNova == "ESQUERDA")
  lcd.print("ROTA ESQ");

else if (direcaoNova == "DIREITA")
  lcd.print("ROTA DIR");

else
  lcd.print("ROTA CTR");

  tempoInicio = millis();
}

void verificarConfirmacao() {

  static bool ultimoEstado = HIGH;

  bool estadoAtual = digitalRead(btnAzul);

  if (ultimoEstado == HIGH && estadoAtual == LOW) {

    Serial.println("[CONFIRMADO]");
    Serial.println("Nova rota aprovada.");
    Serial.println();

    moverServo();

Serial.println("Executando mudanca de rota...");
Serial.println();

Serial.print("Rota atual alterada para: ");
Serial.println(direcaoNova);

    Serial.println();
    Serial.println("Conflito evitado com antecedencia.");
    Serial.println("Retornando ao monitoramento...");
    Serial.println();

    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("ROTA OK");

    lcd.setCursor(0, 1);
    lcd.print("MONITORANDO");

    digitalWrite(ledVerde, HIGH);
    digitalWrite(ledAmarelo, LOW);
    digitalWrite(ledAzul, LOW);

    aguardandoBtnAzul = false;

    distanciaAtual = 200;

    pausaAtiva = true;
    inicioPausa = millis();
  }

  ultimoEstado = estadoAtual;
}

void estadoEmergencia() {

  digitalWrite(ledVerde, LOW);
  digitalWrite(ledAmarelo, LOW);
  digitalWrite(ledAzul, LOW);
  digitalWrite(ledVermelho, HIGH);

  tone(buzzer, 2000);

  if (!msgEmergencia) {

    Serial.println("================================");
    Serial.println("[CRITICO]");
    Serial.println("Confirmacao nao recebida.");
    Serial.println("Objeto continua em aproximacao.");
    Serial.println();
    Serial.println("COLISAO IMINENTE.");
    Serial.println();
    Serial.println("Aguardando desvio manual...");
    Serial.println();

    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("COLISAO");

    lcd.setCursor(0, 1);
    lcd.print("IMINENTE");

    msgEmergencia = true;

  }

  static bool ultimoEstadoEmergencia = HIGH;

  bool estadoAtualEmergencia = digitalRead(btnVermelho);

  if (ultimoEstadoEmergencia == HIGH &&
      estadoAtualEmergencia == LOW) {

    Serial.println("[EMERGENCIA RESOLVIDA]");
    Serial.println("Desvio manual realizado.");
    Serial.println();
    desvioEmergencial();

    Serial.println("Retornando ao monitoramento...");
    Serial.println();

    noTone(buzzer);

    emergenciaAtiva = false;
    msgEmergencia = false;

    distanciaAtual = 200;

lcd.clear();

lcd.setCursor(0, 0);
lcd.print("DESVIO OK");

lcd.setCursor(0, 1);
lcd.print("MONITORANDO");

pausaAtiva = true;
inicioPausa = millis();
  }

  ultimoEstadoEmergencia = estadoAtualEmergencia;
}

void trocarModo() {

  static bool ultimoEstadoBotao = HIGH;

  bool estadoAtual = digitalRead(botaoModo);

  if (ultimoEstadoBotao == HIGH && estadoAtual == LOW) {

    modoAereo = !modoAereo;

    Serial.println("================================");

    if (modoAereo) {

      Serial.println("MODO ALTERADO: AEREO");

      lcd.clear();
      lcd.print("MODO AEREO");

    } else {

      Serial.println("MODO ALTERADO: ORBITAL");

      lcd.clear();
      lcd.print("MODO ORBITAL");
    }

    delay(300);
  }

  ultimoEstadoBotao = estadoAtual;
}

void gerarObjeto() {

  int sorteio = random(0, 3);

  if (modoAereo) {

    if (sorteio == 0) objeto = "DRONE";
    if (sorteio == 1) objeto = "HELICOP";
    if (sorteio == 2) objeto = "AVIAO";

  } else {

    if (sorteio == 0) objeto = "DETRITO";
    if (sorteio == 1) objeto = "SATELITE";
    if (sorteio == 2) objeto = "FOGUETE";
  }
}

void gerarNovaRota() {

  if (atualServo == 90) {

    int sorteio = random(0, 2);

    if (sorteio == 0) {
      direcaoNova = "ESQUERDA";
      novoServo = 45;
    }
    else {
      direcaoNova = "DIREITA";
      novoServo = 135;
    }
  }

  else if (atualServo == 45) {

    int sorteio = random(0, 2);

    if (sorteio == 0) {
      direcaoNova = "CENTRO";
      novoServo = 90;
    }
    else {
      direcaoNova = "DIREITA";
      novoServo = 135;
    }
  }

  else {

    int sorteio = random(0, 2);

    if (sorteio == 0) {
      direcaoNova = "CENTRO";
      novoServo = 90;
    }
    else {
      direcaoNova = "ESQUERDA";
      novoServo = 45;
    }
  }
}

void moverServo() {

  if (novoServo > atualServo) {

    for (int i = atualServo;
         i <= novoServo;
         i++) {

      foguete.write(i);
      delay(10);
    }
  }
  else {

    for (int i = atualServo;
         i >= novoServo;
         i--) {

      foguete.write(i);
      delay(10);
    }
  }

  atualServo = novoServo;
}

void desvioEmergencial() {

  int novaPosicao;

  do {

    int sorteio = random(0, 3);

    if (sorteio == 0) {
      novaPosicao = 45;
    }
    else if (sorteio == 1) {
      novaPosicao = 90;
    }
    else {
      novaPosicao = 135;
    }

  } while (novaPosicao == atualServo);


  if (novaPosicao > atualServo) {

    for (int i = atualServo;
         i <= novaPosicao;
         i++) {

      foguete.write(i);
      delay(10);
    }
  }
  else {

    for (int i = atualServo;
         i >= novaPosicao;
         i--) {

      foguete.write(i);
      delay(10);
    }
  }

  atualServo = novaPosicao;
}

long medirDistancia() {

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

  long duracao = pulseIn(echoPin, HIGH);

  long distancia = duracao * 0.034 / 2;

  return distancia;
}

void statusInicial() {

  lcd.clear();

  if (modoAereo) {

    lcd.print("MODO AEREO");

  } else {

    lcd.print("MODO ORBITAL");
  }

  lcd.setCursor(0, 1);
  lcd.print("MONITORANDO");
}