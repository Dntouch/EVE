# Forschungsbericht – Forschungsstufe B.4.6

## A. Versuchsaufbau

B.4.6 untersucht 100 Varianten mit 64 gepaarten Seeds. Die Matrix umfasst
acht Aufgaben, K0 bis K7, feste Hemmungsstärken, unveränderte und
beitragsbewusste inhibitorische Plastizität, deaktivierte Wirkung, vertauschte
Motorzuordnung und umgekehrte Konstruktionsreihenfolge. Der beste Kandidat und
eine feste Kontrolle werden mit 64 unabhängigen Seeds bestätigt.

K0 und K1 reproduzieren für die zentrale Mischaufgabe den unveränderten
B.4.5-Aufbau. 421/421 Kerntests bestehen. Die B.4.5-Referenz wird mit
76,8209 % ausgeglichenem Erfolg exakt reproduziert. Sämtliche deterministischen
B.4.5-Verhaltens- und Ereignisdaten bleiben gleich; nur die neu berechnete
Speicherschätzung steigt durch die zusätzliche synaptische Wirkungsart. Die
gespeicherten bisherigen Forschungsdaten werden nicht überschrieben.

## B. Technischer Nachweis lokaler Hemmung

Getrennte inhibitorische Wirkungsart beeinflusst Feuergrenze und Membran lokal,
ohne selbst zu feuern. Gleichzeitige Erregung und Hemmung werden atomar je
Empfänger integriert. Alle getesteten vertauschten Reihenfolgen liefern
identische Ergebnisse. Die bewusst sequenzielle Gegenprobe zeigt dagegen den
erwarteten Reihenfolgeeffekt und begründet die Batch-Grenze.

Keine Variante erzeugt negative Membranwerte, ungültige Netze, Gewichte
außerhalb `[0,3]` oder neue Synapsen. 64/64 Zustände werden exakt geladen und
deterministisch fortgesetzt.

## C. Zentrale Ergebnisse

| Variante | X korrekt | Y korrekt | ausgeglichen | robuste Seeds |
|---|---:|---:|---:|---:|
| K0 B.4.5 `active_phase` | 39,39 % | 32,31 % | 35,85 % | 0/64 |
| K1 B.4.5 negative Ambiguität | 74,31 % | 79,33 % | 76,82 % | 9/64 |
| K2 feste symmetrische Hemmung | 79,42 % | 83,70 % | 81,56 % | 31/64 |
| K3 unveränderte Regel, Start 0 | 84,23 % | 83,46 % | 83,85 % | 35/64 |
| K3 beitragsbewusst, Start 0 | 75,08 % | 83,43 % | 79,26 % | 21/64 |
| K4 Hemmwirkung deaktiviert | 78,75 % | 80,22 % | 79,49 % | 14/64 |
| K7 No-Learning mit Hemmung | 49,11 % | 50,56 % | 49,83 % | 0/64 |

Der beste Kandidat verbessert B.4.5 um 7,03 Punkte. Gegenüber No-Learning
beträgt der gepaarte Vorteil 34,01 Punkte mit 95-%-Intervall
`[30,83; 36,91]`. Die unabhängige Bestätigung erreicht 81,36 % für X,
81,92 % für Y, ausgeglichen 81,64 %, und einen No-Learning-Vorteil von 31,77
Punkten mit positiver Untergrenze 28,55.

## D. Warum das Hauptziel dennoch verfehlt ist

Nur 35/64 Entdeckungs-Seeds und 30/64 Bestätigungs-Seeds überschreiten beide
75-%-Schwellen gleichzeitig; gefordert sind jeweils 48/64. Die
Konstruktionsreihenfolge ist exakt invariant, aber die vertauschte
Motorzuordnung fällt um 13,38 Punkte ab, mit vollständig positivem Intervall.
Damit ist ein verbleibender Handlungsbias nachgewiesen.

Einzelreize werden mit 100 % anwendbarem Erfolg erhalten. X→Y und Y→X bleiben
mit 95,73 % beziehungsweise 94,26 % stark. Strenge Alternierung erreicht nur
66,25 %, Umkehr 58,67 %. Die Fähigkeit bleibt ohne weitere Plastizität im
Retentionstest erhalten, ist aber nicht allgemein robust genug.

## E. Angeborene und erlernte Hemmung

Feste symmetrische Hemmung verbessert die Mischaufgabe bereits deutlich und
eliminiert Doppelaktivierungen im Training, erreicht aber nur 31/64 robuste
Seeds. Lernbare Hemmung aus Stärke null verbessert weiter auf 35/64. Die
vorhandene, unveränderte Drei-Faktor-Regel übertrifft im geprüften Aufbau die
neu untersuchte beitragsbewusste Richtungsumkehr. Der Unterschied zwischen dem
besten lernbaren und dem festen Kandidaten ist mit 2,29 Punkten klein und sein
Intervall `[-0,93; 5,44]` schließt null ein.

Erfahrung kann inhibitorische Stärke verändern, aber ein eigenständiger
kausaler Vorteil dieser Veränderung gegenüber fester Hemmung ist damit nicht
nachgewiesen. Zustandskopie-Ablationen einzelner Wege waren vorab an einen
vollständigen Erfolg gebunden und wurden deshalb nicht als nachgelagerte
Erfolgssuche ausgeführt; die Abschalt- und Fixiergruppen bleiben ausgewiesen.
Beim besten Kandidaten waren in jedem Seed beide allgemeinen inhibitorischen
Wege tatsächlich lernwirksam.

## F. Antworten A bis F

1. **A – lokale Regulation:** Ja. Allgemeine lokale Hemmung verändert
   konkurrierende Feuerbedingungen reproduzierbar.
2. **B – erfahrungsabhängige Veränderung:** Ja, technisch und lokal. Die
   inhibitorischen Gewichte ändern sich über Eligibility und Modulation.
3. **C – bessere Unterscheidung:** Ja, experimentell gegenüber B.4.5 und
   No-Learning; nein hinsichtlich der festgelegten Robustheit.
4. **D – Unabhängigkeit:** Queue-Reihenfolge ja, technische
   Handlungszuordnung noch nein.
5. **E – Erhalt:** Der erreichte Zustand bleibt ohne weitere Plastizität
   erhalten.
6. **F – Stammhirnrolle:** Allgemeine inhibitorische Wirkungsfähigkeit darf
   als experimentelle Grundfunktion bestehen. Eine angeborene konkrete
   Konkurrenzverschaltung oder allgemeine Lernfreigabe folgt daraus nicht.

## G. Abschluss

B.4.6 ist technisch positiv und verhaltensseitig verbessert, verfehlt jedoch
das vorab definierte Hauptziel. Der Mechanismus bleibt experimentell. B.4.7
soll zuerst den nachgewiesenen Zuordnungsbias und die fehlende Robustheit
isolieren, statt weitere globale Auswahlregeln einzuführen.
