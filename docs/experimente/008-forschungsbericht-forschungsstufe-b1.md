# Forschungsstufe B.1 – Eigenständige neuronale Erholung

## Kurzfassung

Forschungsstufe B.1 führt eine deterministische simulierte Zeit und
ereignisgesteuerte neuronale Erholung ein. Zukünftige Ereignisse werden ohne
globalen Tick nach Zeitpunkt und Einfügereihenfolge verarbeitet. Ein
Antwortende arbeitet zukünftige Erholungen nicht vorzeitig ab.

Die Ausgangshypothese wird bestätigt: Die bisherige eingangsabhängige
Refraktärregel R0 ist die Hauptursache dafür, dass motorische Aktivität nach der
ersten Handlung verschwindet. In der 64-Seed-Zweiphasenmatrix erzeugte die
erste Phase wie Forschungsstufe B 38 Läufe mit Datenaktivität und 14
nachgewiesene M3-Wege. Unter R0 erzeugte die zweite Phase unabhängig von
äußerem Zeitabstand in keinem der 64 Läufe Datenaktivität.

R1, die eigenständige Erregbarkeitserholung ohne Membranänderung, stellte nach
Fälligkeit der Erholung in allen 38 zuvor aktiven Fällen die Datenaktivität
wieder her, rekonstruierte aber nur 11 M3-Wege. Das während der Refraktärphase
akkumulierte Membranpotenzial blieb damit eine relevante Störgröße.

R2 mit 50 oder 100 Prozent einmaliger Membranentspannung stellte alle 14
nachgewiesenen M3-Wege in der zweiten Phase wieder her. R2F mit vollständiger
Rückkehr des kurzfristigen Membranpotenzials auf 0 reproduzierte in der
achtphasigen `AB`-Matrix exakt die 112 Wege der wiederhergestellten K0-
Referenz, während R0 nur die 14 Wege der jeweils ersten Phase und R1 87 Wege
erreichte.

In den 128-Phasen-Langläufen über sechs etablierte Seeds erzeugten K0, R1 und
R2F jeweils 256 nachgewiesene M3-Wege; fortgeführtes R0 nur vier. R1 und R2F
ermöglichen damit nachweislich wiederholte Handlungen ohne Wiederherstellung.
R2F ist über die breitere 64-Seed-Stichprobe stabiler und wird als vorläufige
Grundlage empfohlen.

Erholung veränderte in 192 Integritätsläufen keine Synapse, keinen
Aktivitätszähler und keinen lokalen G3-Ring. Alle Persistenzkontrollen mit
ausstehenden Ereignissen setzten dieselbe Folge fort. Eigenrückmeldung F1/F2
blieb funktionsfähig, änderte die Zahl kausaler Wege in der untersuchten
Sechs-Seed-Matrix nicht und erzeugte keinen Abbruch.

## A. Technische Umsetzung

### A.1 Zeitplanung im BIOS

Die Laufzeit unterscheidet unmittelbare Feuer- und Impulsereignisse,
zukünftige Erholungen und wirkungsfreie technische Versuchsgrenzen. Jedes
Ereignis besitzt:

- Art und Ziel-ID,
- einen 64-Bit-Simulationszeitpunkt,
- eine monoton steigende Reihenfolgenummer,
- bei Erholung eine Generation.

Die Warteschlange ist nach Zeitpunkt und Reihenfolge sortiert. Damit bleibt die
bisherige FIFO-Semantik unmittelbarer Ereignisse erhalten. `run()` verarbeitet
nur fällige Ereignisse. Zeit schreitet ausschließlich durch explizite
`advance_time_to()`- oder `advance_time_by()`-Aufrufe fort.

### A.2 R0, R1 und R2

| Variante | Ende der Refraktärität | Membran beim Erholen |
|---|---|---|
| R0 | nach 16 weiteren Eingangsimpulsen | unverändert |
| R1 | geplantes Erholungsereignis | unverändert |
| R2H | geplantes Erholungsereignis | 50 % verbleiben |
| R2F | geplantes Erholungsereignis | Rückkehr auf 0 |

R1/R2 reduzieren den Refraktärzähler während eingehender Impulse nicht. Das
Erholungsereignis setzt ihn auf 0. Es erzeugt kein Feuern. R2 skaliert danach
einmalig nur das Membranpotenzial; dies ist weder Synapsenlernen noch ein
kontinuierliches Membranleck.

Generationen verhindern, dass veraltete oder doppelte Erholungsereignisse
wirken. Normale Forschungsdurchläufe erzeugten keine veralteten Ereignisse;
deren wirkungslose Behandlung ist automatisiert getestet.

### A.3 Persistenz

Format 5 speichert Konfiguration, aktuelle Zeit, nächste Reihenfolgenummer,
ausstehende Ereignisse, Generationen und Erholungszähler. Formate 1 bis 4
bleiben als R0-Zustände lesbar. Speichern während einer laufenden Erholung und
anschließendes Fortsetzen erzeugt denselben Zustandsfingerabdruck.

## B. Versuchsgrundlage

Die neuronale Konfiguration entspricht Forschungsstufe B:

| Parameter | Wert |
|---|---:|
| Neuronen | 256 |
| reguläre Startsynapsen | 1.024 |
| angeborene Pfade | V2, Gewicht 3 |
| Motorschaltung | M3, Gewichte 1 und 3 |
| Feuergrenze | 3 |
| R0-Refraktärereignisse | 16 |
| lokales Gedächtnis | G3, 8 Einträge |
| Lernen und Wachstum | deaktiviert |
| Exploration | E4-neu, 16 Aktivierungen |
| Seeds | alle 64 aus B |

Motor-, Topologie-, Schaltungs- und Explorationssalze sind mit B identisch.
Die erste Phase reproduziert deshalb exakt 38 Datenläufe und 14 kausale
M3-Wege.

Die Rohdaten umfassen 25.885 CSV-Zeilen:

- 4.160 Zeit-/Abstandsfälle,
- 14.128 einzelne Antwortphasen,
- 6.909 relevante Neuronendiagnoseereignisse,
- 192 Integritätsfälle,
- 432 Rückmeldungsphasen,
- 36 Performancefälle.

## C. Phase I – Ursache der Wiederholungsblockade

R0 erhielt zwischen zwei Antworten dieselben äußeren Zeitabstände wie R1/R2.
Da R0 nicht zeitabhängig ist, änderte sich sein Ergebnis nicht:

| R0, 64 Seeds | erste Phase | zweite Phase |
|---|---:|---:|
| Läufe mit Datenaktivität | 38 | 0 |
| nachgewiesene M3-Wege | 14 | 0 |
| refraktär unterdrückte Entscheidungen | 90 | 357 |
| refraktäre Neuronen vor Phase, Summe | 0 | 810 |

Im Diagnoseausschnitt der etablierten Seeds und Eingabemuster entfielen in
zweiten R0-Phasen 123 unterdrückte Entscheidungen auf angeborene Relais, fünf
auf den Akkumulator und vier auf den Abschlusskontakt. Die Blockade beginnt
damit überwiegend vor dem Motorbereich. Akkumulator und Abschluss sind weitere,
aber nicht die einzigen Engstellen.

Zeitfortschritt allein löst R0 nicht. Die fehlenden Eingänge, die zum Abbau des
Zählers erforderlich wären, entstehen gerade deshalb nicht, weil vorgelagerte
Relais refraktär bleiben. Das Modell bildet eine selbststabilisierende
Inaktivität.

## D. Phasen II bis IV – Dauer und Membranregel

Die zweite Antwort wurde ohne Wartezeit, nach halber, gleicher, doppelter und
vierfacher Erholungsdauer gestartet. R1/R2 wurden mit T1 = 1, T2 = 4,
T3 = 16 und T4 = 64 untersucht.

Solange der Abstand kürzer als die konfigurierte Dauer war, entsprach jede
Variante R0: 0 Datenläufe und 0 M3-Wege. Ab exakt fälliger Erholung waren die
Ergebnisse für alle vier Dauern identisch:

| Variante | Daten in Phase 2 | M3-Wege in Phase 2 | Unterdrückungen in Phase 2 |
|---|---:|---:|---:|
| R0 | 0/64 | 0/64 | 357 |
| R1 | 38/64 | 11/64 | 141 |
| R2H | 38/64 | 14/64 | 118 |
| R2F | 38/64 | 14/64 | 99 |

Die absolute Dauer besitzt in diesem diskreten Aufbau keinen eigenen Effekt;
entscheidend ist, ob der nächste Reiz vor oder nach dem Erholungsereignis
eintrifft. T3 = 16 wird für Folgeversuche aus Kontinuität zur bisherigen
Refraktärzahl verwendet, nicht als biologisch begründete Konstante.

R1 beantwortet die zentrale technische Frage positiv: Ein Neuron kann ohne
weiteren Eingang wieder erregbar werden. Dass R1 drei der 14 Wege verliert,
zeigt jedoch, dass Erregbarkeit allein den kurzfristigen Zustand nicht
vollständig normalisiert. R2H genügte für zwei Phasen; R2F war in längeren und
breiteren Vergleichen am stabilsten.

## E. Acht fortgeführte Handlungen

Jede Kombination aus 64 Seeds, fünf Eingabemustern und vier Kontrollen wurde
acht Phasen lang untersucht. Zwischen den Phasen lagen 16 Simulationseinheiten.

| Kontrolle | Zustand | Versuche | mit Daten | M3-Wege | ohne Daten abgeschlossene Antworten |
|---|---|---:|---:|---:|---:|
| K0/R0 | wiederhergestellt | 2.560 | 1.520 | 568 | 16 |
| K1/R0 | fortgeführt | 2.560 | 190 | 71 | 3 |
| K2/R1 | fortgeführt | 2.560 | 1.517 | 490 | 9 |
| K3/R2F | fortgeführt | 2.560 | 1.520 | 598 | 8 |

R1 stellte 99,8 % der K0-Datenphasen wieder her, aber nur 86,3 % der
K0-M3-Wege. R2F reproduzierte alle Datenphasen und erzeugte 30 zusätzliche
kausal nachgewiesene Wege. Diese sind keine zufälligen Abschlüsse: Jeder zählt
nur, wenn die unabhängige Diagnose den realen M3-Impulsweg rekonstruiert.

Nach Eingabemuster:

| Muster | K0 M3 | K1/R0 M3 | K2/R1 M3 | K3/R2F M3 |
|---|---:|---:|---:|---:|
| `AB` | 112 | 14 | 87 | 112 |
| `BA` | 112 | 14 | 87 | 112 |
| `ALT` | 112 | 14 | 87 | 112 |
| `AA` | 96 | 12 | 108 | 114 |
| `BB` | 136 | 17 | 121 | 148 |

Bei `AB` lieferte R2F in jeder Phase dieselben 14 erfolgreichen Seeds wie K0.
R1 schwankte ab Phase 2 zwischen 10 und 11 Wegen. R0 erzeugte nach Phase 1
keinen weiteren `AB`-Weg.

Die höheren R2F-Werte für `AA` und `BB` zeigen, dass vollständige Erholung
nicht mit vollständiger Zustandswiederherstellung identisch ist. Nicht feuernde
Neuronen behalten Membranreste und alle lokalen Ereignisse bleiben erhalten.
Diese fortgeführte Zustandswirkung wird als Befund, nicht als Fehler, gewertet.

## F. Langläufe mit 2, 32 und 128 Phasen

Die sechs etablierten Seeds wurden zusätzlich mit `AB` untersucht:

| Kontrolle | Phasen bei 128er-Läufen | Datenphasen | M3-Wege | Unterdrückungen |
|---|---:|---:|---:|---:|
| K0 wiederhergestellt | 768 | 384 | 256 | 1.184 |
| K1/R0 fortgeführt | 768 | 18 | 4 | 3.899 |
| K2/R1 fortgeführt | 768 | 384 | 256 | 1.486 |
| K3/R2F fortgeführt | 768 | 384 | 256 | 1.184 |

Die entsprechenden 2- und 32-Phasen-Läufe zeigen dasselbe Muster. R1 und R2F
erreichen in dieser etablierten Teilstichprobe dieselbe Zahl kausaler Wege wie
die wiederhergestellte Referenz. R2F stimmt zusätzlich bei der Zahl
refraktärer Unterdrückungen mit K0 überein.

## G. Erhaltung von Synapsen und Erinnerungen

Für R1, R2H und R2F wurden je 64 Zustände unmittelbar nach einer Handlung und
nach alleiniger Erholung verglichen.

| Prüfung | R1 | R2H | R2F |
|---|---:|---:|---:|
| Synapsenvektor unverändert | 64/64 | 64/64 | 64/64 |
| alle lokalen G3-Ringe unverändert | 64/64 | 64/64 | 64/64 |
| geänderte Gewichte | 0 | 0 | 0 |
| geänderte Aktivitätszähler | 0 | 0 | 0 |
| Neuronen mit Membranänderung, Summe | 0 | 100 | 100 |

Die Membranänderungen in R2 betrafen ausschließlich Neuronen mit fälligem
Erholungsereignis und positivem Restpotenzial. Ringreihenfolge und
Überschreibungszustand blieben erhalten, weil während der Erholung keine neuen
Impulse erzeugt wurden.

18 Speicher-/Ladepaare mit ausstehenden R1/R2-Ereignissen waren sowohl vor als
auch nach dem Zeitfortschritt bitweise im Zustandsfingerabdruck identisch.

## H. Eigenrückmeldung

Sechs Seeds, drei Erholungsvarianten, F0 bis F2 und acht Phasen ergaben:

| Erholung | Rückmeldung | Datenphasen | M3-Wege | Rückmeldeereignisse | Abbrüche |
|---|---|---:|---:|---:|---:|
| R0 | F0/F1/F2 je | 3 | 2 | 0/5/6 | 0 |
| R1 | F0/F1/F2 je | 24 | 16 | 0/40/48 | 0 |
| R2F | F0/F1/F2 je | 24 | 16 | 0/40/48 | 0 |

F1 und F2 änderten in dieser Stichprobe die Zahl gültiger M3-Wege nicht. Sie
erzeugten weder technische Abbrüche noch selbstverstärkende Endlosschleifen.
Zusätzliche sensorische Aktivität und ihre eigenen Erholungsereignisse blieben
deterministisch.

## I. Performance und Speicher

Tatsächlich initialisiert und gemessen wurden 256, 1.024 und 10.000 Neuronen
mit jeweils vier Startsynapsen. 100.000 und eine Million Neuronen wurden aus
den gemessenen Strukturgrößen extrapoliert.

Auf der Messplattform belegt ein Neuron 56 Byte, davon 16 Byte für Generation
und geplanten Zeitpunkt. Ein geplantes Ereignis belegt 32 Byte. Einschließlich
G3-Ring und Ringverwaltung ergeben sich 104 Byte pro Neuron, also ungefähr
10,4 MB bei 100.000 und 104 MB bei einer Million Neuronen, zuzüglich Synapsen
und tatsächlich ausstehender Ereignisse.

| Neuronen | R0 Ereignisse/s | R1 Ereignisse/s | R2F Ereignisse/s | mittlere R1-Erholungen |
|---:|---:|---:|---:|---:|
| 256 | 13,4 Mio. | 6,0 Mio. | 6,1 Mio. | 203 |
| 1.024 | 14,1 Mio. | 4,9 Mio. | 4,9 Mio. | 321 |
| 10.000 | 12,7 Mio. | 4,2 Mio. | 4,2 Mio. | 253 |

R1/R2 verarbeiten zusätzliche reale Ereignisse und sortieren sie in die
Zukunftswarteschlange; die Werte sind deshalb kein reiner Kostenvergleich pro
identischer Nutzlast. R1 und R2 unterscheiden sich praktisch nicht. Kein
Erholungsereignis durchlief das Netzwerk; jedes bearbeitete genau ein Neuron.

Die aktuelle sortierte Sequenz hat lineare Einfügekosten. Bei wenigen aktiven
Neuronen ist sie ausreichend. Eine Heap- oder Kalenderwarteschlange bleibt als
Performanceoption für sehr große gleichzeitig aktive Populationen offen.

## J. Tests und Reproduzierbarkeit

Alle 98 Tests bestehen. Neu geprüft werden unter anderem:

- Zeitfortschritt und zukünftige Planung,
- gleiche Zeitpunkte und deterministische Reihenfolge,
- technische Versuchsgrenzen,
- keine Vorverarbeitung am Antwortende,
- Erholung ohne neue Eingänge,
- veraltete und doppelte Generationen,
- unverändertes R0,
- R1 ohne Membranänderung,
- R2H und R2F,
- kein Feuer durch Erholung und erneutes Feuern danach,
- Synapsen-, Lernzähler- und G3-Erhaltung,
- Persistenz ausstehender Ereignisse.

Alle 14 B-Erfolgsseeds wurden in Phase 1 reproduziert. Die CSV-Spalten besitzen
über sämtliche Zeilen konsistente Schemata. Kein Wiederholungs- oder
Rückmeldungsversuch endete mit technischem Abbruch.

## K. Antworten auf die Abschlussfragen

1. **Verhindert R0 wiederholte Handlungen?** Ja. Vorgelagerte Relais bleiben
   ohne weitere Eingänge refraktär und unterbrechen dadurch die Aktivität, die
   sie zu ihrer eigenen Erholung benötigen würden.
2. **Verbessert unabhängige Erholung die Wiederholung?** Ja. R1 stellt alle
   Datenphasen und den Großteil der M3-Wege wieder her.
3. **Welchen Einfluss hat Membranentspannung?** R2H/R2F stellen in der
   Zweiphasenmatrix alle 14 Wege wieder her; R2F ist über acht Phasen am
   stabilsten.
4. **Welche Dauer ist geeignet?** Jede getestete Dauer funktioniert, sobald
   der äußere Abstand sie erreicht. T3 = 16 bleibt ein technischer
   Vergleichswert, keine biologische Festlegung.
5. **Entstehen fortgeführte M3-Handlungen?** Ja, bis 128 Phasen ohne
   Wiederherstellung.
6. **Bleiben Synapsen und Erinnerungen erhalten?** Ja, vollständig in allen
   192 Integritätsfällen.
7. **Funktioniert Eigenrückmeldung?** Ja, ohne Abbruch oder beobachtete
   Schleifenverstärkung.
8. **Wie hoch sind die Kosten?** 16 Byte zusätzlicher Neuronenzustand und
   32 Byte je ausstehendem Ereignis; aktive Läufe verarbeiten erwartungsgemäß
   zusätzliche Erholungsereignisse.
9. **Welche Variante eignet sich für die nächste Stufe?** R2F mit
   konfigurierbarer Dauer, vorläufig T3 = 16. R1 bleibt wichtige
   Ablationskontrolle.

## Schlussfolgerung

Eigenständige Erholung trennt kurzfristige Erregbarkeit erfolgreich von
langfristiger Erinnerung. R1 beweist, dass weitere Eingänge für Erholung nicht
notwendig sind. R2F zeigt darüber hinaus, dass die vollständige Entspannung des
kurzfristigen Membranzustands die bisherige M3-Handlungsfähigkeit über lange
fortgeführte Sequenzen wiederherstellen kann.

Damit sind wiederholt ausführbare Handlungen und lokale Beteiligungsspeicher
gleichzeitig vorhanden. B.2 darf darauf aufbauen, muss R2F jedoch weiterhin
gegen R1, R0 und wiederhergestellte Zustände kontrollieren. Eine Lernregel ist
aus B.1 ausdrücklich noch nicht abzuleiten.
