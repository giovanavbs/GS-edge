# Global Solution - Space Route

## Link do projeto no Wowki

https://wokwi.com/projects/465383585185280001

## Integrantes

| Nome                             | RM       |
| -------------------------------- | -------- |
| Giovana Vitória Borba dos Santos | RM574054 |
| Giovane da Costa Guerra          | RM568709 |
| Mie Flora Imaizumi               | RM570303 |
| Pedro Simionato Squillante       | RM572477 |
| Thiago Santos Andrade            | RM570806 |


---

## Descrição do projeto

O Global SpaceRoute é uma plataforma inteligente de monitoramento e gerenciamento do espaço aéreo e orbital baseada na integração de dados em tempo real provenientes de satélites, radares, sistemas aeronáuticos e operações espaciais.

A solução reúne informações de aviões, helicópteros, drones, foguetes, satélites e detritos espaciais em uma única infraestrutura, permitindo a análise contínua do tráfego para identificar riscos de colisão, prever conflitos de rota e gerar desvios automáticos de forma dinâmica e precisa, reduzindo risco de mudanças de rota inesperadas e possibilitando um maior controle prévio de rotas.

Além de aumentar a segurança operacional, o sistema auxilia operações aeroespaciais críticas, como lançamentos e retornos de foguetes, através do monitoramento simultâneo de obstáculos aéreos e orbitais.

O objetivo é centralizar informações atualmente distribuídas em diferentes sistemas, possibilitando decisões mais rápidas, seguras e eficientes para o gerenciamento do tráfego aéreo e espacial.

---

# Explicação do Protótipo de Edge Computing

Para representar o funcionamento da solução proposta, foi desenvolvido um protótipo utilizando Arduino.

O protótipo simula o comportamento do Space Route em um cenário reduzido, demonstrando como o sistema seria capaz de identificar riscos futuros, recalcular rotas preventivamente e auxiliar operadores na tomada de decisão.

---

## Resumo de Funcionamento

O sistema permanece monitorando continuamente o ambiente através do sensor ultrassônico.

Com base na distância detectada, o sistema classifica a situação em três estados:

* Situação segura
* Alerta preventivo
* Emergência

Cada estado é representado por LEDs, mensagens no display LCD, mensagens no console e alertas sonoros.

---

## Modo Aéreo

O modo aéreo simula o monitoramento enquanto o foguete está passando pelo espaço aéreo.

Durante a execução, o sistema sorteia automaticamente o objeto detectado entre:

* Avião
* Helicóptero
* Drone

Quando um possível conflito é identificado, o sistema calcula uma nova rota preventiva para evitar uma possível colisão futura.

---

## Modo Orbital

O modo orbital simula o monitoramento enquanto o foguete está passando pelo espaço orbital.

Durante a execução, o sistema sorteia automaticamente o objeto detectado entre:

* Detrito espacial
* Satélite
* Foguete

Da mesma forma, ao identificar um possível conflito, o sistema realiza o recálculo preventivo da trajetória.

---

## Estado Seguro

Quando a distância detectada entre o foguete e o objeto é superior a 150 cm:

* LED verde permanece ligado;
* O sistema continua monitorando;
* Nenhum risco é identificado.

### Mensagem exibida:

> Monitorando tráfego...
> Análise preventiva ativa.

---

## Alerta Preventivo

Quando a distância detectada entre o foguete e o objeto está entre 80 cm e 150 cm:

* LED amarelo é acionado;
* LED azul é acionado;
* O buzzer emite um alerta;
* Um objeto é identificado;
* O sistema calcula uma nova rota preventiva.

Nesse momento o sistema entende que existe um possível conflito futuro, mas ainda não há risco imediato de colisão.

### Exemplo de funcionamento:

1. Identificação do objeto.
2. Cálculo de rota segura.
3. Recálculo da trajetória.
4. Geração de nova rota sugerida.
5. Solicitação de confirmação do operador.

---

### Confirmação da Nova Rota

Após o cálculo da nova trajetória, o sistema aguarda a confirmação do operador através do botão azul.

Quando o botão é pressionado:

* A nova rota é aprovada;
* O servo motor executa o movimento representando a mudança de trajetória;
* O sistema retorna ao estado de monitoramento.

Essa etapa representa a validação operacional de uma nova rota sugerida pelo sistema inteligente.

---

### Recálculo de Rota

O servo motor representa visualmente a mudança de trajetória do foguete.

Dependendo da posição atual, o sistema calcula uma nova direção possível:

* Esquerda;
* Direita;
* Centro.

A nova rota é escolhida automaticamente e executada apenas após a confirmação do operador.

Após o movimento, o servo permanece na nova posição, representando que a aeronave ou veículo espacial continua seguindo a rota corrigida.

### Mensagem exibida 

> Possível conflito detectado...
> Calculando nova rota segura.

---

## Estado de Emergência

A emergência ocorre em duas situações:

* O objeto está muito próximo (menos de 80 cm);
* O operador não confirma a nova rota dentro do tempo limite após o alerta preventivo.

Quando isso acontece:

* LED vermelho é acionado;
* O buzzer permanece ativo;
* O sistema informa risco crítico de colisão.

Essa situação representa um cenário onde o conflito não foi resolvido preventivamente e é justamente o que o sistema Space Route pretende evitar ao máximo.

---

### Desvio Emergencial

Quando o estado crítico é atingido, o operador deve pressionar o botão vermelho.

Ao receber essa ação:

* O sistema executa uma manobra emergencial;
* O servo realiza um novo desvio;
* A nova trajetória é escolhida automaticamente;
* O sistema retorna ao monitoramento.

O desvio emergencial nunca mantém a mesma rota anterior, simulando uma tentativa imediata de evitar uma colisão iminente.

### Mensagem exibida
> COLISÃO IMINENTE!
> Aguardando desvio manual...

---

## Componentes Utilizados

* Arduino Uno
* Sensor Ultrassônico HC-SR04
* Servo Motor SG90
* Display LCD 16x2 com módulo I2C
* Buzzer
* LEDs
* Botões
* Resistores
* Protoboard

---

## Tabela de Representação

| Componente                  | Representação                                                                                      |
| --------------------------- | -------------------------------------------------------------------------------------------------- |
| Servo Motor SG90            | foguete e sua trajetória de voo                                                                    |
| Sensor Ultrassônico HC-SR04 | sistema de detecção de proximidade  simulando o acompanhamento do espaço aéreo e orbital           |
| LED Verde                   | situação segura  sem risco de colisão identificado                                                 |
| LED Amarelo                 | alerta preventivo de conflito de rota                                                              |
| LED Vermelho                | situação crítica com risco iminente de colisão                                                     |
| LED Azul                    | nova rota calculada aguardando confirmação do operador                                             |
| Buzzer                      | sistema de alertas sonoros para avisos preventivos e emergenciais                                  |
| Botão Azul                  | confirmação da nova rota sugerida pelo sistema                                                     |
| Botão Vermelho              | acionamento da manobra de desvio emergencial                                                       |
| Botão Preto                 | trocar entre modo aéreo e modo orbital                                                             |
| Display LCD 16x2            | central de monitoramento, exibindo status, alertas, objetos detectados e informações operacionais  |
