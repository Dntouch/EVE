# Forschungsstufe B.2 – Lokale neuronale Lernberechtigung

## Kurzfassung

B.2 führt einen optionalen, lokal begrenzten Eligibility-Zustand ein. Er wird
unmittelbar bei einer tatsächlichen Feuerentscheidung ausschließlich aus G3
gebildet, ist von G3 getrennt, besitzt eine lokale Entscheidungsgeneration und
verfällt durch ein eigenes zukünftiges Ereignis. Die technische
M3-Kausalitätsdiagnose erzeugt, ergänzt und korrigiert keine Markierung.

Die zentrale Forschungsfrage wird positiv beantwortet. In der gepaarten
64-Seed-Matrix erfassten sowohl L1 als auch L2 bei allen Kapazitäten 100 % der
336 technisch beobachteten relevanten Schaltungseinträge. Bereits zwei
Kandidaten je Entscheidung reichten. L2/K1 war am selektivsten: 9,79 % der
eindeutig markierten Kandidaten waren technisch relevant; L1/K1 erreichte
8,70 %. L2 schloss alle 444 bei L1/K1 beobachteten Nullgewicht-Markierungen aus.

Größere Puffer verbesserten die Abdeckung nicht. Bei L2 sank die Selektivität
von 9,79 % bei zwei auf 9,28 % bei vier oder acht Kandidaten. Eligibility ist
damit selektiver als der unveränderte G3-Ring, aber weiterhin stark
mehrdeutig. Eine Markierung allein reicht nicht für eine verantwortbare
Gewichtsänderung.

Alle 123 Tests bestehen, darunter 98 unveränderte Referenztests und 25 neue
B.2-Prüfungen. In sämtlichen 64-Seed-Kontrollen blieben Gewichte unverändert.
Alle 64 Persistenzpaare waren vor und nach dem Ablauf identisch fortsetzbar.

## A. Technische Umsetzung

Eligibility wird als feste Structure-of-Arrays im Netzwerk gespeichert. Eine
Entscheidungsmarkierung enthält Generation, gemeinsame Ablaufzeit und bis zur
konfigurierten Grenze kopierte `SynapseId`-Kandidaten. G3 bleibt unverändert.

Das Stammhirn implementiert L0/L1/L2, wiederholte oder eindeutige IDs und die
Verlaufsregeln Überschreiben, begrenztes Beibehalten und Zusammenführen. Das
BIOS plant `eligibility_expiry`. Ein Ereignis wirkt nur auf dieselbe noch
aktive Generation; alte Ereignisse sind wirkungslos.

Persistenzformat 6 speichert Konfiguration, aktive Markierungen,
Generationszähler, Überschreibungszähler und Ablaufereignisse. Formate 1 bis 5
bleiben lesbar und erhalten deaktivierte Eligibility.

## B. Versuchsgrundlage

| Parameter | Wert |
|---|---:|
| Neuronen | 256 |
| reguläre Startsynapsen | 1.024 |
| angeborene Verschaltung | V2, Gewicht 3 |
| Motorschaltung | M3, Gewichte 1 und 3 |
| Feuergrenze | 3 |
| lokales Gedächtnis | G3, 8 Einträge |
| Erholung | R2F, Dauer 16 |
| Exploration | E4-neu, 16 Aktivierungen |
| Lernen / Wachstum | deaktiviert / deaktiviert |
| Seeds | alle 64 aus B und B.1 |

Motor-, Topologie-, Schaltungs- und Explorationssalze bleiben gegenüber B.1
identisch. L0 reproduziert 15 technisch abgeschlossene Antworten und 14
nachgewiesene M3-Wege. Die Rohdaten umfassen 6.160 Datenzeilen in acht CSV-
Dateien.

## C. Kandidatenauswahl, Abdeckung und Selektivität

Jede L1/L2-Zeile wurde für D0, D1, D2 und D3 gepaart wiederholt. Deshalb
enthalten die folgenden Summen je Kapazität 256 Versuche.

| Regel | Kapazität | Kandidaten | Gewicht 0 | relevante markiert | Abdeckung | Selektivität |
|---|---:|---:|---:|---:|---:|---:|
| L1 | 2 | 3.860 | 444 | 336/336 | 100 % | 8,70 % |
| L1 | 4 | 4.164 | 548 | 336/336 | 100 % | 8,07 % |
| L1 | 8 | 4.172 | 552 | 336/336 | 100 % | 8,05 % |
| L2 | 2 | 3.432 | 0 | 336/336 | 100 % | 9,79 % |
| L2 | 4 | 3.620 | 0 | 336/336 | 100 % | 9,28 % |
| L2 | 8 | 3.620 | 0 | 336/336 | 100 % | 9,28 % |

L2/K1 ist damit der beste kompakte Ausgangspunkt dieser Matrix. Das Ergebnis
ist keine allgemeine Kausalitätsregel: L2 kann potenziell später lernrelevante
Nullgewicht-Synapsen nicht markieren, und 90,21 % seiner eindeutigen
Markierungen hatten keinen in dieser Diagnose nachgewiesenen M3-Beitrag.

## D. Wiederholung, Darstellung und Mehrdeutigkeit

Bei zwei aufeinanderfolgenden Phasen erzeugte die wiederholte Darstellung
3.548 Kandidateneinträge, aber nur 2.476 unterschiedliche Markierungen. Die
eindeutige Darstellung erzeugte direkt 2.476. Duplikate erhöhten damit den
Speicher- und Diagnoseaufwand ohne zusätzliche Synapsenabdeckung.

`retain` hielt im Mittel 19,10 Entscheidungsmarkierungen nach einer Phase,
ohne in den ersten zwei Phasen zu überschreiben. `overwrite` und
`merge_unique` hielten 12,77 und ersetzten insgesamt jeweils 810 ältere
Entscheidungen. `merge_unique` spart Kandidaten, verliert aber die Trennung der
zusammengeführten Feuerentscheidungen. Die Standardwahl bleibt deshalb
`unique` + zwei getrennte `retain`-Plätze.

Eine Generation unterscheidet Entscheidungen, nicht einzelne synaptische
Aktivierungsinstanzen. Mehrere Eingänge können gemeinsam oder zufällig zeitnah
vorliegen. Diese Mehrdeutigkeit bleibt bewusst erhalten.

## E. Gültigkeitsdauer und verzögerte Prüfrückmeldung

D0 war bei der Impulsbeobachtung der Feuerentscheidung vorhanden und nach den
gleichzeitigen Feuer-/Ablaufereignissen nicht mehr aktiv. D1, D2 und D3 blieben
bis unmittelbar vor 4, 16 beziehungsweise 64 erhalten und waren am exakten
Ablaufzeitpunkt nicht mehr sichtbar.

In allen 192 exakten Ablaufproben für D1 bis D3 waren aktive Markierungen und
Kandidaten 0. Die Reihenfolge ist definiert als: fällige Ablaufereignisse
zuerst, technisches Prüfauslesen danach. Alle 192 Proben bestätigten
unveränderte Gewichte. Die K7-Kontrolle ohne Handlung erzeugte keine
Markierung.

## F. Fortlaufender Betrieb

Die sechs etablierten Seeds wurden mit L1 und L2 über 2, 8, 32 und 128 Phasen
ohne Zustandswiederherstellung betrieben. Bei 128 Phasen entstanden je Regel
7.720 neue Entscheidungsmarkierungen. 7.308 ältere Markierungen wurden wegen
der lokalen Begrenzung überschrieben. Trotzdem reproduzierten beide Regeln
256 nachgewiesene M3-Wege in 768 Phasen – exakt die B.1-Referenz dieser Seeds.

Der hohe Überschreibungsanteil zeigt die Grenze einer Dauer von 64 bei nur zwei
Entscheidungsplätzen: Eine späte Rückmeldung darf nicht annehmen, dass alle
noch aktiven Markierungen zur letzten Handlung gehören oder dass alle früheren
Entscheidungen noch verfügbar sind.

## G. Kontrollen und Zustandskonsistenz

| Kontrolle | M3-Wege | Entscheidungen | Befund |
|---|---:|---:|---|
| K0, G0 ohne Eligibility | 14 | 0 | Referenz ohne lokalen Zustand |
| K1, G3 ohne Eligibility | 14 | 0 | G3 allein erzeugt keine Markierung |
| K2, Eligibility ohne Wirkung | 14 | 810 | keine Gewichtsänderung |
| K3, unterbrochener M3-Pfad | 0 | 729 | Aktivität markiert, aber kein bewiesener Pfad |
| K4, L1 mit Nullgewichten | 14 | 810 | Nullgewichte sichtbar |
| K5, gezielte Unterbrechung | 12 | 797 | zwei Pfade gingen verloren |
| K6, R1 statt R2F | 14 | 810 | erste Phase identisch |
| K7, Prüfsignal ohne Handlung | 0 | 0 | keine nachträgliche Markierung |

K3 schloss vier Antworten ohne nachgewiesenen M3-Weg ab. Das bestätigt erneut,
dass technischer Abschluss nicht mit kausaler Handlung oder Eligibility-
Qualität gleichgesetzt werden darf.

Alle 64 aktiven Persistenzzustände hatten nach Laden denselben Fingerabdruck.
Nach Zeit 64 waren Original und Wiederherstellung erneut identisch, alle 810
Markierungen abgelaufen und keine mehr aktiv. Tests decken zusätzlich
veraltete Ablaufereignisse nach Überschreiben ab.

## H. Ressourcenverbrauch

Die Standardreservierung benötigt 124 Byte je Neuron. Gemessen wurden:

| Neuronen | ohne Eligibility | mit Eligibility | Differenz |
|---:|---:|---:|---:|
| 256 | 52.448 B | 84.192 B | 31.744 B |
| 10.000 | 2.001.248 B | 3.241.248 B | 1.240.000 B |
| 100.000, geschätzt | – | zusätzlich 12,4 MB | 124 B/Neuron |
| 1.000.000, geschätzt | – | zusätzlich 124 MB | 124 B/Neuron |

Im aktiven 256er-Lauf wurden 181 Ablaufereignisse geplant und verarbeitet.
Der Durchsatz sank in dieser Einzelmessung von 8,91 auf 7,64 Millionen
Ereignisse pro Sekunde. Das ist ein orientierender Messwert, kein belastbarer
Mikrobenchmark. Beim 10.000er Speicherlauf entstand wegen geringer Aktivierung
keine Feuerentscheidung; er validiert Speicherallokation, nicht Ablaufdurchsatz.

## I. Schlussfolgerung

Ein Neuron kann lokal und ohne technische Rückwärtsdiagnose mögliche
Beteiligung festhalten. L2 mit zwei eindeutigen Kandidaten bietet in dieser
M3-Konfiguration die beste Kombination aus vollständiger beobachteter
Abdeckung, höherer Selektivität und kleinem Puffer. Ereignisgesteuerter Verfall,
R2F-Koexistenz, Persistenz und fortlaufender Betrieb funktionieren.

Für rückmeldungsabhängige Gewichtsänderungen fehlen weiterhin mindestens:

- eine neuronale Route, die Rückmeldung zum richtigen lokalen Zustand bringt,
- eine Regel zur Zuordnung einer Rückmeldung zu einer Handlungsgeneration,
- ein Umgang mit nicht markierten Nullgewicht-Synapsen,
- Schutz vor den vielen nicht nachgewiesen relevanten Kandidaten,
- Richtung, Stärke und Begrenzung einer Änderung,
- eine eigene Architekturentscheidung und neue Kontrollen.

Deshalb wird in B.2 keine Gewichtsänderung freigegeben.

## Rohdaten und Reproduktion

- `scripts/run-research-stage-b2.sh`
- `research/results/research-stage-b2-selection.csv`
- `research/results/research-stage-b2-variants.csv`
- `research/results/research-stage-b2-feedback-timing.csv`
- `research/results/research-stage-b2-repetitions.csv`
- `research/results/research-stage-b2-controls.csv`
- `research/results/research-stage-b2-persistence.csv`
- `research/results/research-stage-b2-performance.csv`

