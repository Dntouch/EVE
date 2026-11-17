# EVE – Experiment 003

## Ergänzungsauftrag 001: Erweiterung der synaptischen Gewichtsgrenzen

**Auftrag an:** Nova-EVE

**Ausgangsbasis:** Experiment 003 – Kombinierte neuronale Selbstregulation und
langfristige Lernstabilität

## Anlass

Die ursprüngliche Beschränkung auf C5+A1 und C5+A2 ist für die Forschungsfrage
zu eng. Untersucht werden soll, wie unterschiedlich große synaptische
Gewichtungsspielräume Stabilität, Lernfähigkeit und Netzwerkentwicklung
beeinflussen. Eine niedrige Grenze kann Kaskaden verhindern, aber zugleich
Differenzierung und weiteres Lernen einschränken.

## Erweiterte Versuchsvarianten

| Variante | Refraktärereignisse | maximales Synapsengewicht |
|---|---:|---:|
| C5+A1 | 16 | 1 |
| C5+A2 | 16 | 2 |
| C5+A3 | 16 | 4 |
| C5+A4 | 16 | 8 |
| C5+A5 | 16 | 16 |
| C5 | 16 | unbegrenzt |

C5 bleibt die direkte Referenz; die bisherigen Referenzen aus Experiment 002
bleiben erhalten. Alle sechs Varianten durchlaufen mit identischen Seeds,
Trainingsfolgen und Bedingungen den vollständigen Aufbau: Langzeitstabilität,
Sättigung, Wachstum, verschiedene Erfahrungen, Wiedererkennung, Interferenz,
Beständigkeit, Performance und Speicherverbrauch.

Verbindliche Seeds sind `20261117`, `7`, `42`, `99`, `2026` und `314159`.

## Zusätzliche Forschungsfragen und Messungen

Zu untersuchen sind die Stabilität bei steigender Gewichtsgrenze, eine mögliche
Instabilitätsschwelle, Geschwindigkeit und Anteil der Sättigung, verbleibende
Gewichtsdifferenzierung, Unterschiede im neuronalen Verhalten,
Wiedererkennungshinweise, weitere Erfahrungsverarbeitung sowie Auswirkungen auf
Wachstum, Feuerhäufigkeit und Warteschlange. Der Zielkonflikt zwischen
Stabilität und Anpassungsfähigkeit ist ausdrücklich zu bewerten.

Für jede Variante werden mindestens minimales, maximales und durchschnittliches
Gewicht, vollständige Gewichtsverteilung, Zahl und Anteil der Nullgewichte, Zahl
und Anteil am Maximalgewicht sowie erfolgreiche und wegen der Grenze abgewiesene
Verstärkungen über den Trainingsverlauf dokumentiert.

Eine Gewichtsbegrenzung ist nicht automatisch eine Stabilisierung; eine hohe
Sättigungsquote ist nicht automatisch ein Versagen. Entscheidend bleibt, ob das
Netz neue Erfahrungen verarbeitet und erlernte Unterschiede nutzen kann.

## Gemeinsame Bewertung

Die Vergleichstabelle bewertet alle sechs Varianten datenbasiert nach:

- Stabilität ohne unkontrollierte Ereigniskaskaden;
- fortbestehender Aktivität;
- Veränderbarkeit und Lernfähigkeit;
- differenzierten synaptischen Stärken;
- spezifischen Reaktionen auf erlernte Muster;
- Erhalt früherer Erfahrungen;
- Rechen- und Speicherressourcen.

Keine Gewichtsgrenze wird allein aufgrund ihrer Stabilitätswerte als optimal
bezeichnet.

## Unveränderte Vorgaben

Alle übrigen Vorgaben des Hauptauftrags bleiben unverändert. Insbesondere werden
keine Lernmechanismen, Grundarchitektur, Sensorik, zeitabhängige Plastizität,
STDP, Zeitstempel, Abschwächungs- oder Schwundregeln oder semantischen Neuronen
eingeführt. Varianten bleiben reproduzierbar und separat konfigurierbar;
vorhandene Messdaten bleiben erhalten.

Der Forschungsbericht beantwortet empirisch, wie groß der Gewichtungsspielraum
mindestens sein muss, damit EVE stabil bleibt und weiterhin unterscheidbare
Erfahrungen lernen und verarbeiten kann. Das zulässige Ergebnis ist auch, dass
keine untersuchte Kombination alle Anforderungen erfüllt.

Dieser Ergänzungsauftrag ersetzt ausschließlich die ursprüngliche Beschränkung
auf C5+A1 und C5+A2.
