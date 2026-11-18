# EVE – Experiment 001: Technische Umsetzung

**Stand:** 17. November 2026  
**Status:** ausführbarer Referenzprototyp  
**Implementierung:** C++17 ohne externe Laufzeit- oder KI-Abhängigkeiten

## Ziel und Umfang

Dieser Stand setzt den Entwicklungsauftrag für Experiment 001 als beobachtbaren,
reproduzierbaren Referenzprototyp um. Er soll die spezifizierten Mechanismen
isoliert prüfbar machen. Er erhebt keinen Anspruch auf Sprachfähigkeit oder auf
eine bereits bestätigte Lernarchitektur.

Die Umsetzung umfasst:

- 256 Neuronen und 1.024 initiale gerichtete Synapsen,
- ereignisgesteuerte FIFO-Verarbeitung,
- eingangsseitige synaptische Gewichte und lokale Lernzähler,
- neuronale Aktivitätsphasen,
- aktivitätsabhängiges räumliches Synapsenwachstum,
- rohe UTF-8-Byte-Verarbeitung über acht technische Kanäle,
- vollständige Zustandspersistenz,
- Diagnose, Ereignisprotokoll und reproduzierbare Zufallsfolgen,
- ein isoliertes `AB`/`BA`-Sequenzexperiment sowie automatisierte Tests.

Nicht implementiert sind die ausdrücklich zurückgestellten Mechanismen wie
Abschwächung, Synapsenentfernung, Normalisierung, Gewichtsbudgets, STDP,
semantische Zuordnungen, spontane Aktivität und produktive Kommunikationssysteme.

## Umsetzungsplanung

1. Die spezifizierten Datenstrukturen und lokalen Regeln isoliert implementieren.
2. BIOS, Stammhirn und neuronales Netzwerk in getrennten Modulen halten.
3. Alle noch offenen, für die Ausführung zwingenden Parameter vorläufig und
   austauschbar festlegen.
4. Die geforderten Mechanismen mit automatisierten Tests absichern.
5. Reproduzierbarkeit, Persistenz und Diagnose über die Kommandozeile prüfen.
6. `AB` und `BA` mit identischen Startbedingungen vergleichen und das Ergebnis
   ohne zusätzliche zeitliche Lernregel dokumentieren.

## Modultrennung

### BIOS

`include/eve/bios` und `src/bios` enthalten:

- FIFO-Ereigniswarteschlange,
- deterministischen Zufallszahlengenerator,
- Konfiguration und Ausführungsgrenze,
- Diagnosewerte und detaillierte Ablaufprotokolle,
- binäre Persistenz und Wiederherstellung.

### Stammhirn

`include/eve/brainstem` und `src/brainstem` enthalten:

- Initialisierung der Grundvernetzung,
- synaptische Lern- und Phasenregeln,
- Wachstumswahrscheinlichkeit und räumliche Partnersuche,
- Umwandlung roher Bytes in geordnete sensorische Kanalereignisse.

### Neuronales Netzwerk

`include/eve/neural` und `src/neural` enthalten:

- kompakte Neuronen- und Synapsenstrukturen,
- numerische Indizes statt Objektzeiger,
- eingehende und ausgehende Indexketten,
- logische 3D-Koordinatenberechnung,
- Strukturprüfung, Gewichtsverteilung und Zustandsfingerabdruck.

Neuronen und Synapsen liegen jeweils in zusammenhängenden `std::vector`-Bereichen.
Die virtuelle Position wird nicht gespeichert.

## Vorläufige, austauschbare Entscheidungen

Die folgenden Punkte waren für einen ausführbaren Prototyp notwendig. Sie sind
Versuchsparameter oder klar abgegrenzte Implementierungsvarianten und keine
abschließend bestätigten Lernregeln.

| Offener Punkt | Variante für Experiment 001 |
|---|---|
| Membranpotenzial | vorzeichenbehaftete 64-Bit-Ganzzahl, Startwert 0 |
| Synapsengewicht | nichtnegative 32-Bit-Ganzzahl |
| Initialgewicht | 0 für initiale und später gewachsene Synapsen |
| Erregungsschwelle | 3, pro Neuron gespeichert |
| Reset nach Feuern | standardmäßig auf 0; alternativ Schwelle abziehen |
| Zeitmodell | ausschließlich lokale Ereigniszeit, kein Zerfall und keine Zeitstempel |
| Ereignisreihenfolge | deterministische FIFO-Verarbeitung |
| Lernschwelle | 3 Impulse derselben Synapse |
| Phasenlimit | 8 Eingangsimpulse am empfangenden Neuron |
| Gleichzeitige Schwellen | erst verstärken, dann gewichteten Impuls anwenden, dann Phase beenden |
| Räumliche Distanz | Chebyshev-Distanz im 8 × 8 × 4-Raum |
| Partnersuche | Radius 1, danach schalenweise erweitern; zufällige Wahl unter Kandidaten |
| Wachstumsauslösung | ein Wahrscheinlichkeitsentscheid nach jedem Feuerereignis |
| Wirkung neuer Synapse | ab dem nächsten Feuerereignis des Senders |
| Endlosschutz | höchstens 1.000.000 Ereignisse pro Verarbeitungslauf |
| Persistenz | versioniertes Little-Endian-Binärformat |

Eine synaptische Verstärkung wird auf den Impuls angewendet, der die
Lernschwelle erreicht. Der neue Gewichtswert beeinflusst diesen Impuls also
bereits. Beim Phasenende werden danach alle synaptischen Aktivitätszähler des
Empfängers zurückgesetzt.

## Sensorische Anbindung

Das BIOS übernimmt die Bytes ohne Zeichen- oder Bedeutungsinterpretation. Das
Stammhirn ordnet Bit 0 bis Bit 7 eines Bytes den technischen Kanälen 0 bis 7 zu.
Ein gesetztes Bit löst ein Feuerereignis des gleich nummerierten
Eingangsneurons aus.

Diese Zuordnung ist eine reproduzierbare technische Einspeisung, keine
semantische Festlegung. Kanal 0 bedeutet beispielsweise nicht „A“ oder einen
anderen Begriff. Die Bits werden aufsteigend verarbeitet. Nach jedem Byte wird
die ausgelöste Ereigniskaskade vollständig abgearbeitet; dadurch bleiben
Byte-Reihenfolge und Byte-Grenzen eindeutig. Eine globale Byte-Nummer wird auch
über mehrere Eingabeaufrufe hinweg fortgeführt.

## Reproduzierbarkeit

Der Prototyp verwendet einen eigenen `xorshift64*`-Generator. Dadurch hängen die
Zufallsfolgen nicht von einer konkreten Implementierung der C++-Standardbibliothek
ab. Initiale Vernetzung, Wachstumsentscheidungen, Partnerwahl und RNG-Zustand
sind bei gleichem Startwert reproduzierbar.

Zustandsfingerabdrücke dienen dem schnellen Vergleich von Läufen. Sie ersetzen
keinen kryptografischen Integritätsnachweis.

## Persistenz

Gespeichert werden:

- vollständige Konfiguration,
- Zustand des Zufallszahlengenerators,
- sämtliche Diagnosezähler,
- alle Neuronen einschließlich Membranpotenzial und lokaler Zähler,
- alle Synapsen einschließlich Gewicht und Aktivitätszähler,
- Verknüpfungsindizes,
- noch nicht verarbeitete Ereignisse.

Beim Laden werden Formatversion, Geometrie, Indizes, Selbstverbindungen,
Dubletten und Adjazenzketten geprüft.

## Bauen und ausführen

Voraussetzung ist ein C++17-fähiger Compiler. Der lokal geprüfte Weg verwendet
GNU Make oder eine kompatible Make-Implementierung.

```sh
make all
make test
```

Alternativ steht eine plattformübergreifende CMake-Konfiguration bereit:

```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

Ein normaler Lauf:

```sh
_build/eve_experiment --seed 1 --input AB --repeat 8 --no-growth
```

Ein detailliertes Ereignisprotokoll und ein persistenter Zustand:

```sh
_build/eve_experiment --seed 1 --input AB --trace eve.log --save eve.state
_build/eve_experiment --load eve.state --input BA
```

Das isolierte Sequenzexperiment:

```sh
_build/eve_sequence_experiment --seed 1
```

## Sequenzexperiment `AB` und `BA`

Für den isolierten Vergleich ist Synapsenwachstum deaktiviert. Synaptisches
Lernen bleibt aktiv. Alle vier Läufe beginnen separat mit Startwert 1 und
identischer Grundkonfiguration. „Wiederholt“ bedeutet acht Wiederholungen.

| Fall | Ereignisse | Feuerungen | Phasen | Verstärkungen | Neuronaler Zustand |
|---|---:|---:|---:|---:|---|
| `AB` | 20 | 4 | 0 | 0 | `0883c40cfda685ef` |
| `BA` | 20 | 4 | 0 | 0 | `0883c40cfda685ef` |
| `AB` × 8 | 660 | 132 | 34 | 123 | `d1cf4f35fd51d2da` |
| `BA` × 8 | 660 | 132 | 34 | 123 | `601af403fd1dc7cc` |

Die einmaligen Folgen erzeugen unterschiedliche Ereignisverläufe, enden aber
im selben neuronalen Zustand. Nach acht Wiederholungen sind auch die neuronalen
Zustände verschieden. Das Modell kann die beiden festen Folgen in diesem Lauf
somit unterscheidbar verändern.

Das ist noch kein Nachweis eines verallgemeinerten Lernens von Reihenfolge. Die
Unterscheidung kann aus FIFO-Reihenfolge, fortbestehendem Membranpotenzial und
der Lage lokaler Phasengrenzen entstehen. Ohne STDP, Zeitstempel oder eine
gesonderte Erfolgskopplung gibt es keine Regel, die abstrakt „AB“ gegenüber
„BA“ verstärkt. Weitere Seeds, Kontrollen und Abrufversuche müssen zeigen, ob
die Unterschiede stabil, reproduzierbar und verhaltenswirksam sind.

## Tests

Das Testprogramm deckt die 13 Punkte des Entwicklungsauftrags ab:

1. Netzwerkinitialisierung,
2. Ausschluss doppelter Synapsen und Selbstverbindungen,
3. 3D-Indexberechnung,
4. binäre Weiterleitung an alle Ausgänge,
5. gewichtete Eingangsverarbeitung,
6. Verstärkung nach drei Impulsen,
7. Phasenrücksetzung nach acht Impulsen,
8. Reihenfolge gleichzeitig erreichter Schwellen,
9. Lernen über Gewicht 0,
10. Wachstumswahrscheinlichkeit und Partnerwahl,
11. reproduzierbare Ausführung,
12. Speichern, Wiederherstellen und identisches Fortsetzen,
13. rohe UTF-8-Byte-Verarbeitung.

## Verbleibende Forschungsfragen

- Welcher Membran-Reset und welches Abklingmodell sind experimentell geeignet?
- Sind nichtnegative Ganzzahlgewichte ausreichend oder werden hemmende Eingänge
  benötigt?
- Welche Ereignisgrenze erkennt echte Instabilität, ohne zulässige Kaskaden zu
  früh abzubrechen?
- Wie soll synaptische Abschwächung ohne semantische Wichtigkeitsvorgabe erfolgen?
- Welche Normalisierung verhindert unkontrollierte positive Rückkopplung?
- Welche Kontrollen belegen, dass Sequenzunterschiede erlernt, abrufbar und
  generalisierbar sind?
- Soll das Persistenzformat künftig Prüfsummen und atomare Dateiersetzung erhalten?
- Wie werden realer Stromverbrauch und interner Ereignisaufwand getrennt gemessen?
