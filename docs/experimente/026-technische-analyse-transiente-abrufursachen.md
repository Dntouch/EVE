# Technische Analyse – transiente Abrufursachen in B.5.3

## Referenz und Stichprobe

B.5.3 reproduziert B.5.2 vollständig in einem getrennten Ergebnisordner. Die
768 Netze, 393.216 Episoden, 66.159 Verluste, 15.194 Detailzustände, 12.452
Verluste ohne Gewichtsänderung und 14.731 Wiederherstellungen durch den alten
transienten Gesamtzustand stimmen exakt überein. Entdeckung und Bestätigung
verwenden weiterhin je 64 Seeds, drei Stromfamilien sowie normale und inverse
Zuordnung. Historische Ergebnisordner werden nicht verändert.

Die Detailstichprobe bleibt bewusst dieselbe wie in B.5.2: alle 12.452
Verluste ohne Gewichtsänderung sowie erster und zweiter Verlust jedes Pfades.
Sie ist keine ungewichtete Zufallsstichprobe der 66.159 Gesamtverluste.
Ergebnisse werden deshalb nach Phase, Gewichtsursache und Verlustindex
getrennt ausgewiesen. Alle 15.194 Ausgangskontrollen sind gültig: A antwortet
korrekt, B nicht korrekt, identische Kopien stimmen überein und die bisherige
Gesamttransplantation wird reproduziert.

## Technische Zustandsinventur

Die Codepfade trennen unmittelbare Feuerzustände von Plastizitäts- und
Diagnosezuständen:

| Faktor | unmittelbarer E1-Einfluss | spätere Wirkung |
|---|---|---|
| T1 Membranpotenzial | direkte Schwellenprüfung | beeinflusst nachfolgende Integration |
| T2 Eingangsereigniszähler | nein | Phasenabschluss und Aktivitätsreset |
| T3 G3-Ereignisgedächtnis | nein | Kandidatenquelle für Eligibility |
| T4 Eligibility | ohne Modulation nein | Drei-Faktor-Plastizität |
| T5 Synapsenaktivität | nein | Lernschwelle und `active_phase`-Auswahl |
| T6 Refraktär-/Recoveryzustand | direkte Feuersperre möglich | lokale Recoveryereignisse |
| T7/T8 Zeit und Queue | über fällige Ereignisse | Ablauf und Reihenfolge aller Zeitspuren |
| T9 Kontext | ohne Modulation nein | Bindung späterer Eligibility |
| T9 Zufallszustand | im E1 nein | Exploration, Wachstum, Konkurrenzwahl |
| T9 Metriken | nein | ausschließlich Diagnose und Persistenz |

Zeit und vollständige Queue werden nicht künstlich einzeln transplantiert.
Sie sind mit Recovery-, Eligibility- und Kontextgenerationen gekoppelt. Für
Eligibility, Recovery und Kontext übernimmt die Diagnoseschnittstelle die
jeweils zugehörigen Queueereignisse gemeinsam. Die Schnittstelle ist nur
explizit experimentell aufrufbar, prüft identische Topologie und verändert
keinen Produktionspfad automatisch.

## Einzelkomponenten

Die Ergebnisse der alten Komponente in Zustand B und der neuen Komponente in
Zustand A lauten über beide Phasen:

| Komponente | alte Komponente stellt wieder her | neue Komponente löst Verlust aus |
|---|---:|---:|
| Membranpotenziale | 14.731/15.194 | 14.636/15.194 |
| Eingangsereigniszähler | 0/15.194 | 0/15.194 |
| G3 | 0/15.194 | 0/15.194 |
| Eligibility | 0/15.194 | 0/15.194 |
| Synapsenaktivität | 0/15.194 | 0/15.194 |
| Recovery | 0/15.194 | 0/15.194 |
| Kontext | 0/15.194 | 0/15.194 |
| Zufallszustand | 0/15.194 | 0/15.194 |
| Diagnosemetriken | 0/15.194 | 0/15.194 |

Damit erklärt T1 exakt den bisher durch den vollständigen transienten Faktor
wiederhergestellten Anteil. Bei allen 12.452 Verlusten ohne Gewichtsänderung
ist die alte Membrankomponente notwendig und die neue Membrankomponente
hinreichend. G3, Eligibility, Eingangs- und Aktivitätszähler unterscheiden
sich fast überall, sind für den unmittelbaren E1-Abruf aber wirkungslos. Ihre
Rolle bleibt auf späteres Lernen beschränkt.

Die fünf codebegründeten Kombinationen Membran+G3, Membran+Eingangsaktivität,
Membran+Synapsenaktivität, G3+Eligibility und Membran+G3+Eligibility liefern
keinen zusätzlichen Effekt. Jede Kombination mit Membran entspricht T1
allein; G3+Eligibility bleibt wirkungslos. Der kleinste Zustandsfaktor ist
somit das Membranpotenzial.

## Lokalisierung auf einzelne Neuronen

In den 15.194 Detailfällen unterscheiden sich insgesamt 16.427
Neuronmembranen. 13.731 Fälle besitzen genau eine, 1.348 genau zwei und 115
keine Membrandifferenz. Sämtliche Unterschiede liegen auf Entscheidungs- oder
Motorneuronen:

- 16.326 Änderungen betreffen Entscheidungsneuronen,
- 101 Änderungen betreffen Motorneuronen,
- Sensor-, Explorations- und Modulationsneuronen tragen keine Differenz.

Eine einzelne alte Entscheidungsmembran stellt 14.728 Fallantworten wieder
her. Nur drei wiederherstellbare Fälle benötigen die Kombination beider
geänderten Membranen. Einzelne Motortransplantationen stellen zwei Antworten
her. Damit ist die Ursache nicht nur als Feldklasse, sondern fast vollständig
auf ein lokales Entscheidungsneuron reduziert.

## Wechselwirkung mit Gewichten

Alte Membranpotenziale ergeben bei unterschiedlichen Gewichtskonfigurationen:

| Sensorik | Hemmung | betrachtete Antwort korrekt | andere Beziehung korrekt |
|---|---|---:|---:|
| neu | neu | 14.731/15.194 | 3.422/15.194 |
| neu | alt | 15.006/15.194 | 3.447/15.194 |
| alt | neu | 14.919/15.194 | 3.372/15.194 |
| alt | alt | 15.194/15.194 | 3.397/15.194 |

Die Membran erklärt alle transient wiederherstellbaren Fälle, kann aber 463
synaptisch verlorene Antworten nicht allein retten. Sensorische und
inhibitorische Zustände tragen dort zusätzlich bei. Nach alter Membran sind
nur 3.209/15.194 Kopien für beide Beziehungen gleichzeitig korrekt. Eine
diagnostische Wiederherstellung eines verlorenen Pfads ist daher keine
allgemeine Gedächtnisstabilisierung.

## Probeimpuls und Zeitabstand

Für jedes der 768 Netze wird der erste Detailverlust mit X→X, Y→Y, X→Y,
Y→X, X→Y→X und Y→X→Y bei Abständen 0, 1, 2, 4, 8, 16, 32 und 64 untersucht.
Jede frische Kopie reproduziert ihren Ausgangswert. Auf fortgesetzten
Runtimes ändert bereits der erste Probeimpuls Membran, Eingangsaktivität, G3,
Eligibility und Synapsenaktivität und damit die folgende Antwort.

Beispielsweise steigt die korrekte zweite X-Antwort in X→X von 292/768 aus
frischen Kopien auf 409/768 in der fortgesetzten Runtime. Bei X→Y sind es für
die zweite Antwort 519/768 statt 294/768 aus einer frischen Kopie. Der Effekt
ist für alle untersuchten Abstände identisch. Ereignisfreies Verstreichen von
bis zu 64 Zeiteinheiten verändert die Membran in der unveränderten
R0-Konfiguration nicht; es existiert kein globaler Tick und kein passiver
Membranzerfall.

Die Ursache des Probeeffekts ist damit die lokale Membranintegration des
ersten Reizes. G3 und Eligibility werden dabei ebenfalls verändert, erklären
den unmittelbaren zweiten E1-Abruf aber nicht.

## Biologische Einordnung und Grenze

Das Membranpotenzial entspricht funktional einer kurzzeitigen lokalen
Erregungslage. Das nachfolgende B.5.4-Audit hat klargestellt, dass die
B.5.3-Referenz den Standard `to_zero` verwendet; die frühere Angabe
`subtract_threshold` war unzutreffend. Die in B.5.3 gemessenen Unterschiede
entstehen damit durch subthreshold sowie nachfolgend integrierte erregende und
hemmende Eingänge, nicht durch einen Schwellenrest beim Feuern. Bei
`RecoveryMode::input_dependent` und null Refraktärereignissen wird ohne neuen
Eingang kein Recoveryereignis geplant. Deshalb bleibt eine subthreshold
Erregungslage auch bei ereignisfreier Zeit stabil. Der Kausalbefund von B.5.3
ändert sich dadurch nicht; Ursache, Transplantationen und Antworten wurden am
tatsächlichen Runtime-Zustand gemessen.

Der Kausalbefund rechtfertigt noch keinen automatischen Reset. Die
Transplantation kennt einen früher erfolgreichen Zustand, eine natürliche
lokale Regel nicht. Zudem werden nach der Intervention nur 3.209 Detailfälle
für beide Beziehungen gleichzeitig korrekt. K11/K12 bleiben deaktiviert.
B.5.4 kann kontrolliert prüfen, ob die bereits vorhandene lokale,
ereignisgesteuerte Recovery mit Membranrelaxation die diagnostizierte
Dynamik verbessert, ohne zeitliche Integration, die zweite Beziehung oder
Umweltanpassung zu zerstören.

64/64 transplantierte Zustände werden fingerprint- und abrufidentisch
persistiert. Alle 525 bestehenden und 18 neuen Tests bestehen: 543/543.
