# Experiment 002 – Neuronale Selbstregulation und synaptische Stabilität

## Kurzfassung

Von 18 einzeln untersuchten Varianten stabilisierte nur die ereignisbasierte
Refraktärzeit C5 mit 16 gesperrten Eingangsereignissen alle Hauptläufe bis 512
Wiederholungen. Sie verhinderte weder Lernen noch Wachstum und legte das Netzwerk
nicht still. Gewichtsobergrenzen und Eingangsbudgets reichten isoliert nicht aus.

C5 ist dennoch keine dauerhafte Lösung: Nach zusätzlicher Erfahrung mit `CD`,
`EF` und `GH` wurden alle sechs Seeds erneut instabil. Ebenso gibt es keinen
Nachweis für spezifische `AB`-Wiedererkennung. C5 ist daher ein Kandidat für
weitere Kombinationstests, keine Architekturentscheidung.

## A. Technische Umsetzung

### Konfiguration und Zustände

`ExperimentConfig` enthält nun:

- `maximum_synapse_weight` als bereits vorhandene, nun systematisch untersuchte
  Einzelgewichtsgrenze;
- `input_weight_budget` mit „unbegrenzt“ als deaktivierter Referenz;
- `refractory_input_events` mit null als deaktivierter Referenz.

Jedes Neuron speichert `refractory_events_remaining`. Die Konkurrenzregel des
Budgets ist in `Brainstem::select_competing_synapse` gekapselt. Sie verwendet den
persistierten deterministischen Zufallszahlengenerator, schließt die lernende
Synapse aus und berücksichtigt nur andere positive Eingangsgewichte.

Neue Metriken zählen Abschwächungen, wegen Refraktärzeit unterdrückte
Feuerversuche und die maximale Ereigniswarteschlange. Konfiguration, Metriken und
Neuronenzustand sind in Persistenzversion 2 enthalten; Version 1 bleibt lesbar.

### Refraktärsemantik

Nach einem Feuerereignis wird der Zähler auf N gesetzt. Die nächsten N
eingehenden Impulse werden vollständig verarbeitet: Synapsen- und Phasenzähler,
Lernen sowie Membranaddition laufen weiter. Nur das erneute Feuern wird
unterdrückt. Das Membranpotenzial akkumuliert während der Sperre und wird beim
nächsten erlaubten Feuerereignis nach der bestehenden Reset-Regel behandelt.

Das ist eine ereignisbasierte Durchsatzbegrenzung und keine biologische,
zeitabhängige Refraktärphase.

### Speicherbedarf

Auf der gemessenen 64-Bit-Plattform sind `Neuron`, `Synapse` und Ereignis 40, 24
und 8 Byte groß. Das neue 32-Bit-Refraktärfeld nutzt vorhandenes Alignment; die
Neuronengröße steigt deshalb hier nicht. Der initiale Runtime-Schätzwert steigt
gegenüber Forschungsauftrag 002 von 35.152 auf 35.192 Byte, also um 40 Byte für
zusätzliche Konfigurations- und Metrikzustände. Das Budget benötigt keine
persistenten Daten je Synapse, erzeugt bei einer Konkurrenzentscheidung aber eine
temporäre Kandidatenliste.

## Versuchsaufbau

Jede Variante wurde mit 256 Neuronen, 1.024 initialen Synapsen, den Seeds
`20261117`, `7`, `42`, `99`, `2026` und `314159` sowie den Trainingsfolgen `AB`,
`BA`, `AA` und `BB` ausgeführt. Das ergibt 24 Hauptläufe je Variante. Ziel waren
512 Wiederholungen; ein Lauf mit erreichtem Ereignislimit gilt als instabil und
nicht abgeschlossen.

Wiedererkennung wurde aus identisch initialisierten Zuständen mit `AB`-Training,
gleichvolumigem `BA`-Training und untrainierter Kontrolle geprüft. Jeder Test
startete von einer eigenen Kopie des gespeicherten Zustands. Die Testfolgen waren
`AB`, `BA`, `AC`, `AA` und `BB`.

## B. Stabilität – gemessene Beobachtungen

| Variante | abgeschlossen | Wiederholungen min / Mittel / max | mittlere max. Queue |
|---|---:|---:|---:|
| Referenz A0/B0/C0 | 0/24 | 3 / 7,71 / 11 | 3.923.118 |
| A1, Gewicht 1 | 0/24 | 35 / 52,92 / 77 | 291.034 |
| A2, Gewicht 2 | 0/24 | 7 / 14,42 / 20 | 1.212.416 |
| A3–A5 | je 0/24 | 3 / 7,71 / 11 | 3.923.118 |
| B1, Budget 4 | 0/24 | 3 / 8,21 / 12 | 156.469 |
| B2, Budget 8 | 0/24 | 3 / 7,75 / 11 | 971.665 |
| B3–B5 | je 0/24 | 3 / 7,71 / 11 | 2.259.527–3.867.113 |
| C1, Refraktär 1 | 0/24 | 5 / 9,92 / 13 | 1.967.595 |
| C2, Refraktär 2 | 0/24 | 10 / 16,21 / 20 | 1.133.845 |
| C3, Refraktär 4 | 0/24 | 32 / 43,62 / 52 | 685.722 |
| C4, Refraktär 8 | 0/24 | 147 / 181,75 / 207 | 404.121 |
| **C5, Refraktär 16** | **24/24** | **512 / 512 / 512** | **136** |

Die drei Nullvarianten erzeugten identische Ereignis-, Feuer- und Lernzahlen;
die deaktivierten Mechanismen verändern das Referenzverhalten nicht.

C5 verarbeitete über alle 24 Hauptläufe 2.845.295 Ereignisse, erzeugte 204.969
Feuerereignisse und unterdrückte 2.353.839 potenzielle Feuerereignisse. Pro Lauf
waren im Mittel 252,96 von 256 Neuronen aktiv. Damit handelt es sich nicht um
Inaktivität.

### Interferenz

Alle C5-Seeds beendeten zunächst 128 `CD`-Wiederholungen. Seed 7 wurde nach 121
`EF`-Wiederholungen und Seed 42 nach 53 `EF`-Wiederholungen instabil. Die übrigen
Seeds beendeten `EF`, scheiterten aber während `GH` nach 7 bis 25
Wiederholungen. Kein Seed erreichte deshalb den abschließenden `AB`-Test.

## C. Lernen und Wiedererkennung

### Gemessene Beobachtungen

C5 führte in den Hauptläufen 475.772 Verstärkungen aus, im Mittel 19.824 pro
Lauf. Die Netze wuchsen im Mittel um 913 Synapsen. Es wurden Gewichte bis 319
beobachtet. Die Refraktärzeit verhindert Lernen somit nicht, begrenzt aber auch
keine Gewichte.

Für C5 ergaben die Wiedererkennungstests folgende Mittelwerte über sechs Seeds:

| Gruppe / Test | Ereignisse | Feuer | aktive Neuronen |
|---|---:|---:|---:|
| `AB`-trainiert / `AB` | 348,17 | 20,67 | 19,17 |
| `AB`-trainiert / `BA` | 348,17 | 20,67 | 19,17 |
| `AB`-trainiert / `AC` | 430,50 | 26,67 | 24,17 |
| `AB`-trainiert / `AA` | 355,33 | 21,33 | 19,00 |
| `AB`-trainiert / `BB` | 352,00 | 20,83 | 18,67 |
| gleichvolumig `BA` / `AB` | 372,17 | 23,17 | 21,83 |
| untrainiert / `AB` | 20,00 | 4,00 | 3,00 |

Training verändert Reaktion, Gewichte und Struktur deutlich. `AB` und `BA`
haben nach `AB`-Training jedoch identische aggregierte Aktivität. Ihre
Aktivitätsreihenfolge und Hashes unterscheiden sich, doch unterschiedliche
Fingerabdrücke allein sind ausdrücklich kein Wiedererkennungsnachweis. `AC`
erzeugt sogar die stärkere Reaktion. Spezifische `AB`-Wiedererkennung ist deshalb
nicht belegt.

## D. Ressourcenverbrauch

Der aktuelle statische Kern erreichte in drei Läufen mit je einer Million
synaptischen Ereignissen im Mittel rund 3,00 Millionen Ereignisse pro Sekunde bei
nahezu vollständiger Auslastung eines CPU-Threads. Die aktiven Lernläufe lagen je
nach Variante zwischen 2,38 und 2,93 Millionen Ereignissen pro Sekunde; C5
erreichte 2,83 Millionen. Diese Raten sind nur bedingt direkt vergleichbar, weil
die Mechanismen unterschiedliche Ereignismischungen und Abbruchzeitpunkte
erzeugen.

Die Gewichtsgrenze ist konstant teuer. Die Refraktärprüfung benötigt pro Impuls
einen Zählertest und gegebenenfalls ein Dekrement. Das Budget summiert bei jeder
Lernentscheidung die Eingangsgewichte und baut bei ausgeschöpftem Budget eine
Kandidatenliste auf; sein Aufwand wächst mit dem Eingangsgrad. Für große Netze
ist diese Auswahlregel vor einer Übernahme zu optimieren.

## E. Interpretation und Entscheidungsvorlage

1. **Zuverlässigste Stabilisierung:** C5 stabilisiert als einzige Variante alle
   Hauptläufe, aber nicht die anschließende Interferenzphase.
2. **Aktivitätsvielfalt:** C5 hält im Mittel 253 Neuronen aktiv und erhält damit
   die größte nachgewiesen stabile Vielfalt.
3. **Weiteres Lernen:** C5 erlaubt zahlreiche Verstärkungen und Wachstum. Auch B
   verteilt Gewichte um, stabilisiert aber nicht.
4. **Wiedererkennung:** Es gibt keine belastbaren Hinweise auf spezifische
   `AB`-Wiedererkennung.
5. **Nebenwirkungen:** C5 akkumuliert Membranpotenzial und unbegrenzte Gewichte;
   bei fortgesetzten neuen Erfahrungen verschiebt es Instabilität nur. A1
   sättigt früh. B benachteiligt positive Konkurrenzsynapsen und verursacht
   eingangsgradabhängigen Rechenaufwand.
6. **Nächste Kombinationen:** Vorrangig sollten C5+A1 und C5+A2 kontrolliert
   gegen C5 allein getestet werden. Eine Budgetkombination ist erst nach einer
   günstigeren Konkurrenzimplementierung sinnvoll.
7. **Weitere Hypothesen:** Membranleck, langsame synaptische Abschwächung,
   neuronale Aktivitätshomöostase und explizite inhibitorische Einflüsse sind
   naheliegende getrennte Folgehypothesen.

Ein dynamisches Budget `Grundbudget + Faktor × ausgehende Synapsen` ist derzeit
nicht zu empfehlen. Eingangsbudget und Ausgangsgrad messen verschiedene Rollen;
zudem könnte Wachstum das eigene Budget erhöhen und eine positive Rückkopplung
erzeugen. Da kein statisches Budget Stabilität erreichte, fehlt eine belastbare
Basis für die komplexere Regel.

## Schlussfolgerung

Die ereignisbasierte Refraktärzeit ist der erste untersuchte Mechanismus, der
EVE über 512 wiederholte Erfahrungen stabil und zugleich aktiv hält. Sie löst
die langfristige Selbstregulation aber nicht. Die gemessenen Ergebnisse
rechtfertigen weitere, klar gekennzeichnete Kombinationstests; sie rechtfertigen
keine Festlegung als endgültige EVE-Architektur.

## Reproduzierbarkeit und Daten

- Runner: `scripts/run_experiment_002.sh`
- Parameter: `research/experiment-002-parameters.csv`
- Hauptläufe: `research/results/experiment-002-training.csv`
- Wiedererkennung: `research/results/experiment-002-recognition.csv`
- Interferenz: `research/results/experiment-002-interference.csv`,
  `experiment-002-interference-experiences.csv` und
  `experiment-002-interference-summary.csv`
- Aggregation: `research/results/experiment-002-summary.csv`
- Statische Performance: `research/results/experiment-002-static-performance.csv`

Der vollständige Lauf lautet:

```bash
scripts/run_experiment_002.sh research/results 512
```
