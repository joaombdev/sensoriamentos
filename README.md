# sensoriamentos
Respositório contém os diversos arquivos com códigos para diferentes tipo de funções, mas o que todos têm em comum são que todos eles operam sensores, como sensores ultrasônicos, de temperatura e luz. O arquivo final é o <a targe_blaknk href='https://github.com/joaombdev/sensoriamentos/blob/main/colmeia.ino'>colmeia.ino</a> que envia os dados o banco de dados no firebase, e são puxados para a página html.

Sensor de temperatura: DHT22
Sensor de luminosidade: LDR
Sensor de carga: Hx711

### Guia:
- julio_cesar.ino
  Código com validação para mudar os leds, só se a mudança for maior que 11, sem firebase

- kelvin_do_skate.ino
  Código com a condicional de mudanças pequenas e conexão com o firebase

- led-e-ldr-china.ino
  código com conexão ao firebase, com os sensores e led, mas sem a condicional de mudanças
