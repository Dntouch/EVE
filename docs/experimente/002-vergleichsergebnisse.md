# Experiment 002 – Vergleichsergebnisse

Alle Werte stammen aus sechs Seeds und vier Trainingsfolgen, also 24 Läufen je
Variante mit einem Ziel von 512 Wiederholungen.

| Variante | abgeschlossene Läufe | Wiederholungen min / Mittel / max | Ergebnis |
|---|---:|---:|---|
| A0 | 0/24 | 3 / 7,71 / 11 | instabil |
| A1 | 0/24 | 35 / 52,92 / 77 | instabil, aber deutlich verzögert |
| A2 | 0/24 | 7 / 14,42 / 20 | instabil |
| A3–A5 | jeweils 0/24 | 3 / 7,71 / 11 | wie Referenz instabil |
| B0 | 0/24 | 3 / 7,71 / 11 | instabil |
| B1 | 0/24 | 3 / 8,21 / 12 | instabil trotz Umverteilung |
| B2–B5 | jeweils 0/24 | etwa 3 / 7,7 / 11 | instabil |
| C0 | 0/24 | 3 / 7,71 / 11 | instabil |
| C1 | 0/24 | 5 / 9,92 / 13 | instabil |
| C2 | 0/24 | 10 / 16,21 / 20 | instabil |
| C3 | 0/24 | 32 / 43,62 / 52 | instabil |
| C4 | 0/24 | 147 / 181,75 / 207 | instabil |
| C5 | **24/24** | **512 / 512 / 512** | stabil im Hauptversuch |

C5 blieb aktiv: im Mittel wurden je Lauf 118.554 Ereignisse, 8.540 Feuerereignisse,
253 aktive Neuronen, 19.824 Verstärkungen und 913 neue Synapsen gemessen. Die
mittlere maximale Warteschlange betrug 136 Ereignisse.

Die Stabilität war nicht dauerhaft. Nach zusätzlicher Erfahrung mit `CD`, `EF`
und `GH` erreichte jeder C5-Seed erneut das Sicherheitslimit. Eine abschließende
Beständigkeitsmessung war deshalb nicht möglich.

Nach `AB`-Training erzeugten `AB` und `BA` in C5 im Mittel jeweils 20,67
Feuerereignisse und 19,17 aktive Neuronen. Damit liegt kein Nachweis spezifischer
`AB`-Wiedererkennung vor.
