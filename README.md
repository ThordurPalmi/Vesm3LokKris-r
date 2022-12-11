# Vesm3LokKris-Thor
Lokaverkefni Kristófer og Þórður

## Hugmyndin
Upprunalega hugmyndin var að gera dælubúnað sem ætti að fara ofan í brunndælukassa sem á það til að fyllast af vatni. Búnaðurinn var hugsaður þannig að vera með tvo hitaskynjara einn sem væri í boxinu með esp32 og hinn utan á boxinu inn í dælukassanum til að fylgjast með brunndælunni, síðan eiga að vera tveir vatnshæðaskynjarar sem er settir ofan á hvorn annan geta mælt allt að 100mm af vatns hæð. Síðan var hugsunin að vera með tvö relay eitt til að kveikja á 12v vatnsdælu til að dæla upp úr dælukassanum, og hitt til að kveikja á 240v hita streng ef hitin færi undir x°C. Síðan fyrir samskipti þar sem að við þurfum að senda gögn yfir 100m vorum við að fyrst að hugsa LORA network-ið eða 2.4Ghz útvarps samskipti 

## Lýsing
Í Bunndælu kassanum verður ESP32 sem keyrir C++ arduino kóða við hann eru tengdir báðir hita og rakaskynjaranir, tveir vatshæðanemar, NRF24 útvarpssamskipta módullinn og tvö relay til að stjórna lensi dælu og hitastreng

120m í burtu inn í sumarbústað er raspberry pi zero sem keyrir python og við hann væri eingöngu tengdur NRF24 útvarpssamskipta módullinn. hún keyrir síðan gögnin upp í hýstan flask vef sem sér um að birta gögnin

Útvarps samskeitin nota svokallaðar gagna pípur, Hvor pípan fer í aðra áttina þannig að ESP32 skrifar í fyrstu or raspberry pi hlusta og öfugt með seinni pípuna         

## Myndbönd

[![IMAGE ALT TEXT](http://img.youtube.com/vi/A4VchyImY9A/0.jpg)](http://www.youtube.com/watch?v=A4VchyImY9A "Video Title")


## Samsetning

## ESP32 

### NRF24

* CE > 22
* CSN > 21
* SCK > 18
* M0 > 23
* M1 > 19

### AM2301

* Signa > 15

### DHT22

* Out > 32

### Vatns skynjari 1

* S > VP (Gpio 36)

### Vatns skynjari 2

* S > VN (Gpio 39)

## Raspberry Pi

### NRF24

* CE > 22
* CSN > 21
* SCK > 18
* M0 > 23
* M1 > 19



<html>
<img src="https://raw.githubusercontent.com/ThordurPalmi/Vesm3LokKris-r/main/finalTenging.jpg" alt="Samsetning" style="width:100%; max-width:300px;">
</html>

## Kóði
NRF pípur notaðar [b'\xe1\xf0\xf0\xf0\xf0', b'\xd2\xf0\xf0\xf0\xf0'] python

[0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL] C++
                                
ESP32

    // Config fyrir NRF24 útvarpsendi
    static char send_payload[256];

    const int min_payload_size = 16;
    const int max_payload_size = 32;
    const int payload_size_increments_by = 1;
    int next_payload_size = min_payload_size;

    // CE = pin 22, CSN = pin 21
    RF24 radio(22, 21 );

    // Addressur ein til að hlusta á og hin til að skrifa á sem víxlast síðan á hinni nóðuni
    const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };
    char receive_payload[max_payload_size + 1];
    String message = "";
    
    void setup()
    {
      Serial.begin(115200);

      // ----- Radio Setup ----------
      radio.begin();
      // enable dynamic payloads
      radio.enableDynamicPayloads();
      radio.setRetries(5, 15);

      radio.openWritingPipe(pipes[0]);
      radio.openReadingPipe(1, pipes[1]);
      radio.startListening();  
    }

Raspberry Pi

      from circuitpython_nrf24l01.rf24 import RF24
      import spidev

      SPI_BUS, CSN_PIN, CE_PIN = (None, None, None)


      SPI_BUS = spidev.SpiDev() 
      CSN_PIN = 0  
      CE_PIN = DigitalInOut(board.D22) 
      SPI_BUS = board.SPI() 
      CE_PIN = DigitalInOut(board.D4)
      CSN_PIN = DigitalInOut(board.D5)

      nrf = RF24(SPI_BUS, CSN_PIN, CE_PIN)

      nrf.pa_level = -12

      # pípurnar í c++ eru þær [0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL]
      address = [b'\xe1\xf0\xf0\xf0\xf0', b'\xd2\xf0\xf0\xf0\xf0']

      # skrifar í pípu 2
      nrf.open_tx_pipe(address[1])  

      # hlustar á pípu 1
      nrf.open_rx_pipe(1, address[0])  # using pipe 1


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

NRF24 RPi og arduino  https://medium.com/@anujdev11/communication-between-arduino-and-raspberry-pi-using-nrf24l01-818687f7f363

nRF24L01 sending  https://create.arduino.cc/projecthub/muhammad-aqib/nrf24l01-interfacing-with-arduino-wireless-communication-0c13d4

nrf24 - https://github.com/bjarne-hansen/py-nrf24

ESP32 and AM2301 - http://www.esp32learning.com/code/esp32-and-am2301-sensor-example.php

Water sensor ESP32 - https://esp32io.com/tutorials/esp32-water-sensor
