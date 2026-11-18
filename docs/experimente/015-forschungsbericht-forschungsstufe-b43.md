# Forschungsbericht – Forschungsstufe B.4.3

## A. Ausgangslage und Reproduktion

B.4.2 hatte Einzelreiz-Lernen für A und B, beide Umweltumkehrungen und den
Erhalt geschützter Exploration nachgewiesen. Vor B.4.3 wurden `summary.csv`,
`trials.csv` und `paired-comparisons.csv` des B.4.2-Runners vom Basiscommit mit
64 Seeds byteidentisch reproduziert. Die neuen Daten liegen getrennt unter
`research/results/b43/`.

Die Einzelreizregressionen endeten erneut mit Bewertung +1 für beide Ziele;
beide Umkehrungen endeten ebenfalls bei +1. 345/345 Tests bestehen, darunter
alle bisherigen 303 und 42 neue B.4.3-Fälle.

## B. Interferenzdiagnose

Im unveränderten L1/G3 befanden sich 11.950 technisch dem jeweils anderen Reiz
zuordenbare sensorische Synapsen unter 39.632 Eligibility-Kandidaten (30,15 %).
Die Diagnose kennt den äußeren Reiz ausschließlich zur Auswertung. Die
neuronale Auswahl erhält diese Information nicht.

Damit sind H1, H3, H4 und H6 gemeinsam belegt: G3 hält alte Ereignisse,
zeitliche Nähe selektiert sie erneut, gültige überlappende Spuren werden von
späterer Modulation erreicht, und dieselben wenigen Sensor→Motor-Gewichte
werden abwechselnd verändert. H5 ist zusätzlich an den vielen doppelten
Ausgaben sichtbar.

## C. Lokale Lernspuren L0–L4

| Variante | Erfolg X | Erfolg Y | ausgeglichen | Fremdreizanteil |
|---|---:|---:|---:|---:|
| L0, keine Plastizität | 30,77 % | 30,80 % | 30,79 % | 0 % |
| L1, G3 | 33,61 % | 44,44 % | 39,03 % | 30,15 % |
| L2, letzter Feuereingang | 30,77 % | 30,80 % | 30,79 % | 0 % |
| L3, Entscheidungsfenster | 39,76 % | 32,64 % | 36,20 % | 6,33 % |
| L4, Membranbeteiligung | 0 % | 1,84 % | 0,92 % | 0 % |

L1 übertraf L0 gepaart um 8,28 Prozentpunkte, 95-%-Bootstrapintervall
[5,75; 11,02]. Das ist belastbar, unterschreitet aber das vorab definierte
15-Punkte-Ziel. L3 reduziert die gemessene Vermischung stark, ist gegenüber L1
jedoch um 3,22 Punkte schlechter, Intervall [−6,26; −0,14]. Eine bloß engere
Spur ist daher nicht ausreichend.

## D. Kapazität und zeitliche Trennung

Ein Kandidat erzeugte keine Gewichtsänderung; zwei Kandidaten erreichten
40,00 %, vier und acht je 39,03 %. Die Entscheidungskapazitäten 1–8 änderten
das Ergebnis in diesem Ablauf nicht, weil Modulation die vorhandenen Spuren
verbraucht.

Bei Dauer 4, 8 und 16 blieb der ausgeglichene Erfolg 39,03 %. Dauer 32 und 64
überlappt den Episodenabstand von 16 und senkte ihn auf 15,25 %. Feedback eine
Einheit vor Ablauf änderte Gewicht 0→1; am exakten Ablaufzeitpunkt und danach
blieb es 0. Die deterministische Ereignisreihenfolge verarbeitet den Ablauf
also vor gleichzeitiger Modulation.

## E. Lernreihenfolge und Vergessen

Nur X→A und nur Y→B erreichten jeweils 100 %. Nach X gefolgt von Y blieb Y bei
100 %, X fiel auf 14,86 %. Nach Y gefolgt von X blieb X bei 100 %, Y fiel auf
17,91 %. Das mittlere gepaarte Vergessensmaß betrug 85,93 beziehungsweise
81,14 Prozentpunkte. Alternierendes Lernen erreichte 28,25 %, zufällig
gemischtes 39,03 % und 16er-Blöcke 46,09 %, jedoch mit stark asymmetrischen
18,37 % für X und 73,81 % für Y.

Die zweite Fähigkeit überschreibt die erste somit nicht nur korrelativ. In
einer diagnostischen Kopie wurden ausschließlich die im Y-Block veränderten
Gewichte auf den Stand nach X zurückgesetzt. X lag vor Y bei 100 %, nach Y bei
10,94 % und nach Rücknahme wieder bei 100 %. Im Mittel waren 2,56 Synapsen je
Seed betroffen. Der Eingriff verbesserte 61/64 Seeds; die übrigen drei hatten
X zufällig bereits ohne Eingriff erhalten. Der Trainingslauf selbst wurde nie
zurückgesetzt.

## F. Sensorische Repräsentation und Modulation

Separate Kanäle erreichten 39,03 %. Zwei binäre Kanäle pro Reiz erhöhten den
Wert auf 44,11 %, gepaarter Vorteil +5,16 Punkte mit Intervall
[2,38; 7,93]. Teilweise überlappende Muster fielen auf 36,73 %. Verteilte
Eingänge helfen messbar, erfüllen aber keines der Mehrfachlernkriterien.

Nur positive Modulation war mit 44,48 % die beste gemischte
Modulationsvariante; ternär erreichte 39,03 %, 50 % negative Modulation
34,66 %. Positive Modulation erzeugte jedoch 713 doppelte Ausgaben. Negative
Änderungen sind im jetzigen Aufbau weder hinreichend noch eindeutig hilfreich.

## G. Gleichzeitiges Mehrfachlernen und Kontrollen

Keine der 64-Seed-Gruppen enthielt auch nur einen Seed mit gleichzeitig
mindestens 75 % Erfolg für X und Y. Zufällige Rückmeldung erreichte 34,81 %,
vertauschte 34,73 % und die inverse Zielzuordnung 34,73 %. Gemeinsame
Motorpräferenzen, doppelte und fehlende Ausgaben wurden stets getrennt
gezählt. B.4.3 weist daher kein erfolgreiches Mehrfachlernen nach.

Der bedingte Kapazitätsversuch mit 4, 8 und 16 Reizen wurde regelkonform nicht
ausgeführt.

## H. Übertragbarkeit, Stabilität und Ressourcen

64 Neuronen erreichten 39,03 %, 256 Neuronen 38,27 %. Der gepaarte Unterschied
−1,17 Punkte hatte das Intervall [−3,24; 0,88]. Mehr Neuronen ersetzen den
fehlenden lokalen Trennmechanismus nicht.

Gewichte blieben in [0,3], Wachstum blieb deaktiviert, technische Abbrüche und
unbegrenzte Ereigniskaskaden traten nicht auf. Gemessene Speicherwerte und
lineare, ausdrücklich markierte Projektionen für 10.000, 100.000 und eine
Million Neuronen stehen in `resources.csv`; Laufzeit und Peak-RSS sind
maschinenabhängig in `performance.csv` dokumentiert.

## I. Persistenz

64/64 Zustände wurden mit geschützter Exploration, aktiven Spuren und
ausstehender Modulation exakt geladen und nach Verarbeitung deterministisch
fortgesetzt. Bereits verarbeitete Modulation wurde nicht doppelt angewandt.
`active_phase` nutzt den vorhandenen persistierten Auswahlwert und fügt keinen
Zeitstempel oder Reizkontext pro Synapse hinzu.

## J. Schlussfolgerung

Einzelreiz-Lernen gelingt, weil jede gültige positive Spur dieselbe Beziehung
stützt. Bei zwei Reizen enthält G3 dagegen häufig Kandidaten der anderen
Erfahrung; spätere Rückmeldung ändert dieselben wenigen Gewichte in
konkurrierende Richtungen. Lange Spuren verschärfen dies, sequenzielle Blöcke
zeigen starkes Aktualitätslernen, und die Kausalablation weist tatsächliches
synaptisches Überschreiben nach.

Eine lokale Feuerfenstergrenze reduziert Fremdreizkandidaten, reicht allein
aber nicht für Verhaltenserfolg. Verteilte sensorische Aktivität hilft leicht.
Die nächste Stufe sollte deshalb keine globale Kennung und keinen vollständig
neuen Lernalgorithmus einführen, sondern eine lokale Koinzidenzbindung prüfen,
die mehrere tatsächlich entscheidungsrelevante Eingänge gemeinsam erhält,
Nullgewicht-Verbindungen einschließt und alte Entscheidungskandidaten vor
späterer Modulation schützt.

B.4.3 ist als Ursachenstudie abgeschlossen, das Erfolgsziel des gleichzeitigen
Mehrfachlernens ist ausdrücklich nicht erreicht.

