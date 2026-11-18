# Technische Analyse – natürliche Lernwiederholung in B.5.1

## Methodische Absicherung

B.5.1 verwendet unverändert das 32-Neuronen-System aus B.5.0: Feuergrenze 2,
Gewichtsgrenze 3, lokales G3, `active_phase`-Eligibility, ternäre
Drei-Faktor-Plastizität, geschützte Exploration und lernbare lokale Hemmung.
Wachstum, globale Motorselektion, semantische Reizkennungen und neue
Lernzustände bleiben deaktiviert.

Der B.5.0-Runner wird eingebettet und in einem getrennten Ergebnisordner
erneut ausgeführt. Alle historischen Dateien außer der bewusst
maschinenabhängigen Performance-Datei stimmen bytegenau mit B.5.0 überein.
Die historischen Ergebnisordner werden nicht verändert.

Die B.5.1-Matrix enthält 768 natürliche Netze:

- Entdeckung und Bestätigung mit je 64 Seeds,
- drei unabhängige Stromfamilien,
- normale und inverse Zuordnung,
- 512 natürliche Episoden je Netz,
- E1/E2-Checkpoints nach 128, 256 und 512 Episoden.

Damit entstehen 393.216 natürliche Trainingsepisoden, 2.304 getrennte
Checkpointauswertungen und 1.536 beobachtete Sensorpfade. Die 128er-Werte der
Entdeckung reproduzieren die B.5.0-K0-Werte exakt. Bestätigungs-Seeds werden
nicht für Entwicklung oder Auswahl verwendet.

## Tatsächliche Handlung vor Modulation

Jede Episode wird in dieser Reihenfolge ausgeführt:

1. Sensorreiz und vollständige autonome Ereignisverarbeitung,
2. geschützte Exploration nur bei vollständig fehlender autonomer Ausgabe,
3. Zusammenführung der tatsächlich gefeuerten Motorwege,
4. äußere Bewertung der eindeutigen vollständigen Endhandlung,
5. positive Modulation nur bei exakt einem richtigen Motorweg, sonst negative
   Modulation,
6. vollständige Erfassung aller Gewichtsänderungen.

Die validierte Wiederholung verändert die B.5.0-Diagnose nicht: 0/384 Kopien
sind nach einer, 364/384 nach zwei und 384/384 nach drei und vier real
erfolgreich geprüften Erfahrungen autonom korrekt. Keine positive Bewertung
wird ohne beobachteten Erfolg erzeugt.

## Vollständige natürliche Trajektorie

`natural-trajectories.csv.gz` enthält für jede Episode Reiz, Ziel, autonome
Antwort, Exploration, Endhandlung, Bewertung, Modulation, alle vier
Sensorgewichte, beide Hemmgewichte, Feuerzahlen und Membranspitzen der
Entscheidungs- und Motorneuronen. `eligibility-events.csv.gz` löst jeden
Eligibility-Kandidaten auf Impuls, Generation, Rolle und Gewicht auf.
`natural-weight-changes.csv.gz` enthält jede einzelne synaptische Änderung;
keine Kausaldiagnose beruht nur auf einem aggregierten Zähler.

Netze ohne Ereignis würden mit dem Sentinel `4294967295` als zensiert
erhalten. Tatsächlich erleben in 512 Episoden alle 1.536 Pfade mindestens
zweimal eine positive Änderung, einen ersten autonomen Erwerb, mindestens
einen Verlust und mindestens einen Wiedererwerb.

## Natürliche Wiederholung und Schwelle

Die Hypothese fehlender Wiederholung wird widerlegt. Zwischen erster und
zweiter erfolgreicher Erfahrung liegen im Mittel 3,13 Episoden in der
Entdeckung und 3,15 in der Bestätigung. Zwischen erster und zweiter positiver
Änderung derselben Zielverbindung liegen 3,33 beziehungsweise 3,29 Episoden.
Der erste autonome Erwerb tritt im Mittel bei Episode 5,86 beziehungsweise
5,89 auf.

Je Pfad entstehen über 512 Episoden im Mittel rund 182 erfolgreiche
Erfahrungen und 64 positive Zielgewichtsschritte. Wiederholung ist damit nicht
der Langzeitengpass. Zwischen den ersten beiden positiven Zielschritten
liegen über alle Pfade 1.705 negative und 1.753 konkurrierende positive
Episoden; sie verhindern den ersten Erwerb nicht allgemein.

Die natürlichen Zielübergänge umfassen insbesondere 3.447 Schritte 0→1,
26.511 Schritte 1→2, 70.795 Schritte 2→3, 69.791 Schritte 3→2, 25.149 Schritte
2→1 und 1.920 Schritte 1→0. Gewichte erreichen die Schwelle also häufig,
oszillieren danach jedoch weiter.

Bei 2.828 Erwerbsereignissen mit einem Zielschritt 1→2 ist dieser Schritt in
kontrollgleichen Kopien ausnahmslos notwendig und hinreichend. Zusätzlich
können 17.414 Absenkungen des konkurrierenden Pfads von 2 auf 1 den eindeutigen
Abruf notwendig herstellen. Gewicht 2 ist damit für einen isolierten
Sensorpfad wirksam, aber nicht unabhängig von Konkurrenzgewichten und
inhibitorischem Zustand allgemein hinreichend.

## Stabilität, Verlust und Wiedererwerb

Alle 1.536 Pfade werden erworben, aber auch alle später mindestens einmal
verloren. Der erste Verlust folgt im Mittel nach 5,16 weiteren Episoden in der
Entdeckung und 5,74 in der Bestätigung. Der erste Wiedererwerb folgt nach
weiteren 6,50 beziehungsweise 6,96 Episoden. Insgesamt entstehen 66.159
Verluste und 65.284 Wiedererwerbe. Das System lernt nicht zu selten, sondern
wechselt fortlaufend zwischen konkurrierenden Zuständen.

Der Anteil der beim Erwerb korrekten Bindung sinkt in diagnostischen Kopien:

| Abstand nach erstem Erwerb | Entdeckung | Bestätigung |
|---:|---:|---:|
| 0 | 100,00 % | 100,00 % |
| 1 | 70,18 % | 74,35 % |
| 4 | 65,10 % | 65,10 % |
| 8 | 55,21 % | 58,59 % |
| 16 | 44,92 % | 45,83 % |
| 32 | 44,27 % | 42,71 % |
| 64 | 43,88 % | 45,44 % |

Beide Beziehungen werden in allen 768 Netzen mindestens einmal gleichzeitig
korrekt. Dieser Zustand beginnt im Mittel bei Episode 14,86 beziehungsweise
14,98, umfasst aber nur rund 18 % aller späteren Episoden. Die längste
ununterbrochene gleichzeitige Phase beträgt im Mittel nur 9,18 beziehungsweise
9,33 Episoden. Nach Episode 512 sind lediglich 73/384 Entdeckungs- und 67/384
Bestätigungsnetze gleichzeitig korrekt.

## Verlustursachen

Über alle Verluste ist die unmittelbar vorausgehende Klasse:

| Klasse | Entdeckung | Bestätigung |
|---|---:|---:|
| inhibitorische Änderung | 14.139 | 13.957 |
| andere sensorische Änderung | 11.393 | 11.232 |
| keine Gewichtsänderung | 6.235 | 6.217 |
| negative Abschwächung des Zielpfads | 953 | 935 |
| konkurrierende positive Änderung mit Hemmung | 449 | 412 |
| konkurrierende positive Sensoränderung | 115 | 122 |

Beim ersten Verlust dominiert eine inhibitorische Änderung in beiden Phasen
mit 539/768 Pfaden. Sie ist jedoch nicht allein hinreichend als Erklärung:
Die Rücknahme aller inhibitorischen Änderungen der Verlustepisode rettet
132/768 Entdeckungs- und 173/768 Bestätigungspfade. Neutrale Modulation oder
vollständig deaktivierte Plastizität rettet 142/768 beziehungsweise 188/768.
Sensorrollback rettet nur 27/768 beziehungsweise 35/768. Viele Verluste
entstehen somit aus kombinierten Sensor-, Hemm- und transienten
Aktivitätszuständen; 12.452 Verluste treten ganz ohne Gewichtsänderung auf.

## Kontrollierte Fortsetzungen

Aus jedem ersten Erwerbszustand der Entdeckung werden sechs identische
64-Episoden-Fortsetzungen ausgeführt:

| Fortsetzung | erworbener Reiz korrekt | anderer Reiz korrekt | beide korrekt |
|---|---:|---:|---:|
| G1 natürliche Fortsetzung | 43,88 % | 44,40 % | 18,75 % |
| G2 andere Exploration | 45,18 % | 42,45 % | 18,36 % |
| G3 andere Reizfolge | 43,36 % | 43,75 % | 19,01 % |
| G4 jeder weitere Reiz ist der erworbene | 100,00 % | 38,93 % | 38,93 % |
| G4 erworbener Reiz mindestens alle 2 Episoden | 53,78 % | 30,34 % | 19,40 % |
| G4 erworbener Reiz mindestens alle 8 Episoden | 39,71 % | 46,88 % | 19,79 % |
| G5 keine weitere Plastizität | 56,38 % | 38,28 % | 16,80 % |
| G6 Zielgewicht festgehalten | 36,33 % | 38,54 % | 11,98 % |

Eine ausschließlich auf den erworbenen Reiz konzentrierte Fortsetzung hält
diesen in 100 % der Kopien verfügbar, lässt die andere Beziehung aber nur in
38,93 % bestehen. Eine Wiederholung mindestens alle zwei Episoden verbessert
die betrachtete Retention, schädigt die andere Beziehung deutlich; ein
Abstand von acht Episoden stabilisiert den betrachteten Pfad nicht. Selbst
ohne Plastizität bleibt die Fähigkeit wegen dynamischer Membran-, Erholungs-
und Konkurrenzzustände nicht immer abrufbar. Das bloße Festhalten eines
einzelnen Zielgewichts stabilisiert das Gesamtsystem nicht.

## Allgemeine Leistung und Mechanismusentscheidung

Keiner der 36 Entdeckungs- oder Bestätigungsfälle an den drei Checkpoints
erfüllt das allgemeine Erfolgskriterium. Nach 512 Episoden liegt E1 je
Familie und Zuordnung zwischen 40,44 % und 50,10 % in der Entdeckung sowie
40,54 % und 49,97 % in der Bestätigung. Höchstens 8/64 Entdeckungs- und 3/64
Bestätigungs-Seeds sind gleichzeitig robust.

B.5.1 implementiert keinen K9-Mechanismus. Die bestehende Plastizität ist für
Erwerb und Wiedererwerb grundsätzlich ausreichend. Der kausale Restengpass
ist aber nicht auf eine einzelne neue lokale Regel reduziert: inhibitorische
Änderungen dominieren frühe Verluste, gemeinsame Sensoränderungen und
transiente Zustände tragen wesentlich bei. Lernschritt, Schwelle,
Eligibility-Fenster oder automatischer Gewichtsschutz werden deshalb nicht
verändert.

64/64 Langzustände werden exakt persistiert und deterministisch fortgesetzt.
Alle bisherigen 491 und 18 neue B.5.1-Tests bestehen: 509/509.

