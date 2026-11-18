# Forschungsstufe B.3 – Neuronale Rückmeldungswege

## Kurzfassung

B.3 führt getrennte, aber regulär wirkende neuronale Bewertungsereignisse und
die Routen F0 bis F3 ein. Bewertungen werden als one-hot codierte sensorische
Information übertragen. Routen entstehen unabhängig von Handlung,
Eligibility und technischer M3-Diagnose. Rückmeldungsaktivität kann bestehende
Eligibility erreichen, erzeugt aber häufig eigene neue Markierungen; beide
Klassen werden in Format 7 dauerhaft getrennt.

Die Erreichbarkeitsfrage wird positiv beantwortet, die zuverlässige
Handlungszuordnung nicht. F3 erreichte in der 64-Seed-/Fünf-Stufen-Matrix 580
vor der Rückmeldung vorhandene lokale Markierungen, erzeugte jedoch 8.199
Empfänge ohne solche Markierung und 1.461 neue rückmeldungsinduzierte
Eligibility-Entscheidungen. Z0 ist damit stark mehrdeutig. Z1 grenzt
Aktivitätsabschnitte neuronal ab, liefert aber keine gemeinsame Identität.
Eine zulässige Z2-Lösung fehlt weiterhin.

Alle 141 automatisierten Tests bestehen. In 8.702 Rohdatenfällen und allen
Kontrollen gab es keine rückmeldungsabhängige Gewichtsänderung. 64/64 Zustände
mit ausstehender Rückmeldung wurden identisch gespeichert und fortgesetzt.

## A. Umsetzung

Die technische Bitbewertung hält Ziel und Ausgabe außerhalb des neuronalen
Signals. Fünf Eingänge codieren 0/25/50/75/100 Prozent; das einfache Signal
verwendet einen Kontakt. F1 nutzt nur den Eingang, F2 ergänzt 19 allgemeine
Direktsynapsen, F3 21 Synapsen über vier allgemeine Relais.

Der Rückmeldungskontext folgt ausschließlich tatsächlich verarbeiteten
Feuer-/Impulsereignissen. Vor jedem Impuls wird vorhandene Eligibility
diagnostiziert. Neu durch Rückmeldung entstehende Markierungen tragen ein
Herkunftsmerkmal und zählen auch bei späterer Kaskadenaktivität nicht als
vorausgegangene Handlung.

## B. Rückmeldungswege F0 bis F3

Jede Route wurde über 64 Seeds und fünf Bewertungswerte untersucht.

| Route | Feuer | Impulse | vorhandene Eligibility erreicht | ohne vorhandene Eligibility | neu erzeugte Eligibility |
|---|---:|---:|---:|---:|---:|
| F0 | 0 | 0 | 0 | 0 | 0 |
| F1 | 347 | 1.440 | 84 | 1.356 | 27 |
| F2 | 1.644 | 8.015 | 446 | 7.569 | 1.324 |
| F3 | 1.781 | 8.779 | 580 | 8.199 | 1.461 |

F3 besitzt die höchste absolute Erreichbarkeit, aber auch den höchsten
Ereignisaufwand und viele unselektive Empfänge. F2 ist etwas sparsamer. F1
zeigt, dass selbst ein einzelner sensorischer Kontakt über die reguläre
Zufallsvernetzung Markierungen erreichen kann, jedoch deutlich seltener.

Der Bewertungwert änderte nur den aktiven one-hot-Eingang; er übertrug keine
Zielbits. Keine Route las technische Beteiligtenlisten.

## C. Verzögerung und Gültigkeit

Für D1/D2/D3 wurden 192 Fälle je Zeitpunkt untersucht.

| Zeitpunkt | vorhandene Markierungen vor Versuch | erreichte vorausgehende Eligibility | Ablaufereignisse |
|---|---:|---:|---:|
| unmittelbar | 2.916 | 336 | 0 |
| unmittelbar vor Ablauf | 2.916 | 396 | 0 |
| exakt beim Ablauf | 2.916 | 0 | 2.916 |
| nach Ablauf | 2.916 | 0 | 2.916 |

Bei exaktem Zeitpunkt werden bereits geplante Abläufe vor der später
eingefügten Rückmeldung verarbeitet. Rückmeldungsaktivität erzeugte dort neue
Markierungen, diese wurden dank Herkunftsmerkmal nicht als vorausgegangene
Erfahrung gezählt. Abgelaufene Eligibility wurde nie reaktiviert.

## D. Fortlaufende Handlungen

Sechs etablierte Seeds durchliefen je Rückmeldungsmodus 2, 8, 32 und 128
Handlungen, insgesamt 1.020 Phasen je N-Variante. Jede Variante enthielt 342
technisch abgeschlossene Antworten.

| Variante | vorausgehende Eligibility erreicht | Empfänge ohne | neu durch Rückmeldung | lokale Überschreibungen |
|---|---:|---:|---:|---:|
| N0 unmittelbar | 1.665 | 26.315 | 4.700 | 15.991 |
| N1 verzögert | 843 | 28.321 | 4.996 | 15.983 |
| N2 überlappend | 902 | 28.931 | 5.146 | 16.103 |
| N3 vertauschte Reihenfolge | 1.021 | 17.043 | 2.690 | 11.832 |
| N4 fehlend | 0 | 0 | 0 | 11.344 |
| N5 widersprüchlich | 2.172 | 22.464 | 3.949 | 15.070 |

Die Zahlen sind Erreichbarkeit, keine korrekte Handlungszuordnung. Insbesondere
beweist N5, dass der neuronale Weg unterschiedliche Werte transportiert, nicht
dass er ihre Zugehörigkeit versteht. Verzögerung, parallele gültige
Markierungen und häufige Überschreibungen verhindern eine eindeutige Zuordnung.

## E. Selektivitätskontrollen

S0 ohne Eligibility und S4 ohne Handlung erzeugten erwartungsgemäß 0 Empfänge
vorausgehender Markierungen. S6 nach Ablauf ebenfalls 0. S1/L1 und S2/L2
erreichten in dieser Routenstichprobe jeweils 110; L1 hält zusätzlich
Nullgewicht-Kandidaten innerhalb der erreichten Markierungen bereit, was für
spätere Plastizität relevant bleibt. Die Unterbrechung S3 erreichte 109, ohne
daraus einen Erfolg abzuleiten. S5 mit späterer Handlung erreichte 128, kann
aber die beiden Handlungen neuronal nicht sicher trennen.

S0 erzeugte wegen deaktivierter Eligibility auch durch Rückmeldung keine neue
Markierung. S4 und S6 erzeugten dagegen 360 beziehungsweise 311 neue
rückmeldungsinduzierte Markierungen, die korrekt von vorausgehenden Erfahrungen
getrennt blieben.

## F. Zuordnung Z0 bis Z2

- Z0 kann gültige lokale Zustände beim Eintreffen feststellen, aber keine
  Handlung identifizieren.
- Z1 kann Beginn/Ende als normalen neuronalen Reiz sichtbar machen. Ohne
  gemeinsamen Kontext bleibt die Zuordnung mehrerer Neuronen mehrdeutig.
- Z2 wird nicht als gelöst implementiert. Eine technische Handlungs-ID direkt
  in Eligibility wäre eine unzulässige Abkürzung. Benötigt wird ein
  neuronaler, lokal verfügbarer und skalierbarer Kontextweg.

Damit ist die zentrale negative Erkenntnis klar: neuronale Zustellung und
zeitliche Überlappung reichen für spätere Plastizität noch nicht aus.

## G. Persistenz, Ressourcen und Sicherheit

Alle 64 Zustände mit ausstehender F3-Rückmeldung waren vor dem Fortsetzen und
nach Verarbeitung bei Zeit 8 identisch. Keine Fortsetzung änderte Gewichte.

Im 256er Einzelbenchmark benötigten F0/F1/F2/F3 0/0/19/21 zusätzliche
Synapsen. F3 belegte dafür auf der Messplattform rund 504 Byte Synapsenpayload,
verarbeitete 47 Ereignisse und erreichte eine maximale Warteschlange von 35.
Die Route bleibt bei 10.000, 100.000 und 1.000.000 Neuronen konstant klein;
diese Werte sind extrapoliert. Die allgemeine Netzwerk- und Eligibility-
Speicherung skaliert unabhängig davon wie in B.2 dokumentiert.

## H. Schlussfolgerung

Bewertungen können reproduzierbar und ohne technische Zielauswahl neuronal
eintreffen. F3 maximiert Erreichbarkeit, F2 bietet den besseren
Aufwandskompromiss. Beide sind noch zu unselektiv. Rückmeldung kann außerdem
selbst Eligibility erzeugen, weshalb das neue Herkunftsmerkmal unverzichtbar
ist.

B.4 darf noch nicht alle erreichten oder markierten Synapsen verändern. Vor
Plastizität fehlt eine belastbare neuronale Handlungszuordnung. Empfohlen wird
eine kurze B.3.1-Forschung zu lokalem Kontext beziehungsweise eine sehr streng
begrenzte B.4-Probe, die unklare und rückmeldungsinduzierte Markierungen
grundsätzlich ausschließt.

## Reproduktion

- `scripts/run-research-stage-b3.sh`
- `research/results/research-stage-b3-routes.csv`
- `research/results/research-stage-b3-delays.csv`
- `research/results/research-stage-b3-sequences.csv`
- `research/results/research-stage-b3-controls.csv`
- `research/results/research-stage-b3-assignment.csv`
- `research/results/research-stage-b3-persistence.csv`
- `research/results/research-stage-b3-performance.csv`

