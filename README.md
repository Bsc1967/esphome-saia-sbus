# ESPHome SAIA S-Bus

Lokale ontwikkeling voor SAIA ALD1D5FS00A3A00 en ESP32-S3 DevKitC-1.
Gebruik `energiemeter.yaml` in ESPHome Device Builder. De component wordt automatisch uit deze repository gedownload; geen losse componentbestanden nodig. Behoud bestaande wifi-secrets. De voorbeeldconfiguratie behoudt de apparaatnaam esphome-web-56955c.

TX GPIO17 naar DI, RX GPIO18 naar RO, GPIO21 naar gekoppelde DE en /RE (hoog = zenden). Gebruik 3,3V-compatibele logica en de juiste modulevoeding. Sluit D en /D volgens de modulehandleiding aan; A/B-benaming verschilt per fabrikant. Zorg voor passende busafsluiting en slechts een master. Laat werkzaamheden aan de 230V-meter spanningsloos door een bevoegd persoon uitvoeren.

Station 1, 38400 baud, 8N1. Eerste peiling na 30 seconden, daarna elke 5 seconden. Leest negen registers R20 t/m R28 in een telegram (maximum meter: tien). R20 x 0,01 kWh, R24 V, R25 x 0,1 A, R26 x 10 W. Geen schrijfopdrachten, Modbus of EVBox.

Extra optionele sensoren: `partial_energy` = R21 x 0,01 kWh; `reactive_power` = R27 x 10 var; `power_factor` = R28 x 0,01 (dimensieloze cos phi, geen percentage). De deelteller kan via de meter worden gereset; de component biedt geen resetknop. Bij nulbelasting is cos phi niet zinvol te interpreteren. Tekenrepresentatie voor capacitief reactief vermogen is niet expliciet beschreven in de datasheet; de huidige implementatie leest R27 als unsigned, zoals de overige meetregisters. Controleer dit onder een bekende belasting voordat je hierop automatiseert.

Voor deze uitbreiding: vervang je YAML door de bijgewerkte `energiemeter.yaml` en installeer opnieuw. `refresh: 0s` controleert bij elke build op nieuwe componentcode. Bestaande YAML met alleen vier sensoren blijft geldig.

S-Bus Data mode: B5 start, C5 escaping, CRC-16/XMODEM inclusief startbyte, big-endian 32-bit registers. CRC en antwoordlengte worden gecontroleerd. Timeout 500 ms; bij geen geldig antwoord worden metingen onbekend. Er is geen baudratescan.

De component is volledig gecompileerd met ESPHome 2026.11.0 / ESP-IDF 5.5.6, maar nog niet met fysieke hardware getest. Zie VALIDATIE.md voor grenzen en versie-waarschuwingen. De GitHub-voorbeeldconfiguratie gebruikt, net als het oorspronkelijke lege project, geen API-encryptie of OTA-wachtwoord. Voeg die naar behoefte toe via eigen secrets; publiceer nooit sleutels of wachtwoorden.

Controle: `python check_protocol.py` controleert vier gepubliceerde referentietelegrammen; dit is geen C++- of hardwaretest.

Bronnen:
- https://sbc-support.com/uploads/tx_srcproducts/26-523_ENG_DS_EnergyMeter-ALD1-with-S-Bus.pdf
- https://www.racom.eu/eng/support/prot/sbus/index.html

Klik na plakken op Validate en Install. Internet is nodig voor de eerste componentdownload. Gebruik voor reproduceerbare builds een vaste commit-SHA in plaats van `ref: main`. Vergelijk de vier metingen met het meterdisplay voordat je ze voor automatiseringen gebruikt.
