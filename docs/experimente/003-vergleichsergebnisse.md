# Experiment 003 – Vergleichsergebnisse

## Langzeitstabilität

Alle Varianten verwenden sechs Seeds und vier Trainingsfolgen, also 24 Läufe.

| Variante | 512 | 1.024 | 4.096 | 16.384 | min / max Wiederholungen |
|---|---:|---:|---:|---:|---:|
| C5 | 24 | 0 | 0 | 0 | 724 / 882 |
| **C5+A1** | **24** | **24** | **23** | **23** | **1.242 / 16.384** |
| C5+A2 | 24 | 0 | 0 | 0 | 802 / 969 |
| C5+A3 | 24 | 0 | 0 | 0 | 724 / 882 |
| C5+A4 | 24 | 0 | 0 | 0 | 724 / 882 |
| C5+A5 | 24 | 0 | 0 | 0 | 724 / 882 |

Nur die Grenze 1 bewirkt langfristige Stabilität. Grenze 2 verzögert den
Abbruch geringfügig; Grenzen 4, 8 und 16 ändern die Aktivitäts- und
Abbruchzahlen gegenüber C5 nicht.

## Stabilität gegen Anpassungsfähigkeit

C5+A1 wächst bei den 23 abgeschlossenen Läufen auf durchschnittlich 58.596
Synapsen. Davon haben 98,81 % Gewicht 0 und 1,19 % Gewicht 1. Zwischen 4.096 und
16.384 Wiederholungen entstehen im Mittel 26.784 neue Synapsen, aber nur 0,04
erfolgreiche Verstärkungen. Die Variante ist aktiv und stabil, ihre
Gewichtsdifferenzierung und fortgesetzte Lernaktivität sind jedoch weitgehend
erschöpft.

Höhere Grenzen bewahren mehrere Gewichtsstufen, werden aber in allen Läufen vor
1.024 instabil. Keine untersuchte Variante löst daher den Zielkonflikt.

## Wiedererkennung und Beständigkeit

`AB`- und `BA`-Tests haben nach `AB`-Training in allen Varianten dieselben
aggregierten Aktivitätswerte. Unterschiede der Feuerreihenfolge bestehen schon
in untrainierten Kontrollen. Spezifische Wiedererkennung ist nicht belegt.

| Variante | `CD`/`EF`/`GH` je 128 | verlängert bis je 512 |
|---|---:|---:|
| C5 | 0/6 | 0/6 |
| **C5+A1** | **6/6** | **4/6** |
| C5+A2 | 0/6 | 0/6 |
| C5+A3–A5 | jeweils 0/6 | jeweils 0/6 |

Bei den stabilen C5+A1-Läufen wächst die spätere `AB`-Reaktion bis auf nahezu
das gesamte Netzwerk. Das zeigt Veränderbarkeit, aber keinen spezifischen Erhalt
der ursprünglichen Erfahrung.

## Entscheidung

C5+A1 bleibt ein wertvoller Stabilitätskontrollfall, wird jedoch nicht als
endgültige Architektur empfohlen. Keine höhere Gewichtsgrenze rechtfertigt eine
weitere isolierte Untersuchung. Das nächste Experiment sollte Wachstum/Dichte
und langfristige Plastizität als getrennte Hypothesen untersuchen und dabei C5
sowie C5+A1 als Referenzen behalten.
