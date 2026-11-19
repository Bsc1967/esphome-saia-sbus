# Validatie op 19 november 2026

## Uitbreiding naar zeven metingen

Volledige firmwarebuild met alle zeven sensoren geslaagd op ESPHome 2026.11.0 / ESP-IDF 5.5.6 (exitcode 0). RAM 99251 bytes; flash 750195 bytes. Vier onafhankelijke CRC-referentietests opnieuw geslaagd.

De gebruiker heeft inmiddels energie en spanning op de fysieke meter bevestigd, zonder aangesloten belasting. De nieuwe deelteller, reactief vermogen en cos phi moeten nog op hardware worden gecontroleerd. De uitbreiding leest negen registers (R20–R28), binnen het maximum van tien. Het nieuwe request is `B5 00 01 06 08 00 14 FD 0F`. De bestaande vier sensoren blijven verplicht; de drie nieuwe zijn optioneel. Bij ontvangstfouten worden ook de nieuwe sensoren onbekend.

## Oorspronkelijke bouwcontrole

- ESPHome 2026.11.0, Python 3.13.14, ESP-IDF 5.5.6, Windows.
- Configuratievalidatie: geslaagd.
- C++-generatie: geslaagd.
- Volledige ESP32-S3 firmwarecompilatie en linking: geslaagd, exitcode 0.
- Factory- en OTA-image gegenereerd met voorbeeldsecrets. Deze images worden bewust niet meegeleverd: vul eerst eigen wifi, API-sleutel en OTA-wachtwoord in en bouw opnieuw.
- RAM 99147 bytes / 341760; flash 749695 bytes / 1835008.
- Vier onafhankelijke RACOM CRC-referenties: geslaagd via `python check_protocol.py`. Deze controle voert niet de C++-ontvanger uit.
- Nog niet getest: echte meter, RS485-module, verlies/corruptie van telegrammen op hardware, DE/RE-omschakeling en Home Assistant-verbinding.

De eerste bouwpoging strandde op Windows' maximale padlengte. Dezelfde broncode is daarna succesvol vanuit een kortere werkmap gebouwd. Gebruik bij dergelijke fouten een korte projectmap of ESPHome Device Builder op Linux.

Niet-blokkerende waarschuwingen: ESPHome raadt OTA-encryptie aan boven een wachtwoord; `check_uart_settings()` is verouderd en wordt volgens deze versie in 2027.3.0 verwijderd. Dit pakket is gecontroleerd met de hierboven vastgelegde versie. Migreer die UART-controle naar de Python-validatie voordat je een versie gebruikt die de functie verwijdert.
