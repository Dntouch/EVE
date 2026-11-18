# EVE – Experiment 003

## Kombinierte neuronale Selbstregulation und langfristige Lernstabilität

**Auftrag an:** Nova-EVE

**Ausgangsbasis:** `codex/experiment-002-self-regulation`

**Status:** Forschungs- und Entwicklungsauftrag

## Ausgangslage und Forschungsfrage

Experiment 002 zeigte, dass C5 mit 16 gesperrten Eingangsereignissen als einzige
Einzelvariante alle 24 Hauptläufe über 512 Wiederholungen abschloss. Das Netz
blieb aktiv und lernfähig, wurde bei weiteren Erfahrungen aber erneut instabil;
zugleich stiegen einzelne Gewichte bis 319. A1 mit Maximalgewicht 1 verzögerte
die Instabilität isoliert, verhinderte sie jedoch nicht.

Experiment 003 untersucht daher, ob die Kombination aus ereignisbasierter
Refraktärphase und begrenzten Einzelgewichten langfristig stabilisiert, ohne
Speicherung und Unterscheidung von Erfahrungen zu verlieren.

## Ursprünglich verbindliche Varianten

| Variante | Refraktärereignisse | maximales Synapsengewicht |
|---|---:|---:|
| C5+A1 | 16 | 1 |
| C5+A2 | 16 | 2 |

Direkte Referenzen sind C5, A1, A2 und der unveränderte Ursprung. Erforderliche
Referenzläufe werden unter denselben Bedingungen wiederholt. Es werden keine
weiteren Regulationsmechanismen aktiviert.

Der Ergänzungsauftrag 001 erweitert diese Varianten verbindlich; alle übrigen
Bestimmungen dieses Auftrags bleiben bestehen.

## Unveränderte technische Regeln

- C++17 und ereignisgesteuerte Verarbeitung.
- Bestehendes Membranmodell, Reset-Regel und Aktivitätsphasen.
- Bestehende Verstärkungsregel und räumliches Synapsenwachstum.
- Sensorische UTF-8-Bitcodierung und deterministische Initialisierung.
- Refraktärphase und Gewichtsgrenze bleiben unabhängig konfigurierbar.
- Keine Sonderregel darf die Kombination künstlich stabilisieren.

Nicht eingeführt werden insbesondere Abschwächungs- oder Schwundregeln,
Gesamtgewichtsbudgets, neue Hemmung, Membranleck, zeitabhängige Plastizität,
STDP, neue Zeitstempel, semantische Neuronen oder vorgegebene Mustererkennung.

## A – Langfristige Stabilität und Lernen

Alle Läufe verwenden 256 Neuronen, 1.024 initiale Synapsen, dieselbe 3D-Faltung,
dieselben Lernparameter und die Seeds `20261117`, `7`, `42`, `99`, `2026` und
`314159`. Separat trainiert werden `AB`, `BA`, `AA` und `BB`.

Die Prüfstufen sind 512, 1.024, 4.096 und 16.384 Wiederholungen. Nur Varianten,
die eine Stufe vollständig abschließen, gehen in die nächste. Ein erreichtes
Ereignislimit ist kein Abschluss. Stabile Aktivität, Inaktivität und
unkontrollierte Aktivität sind zu unterscheiden.

Zu erfassen sind abgeschlossene Wiederholungen, Ereignisse, Feuerungen,
refraktär unterdrückte Feuerungen, aktive Neuronen, Verstärkungen, an der
Gewichtsgrenze abgewiesene Verstärkungen, Wachstum, Synapsen- und
Gewichtsverteilungen, maximale Warteschlange, Speicher und Laufzeit.

Sättigung ist ausdrücklich zu untersuchen: Anteil am Maximalgewicht, Zeitpunkt
der Sättigung, neue Nullgewichtssynapsen und deren späteres Lernen, fortgesetzte
Strukturänderung und veränderbare Reaktionen auf neue Eingaben. Die Existenz
neuer Synapsen allein weist keine sinnvolle Lernfähigkeit nach.

## B – Unterschiedliche Erfahrungen und Wiedererkennung

Identisch initialisierte Netze werden getrennt mit `AB`, `BA`, `AA` und `BB`
trainiert. Synaptischer Zustand, Eingangsgewichtungen, Struktur,
Aktivitätsverlauf und spätere Reaktionen werden verglichen. Verschiedene
Hashwerte allein gelten nicht als Lernnachweis.

Für die Wiedererkennung werden je Seed drei Zustände verglichen: `AB`-trainiert,
gleichvolumig `BA`-trainiert und untrainiert. Die Tests `AB`, `BA`, `AC`, `AA`
und `BB` starten jeweils von einer eigenen Wiederherstellung des betreffenden
Zustands. Ausgewertet werden Zahl und Reihenfolge feuernder Neuronen,
gemeinsame Gruppen, Unterschiede der Trainingsgruppen und reproduzierbare
Aktivitätsfolgen. Mehr Gesamtaktivität allein belegt keine bevorzugte Reaktion.

## C – Interferenz und Beständigkeit

Nach `AB`-Training wird die `AB`-Reaktion dokumentiert. Danach folgen `CD`,
`EF` und `GH` zunächst je 128-mal, bei stabiler Verarbeitung in längeren Serien.
Anschließend wird `AB` erneut geprüft. Bewertet werden Erhalt beobachtbaren
Verhaltens, Integration neuer Muster, Sättigung, Überlagerung, Aktivität und
Stabilität. Unveränderte Gewichtungen allein belegen keine Beständigkeit.

## D – Ressourcenverbrauch

Kombinationen werden mit C5 verglichen. Zu erfassen sind Ereignis- und
Feuerraten, Laufzeit, Speicher je Neuron und Synapse, Kosten der Gewichtsgrenze,
Warteschlangenentwicklung und Skalierung größerer Netze. Ein statischer
Benchmark ohne Lernen und Wachstum wird getrennt von aktiven Lernbenchmarks
ausgewiesen. Eine GPU-Implementierung ist nicht vorgesehen.

## Technische Prüfungen

Die Testsuite bleibt vollständig grün und wird um Kombinationstests ergänzt:
Refraktärsperre bei weiterlaufender Verarbeitung, Einhaltung der Gewichtsgrenze
auch am Phasenende, Start neuer Synapsen bei Gewicht null, widerspruchsfreie
gemeinsame Zustandsänderung, vollständige Persistenz, identische Ergebnisse nach
Wiederherstellung, Reproduzierbarkeit und unverändertes Referenzverhalten bei
deaktivierten Mechanismen.

## Lieferumfang und Abschluss

Erwartet werden Forschungsbericht, reproduzierbare Skripte, dokumentierte
Parameter, CSV-Rohdaten, Vergleichstabellen, Tests und eine begründete
Entscheidungsvorlage. Beobachtungen, Messwerte und Interpretation sind zu
trennen. Vorhandene Ergebnisse und lokale Änderungen bleiben erhalten.

Das Experiment ist abgeschlossen, wenn Kombinationen und Referenzen geprüft,
Stabilität und Sättigung gemessen, Interferenzläufe durchgeführt oder ihre
Abbrüche dokumentiert, Wiedererkennung mit Kontrollen untersucht, Ressourcen
berichtet und alle automatisierten Tests bestanden sind. Eine endgültige
Architekturentscheidung erfolgt erst nach gemeinsamer Auswertung.
