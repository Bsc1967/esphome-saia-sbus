# Validatie op 19 november 2026

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
