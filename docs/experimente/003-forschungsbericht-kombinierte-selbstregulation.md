# Experiment 003 – Kombinierte neuronale Selbstregulation

## Kurzfassung

C5+A1 ist die einzige Kombination, die die Langzeitmatrix überwiegend bis
16.384 Wiederholungen abschließt: 23 von 24 Läufen erreichen den Endpunkt; ein
`BB`-Lauf des Seeds `20261117` wird nach 1.242 Wiederholungen instabil. C5 allein
und C5+A3 bis C5+A5 scheitern dagegen in allen Läufen zwischen 724 und 882,
C5+A2 zwischen 802 und 969 Wiederholungen.

Die Stabilität von C5+A1 ist kein Nachweis dauerhaft fortbestehenden Lernens.
Zwischen 4.096 und 16.384 entstehen je abgeschlossenem Lauf im Mittel 26.784
neue Synapsen, aber nur 0,04 erfolgreiche Verstärkungen. Am Ende tragen im
Mittel nur 696 von 58.596 Synapsen Gewicht 1; 98,81 % haben Gewicht 0. Das Netz
bleibt aktiv, die synaptische Differenzierung ist jedoch binär und für die
trainierte Erfahrung praktisch ausgeschöpft.

Nach 512 Wiederholungen verarbeitet C5+A1 als einzige Variante bei allen sechs
Seeds zunächst `CD`, `EF` und `GH` je 128-mal. In der verlängerten
Interferenzserie bleiben vier Seeds stabil, zwei scheitern bereits während der
zusätzlichen `CD`-Erfahrung. Die spätere `AB`-Reaktion steigt stark an und
umfasst schließlich fast alle Neuronen. Das belegt Veränderbarkeit, aber keinen
verhaltensmäßigen Erhalt einer spezifischen `AB`-Erfahrung.

Keine Variante weist spezifische Wiedererkennung nach. Bei `AB`-trainierten
Netzen haben `AB` und `BA` in allen Varianten dieselben aggregierten
Aktivitätswerte; Sequenzhashes unterscheiden sich bereits in untrainierten
Kontrollen. `AC` löst regelmäßig mehr Aktivität aus als `AB`.

Damit beantwortet das Experiment die zentrale Frage negativ: Ein
Gewichtungsspielraum von 1 stabilisiert EVE weitgehend, erhält aber keine
nachgewiesene, langfristig differenzierte Lernfähigkeit. Größere Spielräume
erhalten mehr Gewichtsstufen, stabilisieren jedoch nicht einmal bis 1.024.

## A. Stabilität – gemessene Beobachtungen

Alle Angaben beziehen sich auf 24 Läufe je Variante.

| Variante | 512 | 1.024 | 4.096 | 16.384 | erreichte Wiederholungen min / max |
|---|---:|---:|---:|---:|---:|
| C5 | 24 | 0 | 0 | 0 | 724 / 882 |
| **C5+A1** | **24** | **24** | **23** | **23** | **1.242 / 16.384** |
| C5+A2 | 24 | 0 | 0 | 0 | 802 / 969 |
| C5+A3 | 24 | 0 | 0 | 0 | 724 / 882 |
| C5+A4 | 24 | 0 | 0 | 0 | 724 / 882 |
| C5+A5 | 24 | 0 | 0 | 0 | 724 / 882 |

Ursprung, A1 und A2 ohne C5 beenden keinen 512er-Lauf. Ihre Bereiche von
3–11, 35–77 und 7–20 Wiederholungen reproduzieren die Einordnung aus Experiment
002. C5+A3 bis C5+A5 erzeugen trotz anderer Gewichtsverteilungen dieselben
Ereignis-, Feuer- und Abbruchzahlen wie C5. C5+A2 verschiebt die Instabilität nur
um ungefähr 78–87 Wiederholungen. Erst die harte Grenze 1 verändert die
Langzeitstabilität grundlegend.

C5+A1 bleibt nicht vollständig zuverlässig: Seed `20261117` mit `BB` erreicht
in der 4.096er-Stufe nach insgesamt 1.242 Wiederholungen das Ereignislimit.
Dieser Lauf gilt als instabil; er wird nicht in die 16.384er-Stufe übernommen.

## B. Synaptisches Lernen und Sättigung

### Messwerte für C5+A1

| Prüfpunkt | Läufe | Synapsen | Gewicht 0 | am Maximum | erfolgreiche Verstärkungen gesamt | abgewiesen gesamt | neu gelernte gewachsene Synapsen |
|---:|---:|---:|---:|---:|---:|---:|---:|
| 512 | 24 | 1.947 | 71,00 % | 29,00 % | 564,1 | 19.237,0 | 242,5 |
| 1.024 | 24 | 5.970 | 88,25 % | 11,75 % | 690,3 | 36.533,8 | 332,5 |
| 4.096 | 24 | 30.992 | 97,54 % | 2,46 % | 695,8 | 36.728,2 | 336,0 |
| 16.384 | 23 | 58.596 | 98,81 % | 1,19 % | 696,2 | 36.665,7 | 336,6 |

Der sinkende prozentuale Sättigungswert bedeutet keine wiedergewonnene
Gewichtsdifferenzierung. Der Nenner wächst durch neue Nullgewichtssynapsen stark,
während die Zahl positiver Synapsen bei ungefähr 696 stehen bleibt. In der
Stufe von 4.096 auf 16.384 entstehen durchschnittlich 26.784 Synapsen, aber nur
0,04 Verstärkungen; abgewiesene Versuche treten in dieser Stufe im Mittel gar
nicht mehr auf. Die wiederholt trainierte Erfahrung erreicht die Lernschwelle
auf den neuen Verbindungen nicht mehr.

Bei 512 Wiederholungen steigt der mittlere Gewichtswert mit der Grenze von
0,290 bei A1 über 0,535, 0,949, 1,606 bis 2,533 bei A5. Mehr Gewichtsstufen
erhalten somit numerische Differenzierung, führen aber nicht zu längerer
Stabilität. An den jeweils letzten, unvollständigen 1.024er-Stufen liegen die
Sättigungsanteile bei 7,66 % für A2, 8,26 % für A3, 6,61 % für A4 und 4,95 %
für A5. Eine niedrigere Sättigungsquote ist daher ebenfalls kein
Stabilitätsnachweis.

### Interpretation

C5+A1 verhindert die Verstärkungsrückkopplung wirksam, erzeugt aber einen
anderen Grenzzustand: fast vollständiges Strukturwachstum mit überwiegend
wirkungslosen Verbindungen. Das Netzwerk ist aktiv und strukturell veränderbar,
doch zusätzliche Struktur ist nicht gleichbedeutend mit zusätzlicher
synaptischer Lernfähigkeit.

## C. Wachstum und unterschiedliche Erfahrungen

Die Zustands-, Neuronen-, Gewichts- und Strukturhashes der getrennt mit `AB`,
`BA`, `AA` und `BB` trainierten C5+A1-Netze unterscheiden sich für jeden Seed.
Das belegt unterschiedliche gespeicherte Zustände, aber für sich allein keine
wiederverwendbare Unterscheidungsfähigkeit.

Die abgeschlossenen C5+A1-Läufe wachsen im Mittel auf 58.596 Synapsen. Das ist
nahe an den 65.280 möglichen gerichteten Verbindungen ohne Selbstverbindung.
Der Ressourcenbedarf der Langzeitmatrix wird deshalb nicht nur durch
Wiederholungszahl, sondern durch die stark steigende Dichte bestimmt. Über alle
C5+A1-Läufe wurden rund 58,55 Milliarden Ereignisse verarbeitet.

## D. Wiedererkennung

Gemittelte Beobachtungen nach 512 Wiederholungen:

| Variante / Trainingsgruppe / Test | Ereignisse | Feuer | aktive Neuronen |
|---|---:|---:|---:|
| C5 / `AB`-trainiert / `AB` | 348,2 | 20,7 | 19,2 |
| C5 / `AB`-trainiert / `BA` | 348,2 | 20,7 | 19,2 |
| C5+A1 / `AB`-trainiert / `AB` | 282,3 | 15,0 | 13,7 |
| C5+A1 / `AB`-trainiert / `BA` | 282,3 | 15,0 | 13,7 |
| C5+A1 / `AB`-trainiert / `AC` | 361,8 | 20,0 | 17,5 |
| C5+A1 / `BA`-trainiert / `AB` | 456,3 | 28,3 | 26,3 |
| C5+A1 / `BA`-trainiert / `BA` | 456,5 | 28,3 | 26,3 |
| untrainiert / `AB` oder `BA` | 20,0 | 4,0 | 3,0 |

Training verändert das Verhalten deutlich. Es entsteht jedoch keine bevorzugte
aggregierte Reaktion auf die trainierte Reihenfolge. `AB` und `BA` sind nach
`AB`-Training in allen sechs zentralen Varianten aggregiert identisch; bei
C5+A1 nach `BA`-Training unterscheidet sich nur einer von sechs Seeds um ein
Ereignis. Ihre Sequenzhashes unterscheiden sich, doch das gilt ebenfalls für
alle untrainierten Kontrollen. Die stärkere Reaktion auf `AC` widerspricht einer
einfachen `AB`-Präferenz. Spezifische Wiedererkennung ist somit nicht belegt.

## E. Interferenz und Beständigkeit

| Variante | alle drei Muster je 128 | alle drei Muster bis je 512 |
|---|---:|---:|
| C5 | 0/6 | 0/6 |
| **C5+A1** | **6/6** | **4/6** |
| C5+A2 | 0/6 | 0/6 |
| C5+A3 | 0/6 | 0/6 |
| C5+A4 | 0/6 | 0/6 |
| C5+A5 | 0/6 | 0/6 |

C5+A1 verarbeitet bei allen Seeds zunächst `CD`, `EF` und `GH` je 128-mal.
Seed 99 wird in der verlängerten `CD`-Stufe nach 315 von 384 zusätzlichen
Wiederholungen instabil, Seed 314159 nach 175. Die übrigen vier Seeds schließen
auch die verlängerten `EF`- und `GH`-Stufen ab.

Die `AB`-Probe von C5+A1 steigt von durchschnittlich 282 Ereignissen, 15
Feuerungen und 13,7 aktiven Neuronen vor der Interferenz auf 34.682, 791 und
226,8 nach je 128 neuen Erfahrungen. Bei den vier vollständig verlängerten
Läufen steigt sie auf 438.898 Ereignisse, 2.402 Feuerungen und 255,5 aktive
Neuronen. Das ursprüngliche Verhalten bleibt daher nicht erhalten; es wird von
einer breiten, stark verstärkten Netzwerkreaktion überlagert. Beständigkeit im
Sinne spezifischer Wiederverwendung ist nicht nachgewiesen.

## F. Performance und Speicher

Im statischen Benchmark mit 256 Neuronen liegen die sechs Varianten zwischen
48,4 und 56,7 Millionen verarbeiteten Ereignissen pro Sekunde. Bei aktivem
Lernen liegen sie zwischen 18,5 und 21,5 Millionen. Die Streuung ist nicht
monoton zur Gewichtsgrenze; ein systematischer Zusatzaufwand der konstanten
Grenzprüfung ist damit nicht messbar.

Für C5 werden statisch bei 256, 1.024 und 10.000 Neuronen rund 54,2, 54,3 und
44,7 Millionen Ereignisse pro Sekunde erreicht. Im aktiven Modus sind es 19,0,
22,0 und 21,8 Millionen. Alle 216 Benchmarkläufe schließen ab.

Der initiale geschätzte Speicher beträgt bei allen Varianten identisch 35.200,
139.648 und 1.360.384 Byte. `Neuron`, `Synapse` und Ereignis belegen auf der
Messplattform 40, 24 und 8 Byte. Die Gewichtsgrenze benötigt keinen zusätzlichen
Zustand je Synapse. Der wesentliche Ressourcenanstieg im Langzeitversuch stammt
vom Wachstum und der Ereignismenge, nicht von der Grenzprüfung.

## G. Bewertung und Entscheidungsvorlage

### Datenbasierte Bewertung

| Kriterium | C5+A1 | C5+A2 | C5+A3 bis A5 |
|---|---|---|---|
| Stabilität | 23/24 bis 16.384 | 0/24 bis 1.024 | wie C5, 0/24 bis 1.024 |
| Aktivität | bleibt aktiv | aktiv bis Abbruch | aktiv bis Abbruch |
| Lernfähigkeit | nach 512 für neue Muster veränderbar; Langzeittraining sättigt aus | mehr Stufen, aber früher Abbruch | mehr Stufen, aber früher Abbruch |
| Differenzierung | nur 0/1 | vorhanden | zunehmend vorhanden |
| Wiedererkennung | nicht belegt | nicht belegt | nicht belegt |
| Beständigkeit | 4/6 verlängert stabil, ursprüngliche Reaktion stark überlagert | 0/6 schon nach je 128 | 0/6 schon nach je 128 |
| Ressourcen | sehr hohes Wachstum und kumulative Ereignismenge | früher Abbruch | früher Abbruch |

### Entscheidung

Keine untersuchte Grenze erfüllt gleichzeitig Stabilität, langfristige
synaptische Anpassungsfähigkeit, spezifische Wiedererkennung und Beständigkeit.

1. **C5+A1 nicht als endgültige Architektur übernehmen.** Die Variante ist der
   beste Stabilitätskontrollfall, aber ihre binäre Gewichtung und das praktisch
   auslaufende Lernen beantworten die Forschungsfrage nicht hinreichend.
2. **C5+A1 als Referenz für das nächste Experiment behalten.** Künftige
   Hypothesen sollten Wachstum/Dichte und langfristige Plastizität getrennt
   untersuchen, jeweils gegen C5 und C5+A1.
3. **Keine höhere Gewichtsgrenze allein weiterverfolgen.** A2 verbessert den
   Zeitpunkt der Instabilität geringfügig; A3 bis A5 verändern ihn nicht.
4. **Wiedererkennung weiterhin als offene Forschungsfrage behandeln.** Hash- und
   Aktivitätsunterschiede reichen nicht; erforderlich ist ein kontrolliertes,
   trainigsspezifisches Verhaltensmaß.

Die Daten rechtfertigen damit eine neue Hypothesenrunde, aber keine endgültige
Festlegung der Stammhirnarchitektur.

## Technische Umsetzung

Experiment 003 führt keine neue neuronale Regel ein. Die in Experiment 002
separat konfigurierbaren Mechanismen werden unverändert kombiniert:

- C5 sperrt ein Neuron nach dem Feuern für 16 eingehende Ereignisse;
- A1 bis A5 begrenzen ein Einzelgewicht auf 1, 2, 4, 8 oder 16;
- C5 ohne Gewichtsgrenze bleibt die direkte Referenz.

Der Kern zählt zusätzlich jeden Lernschwellenübertritt, bei dem die
Einzelgewichtsgrenze eine Verstärkung ablehnt. Dieser Zähler ist Teil der
Metriken, des Zustandsfingerabdrucks und der Persistenzversion 3. Zustände der
Versionen 1 und 2 bleiben lesbar. Die Zählung verändert weder Lernzähler,
Membranpotenzial noch Ereignisreihenfolge.

Eine technische Profilierung zeigte, dass der Runtime-Kern auch bei
deaktiviertem Diagnose-Trace für jedes Ereignis Text formatierte. Die
Formatierung wird nun nur bei aktivem Trace ausgeführt. Ebenso erhebt der Runner
in Langzeitläufen nur die dort benötigten Aktivitätsmengen; vollständige
Feuerfolgen und Übergangshäufigkeiten werden weiterhin für jede
Wiedererkennungs- und Beständigkeitsprobe aufgezeichnet. Beides verändert keine
neuronalen Zustände oder Versuchsparameter.

Die unabhängigen Seed-/Eingabefälle laufen parallel. Ihre CSV-Zeilen werden
anschließend in fester Varianten-, Seed- und Eingabereihenfolge geschrieben.
Zustände und Zähler bleiben deterministisch; Laufzeiten hängen von Rechner und
Parallelbelegung ab.

## Versuchsaufbau

### Langzeitmatrix

Die sechs zentralen Varianten C5, C5+A1, C5+A2, C5+A3, C5+A4 und C5+A5 sowie
die Referenzen Ursprung, A1 und A2 verwenden:

- 256 Neuronen und 1.024 initiale Synapsen;
- die Seeds `20261117`, `7`, `42`, `99`, `2026` und `314159`;
- getrennte Trainingsfolgen `AB`, `BA`, `AA` und `BB`;
- kumulative Prüfpunkte 512, 1.024, 4.096 und 16.384;
- ein Sicherheitslimit von 1.000.000 Ereignissen je Eingabe.

Eine nächste Stufe wird nur nach vollständig abgeschlossener vorheriger Stufe
ausgeführt. Ein Ereignislimit gilt als Instabilität, nicht als Abschluss.

### Lernen und Sättigung

Je Prüfpunkt werden erfolgreiche und abgewiesene Verstärkungen, Wachstum,
Nullgewichte, Gewichte am Grenzwert, vollständige Gewichtsverteilung,
Minimal-, Maximal- und Mittelgewicht, gelernte gewachsene Synapsen,
Aktivitätsumfang, Warteschlange, Speicher und Laufzeit festgehalten.

### Wiedererkennung

Für jeden Seed werden identische Anfangszustände 512-mal mit `AB`, gleich häufig
mit `BA` oder gar nicht trainiert. Die Tests `AB`, `BA`, `AC`, `AA` und `BB`
starten jeweils von einer eigenen Kopie. Aufgezeichnet werden Feuerreihenfolge,
aktive Neuronenmenge, Sequenzhash, Vorschau und häufigster Übergang.

### Interferenz

Nach 512-mal `AB` folgen `CD`, `EF` und `GH` zunächst jeweils 128-mal. Bei
vollständiger Verarbeitung folgen je weitere 384 Wiederholungen. Eine
unabhängige `AB`-Probe wird vor der Interferenz sowie nach den Stufen 128 und 512
aus einem kopierten Zustand erhoben.

### Performance

Der separate Benchmark vermisst alle sechs zentralen Varianten über sechs Seeds
bei 256, 1.024 und 10.000 Neuronen. Der statische Modus deaktiviert Lernen und
Wachstum und verarbeitet 250.000 Feuer- sowie 1.000.000 Synapsenereignisse. Der
aktive Modus aktiviert Lernen, lässt Wachstum zur Vergleichbarkeit aus und
trainiert 512-mal `AB`.

## Reproduzierbarkeit und Daten

- Auftrag: `003-entwicklungsauftrag-kombinierte-selbstregulation.md`
- Ergänzung: `003-ergaenzungsauftrag-001-gewichtsgrenzen.md`
- Parameter: `research/experiment-003-parameters.csv`
- Runner: `scripts/run_experiment_003.sh`
- Langzeitrohwerte: `research/results/experiment-003-training.csv`
- Wiedererkennung: `research/results/experiment-003-recognition.csv`
- Interferenz: `research/results/experiment-003-interference.csv` und
  `experiment-003-interference-experiences.csv`
- Performance: `research/results/experiment-003-performance.csv`
- Aggregationen: `research/results/experiment-003-summary.csv` und
  `experiment-003-recognition-summary.csv`

Der vollständige Lauf lautet:

```sh
scripts/run_experiment_003.sh research/results
```
