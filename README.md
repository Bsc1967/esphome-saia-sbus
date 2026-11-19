# ESPHome SAIA S-Bus

Lokale ontwikkeling voor SAIA ALD1D5FS00A3A00 en ESP32-S3 DevKitC-1.
Gebruik `energiemeter.yaml` in ESPHome Device Builder. De component wordt automatisch uit deze repository gedownload; geen losse componentbestanden nodig. Behoud bestaande wifi-secrets. De voorbeeldconfiguratie behoudt de apparaatnaam esphome-web-56955c.

TX GPIO17 naar DI, RX GPIO18 naar RO, GPIO21 naar gekoppelde DE en /RE (hoog = zenden). Gebruik 3,3V-compatibele logica en de juiste modulevoeding. Sluit D en /D volgens de modulehandleiding aan; A/B-benaming verschilt per fabrikant. Zorg voor passende busafsluiting en slechts een master. Laat werkzaamheden aan de 230V-meter spanningsloos door een bevoegd persoon uitvoeren.

Station 1, 38400 baud, 8N1. Eerste peiling na 30 seconden, daarna elke 5 seconden. Leest R20 t/m R26 in een telegram. R20 x 0,01 kWh, R24 V, R25 x 0,1 A, R26 x 10 W. Geen schrijfopdrachten, Modbus of EVBox.

S-Bus Data mode: B5 start, C5 escaping, CRC-16/XMODEM inclusief startbyte, big-endian 32-bit registers. CRC en antwoordlengte worden gecontroleerd. Timeout 500 ms; bij geen geldig antwoord worden metingen onbekend. Er is geen baudratescan.

De component is volledig gecompileerd met ESPHome 2026.11.0 / ESP-IDF 5.5.6, maar nog niet met fysieke hardware getest. Zie VALIDATIE.md voor grenzen en versie-waarschuwingen. De GitHub-voorbeeldconfiguratie gebruikt, net als het oorspronkelijke lege project, geen API-encryptie of OTA-wachtwoord. Voeg die naar behoefte toe via eigen secrets; publiceer nooit sleutels of wachtwoorden.

Controle: `python check_protocol.py` controleert vier gepubliceerde referentietelegrammen; dit is geen C++- of hardwaretest.

Bronnen:
- https://sbc-support.com/uploads/tx_srcproducts/26-523_ENG_DS_EnergyMeter-ALD1-with-S-Bus.pdf
- https://www.racom.eu/eng/support/prot/sbus/index.html

Klik na plakken op Validate en Install. Internet is nodig voor de eerste componentdownload. Gebruik voor reproduceerbare builds een vaste commit-SHA in plaats van `ref: main`. Vergelijk de vier metingen met het meterdisplay voordat je ze voor automatiseringen gebruikt.
