# Forschungsbericht – Forschungsstufe B.5.11

## Ergebnis

B.5.11 ersetzt die enge Bewertung einzelner X/Y-Antworten durch eine
Mehrfachlernaufgabe und zehn getrennte Bewertungsdimensionen. A nutzt die
Folge P→Q, B P→R und die Transferaufgabe C Q→R. P, Q und R sind gewöhnliche
sensorische Neuronen; Aufgabenname und Zielhandlung existieren nur in der
Umwelt. Die Lernlogik wurde nicht verändert.

Das Hauptergebnis ist negativ: Unter der verbindlichen zeitlichen Bewertung,
bei der jede Motorhandlung vor dem zweiten Element ein Fehler ist, integriert
die vorhandene Architektur A und B nicht robust. In der unabhängigen
Bestätigung erreicht A→B bedingte Abrufraten von 0,280 und 0,468 sowie ein
Repertoire von 0,901. Gemischte Erfahrungen erreichen 0,432, 0,472 und 0,891.
Unabhängige Eingänge sind mit 0,766, 0,613 und 1,464 deutlich besser.

Gemeinsame Aktivität ist damit nicht automatisch eine gemeinsame funktionale
Repräsentation. In den Detailablationen war keine einzelne nichttriviale
Synapse für A und B zugleich notwendig. Dagegen belegt die isolierte
Schreibgegenprobe erstmals produktive Reorganisation auf Einzelfallebene:
Von 58 B-Schreibereignissen mit unmittelbarer A-Beeinträchtigung waren zehn
nach natürlicher Fortsetzung für ein größeres Endrepertoire notwendig; 48
blieben langfristig nachteilig. Zwei weitere Ereignisse verbesserten A
unmittelbar. Diese kandidatinformierte Detailstichprobe ist
kein Schätzer für deren Populationshäufigkeit.

## Präregistrierte Aufgabe und Stichprobe

| Aufgabe | Folge | äußere Zielhandlung |
|---|---|---|
| A | P → Q | Motor A |
| B | P → R | Motor B |
| C | Q → R | Motor A |
| A-Kontrolle | S → T | Motor A |
| B-Kontrolle | U → V | Motor B |

Alle Elemente besitzen symmetrische, zunächst nullgewichtete Pfade zu beiden
Entscheidungsneuronen. Die geschützte Exploration ist zielunabhängig. Eine
Episode umfasst 16 Zeiteinheiten, der Elementabstand beträgt vier. Es gibt
keinen Reset zwischen Erfahrungen.

Discovery verwendet Seeds 90001–90064, Bestätigung 100001–100064, jeweils
drei unabhängige Stromfamilien und normale/inverse Zuordnung. Jede zentrale
Zelle enthält 64 Seeds, je Phase 384 Netze pro Bedingung. Insgesamt wurden
14.592 zusammengefasste Netzläufe ausgewertet. Bestätigungsdaten wurden weder
zur Aufgaben- noch zur Metrikwahl verwendet.

## Historische Reproduktion

Die Referenzen wurden im B.5.11-Lauf frisch nachgerechnet:

| Referenz | beobachtet | erwartet |
|---|---:|---:|
| B.5.7 Discovery | 242/384 | 242/384 |
| B.5.7 Bestätigung | 254/384 | 254/384 |
| B.5.8/B.5.9 Discovery | 240/384 | 240/384 |
| B.5.8/B.5.9 Bestätigung | 244/384 | 244/384 |
| B.5.10 prospektive W0–W4-Ereignisse | 119 | 119 |

## Sequentielles, gemischtes und wiederholtes Lernen

Die Werte sind Mittel der 384 Netze je Phase; A und B sind bedingte Abrufraten
über sechs Startkontexte; das A/B-Repertoire zählt diese beiden Aufgaben bei
einer bedingten Abrufrate von mindestens 0,5. C bleibt der getrennten
Transferdimension vorbehalten.

| Ablauf | Discovery A / B / Repertoire | Bestätigung A / B / Repertoire |
|---|---:|---:|
| A → B | 0,266 / 0,418 / 0,859 | 0,280 / 0,468 / 0,901 |
| B → A | 0,394 / 0,271 / 0,917 | 0,387 / 0,259 / 0,885 |
| A → B → A | 0,274 / 0,172 / 0,526 | 0,309 / 0,201 / 0,630 |
| A → B → gemischt | 0,391 / 0,461 / 0,797 | 0,432 / 0,472 / 0,891 |
| alternierend | 0,487 / 0,530 / 0,969 | 0,423 / 0,434 / 0,802 |
| zufällig gemischt | 0,432 / 0,411 / 0,896 | 0,467 / 0,466 / 1,036 |
| Blöcke zu 8 | 0,260 / 0,282 / 0,646 | 0,278 / 0,311 / 0,698 |
| unabhängiges A → B | 0,784 / 0,634 / 1,490 | 0,766 / 0,613 / 1,464 |

Der gepaarte Unterschied des mittleren A/B-Abrufs zwischen gemeinsamem und
unabhängigem A→B beträgt in Discovery −0,3672
(95-%-Bootstrap-KI −0,4123 bis −0,3242) und in der Bestätigung −0,3160
(−0,3611 bis −0,2702). Geteilte Elemente verursachen unter der strengen
Zeitbewertung damit reproduzierbare Interferenz. Zufälliges Mischen ist unter
den geteilten Bedingungen am wenigsten schlecht, erreicht aber die
unabhängige Kontrolle nicht.

A wird im ersten Block nach durchschnittlich 2,25 Erfahrungen autonom. Im
dritten A-Block von A→B→A erscheint die erste zeitlich gültige autonome
Antwort nach durchschnittlich 1,64 Erfahrungen; die spätere bedingte
Abrufrate bleibt mit 0,309 jedoch niedrig. Das ist ein Hinweis auf
Vorstrukturierung, kein Nachweis stabiler gemeinsamer Fähigkeit.

## Transfer zu C

Der erste autonome C-Erfolg tritt in der Bestätigung ohne Vortraining nach
2,10 Erfahrungen und nach unabhängiger Kontrolle nach 2,32 auf. Vorheriges A
verlangsamt ihn auf 5,08, A+B auf 5,28 und B auf 4,73 Erfahrungen. Auch die
finale bedingte C-Rate fällt von 0,755 ohne Vortraining auf 0,372 nach
unabhängiger Kontrolle, 0,283 nach A, 0,423 nach B und 0,363 nach A+B. Die
Architektur zeigt daher negativen, keinen positiven Transfer. Die Zahl
gemeinsam aktiver Synapsen steigt trotzdem von 0 ohne Vortraining auf 6,08
nach A, 6,39 nach B und 9,81 nach A+B. Aktivitätsüberlappung allein ist somit
kein Transfernachweis.

## Repräsentation und kausale Reorganisation

In den Einzelablationen waren 228 gemeinsam aktive Synapsenkandidaten
vorhanden. 20 kausale Einzelbeiträge lagen auf allgemeinen geschützten
Ausgabepfaden; keine einzelne nicht geschützte Synapse war für A und B zugleich
notwendig. Kombinierte Ablation getrennt kausaler Beiträge konnte beide
Aufgaben entfernen, belegt aber keine einzelne gemeinsame Repräsentation.

Die W0/W1/W2-Prüfung untersuchte fünf Seeds je Familie und Zuordnung in
Discovery und Bestätigung. Sie fand je Phase 29 konkrete
B-Schreibereignisse, nach denen A unmittelbar nicht mehr abrufbar war, sowie
je ein unmittelbar verbesserndes Ereignis. W1 ließ genau das Ereignis aus,
W2 reproduzierte W0 in 60/60 Fällen exakt. Nach 64 weiteren tatsächlichen
A/B-Erfahrungen waren je 5/29 beeinträchtigende Ereignisse produktiv: W0
besaß ein größeres Repertoire als W1. Die übrigen 24/29 je Phase waren
langfristig nachteilig. Damit sind sowohl
produktive Reorganisation als auch echte negative Interferenz kausal belegt.

## R0, R2 und S4

Für A→B→gemischt ergaben sich:

| Variante | Discovery A / B / Repertoire | Bestätigung A / B / Repertoire |
|---|---:|---:|
| R0 | 0,391 / 0,461 / 0,797 | 0,432 / 0,472 / 0,891 |
| R2-50 | 0,480 / 0,489 / 1,083 | 0,428 / 0,441 / 0,974 |
| S4-d16 | 0,455 / 0,438 / 1,026 | 0,429 / 0,410 / 0,964 |

R2 und S4 erhöhen das Repertoire gegenüber R0 in beiden Phasen leicht, ihre
Bestätigungsintervalle überlappen jedoch und keine Variante erreicht robuste
Integration. S4 bleibt standardmäßig deaktiviert.

Eine 64-Neuronen-Kapazitätskontrolle ohne zusätzliche zielkodierte Verdrahtung
war exakt gleich zur 32-Neuronen-R0-Discovery. Mehr ungenutzte Neuronen lösen
den Engpass nicht. Strukturelle Plastizität wird daraus noch nicht freigegeben,
weil weder fehlende Struktur noch eine zulässige lokale Wachstumsentscheidung
isoliert wurden.

Die diagnostische Schreibsperre erlernt mit 0,000 Repertoire keine Aufgabe.
Nach Reaktivierung ab Erfahrung 128 steigt das Repertoire auf 0,875; aktive
Plastizität bleibt damit für den zusätzlichen Erwerb notwendig, löst die
gemeinsame Integration aber nicht.

## Antworten A–T

**A.** Mehrere Fähigkeiten erscheinen als verteilte Gewichts- und
Aktivitätszustände mit kontextabhängigem Abruf; eine einzelne Speicherstelle
wurde nicht gefunden. **B.** Neuronen und Synapsen werden aktiv wiederverwendet,
funktionale Notwendigkeit ist aber seltener als Überlappung. **C.** Allgemeine
Ausgabepfade sind einzeln kausal; eine einzelne nichttriviale A/B-Synapse wurde
nicht nachgewiesen. **D.** Ja, konkrete B-Schreibungen ändern ihre spätere
funktionale Wirkung. **E.** Nein; zehn isolierte Fälle belegen produktive
Reorganisation. **F.** Ja, auf Einzelfallebene, nicht als allgemeine Rate.

**G.** Teilweise; A erscheint im dritten Block früher, bleibt kontextuell aber
instabil. **H.** Nein; C zeigt negativen Transfer. **I.** Reihenfolge und
Blocklänge wirken stark, aber kein Ablauf integriert die geteilten Aufgaben
robust. **J.** Zeitliche Kontexte verändern den Abruf; zwei robuste geteilte
Fähigkeiten wurden nicht nachgewiesen.
**K.** E1/E2 und Paar-Summen messen primär momentane Leistung unter engem
Startzustand. **L.** V1–V10 müssen getrennt bleiben.

**M.** R2/S4 können die zeitliche Dynamik verändern und erhöhen das Repertoire
leicht, lösen die Integration aber nicht robust. **N.** Eligibility und Modulation ermöglichen
Erwerb und Reorganisation; dieselbe lokale Schreiblogik erzeugt produktive wie
schädliche Fälle. **O.** Die feste Topologie reicht für die unabhängige
Kontrolle besser als für geteilte Folgen. **P.** Nein; geteilte Elemente sind
deutlich weniger effizient als die unabhängige Kontrolle. **Q.** Redundanz ist
sinnvoll, wenn sie gemischte Abrufbarkeit oder Fehlertoleranz kausal erhöht;
das wurde hier nicht allgemein nachgewiesen. **R.** Frühere lokale
Kausalbefunde bleiben gültig, ihre Bezeichnung als Verlust oder Schaden ist
jedoch bewertungs- und zukunftsabhängig. **S.** Nicht robust unter zeitlich
strenger Bewertung. **T.** B.5.12 muss vorzeitige Ausgabe und lokale zeitliche
Kreditbindung innerhalb geteilter Folgen kausal isolieren.

## Validierung, Grenzen und Merge-Empfehlung

756/756 Tests bestehen. Der Persistenztest speichert einen Zustand mit einem
tatsächlich ausstehenden S4-Ereignis; Laden und Fortsetzung sind identisch.
Alle ausgewerteten Netze sind gültig, die Topologie bleibt fest und es
entstehen null zusätzliche persistente Bytes je Neuron oder Synapse.

Die Aufgabe besitzt nur zwei Motorhandlungen und kurze Zweielementfolgen.
V9-Aktivitätsüberlappung ist kein semantischer Repräsentationsbeweis. Die
W0/W1/W2-Stichprobe ist klein und nach unmittelbarer A-Änderung selektiert.
Die 0,5-Repertoiregrenze ist eine vorab festgelegte Aufgabenkennzahl, kein
universelles Lernkriterium. Laufzeit und RSS sind maschinenabhängig.

**B.5.11 nach `main` mergen: Ja.**

Die Empfehlung gilt für Forschungsumgebung, getrennte Auswertung,
Diagnoseschnittstellen, Tests, Rohdaten und Dokumentation. Wissenschaftlich
werden zeitliche Interferenz, negative Transferwirkung und
produktive Einzelfall-Reorganisation integriert. Eine neue neuronale
Produktionsregel, automatische Stabilisierung, strukturelle Plastizität oder
S4-Aktivierung wird nicht freigegeben.
