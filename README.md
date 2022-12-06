# Vesm3LokKris-Thor
Lokaverkefni Kristófer og Þórður

## Hugmyndin
Upprunalega hugmindin var að gera dælubúnað sem ætti að fara ofan í brunndælukassa sem á það til að fyllast af vatni. Búnaðurinn var hugsaður þannig að vera með tvo hitaskynjara einn sem væri í kassanum utan um esp32 og hinn í utan á kassanum til að fylgjast með brunndælunni, síðan eiga að vera tveir vatns hæðaskynjarar sem er settir ofan á hvorn annan geta mælt allt að 100mm af vatsn hæð. Síðan var hugsunin að vera með tvö relay eitt til að kveikja á 12v lensi dælu til að dæla upp úr brunndælu kassanum, og hitt til að kveikja á 240v hita streng ef hitin færi undir x°C. Síðan fyrir samskipti þar sem að við þurfum að senda gögn yfir 100m vorum við að fyrst að hugsa LORA network-ið eða 2.4Ghz útvarps samskipti 

## Lýsing
Í Bunndælu kassanum verður ESP32 sem keyrir C++ arduino kóða við hann eru tengdir báðir hita og rakaskynjaranir, NRF24 útvarpssamskipta módullinn og tvö relay til að stjórna lensi dælu og hitastreng
120m í burtu inn í sumarbústað er raspberry pi zero sem keyrir python og við hann væri eingöngu tengdur NRF24 útvarpssamskipta módullinn. hún keyrir síðan gögnin upp í hýstan flask vef sem sér um að birta gögnin

## Myndbönd

## Samsetning

## Kóði

## Íhlutir

- DHT22

- ASAIR AM2301

- ESP

- RaspberryPi

- Vatnsdæla

- 2x MH Water Sensor

- 2x HW-237

- 2x NRF24L01

- 2x Loftnet


## Linkar

nRF24L01 sending  https://create.arduino.cc/projecthub/muhammad-aqib/nrf24l01-interfacing-with-arduino-wireless-communication-0c13d4

nrf24 - https://github.com/bjarne-hansen/py-nrf24

ESP32 and AM2301 - http://www.esp32learning.com/code/esp32-and-am2301-sensor-example.php

Water sensor ESP32 - https://esp32io.com/tutorials/esp32-water-sensor
